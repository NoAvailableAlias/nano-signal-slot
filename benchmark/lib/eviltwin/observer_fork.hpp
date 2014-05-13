#ifndef OBSERVER_FORK_HPP
#define OBSERVER_FORK_HPP

#include <memory>
#include <functional>
#include <forward_list>

/**
EvilTwin observer fork created by ApEk
*/

namespace Nano
{

namespace Fork
{

using UniversalPtr = std::shared_ptr<void>;
using UniversalRef = std::weak_ptr<void>;

class Observer
{
    UniversalPtr m_heart { this, [](void*){} };

    protected:

    std::forward_list<UniversalPtr> m_slots;

    UniversalPtr connect(UniversalPtr&& f)
    {
        m_slots.emplace_front(std::forward<UniversalPtr>(f));
        auto position = m_slots.before_begin();
        UniversalRef weak_heart = m_heart;

        return { this, [position, weak_heart](void* self)
        {
            if (!weak_heart.expired())
            static_cast<Observer*>(self)->m_slots.erase_after(position);
        }};
    }
};

//------------------------------------------------------------------------------

template <typename T_rv> class Subject;
template <typename T_rv, class... Args> class Subject<T_rv(Args...)>: Observer
{
    using F = std::function <T_rv(Args...)>;

    public:

    void operator() (Args... args) const
    {
        for (auto const& slot : m_slots)
        {
            (*std::static_pointer_cast<F>(slot))(std::forward<Args>(args)...);
        }
    }
    UniversalPtr connect(F&& slot)
    {
        return Observer::connect(std::make_shared<F>(std::forward<F>(slot)));
    }
};

} // namespace Fork ------------------------------------------------------------

} // namespace Nano ------------------------------------------------------------

#endif // OBSERVER_FORK_HPP

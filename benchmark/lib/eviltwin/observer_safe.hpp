#ifndef NANO_SAFE_EVILTWIN_HPP
#define NANO_SAFE_EVILTWIN_HPP

#include <forward_list>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>

namespace Nano
{

namespace Safe
{

//------------------------------------------------------------------------------

using UniversalPtr = std::shared_ptr<void>;
using UniversalRef = std::weak_ptr<void>;

struct ObserverImpl
{
    std::forward_list<UniversalPtr> m_slots;
    std::mutex m_mutex;

    auto connect(UniversalPtr&& f, std::shared_ptr<ObserverImpl> self)->
        UniversalPtr
    {
        std::lock_guard<std::mutex> scope(m_mutex);

        m_slots.emplace_front(std::forward<UniversalPtr>(f));
        auto position = m_slots.before_begin();

        return UniversalPtr { this, [self, position](void*)
            {
                std::lock_guard<std::mutex> scope(self->m_mutex);

                self->m_slots.erase_after(position);
            }
        };
    }
    auto safe_slot_copy()-> decltype(m_slots)
    {
        std::lock_guard<std::mutex> scope(m_mutex);

        return std::forward_list<UniversalPtr>(m_slots);
    }
};

//------------------------------------------------------------------------------

class Observer
{
    std::shared_ptr<ObserverImpl> pimpl;

    protected:

    Observer(): pimpl(std::make_shared<ObserverImpl>()){}

    auto connect(UniversalPtr&& f)-> UniversalPtr
    {
        return pimpl->connect(std::forward<UniversalPtr>(f), pimpl);
    }
    auto safe_slot_copy()-> decltype(ObserverImpl::m_slots)
    {
        return pimpl->safe_slot_copy();
    }
};

//------------------------------------------------------------------------------

template <typename T_rv> class Subject;
template <typename T_rv, class... Args>
class Subject<T_rv(Args...)> : Observer
{
    using F = std::function<T_rv(Args...)>;

    public:

    void operator() (Args... args)
    {
        for (auto const& slot : safe_slot_copy())
        {
            (*std::static_pointer_cast<F>(slot))(std::forward<Args>(args)...);
        }
    }
    template <typename T, T_rv (T::*mem_fun)(Args...)>
    auto connect(T & slot)-> UniversalPtr
    {
        return connect<T, mem_fun>(&slot);
    }
    template <typename T, T_rv (T::*mem_fun)(Args...)>
    auto connect(T * slot)-> UniversalPtr
    {
        return connect(std::bind(mem_fun, slot));
    }
    template <typename T, T_rv (T::*mem_fun)(Args...) const>
    auto connect(T const& slot)-> UniversalPtr
    {
        return connect<T, mem_fun>(&slot);
    }
    template <typename T, T_rv (T::*mem_fun)(Args...) const>
    auto connect(T const* slot)-> UniversalPtr
    {
        return connect(std::bind(mem_fun, slot));
    }
    auto connect(F&& slot)-> UniversalPtr
    {
        return Observer::connect(std::make_shared<F>(std::forward<F>(slot)));
    }
};

} // namespace Safe ------------------------------------------------------------

} // namespace Nano ------------------------------------------------------------

#endif // NANO_SAFE_EVILTWIN_HPP

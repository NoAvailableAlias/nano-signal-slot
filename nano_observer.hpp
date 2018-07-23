#ifndef NANO_OBSERVER_HPP
#define NANO_OBSERVER_HPP

#include <map>

#include "nano_function.hpp"
#include "nano_pool_allocator.hpp"

namespace Nano
{

class Observer
{
    template <typename T> friend class Signal;

    std::map<
        DelegateKey,
        Observer*,
        std::less<DelegateKey>,
        Nano::Pool_Allocator<std::pair<DelegateKey, Observer*>>
    > m_connections;

    void insert(DelegateKey const& key, Observer* obs)
    {
        m_connections.emplace(key, obs);
    }

    void remove(DelegateKey const& key, Observer* obs)
    {
        m_connections.erase(key);
    }

    template <typename Delegate, typename... Uref>
    void on_each(Uref&&... args)
    {
        for (auto const& connection : m_connections)
        {
            // Perfect forward and emit
            Delegate(connection.first)(std::forward<Uref>(args)...);
        }
    }

    template <typename Delegate, typename Accumulate, typename... Uref>
    void on_each_accumulate(Accumulate&& accumulate, Uref&&... args)
    {
        for (auto const& connection : m_connections)
        {
            // Perfect forward, emit, and accumulate the return value
            accumulate(Delegate(connection.first)(std::forward<Uref>(args)...));
        }
    }

    protected:

    // Guideline #4: A base class destructor should be
    // either public and virtual, or protected and non-virtual.
    ~Observer()
    {
        remove_all();
    }

    public:

    bool is_empty() const
    {
        m_connections.empty();
    }

    void remove_all()
    {
        auto iter = m_connections.cbegin();
        auto stop = m_connections.cend();

        while (iter != stop)
        {
            auto const& delegate_key = iter->first;
            auto const& observer = iter->second;

            // Advance the iterator before possible removal
            std::advance(iter, 1);

            observer->remove(delegate_key, observer);
        }
    }
};

} // namespace Nano ------------------------------------------------------------

#endif // NANO_OBSERVER_HPP

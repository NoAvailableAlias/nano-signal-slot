#ifndef NANO_OBSERVER_HPP
#define NANO_OBSERVER_HPP

#include <map>
#include <mutex>
#include <shared_mutex>

#include "nano_function.hpp"
#include "nano_noop_mutex.hpp"
#include "nano_pool_allocator.hpp"

namespace Nano
{

#ifdef NANO_DEFINE_THREADSAFE_OBSERVER
template <typename Mutex = std::shared_mutex>
#else
template <typename Mutex = Noop_Mutex>
#endif
class Observer
{
    template <typename T> friend class Signal;

    std::map<
        DelegateKey,
        Observer*,
        std::less<DelegateKey>,
        Nano::Pool_Allocator<std::pair<DelegateKey, Observer*>>
    > m_connections;

    // Noop_Mutex will be optimized away (hopefully)
    mutable Mutex m_shared_mutex;

    void insert(DelegateKey const& key, Observer* obs)
    {
        std::unique_lock<Mutex> unique_lock(m_shared_mutex);
        m_connections.emplace(key, obs);
    }

    void remove(DelegateKey const& key, Observer* obs)
    {
        std::unique_lock<Mutex> unique_lock(m_shared_mutex);
        m_connections.erase(key);
    }

    void remove(DelegateKey const& key)
    {
        m_connections.erase(key);
    }

    template <typename Delegate, typename... Uref>
    void on_each(Uref&&... args)
    {
        std::shared_lock<Mutex> shared_lock(m_shared_mutex);

        auto iter = m_connections.cbegin();
        auto stop = m_connections.cend();

        while (iter != stop)
        {
            auto const& delegate = iter->first;
            // Prevent iterator invalidation
            std::advance(iter, 1);
            // Perfect forward and fire
            Delegate(connection.first)(std::forward<Uref>(args)...);
        }
    }

    template <typename Delegate, typename Accumulate, typename... Uref>
    void on_each_accumulate(Accumulate&& accumulate, Uref&&... args)
    {
        std::shared_lock<Mutex> shared_lock(m_shared_mutex);

        auto iter = m_connections.cbegin();
        auto stop = m_connections.cend();

        while (iter != stop)
        {
            auto const& delegate = iter->first;
            // Prevent iterator invalidation
            std::advance(iter, 1);
            // Perfect forward, fire, and accumulate the return value
            accumulate(Delegate(connection.first)(std::forward<Uref>(args)...));
        }
    }
    
    public:

    void remove_all()
    {
        std::unique_lock<Mutex> unique_lock(m_shared_mutex);

        auto iter = m_connections.cbegin();
        auto stop = m_connections.cend();

        while (iter != stop)
        {
            auto const& delegate = iter->first;
            auto const& observer = iter->second;
            // Prevent iterator invalidation
            std::advance(iter, 1);
            // Remove the delegate from the observer
            observer->remove(delegate);
        }
    }

    bool is_empty() const
    {
        std::shared_lock<Mutex> shared_lock(m_shared_mutex);
        m_connections.empty();
    }

    protected:

    // Guideline #4: A base class destructor should be
    // either public and virtual, or protected and non-virtual.
    ~Observer()
    {
        remove_all();
    }
};

} // namespace Nano ------------------------------------------------------------

#endif // NANO_OBSERVER_HPP

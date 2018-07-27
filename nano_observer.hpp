#ifndef NANO_OBSERVER_HPP
#define NANO_OBSERVER_HPP

#include <map>
#include <mutex>

#include "nano_function.hpp"
#include "nano_noop_mutex.hpp"
#include "nano_pool_allocator.hpp"

namespace Nano
{

#ifdef NANO_DEFINE_THREADSAFE_OBSERVER
template <typename Mutex = std::recursive_mutex>
#else
template <typename Mutex = Noop_Mutex>
#endif
class Observer
{
    // Only Nano::Signal is allowed access
    template <typename> friend class Signal;

    std::map<
        DelegateKey,
        Observer*,
        std::less<DelegateKey>,
        Nano::Pool_Allocator<std::pair<const DelegateKey, Observer*>>
    > m_connections;

    // Noop_Mutex will be optimized away
    mutable Mutex m_this_mutex;

    void insert(DelegateKey const& key, Observer* observer)
    {
        std::unique_lock<Mutex> lock(m_this_mutex);
        m_connections.emplace(key, observer);
    }

    void remove(DelegateKey const& key, Observer* observer)
    {
        std::unique_lock<Mutex> lock(m_this_mutex);
        m_connections.erase(key);
    }

    template <typename Delegate, typename... Uref>
    void on_each(Uref&&... args)
    {
        std::unique_lock<Mutex> lock(m_this_mutex);

        auto iter = m_connections.cbegin();
        auto stop = m_connections.cend();

        while (iter != stop)
        {
            auto const& delegate = iter->first;
            std::advance(iter, 1);

            // Perfect forward and fire
            Delegate::bind(delegate)(std::forward<Uref>(args)...);
        }
    }

    template <typename Delegate, typename Accumulate, typename... Uref>
    void on_each_accumulate(Accumulate&& accumulate, Uref&&... args)
    {
        std::unique_lock<Mutex> lock(m_this_mutex);

        auto iter = m_connections.cbegin();
        auto stop = m_connections.cend();

        while (iter != stop)
        {
            auto const& delegate = iter->first;
            std::advance(iter, 1);

            // Perfect forward, fire, and accumulate the return value
            accumulate(Delegate::bind(delegate)(std::forward<Uref>(args)...));
        }
    }

    public:

    void remove_all()
    {
        std::unique_lock<Mutex> lock(m_this_mutex);

        auto iter = m_connections.cbegin();
        auto stop = m_connections.cend();

        while (iter != stop)
        {
            auto const& delegate = iter->first;
            auto const& observer = iter->second;
            std::advance(iter, 1);

            // Instead of forcing tree adjustments
            if (observer != this)
            {
                // Remove the delegate from the observer
                observer->remove(delegate, this);
            }
        }
        // Then remove all this connections
        m_connections.clear();
    }

    bool is_empty() const
    {
        std::unique_lock<Mutex> lock(m_this_mutex);
        return m_connections.empty();
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

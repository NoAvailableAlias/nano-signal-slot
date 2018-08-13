#pragma once

#include <forward_list>
#include <memory>

#include "nano_function.hpp"
#include "nano_mutex.hpp"

namespace Nano
{

template <typename Mutex = Noop_Mutex>
class Observer
{
    // Only Nano::Signal is allowed access
    template <typename, typename> friend class Signal;

    using Slot_Pair = std::pair<const Delegate_Key, Observer*>;

    std::forward_list<std::unique_ptr<Slot_Pair>> connections;
    mutable Mutex mutex;

    //--------------------------------------------------------------------------

    void insert(Delegate_Key const& key, Observer* observer)
    {
        std::unique_lock<Mutex> lock(mutex);
        connections.emplace_front(std::make_unique<Slot_Pair>(key, observer));
    }

    void remove(Delegate_Key const& key, Observer*)
    {
        std::unique_lock<Mutex> lock(mutex);
        connections.remove_if([&key](auto const& pair)
        {
            return pair->first == key;
        });
    }

    template <typename Function, typename... Uref>
    void for_each(Uref&&... args)
    {
        std::unique_lock<Mutex> lock(mutex);

        auto iter = connections.cbegin();
        auto stop = connections.cend();

        while (iter != stop)
        {
            auto const& delegate = (*iter)->first;
            std::advance(iter, 1);

            // Perfect forward and fire
            Function::bind(delegate)(std::forward<Uref>(args)...);
        }
    }

    template <typename Function, typename Accumulate, typename... Uref>
    void for_each_accumulate(Accumulate&& accumulate, Uref&&... args)
    {
        std::unique_lock<Mutex> lock(mutex);

        auto iter = connections.cbegin();
        auto stop = connections.cend();

        while (iter != stop)
        {
            auto const& delegate = (*iter)->first;
            std::advance(iter, 1);

            // Perfect forward, fire, and accumulate the return value
            accumulate(Function::bind(delegate)(std::forward<Uref>(args)...));
        }
    }

    public:

    void disconnect_all()
    {
        std::unique_lock<Mutex> lock(mutex);

        auto iter = connections.cbegin();
        auto stop = connections.cend();

        while (iter != stop)
        {
            auto const& delegate = (*iter)->first;
            auto const& observer = (*iter)->second;
            std::advance(iter, 1);

            // Do not remove from this while iterating
            if (observer != this)
            {
                // Remove the delegate from the observer
                observer->remove(delegate, this);
            }
        }
        // Then remove all this connections
        connections.clear();
    }

    bool is_empty() const
    {
        std::unique_lock<Mutex> lock(mutex);
        return connections.empty();
    }

    protected:

    // Guideline #4: A base class destructor should be
    // either public and virtual, or protected and non-virtual.
    ~Observer()
    {
        disconnect_all();
    }
};

} // namespace Nano ------------------------------------------------------------

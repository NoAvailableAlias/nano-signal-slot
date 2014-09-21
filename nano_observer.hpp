#ifndef NANO_OBSERVER_HPP
#define NANO_OBSERVER_HPP

#include "nano_function.hpp"

#include <map>

namespace Nano
{

class Observer
{
    template <typename T> friend class Signal;

    std::map<DelegateKey, Observer*> tracked_connections;

    void insert(DelegateKey const& key, Observer* observer)
    {
        tracked_connections.emplace(key, observer);
    }
    void insert(DelegateKey const& key)
    {
        tracked_connections.emplace(key, this);
    }
    void remove(DelegateKey const& key)
    {
        tracked_connections.erase(key);
    }

    protected:

   ~Observer()
    {
        auto iter = tracked_connections.cbegin();
        auto stop = tracked_connections.cend();

        while (iter != stop)
        {
            auto const& delegate_key = iter->first;
            auto const& observer = iter->second;

            std::advance(iter, 1);

            observer->remove(delegate_key);
        }
    }
};

} // namespace Nano ------------------------------------------------------------

#endif // NANO_OBSERVER_HPP

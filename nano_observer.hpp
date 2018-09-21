#pragma once

#include <algorithm>
#include <vector>

#include "nano_function.hpp"
#include "nano_mutex.hpp"

namespace Nano
{

template <typename MT_Policy = ST_Policy>
class Observer : private MT_Policy
{
    // Only Nano::Signal is allowed private access
    template <typename, typename> friend class Signal;

    struct Connection
    {
        Delegate_Key delegate;
        Observer* observer;

        Connection() noexcept = default;
        Connection(Delegate_Key const& key) : delegate(key), observer(nullptr) {}
        Connection(Delegate_Key const& key, Observer* obs) : delegate(key), observer(obs) {}
    };

    struct Z_Order
    {
        inline bool operator()(Delegate_Key const& lhs, Delegate_Key const& rhs) const
        {
            std::size_t x = lhs[0] ^ rhs[0];
            std::size_t y = lhs[1] ^ rhs[1];

            if ((x < y) && (x < (x ^ y)))
            {
                return lhs[1] < rhs[1];
            }
            return lhs[0] < rhs[0];
        }

        inline bool operator()(Connection const& lhs, Connection const& rhs) const
        {
            return operator()(lhs.delegate, rhs.delegate);
        }
    };

    std::vector<Connection> connections;

    //--------------------------------------------------------------------------

    void insert(Delegate_Key const& key, Observer* observer)
    {
        auto lock = MT_Policy::get_lock_guard();

        auto start = connections.begin();
        auto stop = connections.end();

        if (start == stop || start->observer)
        {
            connections.emplace(std::upper_bound(start, stop, key, Z_Order()), key, observer);
        }
        else
        {
            connections[0] = { key, observer };
            std::sort(start, std::lower_bound(start, stop, key, Z_Order()), Z_Order());
        }
    }

    void remove(Delegate_Key const& key) noexcept
    {
        auto lock = MT_Policy::get_lock_guard();

        auto start = connections.begin();
        auto stop = connections.end();

        auto slot = std::lower_bound(start, stop, key, Z_Order());

        if (slot != stop)
        {
            std::rotate(start, slot, slot + 1);
            connections[0] = {};
        }
    }

    //--------------------------------------------------------------------------

    template <typename Function, typename... Uref>
    void for_each(Uref&&... args) const
    {
        auto lock = MT_Policy::get_lock_guard();

        for (auto const& slot : MT_Policy::copy_or_ref(connections, lock))
        {
            if (slot.observer)
            {
                Function::bind(slot.delegate)(std::forward<Uref>(args)...);
            }
        }
    }

    template <typename Function, typename Accumulate, typename... Uref>
    void for_each_accumulate(Accumulate&& accumulate, Uref&&... args) const
    {
        auto lock = MT_Policy::get_lock_guard();

        for (auto const& slot : MT_Policy::copy_or_ref(connections, lock))
        {
            if (slot.observer)
            {
                accumulate(Function::bind(slot.delegate)(std::forward<Uref>(args)...));
            }
        }
    }

    //--------------------------------------------------------------------------

    public:

    void disconnect_all() noexcept
    {
        auto lock = MT_Policy::get_lock_guard();

        for (auto const& slot : connections)
        {
            if (slot.observer && slot.observer != this)
            {
                slot.observer->remove(slot.delegate);
            }
        }
        connections.clear();
    }

    bool is_empty() const noexcept
    {
        auto lock = MT_Policy::get_lock_guard();

        return connections.empty() || connections.back().observer == nullptr;
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

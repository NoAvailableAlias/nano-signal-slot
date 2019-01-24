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
            auto k = (x < y) && x < (x ^ y);
            return lhs[k] < rhs[k];
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
        auto lock = MT_Policy::lock_guard();

        auto begin = std::begin(connections);
        auto end = std::end(connections);

        connections.emplace(std::upper_bound(begin, end, key, Z_Order()), key, observer);
    }

    void remove(Delegate_Key const& key) noexcept
    {
        auto lock = MT_Policy::lock_guard();

        auto begin = std::begin(connections);
        auto end = std::end(connections);

        auto slots = std::equal_range(begin, end, key, Z_Order());
        connections.erase(slots.first, slots.second);
    }

    //--------------------------------------------------------------------------

    template <typename Function, typename... Uref>
    void for_each(Uref&&... args)
    {
        auto lock = MT_Policy::lock_guard();

        for (auto const& slot : MT_Policy::copy_or_ref(connections, lock))
        {
            Function::bind(slot.delegate)(std::forward<Uref>(args)...);
        }
    }

    template <typename Function, typename Accumulate, typename... Uref>
    void for_each_accumulate(Accumulate&& accumulate, Uref&&... args)
    {
        auto lock = MT_Policy::lock_guard();

        for (auto const& slot : MT_Policy::copy_or_ref(connections, lock))
        {
            accumulate(Function::bind(slot.delegate)(std::forward<Uref>(args)...));
        }
    }

    //--------------------------------------------------------------------------

    public:

    void disconnect_all() noexcept
    {
        auto lock = MT_Policy::lock_guard();

        for (auto const& slot : connections)
        {
            if (slot.observer != this)
            {
                slot.observer->remove(slot.delegate);
            }
        }
        connections.clear();
    }

    bool is_empty() const noexcept
    {
        auto lock = MT_Policy::lock_guard();

        return connections.empty();
    }

    protected:

    // Guideline #4: A base class destructor should be
    // either public and virtual, or protected and non-virtual.
    ~Observer()
    {
        disconnect_all();
    }

    Observer() = default;
    Observer(Observer const&) = delete;
    Observer& operator= (Observer const&) = delete;
};

} // namespace Nano ------------------------------------------------------------

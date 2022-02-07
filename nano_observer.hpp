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
        typename MT_Policy::Weak_Ptr observer;

        Connection() noexcept = default;
        Connection(Delegate_Key const& key) : delegate(key), observer() {}
        Connection(Delegate_Key const& key, Observer* obs) : delegate(key), observer(obs->weak_ptr()) {}
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
    bool movedFrom = false;

    //--------------------------------------------------------------------------

    void insertInternal(Delegate_Key const& key, Observer* obs)
    {
        auto begin = std::begin(connections);
        auto end = std::end(connections);

        connections.emplace(std::upper_bound(begin, end, key, Z_Order()), key, obs);
    }

    void insert(Delegate_Key const& key, Observer* obs)
    {
        [[maybe_unused]] auto lock = MT_Policy::lock_guard();

        insertInternal(key, obs);
    }

    void remove(Delegate_Key const& key) noexcept
    {
        [[maybe_unused]] auto lock = MT_Policy::lock_guard();

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
            if (auto observer = MT_Policy::observed(slot.observer))
            {
                Function::bind(slot.delegate)(args...);
            }
        }
    }

    template <typename Function, typename Accumulate, typename... Uref>
    void for_each_accumulate(Accumulate&& accumulate, Uref&&... args)
    {
        auto lock = MT_Policy::lock_guard();

        for (auto const& slot : MT_Policy::copy_or_ref(connections, lock))
        {
            if (auto observer = MT_Policy::observed(slot.observer))
            {
                accumulate(Function::bind(slot.delegate)(args...));
            }
        }
    }

    void move_connections_from(Observer* other) noexcept
    {
        [[maybe_unused]] auto lock = MT_Policy::lock_guard();
        [[maybe_unused]] auto otherLock = other->lock_guard();

        connections.clear();

        // Disconnect other from everyone else, and connect them to us instead
        for (auto const& slot : other->connections)
        {
            if (auto observer = MT_Policy::visiting(slot.observer))
            {
                auto obsPtr = static_cast<Observer*>(MT_Policy::unmask(observer));
                obsPtr->remove(slot.delegate);
                obsPtr->insert(slot.delegate, this);

                // We already have a lock, don't try to lock again
                insertInternal(slot.delegate, obsPtr);
            }
        }

        other->connections.clear();
    }

    //--------------------------------------------------------------------------

    public:

    void disconnect_all() noexcept
    {
        [[maybe_unused]] auto lock = MT_Policy::lock_guard();

        for (auto const& slot : connections)
        {
            if (auto observer = MT_Policy::visiting(slot.observer))
            {
                static_cast<Observer*>(MT_Policy::unmask(observer))->remove(slot.delegate);
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
        MT_Policy::before_disconnect_all();

        if (!movedFrom)
            disconnect_all();
    }

    Observer() noexcept = default;

    // Observer may be movable depending on policy, but should never be copied
    Observer(Observer const&) noexcept = delete;
    Observer& operator= (Observer const&) noexcept = delete;

    // When moving an observer, make sure everyone it's connected to knows about it
    Observer(Observer&& other) noexcept
    {
        move_connections_from(&other);
        other.movedFrom = true;
    }

    Observer& operator=(Observer&& other) noexcept
    {
        move_connections_from(&other);
        other.movedFrom = true;
        return *this;
    }
};

} // namespace Nano ------------------------------------------------------------

#pragma once

#include <cassert>
#include <atomic>
#include <memory>
#include <mutex>
#include <thread>

namespace Nano
{

class Spin_Mutex
{
    std::atomic_flag lock_flag = ATOMIC_FLAG_INIT;

    public:

    inline void lock()
    {
        while (lock_flag.test_and_set(std::memory_order_acquire))
        {
            std::this_thread::yield();
        }
    }

    inline void unlock()
    {
        lock_flag.clear(std::memory_order_release);
    }
};

//------------------------------------------------------------------------------

class ST_Policy
{
    public:

    template <typename T, typename L>
    inline T const& copy_or_ref(T const& param, L&&) const
    {
        return param;
    }

    constexpr auto lock_guard() const
    {
        return false;
    }

    protected:

    using Weak_Ptr = ST_Policy*;

    constexpr auto weak_ptr()
    {
        return this;
    }

    constexpr auto observed(Weak_Ptr) const
    {
        return true;
    }

    constexpr auto visiting(Weak_Ptr observer) const
    {
        return (observer == this ? nullptr : observer);
    }

    template <typename T>
    constexpr auto static_pointer_cast(Weak_Ptr observer) const
    {
        return (T*)observer;
    }

    constexpr void before_disconnect_all() const
    {

    }
};

//------------------------------------------------------------------------------

template <typename Mutex = Spin_Mutex>
class TS_Policy
{
    mutable Mutex mutex;

    public:

    template <typename T, typename L>
    inline T const& copy_or_ref(T const& param, L&&) const
    {
        return param;
    }

    inline auto lock_guard() const
    {
        return std::lock_guard<TS_Policy>(*const_cast<TS_Policy*>(this));
    }

    inline void lock() const
    {
        mutex.lock();
    }

    inline void unlock() noexcept
    {
        mutex.unlock();
    }

    protected:

    using Weak_Ptr = TS_Policy*;

    constexpr auto weak_ptr()
    {
        return this;
    }

    constexpr auto observed(Weak_Ptr) const
    {
        return true;
    }

    constexpr auto visiting(Weak_Ptr observer) const
    {
        return (observer == this ? nullptr : observer);
    }

    template <typename T>
    constexpr auto static_pointer_cast(Weak_Ptr observer) const
    {
        return (T*)observer;
    }

    constexpr void before_disconnect_all() const
    {

    }
};

//------------------------------------------------------------------------------

class ST_Policy_Safe
{
    public:

    template <typename T, typename L>
    inline T copy_or_ref(T const& param, L&&) const
    {
        return param;
    }

    constexpr auto lock_guard() const
    {
        return false;
    }

    protected:

    using Weak_Ptr = ST_Policy_Safe*;

    constexpr auto weak_ptr()
    {
        return this;
    }

    constexpr auto observed(Weak_Ptr) const
    {
        return true;
    }

    constexpr auto visiting(Weak_Ptr observer) const
    {
        return (observer == this ? nullptr : observer);
    }

    template <typename T>
    constexpr auto static_pointer_cast(Weak_Ptr observer) const
    {
        return (T*)observer;
    }

    constexpr void before_disconnect_all() const
    {

    }
};

//------------------------------------------------------------------------------

template <typename Mutex = Spin_Mutex>
class TS_Policy_Safe
{
    using Shared_Ptr = std::shared_ptr<TS_Policy_Safe>;

    Shared_Ptr tracker { this, [](...){} };
    mutable Mutex mutex;

    public:

    template <typename T, typename L>
    inline T copy_or_ref(T const& param, L&& lock) const
    {
        std::unique_lock<TS_Policy_Safe> unlock_after_copy = std::move(lock);
        return param;
    }

    inline auto lock_guard() const
    {
        return std::unique_lock<TS_Policy_Safe>(*const_cast<TS_Policy_Safe*>(this));
    }

    inline void lock() const
    {
        mutex.lock();
    }

    inline void unlock() noexcept
    {
        mutex.unlock();
    }

    protected:

    using Weak_Ptr = std::weak_ptr<TS_Policy_Safe>;

    inline Weak_Ptr weak_ptr()
    {
        return tracker;
    }

    inline Shared_Ptr observed(Weak_Ptr observer)
    {
        return observer.lock();
    }

    inline Shared_Ptr visiting(Weak_Ptr observer)
    {
        // If this tracker isn't this observer then lock
        return observer.owner_before(tracker)
            || tracker.owner_before(observer) ? observer.lock() : nullptr;
    }

    template <typename T>
    inline auto static_pointer_cast(Shared_Ptr& observer)
    {
        return std::static_pointer_cast<T>(observer);
    }

    inline void before_disconnect_all()
    {
        Weak_Ptr ping { tracker };
        // Reset the tracker and then ping for any lingering references
        tracker.reset();
        // Wait for all shared pointers to release
        while (ping.lock())
        {
            std::this_thread::yield();
        }
    }
};

} // namespace Nano ------------------------------------------------------------

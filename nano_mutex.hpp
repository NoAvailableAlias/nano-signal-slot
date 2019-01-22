#pragma once

#include <cassert>
#include <atomic>
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

    constexpr bool get_lock_guard() const
    {
        return false;
    }

    constexpr void lock() const
    {

    }

    constexpr void unlock() noexcept
    {

    }
};

template <typename Mutex = Spin_Mutex>
class TS_Policy
{
    using Lock_Guard = std::lock_guard<TS_Policy>;

    mutable Mutex m_mutex;

    public:

    template <typename T, typename L>
    inline T const& copy_or_ref(T const& param, L&&) const
    {
        return param;
    }

    inline Lock_Guard get_lock_guard() const
    {
        return Lock_Guard(*const_cast<TS_Policy*>(this));
    }

    inline void lock() const
    {
        m_mutex.lock();
    }

    inline void unlock() noexcept
    {
        m_mutex.unlock();
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

    constexpr bool get_lock_guard() const
    {
        return false;
    }

    constexpr void lock() const
    {

    }

    constexpr void unlock() noexcept
    {

    }
};

template <typename Mutex = Spin_Mutex>
class TS_Policy_Safe
{
    using Lock_Guard = std::unique_lock<TS_Policy_Safe>;

    mutable Mutex m_mutex;

    public:

    template <typename T, typename L>
    inline T copy_or_ref(T const& param, L&& lock) const
    {
        Lock_Guard unlock_after_copy = std::move(lock);
        return param;
    }

    inline Lock_Guard get_lock_guard() const
    {
        return Lock_Guard(*const_cast<TS_Policy_Safe*>(this));
    }

    inline void lock() const
    {
        m_mutex.lock();
    }

    inline void unlock() noexcept
    {
        m_mutex.unlock();
    }
};

} // namespace Nano ------------------------------------------------------------

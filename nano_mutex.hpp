#pragma once

#include <cassert>
#include <atomic>
#include <mutex>
#include <thread>

namespace Nano
{

class Spin_Mutex
{
    std::atomic<std::thread::id> owner_thread_id = std::thread::id();
    std::atomic_flag lock_flag = ATOMIC_FLAG_INIT;

    public:

    bool try_lock()
    {
        if (!lock_flag.test_and_set(std::memory_order_acquire))
        {
            owner_thread_id.store(std::this_thread::get_id(), std::memory_order_release);
        }
        else if (owner_thread_id.load(std::memory_order_acquire) != std::this_thread::get_id())
        {
            return false;
        }
        return true;
    }

    void lock()
    {
        while (!try_lock())
        {
            std::this_thread::yield();
        }
    }

    void unlock()
    {
        assert(owner_thread_id.load(std::memory_order_acquire) == std::this_thread::get_id());
        owner_thread_id.store(std::thread::id(), std::memory_order_release);
        lock_flag.clear(std::memory_order_release);
    }
};

//------------------------------------------------------------------------------

class Recursive_Spin_Mutex
{
    std::atomic<std::thread::id> owner_thread_id = std::thread::id();
    std::uint64_t recursive_counter = 0;
    std::atomic_flag lock_flag = ATOMIC_FLAG_INIT;

    public:

    bool try_lock()
    {
        if (!lock_flag.test_and_set(std::memory_order_acquire))
        {
            owner_thread_id.store(std::this_thread::get_id(), std::memory_order_release);
        }
        else if (owner_thread_id.load(std::memory_order_acquire) != std::this_thread::get_id())
        {
            return false;
        }
        ++recursive_counter;
        return true;
    }

    void lock()
    {
        while (!try_lock())
        {
            std::this_thread::yield();
        }
    }

    void unlock()
    {
        assert(owner_thread_id.load(std::memory_order_acquire) == std::this_thread::get_id());
        assert(recursive_counter > 0);

        if (--recursive_counter == 0)
        {
            owner_thread_id.store(std::thread::id(), std::memory_order_release);
            lock_flag.clear(std::memory_order_release);
        }
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

    constexpr bool try_lock() const
    {
        return true;
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

    inline bool try_lock() const
    {
        return m_mutex.try_lock();
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

class ST_Policy_Strict
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

    constexpr bool try_lock() const
    {
        return true;
    }

    constexpr void lock() const
    {

    }

    constexpr void unlock() noexcept
    {

    }
};

template <typename Mutex = Recursive_Spin_Mutex>
class TS_Policy_Strict
{
    using Lock_Guard = std::unique_lock<TS_Policy_Strict>;

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
        return Lock_Guard(*const_cast<TS_Policy_Strict*>(this));
    }

    inline bool try_lock() const
    {
        return m_mutex.try_lock();
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

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

    template <typename T>
    static inline T const& copy_or_ref(T const& param)
    {
        return param;
    }

    constexpr void lock() const
    {

    }

    constexpr bool try_lock() const
    {
        return true;
    }

    constexpr void unlock() noexcept
    {

    }

    constexpr bool get_lock_guard() const
    {
        return false;
    }
};

template <typename Mutex = Spin_Mutex>
class TS_Policy
{
    mutable Mutex m_mutex;

    public:

    template <typename T>
    static inline T const& copy_or_ref(T const& param)
    {
        return param;
    }

    inline void lock() const
    {
        m_mutex.lock();
    }

    inline bool try_lock() const
    {
        return m_mutex.try_lock();
    }

    inline void unlock() noexcept
    {
        m_mutex.unlock();
    }

    inline std::lock_guard<TS_Policy> get_lock_guard() const
    {
        return std::lock_guard<TS_Policy>(*const_cast<TS_Policy*>(this));
    }
};

//------------------------------------------------------------------------------

class ST_Policy_Strict
{
    public:

    template <typename T>
    static inline T const& copy_or_ref(T const& param)
    {
        return param;
    }

    constexpr void lock() const
    {

    }

    constexpr bool try_lock() const
    {
        return true;
    }

    constexpr void unlock() noexcept
    {

    }

    constexpr bool get_lock_guard() const
    {
        return false;
    }
};

template <typename Mutex = Recursive_Spin_Mutex>
class TS_Policy_Strict
{
    mutable Mutex m_mutex;

    public:

    template <typename T>
    static inline T const& copy_or_ref(T const& param)
    {
        return param;
    }

    inline void lock() const
    {
        m_mutex.lock();
    }

    inline bool try_lock() const
    {
        return m_mutex.try_lock();
    }

    inline void unlock() noexcept
    {
        m_mutex.unlock();
    }

    inline std::lock_guard<TS_Policy_Strict> get_lock_guard() const
    {
        return std::lock_guard<TS_Policy_Strict>(*const_cast<TS_Policy_Strict*>(this));
    }
};

} // namespace Nano ------------------------------------------------------------

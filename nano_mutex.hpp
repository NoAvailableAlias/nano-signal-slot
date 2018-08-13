#pragma once

#include <atomic>
#include <thread>
#include <cassert>

namespace Nano
{

// When using a sane optimization level,
// the use of this class with std::shared_lock
// or std::unique_lock will be completely optimized away.

class Noop_Mutex
{
    public:

    constexpr bool lock() const
    {
        return true;
    }

    constexpr bool try_lock() const
    {
        return true;
    }

    constexpr void unlock() noexcept
    {

    }

    constexpr void lock_shared()
    {

    }

    constexpr bool try_lock_shared() const
    {
        return true;
    }

    constexpr void unlock_shared()
    {

    }
};

//------------------------------------------------------------------------------

class Recursive_Mutex
{
    std::atomic<std::thread::id> owner_thread_id = std::thread::id();
    std::uint32_t recursive_counter = 0;
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

} // namespace Nano ------------------------------------------------------------

#ifndef NANO_NOOP_MUTEX_HPP
#define NANO_NOOP_MUTEX_HPP

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

} // namespace Nano ------------------------------------------------------------

#endif // NANO_NOOP_MUTEX_HPP

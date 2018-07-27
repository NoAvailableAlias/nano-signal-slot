#ifndef NANO_FUNCTION_HPP
#define NANO_FUNCTION_HPP

#include <cstdint>
#include <array>

namespace Nano
{

using DelegateKey = std::array<const std::uintptr_t, 2>;

template <typename RT> class Function;
template <typename RT, typename... Args>
class Function<RT(Args...)>
{
    template <typename> friend class Observer;

    using Thunk = RT(*)(void*, Args...);

    void* m_this_ptr; // instance pointer
    Thunk m_stub_ptr; // free function pointer

    Function(void* this_ptr, Thunk stub_ptr) :
        m_this_ptr(this_ptr), m_stub_ptr(stub_ptr)
    {

    }

    static inline Function bind(DelegateKey const& delegate_key)
    {
        return
        {
            reinterpret_cast<void*>(delegate_key[0]),
            reinterpret_cast<Thunk>(delegate_key[1])
        };
    }

    public:

    template <auto fun_ptr>
    static inline Function bind()
    {
        return
        {
            nullptr, [](void* /*NULL*/, Args... args)
            {
                return (*fun_ptr)(std::forward<Args>(args)...);
            }
        };
    }

    template <auto mem_ptr, typename T>
    static inline Function bind(T* pointer)
    {
        return
        {
            pointer, [](void* this_ptr, Args... args)
            {
                return (static_cast<T*>(this_ptr)->*mem_ptr) (std::forward<Args>(args)...);
            }
        };
    }

    template <typename L>
    static inline Function bind(L* pointer)
    {
        return
        {
            pointer, [](void *this_ptr, Args... args)
            {
                return (static_cast<L*>(this_ptr)->operator()(std::forward<Args>(args)...));
            }
        };
    }

    template <typename... Uref>
    inline RT operator() (Uref&&... args)
    {
        return (*m_stub_ptr)(m_this_ptr, std::forward<Uref>(args)...);
    }

    inline operator DelegateKey() const
    {
        return
        {
            reinterpret_cast<std::uintptr_t>(m_this_ptr),
            reinterpret_cast<std::uintptr_t>(m_stub_ptr)
        };
    }
};

} // namespace Nano ------------------------------------------------------------

#endif // NANO_FUNCTION_HPP

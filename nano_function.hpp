#ifndef NANO_FUNCTION_HPP
#define NANO_FUNCTION_HPP

#include <cstdint>
#include <array>

namespace Nano
{

using DelegateKey = std::array<std::uintptr_t, 2>;

template <typename T_rv> class Function;
template <typename T_rv, typename... Args>
class Function<T_rv(Args...)>
{

//-----------------------------------------------------------------------PRIVATE

    template <typename T> friend class Signal;

    typedef T_rv (*sig_t)(void*, Args...);

    void* m_this_ptr; // instance pointer
    sig_t m_stub_ptr; // free function pointer

    template <typename T, typename F>
    Function (T&& this_ptr, F&& stub_ptr):
        m_this_ptr { std::forward<T>(this_ptr) },
        m_stub_ptr { std::forward<F>(stub_ptr) } {}

    Function (DelegateKey const& _k):
        m_this_ptr { reinterpret_cast<void*>(std::get<0>(_k)) },
        m_stub_ptr { reinterpret_cast<sig_t>(std::get<1>(_k)) } {}

    public:

//--------------------------------------------------------------------------BIND

    template <T_rv (*fun_ptr)(Args...)>
    static inline Function bind()
    {
        return { nullptr, [](void* /*NULL*/, Args... args) {
        return (*fun_ptr)(std::forward<Args>(args)...); }};
    }
    template <typename T, T_rv (T::*mem_ptr)(Args...)>
    static inline Function bind(T* pointer)
    {
        return { pointer, [](void* this_ptr, Args... args) {
        return (static_cast<T*>(this_ptr)->*mem_ptr)
            (std::forward<Args>(args)...); }};
    }
    template <typename T, T_rv (T::*mem_ptr)(Args...) const>
    static inline Function bind(T* pointer)
    {
        return { pointer, [](void* this_ptr, Args... args) {
        return (static_cast<T*>(this_ptr)->*mem_ptr)
            (std::forward<Args>(args)...); }};
    }

//--------------------------------------------------------------INSTANCE METHODS

    inline T_rv operator() (Args... args)
    {
        return (*m_stub_ptr)(m_this_ptr, std::forward<Args>(args)...);
    }
    operator DelegateKey() const
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

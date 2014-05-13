#ifndef NANO_FUNCTION_HPP
#define NANO_FUNCTION_HPP

#include <cstdint>
#include <utility>

namespace Nano
{

typedef std::pair<std::uintptr_t, std::uintptr_t> delegate_key_t;

template <typename T_rv> class function;
template <typename T_rv, typename... Args> class function<T_rv(Args...)>
{
    typedef T_rv (*sig_t)(void*, Args...);

    void* m_this_ptr;
    sig_t m_stub_ptr;

    template <typename I, typename F>
    function (I&& this_ptr, F&& stub_ptr):
        m_this_ptr { std::forward<I>(this_ptr) },
        m_stub_ptr { std::forward<F>(stub_ptr) } {}

    template <typename T> friend class Signal;

    function (delegate_key_t const& _key):
        m_this_ptr { reinterpret_cast<void*>(std::get<0>(_key)) },
        m_stub_ptr { reinterpret_cast<sig_t>(std::get<1>(_key)) } {}

    public:

//------------------------------------------------------------------------------

    template <T_rv (*fun_ptr)(Args...)>
    static inline function bind()
    {
        return { nullptr, [](void* , Args... args) {
        return (*fun_ptr)(std::forward<Args>(args)...); }};
    }
    template <typename T, T_rv (T::*mem_ptr)(Args...)>
    static inline function bind(T* pointer)
    {
        return { pointer, [](void* this_ptr, Args... args) {
        return (static_cast<T*>(this_ptr)->*mem_ptr)
            (std::forward<Args>(args)...); }};
    }
    template <typename T, T_rv (T::*mem_ptr)(Args...) const>
    static inline function bind(T* pointer)
    {
        return { pointer, [](void* this_ptr, Args... args) {
        return (static_cast<T*>(this_ptr)->*mem_ptr)
            (std::forward<Args>(args)...); }};
    }

//------------------------------------------------------------------------------

    T_rv operator()(Args&&... args) const
    {
        return (*m_stub_ptr)(m_this_ptr, std::forward<Args>(args)...);
    }
    bool operator== (delegate_key_t const& other) const
    {
        return other == this->operator delegate_key_t();
    }
    bool operator!= (delegate_key_t const& other) const
    {
        return other != this->operator delegate_key_t();
    }
    operator delegate_key_t() const
    {
        return { reinterpret_cast<std::uintptr_t>(m_this_ptr),
                 reinterpret_cast<std::uintptr_t>(m_stub_ptr) };
    }
};

} // namespace Nano ------------------------------------------------------------

namespace std // std::hash specialization
{

template <> struct hash<Nano::delegate_key_t>
{
    inline std::size_t operator()(Nano::delegate_key_t const& key) const
    {
        return std::get<0>(key) ^ std::get<1>(key);
    }
};

} // namespace std -------------------------------------------------------------

#endif // NANO_FUNCTION_HPP_2_514
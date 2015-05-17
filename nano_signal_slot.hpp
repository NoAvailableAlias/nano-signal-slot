#ifndef NANO_SIGNAL_SLOT_HPP
#define NANO_SIGNAL_SLOT_HPP

#include "nano_function.hpp"
#include "nano_observer.hpp"

namespace Nano
{

template <typename RT> class Signal;
template <typename RT, typename... Args>
class Signal<RT(Args...)> : private Observer
{
    template <typename T>
    void insert_sfinae(DelegateKey const& key, typename T::Observer* instance)
    {
        Observer::insert(key, instance);
        instance->insert(key, this);
    }
    template <typename T>
    void remove_sfinae(DelegateKey const& key, typename T::Observer* instance)
    {
        Observer::remove(key);
        instance->remove(key);
    }
    template <typename T>
    void insert_sfinae(DelegateKey const& key, ...)
    {
        Observer::insert(key);
    }
    template <typename T>
    void remove_sfinae(DelegateKey const& key, ...)
    {
        Observer::remove(key);
    }

    public:

    using Delegate = Function<RT(Args...)>;
    
    //-------------------------------------------------------------------CONNECT

    template <RT (*fun_ptr)(Args...)>
    void connect()
    {
        Observer::insert(Delegate::template bind<fun_ptr>());
    }

    template <typename T, RT (T::*mem_ptr)(Args...)>
    void connect(T* instance)
    {
        insert_sfinae<T>(Delegate::template bind<T, mem_ptr>(instance), instance);
    }
    template <typename T, RT (T::*mem_ptr)(Args...) const>
    void connect(T* instance)
    {
        insert_sfinae<T>(Delegate::template bind<T, mem_ptr>(instance), instance);
    }

    template <typename T, RT (T::*mem_ptr)(Args...)>
    void connect(T& instance)
    {
        connect<T, mem_ptr>(std::addressof(instance));
    }
    template <typename T, RT (T::*mem_ptr)(Args...) const>
    void connect(T& instance)
    {
        connect<T, mem_ptr>(std::addressof(instance));
    }
    
    //----------------------------------------------------------------DISCONNECT

    template <RT (*fun_ptr)(Args...)>
    void disconnect()
    {
        Observer::remove(Delegate::template bind<fun_ptr>());
    }
    
    template <typename T, RT (T::*mem_ptr)(Args...)>
    void disconnect(T* instance)
    {
        remove_sfinae<T>(Delegate::template bind<T, mem_ptr>(instance), instance);
    }
    template <typename T, RT (T::*mem_ptr)(Args...) const>
    void disconnect(T* instance)
    {
        remove_sfinae<T>(Delegate::template bind<T, mem_ptr>(instance), instance);
    }

    template <typename T, RT (T::*mem_ptr)(Args...)>
    void disconnect(T& instance)
    {
        disconnect<T, mem_ptr>(std::addressof(instance));
    }
    template <typename T, RT (T::*mem_ptr)(Args...) const>
    void disconnect(T& instance)
    {
        disconnect<T, mem_ptr>(std::addressof(instance));
    }
    
    //----------------------------------------------------------------------EMIT

	template <typename... Uref>
    void emit(Uref&&... args)
    {
        Observer::onEach<Delegate>(std::forward<Uref>(args)...);
    }
/*
    template <typename Accumulate, typename... Uref>
    void emit_accumulate(Accumulate&& accumulator, Uref&&... args)
    {
        Observer::onEach<Delegate, Accumulate>
            (std::forward<Accumulate>(accumulator), std::forward<Uref>(args)...);
    }
*/
};

} // namespace Nano ------------------------------------------------------------

#endif // NANO_SIGNAL_SLOT_HPP

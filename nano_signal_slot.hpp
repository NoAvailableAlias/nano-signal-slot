#ifndef NANO_SIGNAL_SLOT_HPP
#define NANO_SIGNAL_SLOT_HPP

#include "nano_function.hpp"
#include "nano_observer.hpp"

namespace Nano
{

template <typename T_rv> class Signal;
template <typename T_rv, typename... Args>
class Signal<T_rv(Args...)> : private Observer
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

    using Delegate = Function<T_rv(Args...)>;
    
    //-------------------------------------------------------------------CONNECT

    template <T_rv (*fun_ptr)(Args...)>
    void connect()
    {
        Observer::insert(Delegate::template bind<fun_ptr>());
    }

    template <typename T, T_rv (T::*mem_ptr)(Args...)>
    void connect(T* instance)
    {
        auto delegate  = Delegate::template bind<T, mem_ptr>(instance);
        insert_sfinae<T>(delegate, instance);
    }
    template <typename T, T_rv (T::*mem_ptr)(Args...) const>
    void connect(T* instance)
    {
        auto delegate  = Delegate::template bind<T, mem_ptr>(instance);
        insert_sfinae<T>(delegate, instance);
    }
    
    template <typename T, T_rv (T::*mem_ptr)(Args...)>
    void connect(T& instance)
    {
        connect<T, mem_ptr>(std::addressof(instance));
    }
    template <typename T, T_rv (T::*mem_ptr)(Args...) const>
    void connect(T& instance)
    {
        connect<T, mem_ptr>(std::addressof(instance));
    }
    
    //----------------------------------------------------------------DISCONNECT

    template <T_rv (*fun_ptr)(Args...)>
    void disconnect()
    {
        Observer::remove(Delegate::template bind<fun_ptr>());
    }

    template <typename T, T_rv (T::*mem_ptr)(Args...)>
    void disconnect(T* instance)
    {
        auto delegate  = Delegate::template bind<T, mem_ptr>(instance);
        remove_sfinae<T>(delegate, instance);
    }
    template <typename T, T_rv (T::*mem_ptr)(Args...) const>
    void disconnect(T* instance)
    {
        auto delegate  = Delegate::template bind<T, mem_ptr>(instance);
        remove_sfinae<T>(delegate, instance);
    }
    
    template <typename T, T_rv (T::*mem_ptr)(Args...)>
    void disconnect(T& instance)
    {
        disconnect<T, mem_ptr>(std::addressof(instance));
    }
    template <typename T, T_rv (T::*mem_ptr)(Args...) const>
    void disconnect(T& instance)
    {
        disconnect<T, mem_ptr>(std::addressof(instance));
    }
    
    //----------------------------------------------------------------------EMIT

    void operator() (Args... args)
    {
        for (auto const& slot : tracked_connections)
        {
            Delegate(std::get<0>(slot))(std::forward<Args>(args)...);
        }
    }
    template <typename Accumulator>
    void operator() (Args... args, Accumulator sink)
    {
        for (auto const& slot : tracked_connections)
        {
            sink(Delegate(std::get<0>(slot))(std::forward<Args>(args)...));
        }
    }
};

} // namespace Nano ------------------------------------------------------------

#endif // NANO_SIGNAL_SLOT_HPP

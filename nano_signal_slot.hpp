#pragma once

#include "nano_function.hpp"
#include "nano_observer.hpp"

namespace Nano
{

template <typename RT, typename MT_Policy = ST_Policy>
class Signal;
template <typename RT, typename MT_Policy, typename... Args>
class Signal<RT(Args...), MT_Policy> final : public Observer<MT_Policy>
{
    using Observer = Observer<MT_Policy>;
    using Function = Function<RT(Args...)>;

    template <typename T>
    void insert_sfinae(Delegate_Key const& key, typename T::Observer* instance)
    {
        Observer::insert(key, instance);
        instance->insert(key, this);
    }
    template <typename T>
    void remove_sfinae(Delegate_Key const& key, typename T::Observer* instance)
    {
        Observer::remove(key);
        instance->remove(key);
    }
    template <typename T>
    void insert_sfinae(Delegate_Key const& key, ...)
    {
        Observer::insert(key, this);
    }
    template <typename T>
    void remove_sfinae(Delegate_Key const& key, ...)
    {
        Observer::remove(key);
    }

    public:
 
    //-------------------------------------------------------------------CONNECT

    template <typename L>
    void connect(L* instance)
    {
        Observer::insert(Function::template bind(instance), this);
    }
    template <typename L>
    void connect(L& instance)
    {
        connect(std::addressof(instance));
    }

    template <RT(*fun_ptr)(Args...)>
    void connect()
    {
        Observer::insert(Function::template bind<fun_ptr>(), this);
    }

    template <typename T, RT(T::*mem_ptr)(Args...)>
    void connect(T* instance)
    {
        insert_sfinae<T>(Function::template bind<mem_ptr>(instance), instance);
    }
    template <typename T, RT(T::*mem_ptr)(Args...) const>
    void connect(T* instance)
    {
        insert_sfinae<T>(Function::template bind<mem_ptr>(instance), instance);
    }

    template <typename T, RT(T::*mem_ptr)(Args...)>
    void connect(T& instance)
    {
        connect<mem_ptr, T>(std::addressof(instance));
    }
    template <typename T, RT(T::*mem_ptr)(Args...) const>
    void connect(T& instance)
    {
        connect<mem_ptr, T>(std::addressof(instance));
    }

    template <auto mem_ptr, typename T>
    void connect(T* instance)
    {
        insert_sfinae<T>(Function::template bind<mem_ptr>(instance), instance);
    }
    template <auto mem_ptr, typename T>
    void connect(T& instance)
    {
        connect<mem_ptr, T>(std::addressof(instance));
    }

    //----------------------------------------------------------------DISCONNECT

    template <typename L>
    void disconnect(L* instance)
    {
        Observer::remove(Function::template bind(instance));
    }
    template <typename L>
    void disconnect(L& instance)
    {
        disconnect(std::addressof(instance));
    }

    template <RT(*fun_ptr)(Args...)>
    void disconnect()
    {
        Observer::remove(Function::template bind<fun_ptr>());
    }

    template <typename T, RT(T::*mem_ptr)(Args...)>
    void disconnect(T* instance)
    {
        remove_sfinae<T>(Function::template bind<mem_ptr>(instance), instance);
    }
    template <typename T, RT(T::*mem_ptr)(Args...) const>
    void disconnect(T* instance)
    {
        remove_sfinae<T>(Function::template bind<mem_ptr>(instance), instance);
    }

    template <typename T, RT(T::*mem_ptr)(Args...)>
    void disconnect(T& instance)
    {
        disconnect<T, mem_ptr>(std::addressof(instance));
    }
    template <typename T, RT(T::*mem_ptr)(Args...) const>
    void disconnect(T& instance)
    {
        disconnect<T, mem_ptr>(std::addressof(instance));
    }

    template <auto mem_ptr, typename T>
    void disconnect(T* instance)
    {
        remove_sfinae<T>(Function::template bind<mem_ptr>(instance), instance);
    }
    template <auto mem_ptr, typename T>
    void disconnect(T& instance)
    {
        disconnect<mem_ptr, T>(std::addressof(instance));
    }

    //----------------------------------------------------FIRE / FIRE ACCUMULATE

    template <typename... Uref>
    void fire(Uref&&... args) const
    {
        Observer::template for_each<Function>(std::forward<Uref>(args)...);
    }

    template <typename Accumulate, typename... Uref>
    void fire_accumulate(Accumulate&& accumulate, Uref&&... args) const
    {
        Observer::template for_each_accumulate<Function, Accumulate>
            (std::forward<Accumulate>(accumulate), std::forward<Uref>(args)...);
    }
};

} // namespace Nano ------------------------------------------------------------

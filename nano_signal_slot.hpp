#ifndef NANO_SIGNAL_SLOT_HPP
#define NANO_SIGNAL_SLOT_HPP

#include "nano_function.hpp"

#include <unordered_map>

namespace Nano
{

class Observer
{
    std::unordered_map<delegate_key_t, Observer*> tracked_connections;

    template <typename T> friend class Signal;

    void insert(delegate_key_t const& key, Observer* instance)
    {
        tracked_connections.emplace(key, instance);
    }
    void remove(delegate_key_t const& key)
    {
        tracked_connections.erase(key);
    }

    protected:

   ~Observer()
    {
        for (auto const& connection : tracked_connections)
        {
            std::get<1> (connection)->remove(std::get<0>(connection));
        }
    }
};

//------------------------------------------------------------------------------

template <typename T_rv> class Signal;
template <typename T_rv, typename... Args>
class Signal <T_rv(Args...)> : public Observer
{
    template <typename T>
    void sfinae_con(delegate_key_t const& key, typename T::Observer* instance)
    {
        Observer::insert(key, instance);
        instance->insert(key, this);
    }
    template <typename T>
    void sfinae_dis(delegate_key_t const& key, typename T::Observer* instance)
    {
        Observer::remove(key);
        instance->remove(key);
    }
    template <typename T> void sfinae_con(delegate_key_t const& key, ...)
    {
        Observer::insert(key, this);
    }
    template <typename T> void sfinae_dis(delegate_key_t const& key, ...)
    {
        Observer::remove(key);
    }

    using function_t  = Function<T_rv(Args...)>;

    public: //------------------------------------------------------------------

    template <T_rv (*fun_ptr)(Args...)>
    void connect()
    {
        auto delegate = function_t::template bind<fun_ptr>();
        Observer::insert(delegate, this);
    }
    template <typename T, T_rv (T::*mem_ptr)(Args...)>
    void connect(T* instance)
    {
        auto delegate = function_t::template bind<T, mem_ptr>(instance);
        sfinae_con<T>(delegate, instance);
    }
    template <typename T, T_rv (T::*mem_ptr)(Args...) const>
    void connect(T* instance)
    {
        auto delegate = function_t::template bind<T, mem_ptr>(instance);
        sfinae_con<T>(delegate, instance);
    }

//------------------------------------------------------------------------------

    template <typename T, T_rv (T::*mem_ptr)(Args...)>
    void connect(T& instance)
    {
        auto delegate = function_t::template
            bind<T, mem_ptr>(std::addressof(instance));
        sfinae_con<T>(delegate, std::addressof(instance));
    }
    template <typename T, T_rv (T::*mem_ptr)(Args...) const>
    void connect(T& instance)
    {
        auto delegate = function_t::template
            bind<T, mem_ptr>(std::addressof(instance));
        sfinae_con<T>(delegate, std::addressof(instance));
    }

//------------------------------------------------------------------------------

    template <T_rv (*fun_ptr)(Args...)>
    void disconnect()
    {
        auto delegate = function_t::template bind<fun_ptr>();
        Observer::remove(delegate);
    }
    template <typename T, T_rv (T::*mem_ptr)(Args...)>
    void disconnect(T* instance)
    {
        auto delegate = function_t::template bind<T, mem_ptr>(instance);
        sfinae_dis<T>(delegate, instance);
    }
    template <typename T, T_rv (T::*mem_ptr)(Args...) const>
    void disconnect(T* instance)
    {
        auto delegate = function_t::template bind<T, mem_ptr>(instance);
        sfinae_dis<T>(delegate, instance);
    }

//------------------------------------------------------------------------------

    template <typename T, T_rv (T::*mem_ptr)(Args...)>
    void disconnect(T& instance)
    {
        auto delegate = function_t::template
            bind<T, mem_ptr>(std::addressof(instance));
        sfinae_dis<T>(delegate, std::addressof(instance));
    }
    template <typename T, T_rv (T::*mem_ptr)(Args...) const>
    void disconnect(T& instance)
    {
        auto delegate = function_t::template
            bind<T, mem_ptr>(std::addressof(instance));
        sfinae_dis<T>(delegate, std::addressof(instance));
    }

//------------------------------------------------------------------------------

    void operator()(Args&&... args)
    {
        for (auto const& slot : tracked_connections)
        {
            function_t (std::get<0>(slot))(std::forward<Args>(args)...);
        }
    }
    template <typename Accumulator>
    void accumulate(Args&&... args, Accumulator&& lambda)
    {
        for (auto const& slot : tracked_connections)
        {
            lambda(function_t (std::get<0>(slot))(std::forward<Args>(args)...));
        }
    }

};

} // namespace Nano ------------------------------------------------------------

#endif // NANO_SIGNAL_SLOT_HPP_2_514

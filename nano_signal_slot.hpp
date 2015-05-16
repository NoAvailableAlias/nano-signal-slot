#ifndef NANO_SIGNAL_SLOT_HPP
#define NANO_SIGNAL_SLOT_HPP

//#include "nano_function.hpp"
//#include "nano_observer.hpp"

#include <forward_list>
#include <cstdint>
//#include <memory>
//#include <atomic>
#include <array>

// EXPERIMENTAL BRANCH

namespace Nano
{

//------------------------------------------------------POOL ALLOCATOR INTERFACE

template <typename T> class Singleton
{
    protected:

    Singleton() = default;
    Singleton(Singleton const&) = default;
    Singleton& operator= (Singleton const&) = default;

    public:

    static T& get()
    {
        static T s;
        return s;
    }
};

// DO NOT USE, HASTILY THROWN TOGETHER FOR LEAST OVERHEAD
// HAS A FIXED SIZE AND NO PROTECTIONS OTHER THAN BEING MINIMAL

template <typename T, std::size_t N = 256> class Allocator
{
    class Free_List : public Singleton<Free_List>
    {
        struct Node { Node* next = nullptr; };
        
        typename std::aligned_union<0, T, Node>::type m_data[N];
        
        Node* m_head = nullptr;
        
        public:
        
        Free_List()
        {
            for (auto& node : m_data)
            {
                push(std::addressof(node));
            }
        }
        void* next()
        {
			Node* node = m_head;
            m_head = node->next;
            return reinterpret_cast<void*>(node);
        }
        void push(void* temp)
        {
            Node* node = reinterpret_cast<Node*>(temp);
            node->next = m_head;
            m_head = node;
        }
    };

    public:

    typedef T value_type;
    typedef T* pointer;
    typedef T const* const_pointer;
    typedef T& reference;
    typedef T const& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    Allocator () throw() {}
   ~Allocator () throw() {}
    Allocator (Allocator const&) throw() {}
    template<typename U>
    Allocator (Allocator<U> const&) throw() {}

    Allocator& operator= (Allocator const&)
    {
        return *this;
    }
    template<typename U>
    Allocator& operator= (Allocator<U> const&)
    {
        return *this;
    }
    template <typename U> struct rebind
    {
        typedef Allocator<U> other;
    };
    pointer address(reference ref)
    {
        return std::addressof(ref);
    }
    const_pointer address(const_reference ref)
    {
        return std::addressof(ref);
    }
    pointer allocate(size_type n) const
    {
        return static_cast<pointer>(Free_List::get().next());
    }
    pointer allocate(size_type n, const void*) const
    {
        return static_cast<pointer>(Free_List::get().next());
    }
    void deallocate(pointer ptr, size_type)
    {
        Free_List::get().push(ptr);
    }
    template <typename U, typename... Args>
    void construct(U * ptr, Args&&... args)
    {
        ::new(ptr) U(std::forward<Args>(args)...);
    }
    void destroy(pointer ptr)
    {
        ptr->~value_type();
    }
    template <typename U>
    void destroy(U* ptr)
    {
        ptr->~U();
    }
    size_type max_size() const
    {
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }
};

template <typename L, typename R>
inline bool operator== (Allocator<L> const&, Allocator<R> const&)
{
    return reinterpret_cast<std::uintptr_t>(Allocator<L>::Free_List::get()) ==
           reinterpret_cast<std::uintptr_t>(Allocator<R>::Free_List::get());
}
template <typename L, typename R>
inline bool operator!= (Allocator<L> const& lhs, Allocator<R> const& rhs)
{
    return !(lhs == rhs);
}

//------------------------------------------------------------FUNCTION INTERFACE

using DelegateKey = std::array<std::uintptr_t, 2>;

template <typename RT> class Function;
template <typename RT, typename... Args>
class Function<RT(Args...)>
{
    using Thunk = RT (*)(void*, Args...);

    template <typename T> friend class Signal;

    void* m_this_ptr; // instance pointer
    Thunk m_stub_ptr; // free function pointer

    Function(void* this_ptr, Thunk stub_ptr) :
        m_this_ptr(this_ptr), m_stub_ptr(stub_ptr) {}

    Function(DelegateKey delegate_key) :
        m_this_ptr(reinterpret_cast<void*>(delegate_key[0])),
        m_stub_ptr(reinterpret_cast<Thunk>(delegate_key[1])) {}

    public:

    template <RT (*fun_ptr)(Args...)>
    static inline Function bind()
    {
        return { nullptr, [](void* /*NULL*/, Args... urefs) { return (*fun_ptr)(std::forward<Args>(urefs)...); } };
    }
    template <typename T, RT (T::*mem_ptr)(Args...)>
    static inline Function bind(T* pointer)
    {
        return { pointer, [](void* this_ptr, Args... urefs) { return (static_cast<T*>(this_ptr)->*mem_ptr)(std::forward<Args>(urefs)...); } };
    }
    template <typename T, RT (T::*mem_ptr)(Args...) const>
    static inline Function bind(T* pointer)
    {
        return { pointer, [](void* this_ptr, Args... urefs) { return (static_cast<T*>(this_ptr)->*mem_ptr)(std::forward<Args>(urefs)...); } };
    }
    inline operator DelegateKey() const
    {
        return { reinterpret_cast<std::uintptr_t>(m_this_ptr), reinterpret_cast<std::uintptr_t>(m_stub_ptr) };
    }
    template <typename... Uref>
    inline RT operator() (Uref&&... urefs)
    {
        return (*m_stub_ptr)(m_this_ptr, std::forward<Uref>(urefs)...);
    }
};

//------------------------------------------------------------OBSERVER INTERFACE

class Observer
{
    template <typename T> friend class Signal;

    struct DelegateKeyObserver
    {
        DelegateKeyObserver(DelegateKey k, Observer* o): delegate_key(k), observer(o) {}
        DelegateKey delegate_key;
        Observer* observer;
    };

    std::forward_list<DelegateKeyObserver, Allocator<DelegateKeyObserver>> connections;

    void insert(DelegateKey const& key, Observer* observer)
    {
        connections.emplace_front(key, observer);
    }
    void insert(DelegateKey const& key)
    {
        connections.emplace_front(key, this);
    }
    void remove(DelegateKey const& key)
    {
        connections.remove_if([&key](DelegateKeyObserver const& slot) { return key == slot.delegate_key; });
    }

    protected:

    ~Observer()
    {
        for (auto const& slot : connections) { slot.observer->remove(slot.delegate_key); }
    }
};

//--------------------------------------------------------------SIGNAL INTERFACE

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
    
    //-------------------------------------------------NON-TYPE TEMPLATE CONNECT

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
    
    //----------------------------------------------------TYPE DEDUCTION CONNECT

    /*void connect(RT (*fun_ptr)(Args...))
    {
        connect<fun_ptr>();
    }

    template <typename X, typename Y>
    void connect(Y* instance, RT (X::*mem_ptr)(Args...))
    {
        connect<Y, mem_ptr>(instance);
    }
    template <typename X, typename Y>
    void connect(Y* instance, RT (X::*mem_ptr)(Args...) const)
    {
        connect<Y, mem_ptr>(instance);
    }

    template <typename X, typename Y>
    void connect(Y& instance, RT (X::*mem_ptr)(Args...))
    {
        connect<Y, mem_ptr>(std::addressof(instance));
    }
    template <typename X, typename Y>
    void connect(Y& instance, RT (X::*mem_ptr)(Args...) const)
    {
        connect<Y, mem_ptr>(std::addressof(instance));
    }*/
    
    //----------------------------------------------NON-TYPE TEMPLATE DISCONNECT

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

    //-------------------------------------------------TYPE DEDUCTION DISCONNECT

    /*void disconnect(RT (*fun_ptr)(Args...))
    {
        disconnect<fun_ptr>();
    }
    template <typename X, typename Y>
    void disconnect(Y* instance, RT (X::*mem_ptr)(Args...))
    {
        disconnect<Y, mem_ptr>(instance);
    }
    template <typename X, typename Y>
    void disconnect(Y* instance, RT (X::*mem_ptr)(Args...) const)
    {
        disconnect<Y, mem_ptr>(instance);
    }

    template <typename X, typename Y>
    void disconnect(Y& instance, RT (X::*mem_ptr)(Args...))
    {
        disconnect<Y, mem_ptr>(std::addressof(instance));
    }
    template <typename X, typename Y>
    void disconnect(Y& instance, RT (X::*mem_ptr)(Args...) const)
    {
        disconnect<Y, mem_ptr>(std::addressof(instance));
    }*/
    
    //----------------------------------------------------------------------EMIT

	template <typename... Uref>
    void operator() (Uref&&... urefs)
    {
        for (auto slot : connections)
        {
            Delegate(slot.delegate_key)(std::forward<Uref>(urefs)...);
        }
    }
    // TODO disambiguate this template
	/*template <typename FO, typename... Uref>
	void operator() (FO accumulator, Uref&&... urefs)
    {
        for (auto const& slot : connections)
        {
			accumulator(Delegate(slot.delegate_key)(std::forward<Uref>(urefs)...));
        }
    }*/
};

} // namespace Nano ------------------------------------------------------------

#endif // NANO_SIGNAL_SLOT_HPP

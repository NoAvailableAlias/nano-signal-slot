#ifndef NANO_POOL_ALLOCATOR_HPP
#define NANO_POOL_ALLOCATOR_HPP

#include <limits>
#include <atomic>
#include <memory>
#include <forward_list>

#pragma push_macro("new")
#undef new
#include <new>

namespace Nano
{

template <typename T> class Singleton
{
    protected:

    Singleton () = default;
    Singleton (Singleton const&) = default;
    Singleton & operator= (Singleton const&) = default;

    public:

    typedef T type;

    static T& instance()
    {
        static T t;
        return t;
    }
};

//------------------------------------------------------------------------------

template <
    typename T,
#ifdef NANO_DEFINE_THREADSAFE_ALLOCATOR
    typename Mutex = std::mutex,
#else
    typename Mutex = Noop_Mutex,
#endif
    size_t N = 64
>
class Pool_Allocator
{
    class Free_List : public Singleton<Free_List>
    {
        struct Node
        {
            Node* next = nullptr;
        };

        struct Storage
        {
            // Store internal Free_List pointers in a union with T
            typename std::aligned_union<N, T, Node>::type aligned_storage[N];

            Storage()
            {
                // Add the newly allocated aligned_storage to the Free_List
                for (auto& free_storage_node : aligned_storage)
                {
                    Free_List::instance().push(std::addressof(free_storage_node));
                }
            }
        };

        std::atomic<Node*> m_head;
        std::forward_list<std::unique_ptr<Storage>> m_data;

        // These are only used within the only critical section
        std::size_t m_storage_allocations = 0;
        std::size_t m_storage_factor = 1;

        public:

#ifdef NANO_DEFINE_THREADSAFE_ALLOCATOR

        void* next()
        {
            if (Node* node = m_head.load(std::memory_order_relaxed))
            {
                while (!m_head.compare_exchange_weak(node, node->next,
                    std::memory_order_release, std::memory_order_relaxed));

                return reinterpret_cast<void*>(node);
            }
            static Mutex new_storage_mutex;
            {
                std::unique_lock<Mutex> unique_lock(new_storage_mutex);

                // Prevent previously blocked threads from allocating Storage
                if (!m_head)
                {
                    for (auto i = m_storage_factor; i; --i, ++m_storage_allocations)
                    {
                        // Add the next Storage block to the Free_List
                        m_data.emplace_front(std::make_unique<Storage>());
                    }
                    // Quadratic resizing (get to the watermark as soon as possible)
                    m_storage_factor <<= (m_storage_factor >= m_storage_allocations ? 0 : 1);
                }
                return next();
            }
        }

        void push(void* temp)
        {
            Node* node = reinterpret_cast<Node*>(temp);
            node->next = m_head.load(std::memory_order_relaxed);

            while (!m_head.compare_exchange_weak(node->next, node,
                std::memory_order_release, std::memory_order_relaxed));
        }

#else

        void* next()
        {
            if (Node* node = m_head)
            {
                m_head = node->next;
                return reinterpret_cast<void*>(node);
            }
            for (auto i = m_storage_factor; i; --i, ++m_storage_allocations)
            {
                // Add the next Storage block to the Free_List
                m_data.emplace_front(std::make_unique<Storage>());
            }
            // Quadratic resizing (get to the watermark as soon as possible)
            m_storage_factor <<= (m_storage_factor >= m_storage_allocations ? 0 : 1);
            return next();
        }

        void push(void* temp)
        {
            Node* node = reinterpret_cast<Node*>(temp);
            node->next = m_head;
            m_head = node;
        }

#endif

    };

    public:

    typedef T value_type;
    typedef T* pointer;
    typedef T const* const_pointer;
    typedef T& reference;
    typedef T const& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    Pool_Allocator () throw() {}
   ~Pool_Allocator () throw() {}
    Pool_Allocator (Pool_Allocator const&) throw() {}
    template<typename U>
    Pool_Allocator (Pool_Allocator<U> const&) throw() {}

    Pool_Allocator& operator= (Pool_Allocator const&)
    {
        return *this;
    }
    
    template<typename U>
    Pool_Allocator& operator= (Pool_Allocator<U> const&)
    {
        return *this;
    }

    template <typename U> struct rebind
    {
        typedef Pool_Allocator<U> other;
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
        return allocate(n, nullptr);
    }
    
    pointer allocate(size_type n, const void*) const
    {
        if (n > 1)
        {
            throw std::runtime_error("Pool_Allocator cannot allocate an array.");
        }
        pointer ptr = static_cast<pointer>(Free_List::instance().next());

        if (ptr == nullptr)
        {
            throw std::bad_alloc();
        }
        return ptr;
    }
    
    void deallocate(pointer ptr, size_type)
    {
        Free_List::instance().push(ptr);
    }

    void construct(pointer ptr, value_type const& value)
    {
        ::new(reinterpret_cast<void*>(ptr)) value_type(value);
    }
    
    template <typename U, typename... Args>
    void construct(U * ptr, Args&&... args)
    {
        ::new(reinterpret_cast<void*>(ptr)) U(std::forward<Args>(args)...);
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

template <class T1, class T2>
bool operator== (const Pool_Allocator<T1>& lhs, const Pool_Allocator<T2>& rhs)
{
    return std::is_same<T1, T2>::value;
}

template <class T1, class T2>
bool operator!= (const Pool_Allocator<T1>& lhs, const Pool_Allocator<T2>& rhs)
{
    return std::is_same<T1, T2>::value == false;
}

} // namespace Nano ------------------------------------------------------------

#pragma pop_macro("new")

#endif // NANO_POOL_ALLOCATOR_HPP

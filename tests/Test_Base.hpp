#ifndef NANO_TEST_BASE_HPP
#define NANO_TEST_BASE_HPP

#include <functional>

#include "../nano_signal_slot.hpp"

// Nano::Observer will now use std::recursive_mutex
#define NANO_DEFINE_THREADSAFE_OBSERVER
// Nano::Pool_Allocator will now use atomics and std::mutex
#define NANO_DEFINE_THREADSAFE_ALLOCATOR

namespace Nano_Tests
{

    namespace
    {
        static void anonymous_output(const char* fn, const char* sl, std::size_t ln)
        {
            std::cout << fn << " LINE: " << __LINE__ << " Test: " << sl << " LINE: " << ln << std::endl;
        }
    }

    using Signal_One = Nano::Signal<void(const char*)>;
    using Signal_Two = Nano::Signal<void(const char*, std::size_t)>;

    using Observer = Nano::Observer<>;

    using Delegate_One = std::function<void(const char*)>;
    using Delegate_Two = std::function<void(const char*, std::size_t)>;

    class Foo : public Observer
    {
        public:

        void slot_member_signature_one(const char* sl)
        {
            anonymous_output(__FUNCTION__, sl, __LINE__);
        }
        void slot_member_signature_two(const char* sl, std::size_t ln)
        {
            anonymous_output(__FUNCTION__, sl, ln);
        }

        void slot_const_member_signature_one(const char* sl) const
        {
            anonymous_output(__FUNCTION__, sl, __LINE__);
        }
        void slot_const_member_signature_two(const char* sl, std::size_t ln) const
        {
            anonymous_output(__FUNCTION__, sl, ln);
        }

        void slot_overloaded_member(const char* sl)
        {
            anonymous_output(__FUNCTION__, sl, __LINE__);
        }
        void slot_overloaded_member(const char* sl, std::size_t ln)
        {
            anonymous_output(__FUNCTION__, sl, ln);
        }

        static void slot_static_member_function(const char* sl)
        {
            anonymous_output(__FUNCTION__, sl, __LINE__);
        }
        static void slot_static_member_function(const char* sl, std::size_t ln)
        {
            anonymous_output(__FUNCTION__, sl, ln);
        }

        virtual void slot_virtual_member_function(const char* sl)
        {
            anonymous_output(__FUNCTION__, sl, __LINE__);
        }
        virtual void slot_virtual_member_function(const char* sl, std::size_t ln)
        {
            anonymous_output(__FUNCTION__, sl, ln);
        }
    };

    class Bar : public Foo
    {
        public:

        void slot_virtual_member_function(const char* sl) override
        {
            anonymous_output(__FUNCTION__, sl, __LINE__);
        }
        void slot_virtual_member_function(const char* sl, std::size_t ln) override
        {
            anonymous_output(__FUNCTION__, sl, ln);
        }
    };

    static void slot_static_free_function(const char* sl)
    {
        anonymous_output(__FUNCTION__, sl, __LINE__);
    }

    static void slot_static_free_function(const char* sl, std::size_t ln)
    {
        anonymous_output(__FUNCTION__, sl, ln);
    }

}

#endif // NANO_TEST_BASE_HPP

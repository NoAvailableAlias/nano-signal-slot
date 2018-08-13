#pragma once

#include <mutex>
#include <functional>

#include "../nano_signal_slot.hpp"
#include "../nano_mutex.hpp"

namespace Nano_Tests
{

    namespace
    {
        static void anonymous_output(const char* fn, const char* sl, std::size_t ln)
        {
            std::cout << fn << " LINE: " << __LINE__ << " Test: " << sl << " LINE: " << ln << std::endl;
        }
    }

#if defined(NANO_DEFINE_THREADSAFE_SIGNALS) || true

    using Observer = Nano::Observer<Nano::Recursive_Mutex>;
    using Signal_One = Nano::Signal<void(const char*), Nano::Recursive_Mutex>;
    using Signal_Two = Nano::Signal<void(const char*, std::size_t), Nano::Recursive_Mutex>;

#else

    using Observer = Nano::Observer<>;
    using Signal_One = Nano::Signal<void(const char*)>;
    using Signal_Two = Nano::Signal<void(const char*, std::size_t)>;

#endif

    using Delegate_One = std::function<void(const char*)>;
    using Delegate_Two = std::function<void(const char*, std::size_t)>;

    //--------------------------------------------------------------------------

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

    //--------------------------------------------------------------------------

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

    //--------------------------------------------------------------------------

    static void slot_static_free_function(const char* sl)
    {
        anonymous_output(__FUNCTION__, sl, __LINE__);
    }

    static void slot_static_free_function(const char* sl, std::size_t ln)
    {
        anonymous_output(__FUNCTION__, sl, ln);
    }

}

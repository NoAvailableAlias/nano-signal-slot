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

    using Observer = Nano::Observer<>;
    using Signal_One = Nano::Signal<void(const char*)>;
    using Signal_Two = Nano::Signal<void(const char*, std::size_t)>;

    // TODO Add Policy related testing

    //using Observer_TS = Nano::Observer<Nano::TS_Policy<>>;
    //using Signal_One_TS = Nano::Signal<void(const char*), Nano::TS_Policy<>>;
    //using Signal_Two_TS = Nano::Signal<void(const char*, std::size_t), Nano::TS_Policy<>>;

    using Observer_STS = Nano::Observer<Nano::ST_Policy_Strict>;
    using Signal_One_STS = Nano::Signal<void(const char*), Nano::ST_Policy_Strict>;
    using Signal_Two_STS = Nano::Signal<void(const char*, std::size_t), Nano::ST_Policy_Strict>;

    //using Observer_TSS = Nano::Observer<Nano::TS_Policy_Strict<>>;
    //using Signal_One_TSS = Nano::Signal<void(const char*), Nano::TS_Policy_Strict<>>;
    //using Signal_Two_TSS = Nano::Signal<void(const char*, std::size_t), Nano::TS_Policy_Strict<>>;

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

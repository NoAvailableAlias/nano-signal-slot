#pragma once

#include <iostream>
#include <functional>
#include <random>

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

    using Rng = std::minstd_rand;

    using Observer = Nano::Observer<>;
    using Signal_One = Nano::Signal<void(const char*)>;
    using Signal_Two = Nano::Signal<void(const char*, std::size_t)>;

    using Observer_ST = Nano::Observer<Nano::ST_Policy>;
    using Signal_Rng_ST = Nano::Signal<void(Rng&), Nano::ST_Policy>;

    using Observer_STS = Nano::Observer<Nano::ST_Policy_Safe>;
    using Signal_Rng_STS = Nano::Signal<void(Rng&), Nano::ST_Policy_Safe>;

    using Observer_TS = Nano::Observer<Nano::TS_Policy<>>;
    using Signal_Rng_TS = Nano::Signal<void(Rng&), Nano::TS_Policy<>>;

    using Observer_TSS = Nano::Observer<Nano::TS_Policy_Safe<>>;
    using Signal_Rng_TSS = Nano::Signal<void(Rng&), Nano::TS_Policy_Safe<>>;

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

    //--------------------------------------------------------------------------

    template <typename T>
    class Moo : public T
    {
        public:

        void slot_next_random(Rng& rng)
        {
            rng.discard(1);
        }

        static void slot_static_next_random(Rng& rng)
        {
            rng.discard(1);
        }
    };

    static void slot_next_random_free_function(Rng& rng)
    {
        rng.discard(1);
    }

    //--------------------------------------------------------------------------

    class Copy_Count
    {
        public:

        std::size_t count = 0;

        Copy_Count() = default;
        Copy_Count(Copy_Count const& other) : count(other.count + 1)
        {

        }
        Copy_Count& operator= (Copy_Count const& other)
        {
            count = other.count + 1;
            return *this;
        }
    };
}

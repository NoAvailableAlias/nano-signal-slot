#include <list>

#include <catch2/catch.hpp>

#include "Test_Base.hpp"


namespace Nano_Tests
{
    TEST_CASE("Test_ST_Policy")
    {
        const int N = 64;

        using Moo_T = Moo<Observer_ST>;
        using Subject = Signal_Rng_ST;

        SECTION("Test_Global_Signal")
        {
            Subject subject;

            std::size_t count = 0;

            Rng rng;

            for (; count < N; ++count)
            {
                std::list<Moo_T> moo(N);

                for (auto& moo_instance : moo)
                {
                    subject.connect<&Moo_T::slot_next_random>(moo_instance);
                }
                subject.fire(rng);
            }
            REQUIRE(subject.is_empty()); //L"A signal was found not empty.");
        }

        //----------------------------------------------------------------------

        SECTION("Test_Signal_Move")
        {
            Moo_T foo1;
            Moo_T foo2;

            auto rng1 = Rng();
            auto rng2 = Rng();

            auto sig1 = new Subject();
            auto sig2 = new Subject();

            auto fun1 = [](Rng& rng) { rng.discard(1); };

            sig1->connect<&Moo_T::slot_next_random>(foo1);
            sig1->connect(fun1);
            sig1->connect<&Moo_T::slot_static_next_random>();
            sig1->connect<&slot_next_random_free_function>();

            sig2->connect<&Moo_T::slot_next_random>(foo2);
            sig2->connect(fun1);
            sig2->connect<&Moo_T::slot_static_next_random>();
            sig2->connect<&slot_next_random_free_function>();

            {
                // This should remove all connections from sig2 and add all sig1 connections
                *sig2 = std::move(*sig1);
                REQUIRE(sig1->is_empty()); //L"Signal failed to remove connections during move.");
                REQUIRE(foo2.is_empty()); //L"Signal failed to remove connections prior to move.");
                REQUIRE(!foo1.is_empty()); //L"Signal failed to move subject connections to target.");
                REQUIRE(!sig2->is_empty()); //L"Signal failed to move signal connections to target.");

                // Should have four connections
                sig2->fire(rng1);
                rng2.discard(4);
                REQUIRE(rng1 == rng2); //L"Signal failed to move all connections.");

                delete sig2;
            }

            REQUIRE(foo1.is_empty()); //L"Signal failed to dispose connections.");
            REQUIRE(foo2.is_empty()); //L"Signal failed to dispose connections.");
        }
    }
}

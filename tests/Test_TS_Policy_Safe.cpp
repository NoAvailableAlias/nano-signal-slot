#include <future>
#include <list>

#include <catch2/catch.hpp>

#include "Test_Base.hpp"


namespace Nano_Tests
{
    TEST_CASE("Test_TS_Policy_Safe")
    {
        const int N = 64; // 4 seconds
        //const int N = 256; // 4 minutes

        using Moo_T = Moo<Observer_TSS>;
        using Subject = Signal_Rng_TSS;

        SECTION("Test_Shared_Signal")
        {
            Subject subject;

            auto context = [&]()
            {
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
            };
            std::vector<std::future<void>> future_results;
            for (std::size_t i = std::thread::hardware_concurrency(); i > 0; --i)
            {
                future_results.emplace_back(std::async(std::launch::async, context));
            }
            for (auto& future_result : future_results)
            {
                future_result.get();
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

        //----------------------------------------------------------------------

        SECTION("Test_Fire_Disconnect")
        {
            Moo_T foo;

            Subject signal;

            signal.connect<&Moo_T::slot_next_random>(foo);

            std::function<void(Rng&)> test;

            test = [&](Rng&)
            {
                signal.disconnect(test);
            };

            signal.connect(test);

            signal.fire(Rng());
        }

        SECTION("Test_Fire_Disconnects")
        {
            Moo_T foo;

            Subject signal;

            signal.connect<&Moo_T::slot_next_random>(foo);

            std::function<void(Rng&)> test;

            test = [&](Rng&)
            {
                signal.disconnect<&Moo_T::slot_next_random>(foo);
                signal.disconnect(test);
            };

            signal.connect(test);

            signal.fire(Rng());

            REQUIRE(foo.is_empty()); //L"An observer was not disconnected.");
            REQUIRE(signal.is_empty()); //L"A slot was not disconnected.");
        }

        SECTION("Test_Fire_Connects")
        {
            Moo_T foo;

            Subject signal;

            std::function<void(Rng&)> test;

            test = [&](Rng& rng)
            {
                signal.connect<&Moo_T::slot_next_random>(foo);
                foo.slot_next_random(rng);
            };

            signal.connect(test);

            Rng rng_1;
            Rng rng_2;
            rng_2.discard(3);

            signal.fire(rng_1);
            signal.fire(rng_1);

            REQUIRE(rng_1 == rng_2); //L"A slot was not connected.");
        }

        SECTION("Test_Fire_Disconnect_All")
        {
            Moo_T foo;

            Subject signal;

            signal.connect<&Moo_T::slot_next_random>(foo);

            std::function<void(Rng&)> test;

            test = [&](Rng&)
            {
                signal.disconnect_all();
            };

            signal.connect(test);

            signal.fire(Rng());

            REQUIRE(foo.is_empty()); //L"An observer was not disconnected.");
            REQUIRE(signal.is_empty()); //L"A slot was not disconnected.");
        }

        SECTION("Test_Fire_Fire")
        {
            Moo_T foo;

            Subject signal;

            std::function<void(Rng&)> test;

            Rng stop;
            stop.discard(16);

            test = [&](Rng& rng)
            {
                if (rng != stop)
                {
                    rng.discard(1);
                    signal.fire(rng);
                }
            };

            signal.connect(test);

            signal.fire(Rng());
        }
    };
}

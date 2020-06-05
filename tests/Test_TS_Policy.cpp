#include <future>
#include <list>

#include "CppUnitTest.h"

#include "Test_Base.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Nano_Tests
{
    TEST_CLASS(Test_TS_Policy)
    {
        const int N = 64;

        using Moo_T = Moo<Observer_TS>;
        using Subject = Signal_Rng_TS;

        TEST_METHOD(Test_Shared_Signal)
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
            Assert::IsTrue(subject.is_empty(), L"A signal was found not empty.");
        }

        //----------------------------------------------------------------------

        //TEST_METHOD(Test_Signal_Move)
        //{
        //    Moo_T foo;

        //    Subject* signal_one = new Subject();

        //    signal_one->connect<&Moo_T::slot_next_random>(foo);

        //    {
        //        Subject signal_two = std::move(*signal_one);

        //        Assert::IsTrue(signal_one->is_empty(), L"Signal failed to sink.");

        //        delete signal_one;

        //        signal_two.fire(Rng());

        //        foo.disconnect_all();
        //    }

        //    Assert::IsTrue(foo.is_empty(), L"Signal failed to sink.");
        //}
    };
}

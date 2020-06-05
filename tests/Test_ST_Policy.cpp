#include <list>

#include "CppUnitTest.h"

#include "Test_Base.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Nano_Tests
{
    TEST_CLASS(Test_ST_Policy)
    {
        const int N = 64;

        using Moo_T = Moo<Observer_ST>;
        using Subject = Signal_Rng_ST;

        TEST_METHOD(Test_Global_Signal)
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

#include <list>

#include "CppUnitTest.h"

#include "Test_Base.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Nano_Tests
{
    TEST_CLASS(Test_ST_Policy_Safe)
    {
        const int N = 64;

        using Moo_T = Moo<Observer_STS>;
        using Subject = Signal_Rng_STS;

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
    };
}

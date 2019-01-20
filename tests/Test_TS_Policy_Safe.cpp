#include <future>
#include <list>

#include "CppUnitTest.h"

#include "Test_Base.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Nano_Tests
{
    TEST_CLASS(Test_TS_Policy_Safe)
    {
        using Moo_TSS = Moo<Observer_TSS>;

        TEST_METHOD(Test_Shared_Signal)
        {
            Signal_Rng_TSS subject;

            auto context = [&]()
            {
                std::size_t count = 0;

                Rng rng;

                for (; count < 32; ++count)
                {
                    std::list<Moo_TSS> moo(32);

                    for (auto& moo_instance : moo)
                    {
                        subject.connect<&Moo_TSS::slot_next_random>(moo_instance);
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
    };
}


#include <iostream>
#include <vector>

#include "CppUnitTest.h"

#include "Test_Base.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Nano_Tests
{
    TEST_CLASS(Test_Signal_Fire)
    {
        Signal_One mo_signal_one;
        Signal_Two mo_signal_two;

        Foo mo_foo;
        Bar mo_bar;

        public:

        TEST_METHOD(Test_Member_Fire)
        {
            mo_signal_one.connect<&Foo::slot_member_signature_one>(mo_foo);
            mo_signal_two.connect<&Foo::slot_member_signature_two>(mo_foo);

            mo_signal_one.fire(__FUNCTION__);
            mo_signal_two.fire(__FUNCTION__, __LINE__);
        }

        TEST_METHOD(Test_Const_Member_Fire)
        {
            mo_signal_one.connect<&Foo::slot_const_member_signature_one>(mo_foo);
            mo_signal_two.connect<&Foo::slot_const_member_signature_two>(mo_foo);

            mo_signal_one.fire(__FUNCTION__);
            mo_signal_two.fire(__FUNCTION__, __LINE__);
        }

        TEST_METHOD(Test_Overloaded_Member_Fire)
        {
            mo_signal_one.connect<Foo, &Foo::slot_overloaded_member>(mo_foo);
            mo_signal_two.connect<Foo, &Foo::slot_overloaded_member>(mo_foo);

            mo_signal_one.fire(__FUNCTION__);
            mo_signal_two.fire(__FUNCTION__, __LINE__);
        }

        TEST_METHOD(Test_Overloaded_Static_Member_Fire)
        {
            mo_signal_one.connect<&Foo::slot_static_member_function>();
            mo_signal_two.connect<&Foo::slot_static_member_function>();


            mo_signal_one.fire(__FUNCTION__);
            mo_signal_two.fire(__FUNCTION__, __LINE__);
        }

        TEST_METHOD(Test_Overloaded_Virtual_Member_Fire)
        {
            mo_signal_one.connect<Foo, &Foo::slot_virtual_member_function>(mo_foo);
            mo_signal_two.connect<Foo, &Foo::slot_virtual_member_function>(mo_foo);

            mo_signal_one.fire(__FUNCTION__);
            mo_signal_two.fire(__FUNCTION__, __LINE__);
        }

        TEST_METHOD(Test_Overloaded_Virtual_Derived_Member_Fire)
        {
            mo_signal_one.connect<Bar, &Bar::slot_virtual_member_function>(mo_bar);
            mo_signal_two.connect<Bar, &Bar::slot_virtual_member_function>(mo_bar);

            mo_signal_one.fire(__FUNCTION__);
            mo_signal_two.fire(__FUNCTION__, __LINE__);
        }

        TEST_METHOD(Test_Fire_Accumulate)
        {
            Nano::Signal<std::size_t(std::size_t)> signal_three;
            auto slot_three = [&](std::size_t val)
            {
                return val * val;
            };
            signal_three.connect(slot_three);

            std::vector<std::size_t> signal_return_values;
            auto accumulator = [&](std::size_t srv)
            {
                signal_return_values.push_back(srv);
            };

            signal_three.fire_accumulate(accumulator, __LINE__);
            signal_three.fire_accumulate(accumulator, __LINE__);
            signal_three.fire_accumulate(accumulator, __LINE__);
            signal_three.fire_accumulate(accumulator, __LINE__);
            signal_three.fire_accumulate(accumulator, __LINE__);
            signal_three.fire_accumulate(accumulator, __LINE__);
            signal_three.fire_accumulate(accumulator, __LINE__);
            signal_three.fire_accumulate(accumulator, __LINE__);
            signal_three.fire_accumulate(accumulator, __LINE__);
            signal_three.fire_accumulate(accumulator, __LINE__);

            Assert::IsTrue(signal_return_values.size() == 10, L"An SRV was found missing.");
        }

        //----------------------------------------------------------------------

        TEST_METHOD(Test_Fire_Single_Copy)
        {
            Nano::Signal<void(Copy_Count)> signal_one;

            auto slot_one = [](Copy_Count cc)
            {
                Assert::IsTrue(cc.count == 1, L"A parameter was copied more than once.");
            };

            signal_one.connect(slot_one);

            auto slot_two = [](Copy_Count cc)
            {
                Assert::IsTrue(cc.count == 1, L"A parameter was copied more than once.");
            };

            signal_one.connect(slot_two);

            Copy_Count cc;

            signal_one.fire(cc);
        }

        TEST_METHOD(Test_Fire_LValue_Copy)
        {
            Nano::Signal<void(Copy_Count&)> signal_one;

            auto slot_one = [](Copy_Count& cc)
            {
                Assert::IsTrue(cc.count == 0, L"A reference parameter was copied.");
            };

            signal_one.connect(slot_one);

            auto slot_two = [](Copy_Count& cc)
            {
                Assert::IsTrue(cc.count == 0, L"A reference parameter was copied.");
            };

            signal_one.connect(slot_two);

            Copy_Count cc;

            signal_one.fire(cc);
        }

        TEST_METHOD(Test_Fire_RValue_Copy)
        {
            Nano::Signal<void(Copy_Count)> signal_one;

            auto slot_one = [](Copy_Count cc)
            {
                Assert::IsTrue(cc.count == 1, L"An rvalue parameter wasn't copied once.");
            };

            signal_one.connect(slot_one);

            auto slot_two = [](Copy_Count cc)
            {
                Assert::IsTrue(cc.count == 1, L"An rvalue parameter wasn't copied once.");
            };

            signal_one.connect(slot_two);

            signal_one.fire(Copy_Count());
        }
    };
}

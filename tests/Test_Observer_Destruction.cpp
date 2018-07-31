
#include <iostream>
#include <memory>

#include "CppUnitTest.h"

#include "Test_Base.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Nano_Tests
{
    TEST_CLASS(Test_Observer_Destruction)
    {
        Signal_One mo_signal_one;
        Signal_Two mo_signal_two;

        public:

        TEST_METHOD(Test_Stack_Foo_Observer_Destructor)
        {
            {
                Foo to_foo;

                mo_signal_one.connect<Foo, &Foo::slot_virtual_member_function>(to_foo);
                mo_signal_two.connect<Foo, &Foo::slot_virtual_member_function>(&to_foo);
            }

            Assert::IsTrue(mo_signal_one.is_empty(), L"A slot was found.");
            Assert::IsTrue(mo_signal_two.is_empty(), L"A slot was found.");
        }

        TEST_METHOD(Test_Stack_Bar_Foo_Observer_Destructor)
        {
            {
                Bar to_bar;

                mo_signal_one.connect<Bar, &Bar::slot_virtual_member_function>(&to_bar);
                mo_signal_two.connect<Bar, &Bar::slot_virtual_member_function>(to_bar);
            }

            Assert::IsTrue(mo_signal_one.is_empty(), L"A slot was found.");
            Assert::IsTrue(mo_signal_two.is_empty(), L"A slot was found.");
        }

        TEST_METHOD(Test_Heap_Foo_Observer_Destructor)
        {
            {
                auto to_foo = std::make_unique<Foo>();

                mo_signal_one.connect<Foo, &Foo::slot_virtual_member_function>(*to_foo);
                mo_signal_two.connect<Foo, &Foo::slot_virtual_member_function>(*to_foo);
            }

            Assert::IsTrue(mo_signal_one.is_empty(), L"A slot was found.");
            Assert::IsTrue(mo_signal_two.is_empty(), L"A slot was found.");
        }

        TEST_METHOD(Test_Heap_Bar_Foo_Observer_Destructor)
        {
            {
                auto to_bar = std::make_unique<Bar>();

                mo_signal_one.connect<Bar, &Bar::slot_virtual_member_function>(*to_bar);
                mo_signal_two.connect<Bar, &Bar::slot_virtual_member_function>(*to_bar);
            }

            Assert::IsTrue(mo_signal_one.is_empty(), L"A slot was found.");
            Assert::IsTrue(mo_signal_two.is_empty(), L"A slot was found.");
        }
    };
}


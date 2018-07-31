
#include <iostream>
#include <functional>

#include "CppUnitTest.h"

#include "Test_Base.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Nano_Tests
{
    TEST_CLASS(Test_Signal_Edge_Cases)
    {
        Signal_One mo_signal_one;
        Signal_Two mo_signal_two;

        Foo mo_foo;
        Bar mo_bar;

        public:

        TEST_METHOD(Test_Fired_Disconnect)
        {
            Delegate_One fo1;

            mo_signal_one.connect(fo1);
            mo_signal_one.connect<&Foo::slot_member_signature_one>(mo_foo);
            mo_signal_one.connect<&Foo::slot_const_member_signature_one>(mo_foo);
            mo_signal_one.connect<Foo, &Foo::slot_overloaded_member>(mo_foo);
            mo_signal_one.connect<&Foo::slot_static_member_function>();
            mo_signal_one.connect<Foo, &Foo::slot_virtual_member_function>(mo_foo);
            mo_signal_one.connect<Bar, &Bar::slot_virtual_member_function>(mo_bar);

            fo1 = [&](const char* sl)
            {
                mo_signal_one.disconnect(fo1);
            };

            mo_signal_one.fire(__FILE__);
        }

        TEST_METHOD(Test_Fire_Disconnects)
        {
            Delegate_One fo1;

            mo_signal_one.connect(fo1);
            mo_signal_one.connect<&Foo::slot_member_signature_one>(mo_foo);
            mo_signal_one.connect<&Foo::slot_const_member_signature_one>(mo_foo);
            mo_signal_one.connect<Foo, &Foo::slot_overloaded_member>(mo_foo);
            mo_signal_one.connect<&Foo::slot_static_member_function>();
            mo_signal_one.connect<Foo, &Foo::slot_virtual_member_function>(mo_foo);
            mo_signal_one.connect<Bar, &Bar::slot_virtual_member_function>(mo_bar);

            fo1 = [&](const char* sl)
            {
                mo_signal_one.disconnect(fo1);
                mo_signal_one.disconnect<&Foo::slot_member_signature_one>(mo_foo);
                mo_signal_one.disconnect<&Foo::slot_const_member_signature_one>(mo_foo);
                mo_signal_one.disconnect<Foo, &Foo::slot_overloaded_member>(mo_foo);
                mo_signal_one.disconnect<&Foo::slot_static_member_function>();
                mo_signal_one.disconnect<Foo, &Foo::slot_virtual_member_function>(mo_foo);
                mo_signal_one.disconnect<Bar, &Bar::slot_virtual_member_function>(mo_bar);
            };

            // TODO This should be failing but currently isn't...
            // This should be causing iterator invalidation

            mo_signal_one.fire(__FILE__);
        }

        TEST_METHOD(Test_Fire_Connects)
        {
            Delegate_One fo1;

            mo_signal_one.connect(fo1);

            fo1 = [&](const char* sl)
            {
                mo_signal_one.connect(fo1);
                mo_signal_one.connect<&Foo::slot_member_signature_one>(mo_foo);
                mo_signal_one.connect<&Foo::slot_const_member_signature_one>(mo_foo);
                mo_signal_one.connect<Foo, &Foo::slot_overloaded_member>(mo_foo);
                mo_signal_one.connect<&Foo::slot_static_member_function>();
                mo_signal_one.connect<Foo, &Foo::slot_virtual_member_function>(mo_foo);
                mo_signal_one.connect<Bar, &Bar::slot_virtual_member_function>(mo_bar);
            };

            mo_signal_one.fire(__FILE__);
        }

        TEST_METHOD(Test_Fire_Remove_All)
        {
            Delegate_One fo1;

            mo_signal_one.connect(fo1);
            mo_signal_one.connect<&Foo::slot_member_signature_one>(mo_foo);
            mo_signal_one.connect<&Foo::slot_const_member_signature_one>(mo_foo);
            mo_signal_one.connect<Foo, &Foo::slot_overloaded_member>(mo_foo);
            mo_signal_one.connect<&Foo::slot_static_member_function>();
            mo_signal_one.connect<Foo, &Foo::slot_virtual_member_function>(mo_foo);
            mo_signal_one.connect<Bar, &Bar::slot_virtual_member_function>(mo_bar);

            fo1 = [&](const char* sl)
            {
                mo_signal_one.remove_all();
            };

            mo_signal_one.fire(__FILE__);

            Assert::IsTrue(mo_signal_one.is_empty(), L"A slot was found.");
        }

        TEST_METHOD(Test_Fire_Recursive_Fire)
        {
            Delegate_One fo1;

            mo_signal_one.connect(fo1);
            mo_signal_one.connect<&Foo::slot_member_signature_one>(mo_foo);
            mo_signal_one.connect<&Foo::slot_const_member_signature_one>(mo_foo);
            mo_signal_one.connect<Foo, &Foo::slot_overloaded_member>(mo_foo);
            mo_signal_one.connect<&Foo::slot_static_member_function>();
            mo_signal_one.connect<Foo, &Foo::slot_virtual_member_function>(mo_foo);
            mo_signal_one.connect<Bar, &Bar::slot_virtual_member_function>(mo_bar);

            fo1 = [&](const char* sl)
            {
                // TODO Prevent this infinite loop
                // This would require a custom data structure for Observer
                // in order to realize a no-cost solution to this problem

                //mo_signal_one.fire(__FILE__);
            };

            mo_signal_one.fire(__FILE__);
        }
    };
}

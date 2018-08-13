
#include <iostream>

#include "CppUnitTest.h"

#include "Test_Base.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Nano_Tests
{
    TEST_CLASS(Test_Signal_Disconnect)
    {
        Signal_One mo_signal_one;
        Signal_Two mo_signal_two;

        Foo mo_foo;
        Bar mo_bar;

        public:

        TEST_METHOD(Test_Member_Disconnect)
        {
            mo_signal_one.connect<&Foo::slot_member_signature_one>(mo_foo);
            mo_signal_one.connect<&Foo::slot_member_signature_one>(&mo_foo);
            Assert::IsFalse(mo_signal_one.is_empty(), L"A slot was found missing.");

            mo_signal_one.disconnect<&Foo::slot_member_signature_one>(mo_foo);
            mo_signal_one.disconnect<&Foo::slot_member_signature_one>(&mo_foo);
            Assert::IsTrue(mo_signal_one.is_empty(), L"A slot was found.");
            Assert::IsTrue(mo_foo.is_empty(), L"A slot was found.");

            //------------------------------------------------------------------

            mo_signal_two.connect<&Foo::slot_member_signature_two>(mo_foo);
            mo_signal_two.connect<&Foo::slot_member_signature_two>(&mo_foo);
            Assert::IsFalse(mo_signal_two.is_empty(), L"A slot was found missing.");

            mo_signal_two.disconnect<&Foo::slot_member_signature_two>(mo_foo);
            mo_signal_two.disconnect<&Foo::slot_member_signature_two>(&mo_foo);
            Assert::IsTrue(mo_signal_two.is_empty(), L"A slot was found.");
            Assert::IsTrue(mo_foo.is_empty(), L"A slot was found.");
        }

        TEST_METHOD(Test_Const_Member_Disconnect)
        {
            mo_signal_one.connect<&Foo::slot_const_member_signature_one>(mo_foo);
            mo_signal_one.connect<&Foo::slot_const_member_signature_one>(&mo_foo);
            Assert::IsFalse(mo_signal_one.is_empty(), L"A slot was found missing.");

            mo_signal_one.disconnect<&Foo::slot_const_member_signature_one>(mo_foo);
            mo_signal_one.disconnect<&Foo::slot_const_member_signature_one>(&mo_foo);
            Assert::IsTrue(mo_signal_one.is_empty(), L"A slot was found.");
            Assert::IsTrue(mo_foo.is_empty(), L"A slot was found.");

            //------------------------------------------------------------------

            mo_signal_two.connect<&Foo::slot_const_member_signature_two>(mo_foo);
            mo_signal_two.connect<&Foo::slot_const_member_signature_two>(&mo_foo);
            Assert::IsFalse(mo_signal_two.is_empty(), L"A slot was found missing.");

            mo_signal_two.disconnect<&Foo::slot_const_member_signature_two>(mo_foo);
            mo_signal_two.disconnect<&Foo::slot_const_member_signature_two>(&mo_foo);
            Assert::IsTrue(mo_signal_two.is_empty(), L"A slot was found.");
            Assert::IsTrue(mo_foo.is_empty(), L"A slot was found.");
        }

        TEST_METHOD(Test_Overloaded_Member_Disconnect)
        {
            mo_signal_one.connect<Foo, &Foo::slot_overloaded_member>(mo_foo);
            mo_signal_one.connect<Foo, &Foo::slot_overloaded_member>(&mo_foo);
            Assert::IsFalse(mo_signal_one.is_empty(), L"A slot was found missing.");

            mo_signal_one.disconnect<Foo, &Foo::slot_overloaded_member>(mo_foo);
            mo_signal_one.disconnect<Foo, &Foo::slot_overloaded_member>(&mo_foo);
            Assert::IsTrue(mo_signal_one.is_empty(), L"A slot was found.");
            Assert::IsTrue(mo_foo.is_empty(), L"A slot was found.");

            //------------------------------------------------------------------

            mo_signal_two.connect<Foo, &Foo::slot_overloaded_member>(mo_foo);
            mo_signal_two.connect<Foo, &Foo::slot_overloaded_member>(&mo_foo);
            Assert::IsFalse(mo_signal_two.is_empty(), L"A slot was found missing.");

            mo_signal_two.disconnect<Foo, &Foo::slot_overloaded_member>(mo_foo);
            mo_signal_two.disconnect<Foo, &Foo::slot_overloaded_member>(&mo_foo);
            Assert::IsTrue(mo_signal_two.is_empty(), L"A slot was found.");
            Assert::IsTrue(mo_foo.is_empty(), L"A slot was found.");
        }

        TEST_METHOD(Test_Overloaded_Static_Member_Disconnect)
        {
            mo_signal_one.connect<&Foo::slot_static_member_function>();
            Assert::IsFalse(mo_signal_one.is_empty(), L"A slot was found missing.");

            mo_signal_two.connect<&Foo::slot_static_member_function>();
            Assert::IsFalse(mo_signal_two.is_empty(), L"A slot was found missing.");

            //------------------------------------------------------------------

            mo_signal_one.disconnect<&Foo::slot_static_member_function>();
            Assert::IsTrue(mo_signal_one.is_empty(), L"A slot was found.");

            mo_signal_two.disconnect<&Foo::slot_static_member_function>();
            Assert::IsTrue(mo_signal_two.is_empty(), L"A slot was found.");
        }

        TEST_METHOD(Test_Overloaded_Virtual_Member_Disconnect)
        {
            mo_signal_one.connect<Foo, &Foo::slot_virtual_member_function>(mo_foo);
            mo_signal_one.connect<Foo, &Foo::slot_virtual_member_function>(&mo_foo);
            Assert::IsFalse(mo_signal_one.is_empty(), L"A slot was found missing.");

            mo_signal_one.disconnect<Foo, &Foo::slot_virtual_member_function>(mo_foo);
            mo_signal_one.disconnect<Foo, &Foo::slot_virtual_member_function>(&mo_foo);
            Assert::IsTrue(mo_signal_one.is_empty(), L"A slot was found.");
            Assert::IsTrue(mo_foo.is_empty(), L"A slot was found.");

            //------------------------------------------------------------------

            mo_signal_two.connect<Foo, &Foo::slot_virtual_member_function>(mo_foo);
            mo_signal_two.connect<Foo, &Foo::slot_virtual_member_function>(&mo_foo);
            Assert::IsFalse(mo_signal_two.is_empty(), L"A slot was found missing.");

            mo_signal_two.disconnect<Foo, &Foo::slot_virtual_member_function>(mo_foo);
            mo_signal_two.disconnect<Foo, &Foo::slot_virtual_member_function>(&mo_foo);
            Assert::IsTrue(mo_signal_two.is_empty(), L"A slot was found.");
            Assert::IsTrue(mo_foo.is_empty(), L"A slot was found.");
        }

        TEST_METHOD(Test_Overloaded_Virtual_Derived_Member_Disconnect)
        {
            mo_signal_one.connect<Bar, &Bar::slot_virtual_member_function>(mo_bar);
            mo_signal_one.connect<Bar, &Bar::slot_virtual_member_function>(&mo_bar);
            Assert::IsFalse(mo_signal_one.is_empty(), L"A slot was found missing.");

            mo_signal_one.disconnect<Bar, &Bar::slot_virtual_member_function>(mo_bar);
            mo_signal_one.disconnect<Bar, &Bar::slot_virtual_member_function>(&mo_bar);
            Assert::IsTrue(mo_signal_one.is_empty(), L"A slot was found.");
            Assert::IsTrue(mo_bar.is_empty(), L"A slot was found.");

            //------------------------------------------------------------------

            mo_signal_two.connect<Bar, &Bar::slot_virtual_member_function>(mo_bar);
            mo_signal_two.connect<Bar, &Bar::slot_virtual_member_function>(&mo_bar);
            Assert::IsFalse(mo_signal_two.is_empty(), L"A slot was found missing.");

            mo_signal_two.disconnect<Bar, &Bar::slot_virtual_member_function>(mo_bar);
            mo_signal_two.disconnect<Bar, &Bar::slot_virtual_member_function>(&mo_bar);
            Assert::IsTrue(mo_signal_two.is_empty(), L"A slot was found.");
            Assert::IsTrue(mo_bar.is_empty(), L"A slot was found.");
        }

        TEST_METHOD(Test_Overloaded_Free_Function_Disconnect)
        {
            mo_signal_one.connect<&slot_static_free_function>();
            Assert::IsFalse(mo_signal_one.is_empty(), L"A slot was found missing.");

            mo_signal_two.connect<&slot_static_free_function>();
            Assert::IsFalse(mo_signal_two.is_empty(), L"A slot was found missing.");

            //------------------------------------------------------------------

            mo_signal_one.disconnect<&slot_static_free_function>();
            Assert::IsTrue(mo_signal_one.is_empty(), L"A slot was found.");

            mo_signal_two.disconnect<&slot_static_free_function>();
            Assert::IsTrue(mo_signal_two.is_empty(), L"A slot was found.");
        }

        TEST_METHOD(Test_Disconnect_All)
        {
            mo_signal_one.connect<Foo, &Foo::slot_virtual_member_function>(mo_foo);
            mo_signal_one.connect<Bar, &Bar::slot_virtual_member_function>(mo_bar);
            mo_signal_one.connect<&slot_static_free_function>();
            Assert::IsFalse(mo_signal_one.is_empty(), L"A slot was found missing.");

            mo_signal_one.disconnect_all();
            Assert::IsTrue(mo_signal_one.is_empty(), L"A slot was found.");
            Assert::IsTrue(mo_foo.is_empty(), L"A slot was found.");
            Assert::IsTrue(mo_bar.is_empty(), L"A slot was found.");

            //------------------------------------------------------------------

            mo_signal_two.connect<Foo, &Foo::slot_virtual_member_function>(mo_foo);
            mo_signal_two.connect<Bar, &Bar::slot_virtual_member_function>(mo_bar);
            mo_signal_two.connect<&slot_static_free_function>();
            Assert::IsFalse(mo_signal_two.is_empty(), L"A slot was found missing.");

            mo_signal_two.disconnect_all();
            Assert::IsTrue(mo_signal_two.is_empty(), L"A slot was found.");
            Assert::IsTrue(mo_foo.is_empty(), L"A slot was found.");
            Assert::IsTrue(mo_bar.is_empty(), L"A slot was found.");
        }
    };
}

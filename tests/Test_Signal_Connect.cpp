
#include <iostream>

#include <catch2/catch.hpp>

#include "Test_Base.hpp"


namespace Nano_Tests
{
    TEST_CASE("Test_Signal_Connect")
    {
        Signal_One mo_signal_one;
        Signal_Two mo_signal_two;

        Foo mo_foo;
        Bar mo_bar;


        SECTION("Test_Member_Connect")
        {
            mo_signal_one.connect<&Foo::slot_member_signature_one>(mo_foo);
            mo_signal_one.connect<&Foo::slot_member_signature_one>(&mo_foo);
            REQUIRE(!mo_signal_one.is_empty()); //L"A slot was found missing.");
            REQUIRE(!mo_foo.is_empty()); //L"A slot was found missing.");

            mo_signal_two.connect<&Foo::slot_member_signature_two>(mo_foo);
            mo_signal_two.connect<&Foo::slot_member_signature_two>(&mo_foo);
            REQUIRE(!mo_signal_two.is_empty()); //L"A slot was found missing.");
        }

        SECTION("Test_Const_Member_Connect")
        {
            mo_signal_one.connect<&Foo::slot_const_member_signature_one>(mo_foo);
            mo_signal_one.connect<&Foo::slot_const_member_signature_one>(&mo_foo);
            REQUIRE(!mo_signal_one.is_empty()); //L"A slot was found missing.");
            REQUIRE(!mo_foo.is_empty()); //L"A slot was found missing.");

            mo_signal_two.connect<&Foo::slot_const_member_signature_two>(mo_foo);
            mo_signal_two.connect<&Foo::slot_const_member_signature_two>(&mo_foo);
            REQUIRE(!mo_signal_two.is_empty()); //L"A slot was found missing.");
        }

        SECTION("Test_Overloaded_Member_Connect")
        {
            mo_signal_one.connect<Foo, &Foo::slot_overloaded_member>(mo_foo);
            mo_signal_one.connect<Foo, &Foo::slot_overloaded_member>(&mo_foo);
            REQUIRE(!mo_signal_one.is_empty()); //L"A slot was found missing.");
            REQUIRE(!mo_foo.is_empty()); //L"A slot was found missing.");

            mo_signal_two.connect<Foo, &Foo::slot_overloaded_member>(mo_foo);
            mo_signal_two.connect<Foo, &Foo::slot_overloaded_member>(&mo_foo);
            REQUIRE(!mo_signal_two.is_empty()); //L"A slot was found missing.");
        }

        SECTION("Test_Overloaded_Static_Member_Connect")
        {
            mo_signal_one.connect<&Foo::slot_static_member_function>();
            REQUIRE(!mo_signal_one.is_empty()); //L"A slot was found missing.");

            mo_signal_two.connect<&Foo::slot_static_member_function>();
            REQUIRE(!mo_signal_two.is_empty()); //L"A slot was found missing.");
        }

        SECTION("Test_Overloaded_Virtual_Member_Connect")
        {
            mo_signal_one.connect<Foo, &Foo::slot_virtual_member_function>(mo_foo);
            mo_signal_one.connect<Foo, &Foo::slot_virtual_member_function>(&mo_foo);
            REQUIRE(!mo_signal_one.is_empty()); //L"A slot was found missing.");
            REQUIRE(!mo_foo.is_empty()); //L"A slot was found missing.");

            mo_signal_two.connect<Foo, &Foo::slot_virtual_member_function>(mo_foo);
            mo_signal_two.connect<Foo, &Foo::slot_virtual_member_function>(&mo_foo);
            REQUIRE(!mo_signal_two.is_empty()); //L"A slot was found missing.");
        }

        SECTION("Test_Overloaded_Virtual_Derived_Member_Connect")
        {
            mo_signal_one.connect<Bar, &Bar::slot_virtual_member_function>(mo_bar);
            mo_signal_one.connect<Bar, &Bar::slot_virtual_member_function>(&mo_bar);
            REQUIRE(!mo_signal_one.is_empty()); //L"A slot was found missing.");
            REQUIRE(!mo_bar.is_empty()); //L"A slot was found missing.");

            mo_signal_two.connect<Bar, &Bar::slot_virtual_member_function>(mo_bar);
            mo_signal_two.connect<Bar, &Bar::slot_virtual_member_function>(&mo_bar);
            REQUIRE(!mo_signal_two.is_empty()); //L"A slot was found missing.");
        }

        SECTION("Test_Overloaded_Free_Function_Connect")
        {
            mo_signal_one.connect<&slot_static_free_function>();
            REQUIRE(!mo_signal_one.is_empty()); //L"A slot was found missing.");

            mo_signal_two.connect<&slot_static_free_function>();
            REQUIRE(!mo_signal_two.is_empty()); //L"A slot was found missing.");
        }
    };
}

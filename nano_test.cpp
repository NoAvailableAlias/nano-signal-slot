#include "nano_signal_slot.hpp"

#include <iostream>

struct Foo : public Nano::Observer
{
    void action() const
    {
        std::cout << "Hello, World!" << std::endl;
    }
    void action(std::size_t a) const
    {
        std::cout << __LINE__ << ", " << a << std::endl;
    }
};

void action(std::size_t a)
{
    std::cout << __LINE__ << ", " << a << std::endl;
}

int main()
{
    Foo foo;

    // Declare Nano::Signals using function signature syntax
    Nano::Signal<void()> signal_one;
    Nano::Signal<void(std::size_t)> signal_two;

    // Connect member functions to Nano::Signals
    signal_one.connect<Foo, &Foo::action>(&foo);
    signal_two.connect<Foo, &Foo::action>(&foo);

    // Connect a free function to a Nano::Signal
    signal_two.connect<action>();

    // Emit Signals
    signal_one();
    signal_two(__LINE__);

    // Disconnect a member function from a Nano::Signal
    signal_two.disconnect<Foo, &Foo::action>(&foo);

    // Disconnect a free function from a Nano::Signal
    signal_two.disconnect<action>();

    std::cout << "\n\tAfter Disconnect\n" << std::endl;

    // Emit again to test disconnect
    signal_one();
    signal_two(__LINE__);

    // Pause the screen
    std::cin.get();
}

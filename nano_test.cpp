#include "nano_signal_slot.hpp"

#include <iostream>

struct Foo : public Nano::Observer
{
    void action() const
    {
        std::cout << "Hello, World!" << std::endl;
    }
    long action(std::size_t a) const
    {
        return __LINE__ ^ a;
    }
};

long action(std::size_t a)
{
    return __LINE__ ^ a;
}

int main()
{
    Foo foo;

    // Declare Nano::Signals using function signature syntax
    Nano::Signal<void()> signal_one;
    Nano::Signal<long(std::size_t)> signal_two;

    // Connect member functions to Nano::Signals
    signal_one.connect<Foo, &Foo::action>(&foo);
    signal_two.connect<Foo, &Foo::action>(&foo);

    // Connect a free function to a Nano::Signal
    signal_two.connect<action>();

    // Emit Signals
    signal_one();

    // Emit Signals and accumulate SRVs
    signal_two.accumulate(__LINE__, [](long srv)
    {
        std::cout << srv << ", " << __LINE__ << std::endl;
    });

    // Disconnect a member function from a Nano::Signal
    signal_two.disconnect<Foo, &Foo::action>(foo);

    // Disconnect a free function from a Nano::Signal
    signal_two.disconnect<action>();

    std::cout << "\n\tAfter Disconnect\n" << std::endl;

    // Emit again to test disconnects
    signal_one();
    signal_two(__LINE__);

    // Pause the screen
    std::cin.get();
}

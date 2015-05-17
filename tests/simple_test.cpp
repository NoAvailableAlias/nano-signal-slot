#include "../nano_signal_slot.hpp"

#include <iostream>
#include <vector>

struct Foo : public Nano::Observer
{
    bool handler_a(const char* e) const
    {
        std::cout << e << std::endl;
        return true;
    }
    bool handler_b(const char* e, std::size_t n)
    {
        std::cout << e << " [on line: " << n << "]" << std::endl;
        return true;
    }
    static bool handler_c(const char* e)
    {
        std::cout << e << std::endl;
        return true;
    }
};

bool handler_d(const char* e, std::size_t n)
{
    std::cout << e << " [on line: " << n << "]" << std::endl;
    return false;
}

int main()
{
    Foo foo;

    // Declare Nano::Signals using function signature syntax
    Nano::Signal<bool(const char*)> signal_one;
    Nano::Signal<bool(const char*, std::size_t)> signal_two;

    // Connect member functions to Nano::Signals
    signal_one.connect<Foo, &Foo::handler_a>(&foo);
    signal_two.connect<Foo, &Foo::handler_b>(&foo);

    // Connect a static member function
    signal_one.connect<Foo::handler_c>();

    // Connect a free function
    signal_two.connect<handler_d>();

    // Emit Signals
    signal_one.emit("we get signal");
    signal_two.emit("main screen turn on", __LINE__);

    std::vector<bool> status;

    // Emit Signals and accumulate SRVs (signal return values)
    /*signal_one.emit_accumulate([&](bool srv)
    {
        status.push_back(srv);
    }
	,"how are you gentlemen");*/

    // Disconnect member functions from a Nano::Signal
    signal_one.disconnect<Foo, &Foo::handler_a>(foo);
    signal_two.disconnect<Foo, &Foo::handler_b>(foo);

    // Disconnect a static member function
    signal_one.disconnect<Foo::handler_c>();

    // Disconnect a free function
    signal_two.disconnect<handler_d>();

    // Emit again to test disconnects
    signal_one.emit("THIS SHOULD NOT APPEAR");
    signal_two.emit("THIS SHOULD NOT APPEAR", __LINE__);

    // Pause the screen
    std::cin.get();
}

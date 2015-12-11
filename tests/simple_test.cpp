#include "../nano_signal_slot.hpp"

#include <functional>
#include <iostream>
#include <vector>
#include <cassert>

//------------------------------------------------------------------------------

// To utilize automatic disconnect you must inherit from Nano::Observer
struct Foo : public Nano::Observer
{
    bool handler_a(const char* sl) const
    {
        std::cout << sl << std::endl;
        return true;
    }
    bool handler_b(const char* sl, std::size_t ln)
    {
        std::cout << sl << " [on line: " << ln << "]" << std::endl;
        return true;
    }
    static bool handler_c(const char* sl)
    {
        std::cout << sl << std::endl;
        return true;
    }
};

bool handler_d(const char* sl, std::size_t ln)
{
    std::cout << sl << " [on line: " << ln << "]" << std::endl;
    return false;
}

void handler_e()
{
    // Empty definition
}

//------------------------------------------------------------------------------

int main()
{
    // Declare Nano::Signals using function signature syntax
    Nano::Signal<bool(const char*)> signal_one;
    Nano::Signal<bool(const char*, std::size_t)> signal_two;

    // Test void slot types
    Nano::Signal<void()> signal_three;
    signal_three.connect<handler_e>();
    signal_three.emit();

    // Test using function objects
    std::function<bool(const char*, std::size_t)> fo;

    fo = [&](const char* sl, std::size_t ln)
    {
        std::cout << sl << " [on line: " << ln << "]" << std::endl;
        // Test indirectly disconnecting the currently emitting slot
        signal_two.disconnect(fo);
        return true;
    };

    // Create a new scope to test automatic disconnect
    {
        Foo foo;

        // Connect member functions to Nano::Signals
        signal_one.connect<Foo, &Foo::handler_a>(&foo);
        signal_two.connect<Foo, &Foo::handler_b>(&foo);

        // Signal one should not be empty
        assert(!signal_one.empty());

        // Connect a static member function
        signal_one.connect<Foo::handler_c>();

        // Connect a free function
        signal_two.connect<handler_d>();

        // Emit Signals
        signal_one.emit("we get signal");
        signal_two.emit("main screen turn on", __LINE__);

        std::vector<bool> status;

        // Emit Signals and accumulate SRVs (signal return values)
        signal_one.emit_accumulate([&](bool srv)
        {
            status.push_back(srv);
        }
        ,"how are you gentlemen");

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

        // Connecting function objects (or any object defining a suitable operator())
        signal_two.connect(&fo);

        // Test indirectly disconnecting the currently emitting slot
        signal_two.emit("indirect disconnect", __LINE__);

        // Disconnecting function objects (test convenience overload)
        signal_two.disconnect(fo);

        // Test auto disconnect
        signal_one.connect<Foo, &Foo::handler_a>(foo);

        // Test copying, (currently deleted because of issue #15)
        //Nano::Signal<void()> signal_four = signal_three;
        //signal_four.emit();

        // Test removeAll()
        signal_two.connect<Foo, &Foo::handler_b>(&foo);
        signal_two.removeAll();

        // Test multiple explicit removeAll()
        signal_two.removeAll();
    }
    // Signal one should be empty
    assert(signal_one.empty());

    // If this appears then automatic disconnect did not work
    signal_one.emit("THIS SHOULD NOT APPEAR");

    #ifdef NANO_USE_DEPRECATED

    // Test deprecated emit interface
    signal_one("we get signal");
    signal_two("main screen turn on", __LINE__);

    std::vector<bool> status;

    // Emit Signals and accumulate SRVs (signal return values)
    signal_one("how are you gentlemen", [&](bool srv)
    {
        status.push_back(srv);
    });

    #endif

    // Pause the screen
    std::cin.get();
}

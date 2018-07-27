#include <functional>
#include <iostream>
#include <vector>
#include <cassert>

// Nano::Observer will now use std::recursive_mutex
#define NANO_DEFINE_THREADSAFE_OBSERVER
// Nano::Pool_Allocator will now use atomics and std::mutex
#define NANO_DEFINE_THREADSAFE_ALLOCATOR

// #include "nano_function.hpp"         // Nano::Function, Nano::DelegateKey
// #include "nano_pool_allocator.hpp"   // Nano::Pool_Allocator
// #include "nano_noop_mutex.hpp"       // Nano::Noop_Mutex
// #include "nano_observer.hpp"         // Nano::Observer
#include "../nano_signal_slot.hpp"      // Nano::Signal / All the above

// To utilize automatic disconnect you must inherit from Nano::Observer
struct Foo : public Nano::Observer<>
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
    virtual void handler_f()
    {
        std::cout << __LINE__ << std::endl;
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
    signal_three.connect<&handler_e>();
    signal_three.fire();

    // Test using function objects
    std::function<bool(const char*, std::size_t)> fo;

    auto fo2 = [&](const char* sl, std::size_t ln)
    {
        std::cout << sl << " [on line: " << ln << "]" << std::endl;
        // Test indirectly disconnecting the currently firing slot
        signal_two.disconnect(fo);
        return true;
    };

    // Create a new scope to test automatic disconnect
    {
        Foo foo;

        fo = [&](const char* sl, std::size_t ln)
        {
            std::cout << sl << " [on line: " << ln << "]" << std::endl;
            // Test indirectly disconnecting the currently firing slot
            signal_two.disconnect(fo);
            // Test connecting slots as this slot is firing
            signal_two.connect(&fo2);
            signal_two.connect<&handler_d>();
            return true;
        };

        // Connect member functions to Nano::Signals
        signal_one.connect<&Foo::handler_a>(&foo);
        signal_two.connect<&Foo::handler_b>(&foo);

        // Signal one should not be empty
        assert(!signal_one.is_empty());

        // Connect a static member function
        signal_one.connect<&Foo::handler_c>();

        // Connect a free function
        signal_two.connect<&handler_d>();

        // Fire Signals
        signal_one.fire("we get signal");
        signal_two.fire("main screen turn on", __LINE__);

        std::vector<bool> status;

        // Fire Signals and accumulate SRVs (signal return values)
        signal_one.fire_accumulate([&](bool srv)
        {
            status.push_back(srv);
        }
        ,"how are you gentlemen");

        // Disconnect member functions from a Nano::Signal
        signal_one.disconnect<&Foo::handler_a>(foo);
        signal_two.disconnect<&Foo::handler_b>(foo);

        // Disconnect a static member function
        signal_one.disconnect<&Foo::handler_c>();

        // Disconnect a free function
        signal_two.disconnect<&handler_d>();

        // Emit again to test disconnects
        signal_one.fire("THIS SHOULD NOT APPEAR");
        signal_two.fire("THIS SHOULD NOT APPEAR", __LINE__);

        // Connecting function objects (or any object defining a suitable operator())
        signal_two.connect(&fo);
        signal_two.connect(&fo2);

        // Test indirectly disconnecting the currently firing slot
        signal_two.fire("indirect disconnect", __LINE__);

        // Disconnecting function objects (test convenience overload)
        signal_two.disconnect(fo);

        // Test auto disconnect
        signal_one.connect<&Foo::handler_a>(foo);

        // NEVERMIND COPYING WILL REMAIN DELETED

        //// Test copying (issue #15)
        //Nano::Signal<void()> signal_four = signal_three;
        //signal_four.fire();

        // Disconnect all slots
        signal_two.connect<&Foo::handler_b>(&foo);
        signal_two.remove_all();
        // This should have no output
        signal_two.fire("THIS SHOULD NOT APPEAR", __LINE__);

        // Test multiple explicit remove_all()
        signal_one.remove_all();

        // Test virtual
        signal_three.connect<&Foo::handler_f>(&foo);
        signal_three.fire();
        signal_three.disconnect<&Foo::handler_f>(&foo);
        signal_three.fire();
    }
    // Signal one should be empty
    assert(signal_one.is_empty());

    // If this appears then automatic disconnect did not work
    signal_one.fire("THIS SHOULD NOT APPEAR");

    std::cout << "TESTS COMPLETE" << std::endl;

    // Pause the screen
    std::cin.get();
}

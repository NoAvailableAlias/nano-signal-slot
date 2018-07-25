nano-signal-slot
================

Pure C++17 Signals and Slots

#### Prerequisites

_*C++17 is now required for std::aligned_union and std::shared_mutex.*_

#### Include
```
// #include "nano_function.hpp"         // Nano::Function, Nano::DelegateKey
// #include "nano_pool_allocator.hpp"   // Nano::Pool_Allocator
// #include "nano_noop_mutex.hpp"       // Nano::Noop_Mutex
// #include "nano_observer.hpp"         // Nano::Observer
#include "nano_signal_slot.hpp"         // Nano::Signal / All the above
```

#### Declare
```
// Declare Nano::Signals using function signature syntax
Nano::Signal<bool(const char*)> signal_one;
Nano::Signal<bool(const char*, std::size_t)> signal_two;
```

#### Connect
```
// Connect member functions to Nano::Signals
signal_one.connect<Foo, &Foo::handler_a>(&foo);
signal_two.connect<Foo, &Foo::handler_b>(&foo);

// Connect a static member function
signal_one.connect<Foo::handler_c>();

// Connect a free function
signal_two.connect<handler_d>();
```

#### Fire / Fire Accumulate
```
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
```

#### Disconnect

_Additionally test convenience overloads for references._

```
// Disconnect member functions from Nano::Signals
signal_one.disconnect<Foo, &Foo::handler_a>(foo);
signal_two.disconnect<Foo, &Foo::handler_b>(foo);

// Disconnect a static member function
signal_one.disconnect<Foo::handler_c>();

// Disconnect a free function
signal_two.disconnect<handler_d>();
```

#### Connection Management

_To utilize automatic connection management you must inherit from Nano::Observer._

```
struct Foo : public Nano::Observer
{
    bool handler_a(const char* sl) const
    {
        std::cout << sl << std::endl;
        return true;
    }
	...
```

#### Function Objects

_*Must guarantee that object lifetimes are compatible.*_

```
// Test using function objects
auto fo = [&](const char* sl)
{
    std::cout << sl << std::endl;
    return true;
};

...

// Connecting function objects (or any object defining a suitable operator())
signal_one.connect(&fo);

...

// Disconnecting function objects (convenience overload is used here)
signal_one.disconnect(fo);
```

### Preprocessor Definitions
```
// Nano::Observer will now use std::shared_mutex
#define NANO_DEFINE_THREADSAFE_OBSERVER
// Nano::Pool_Allocator will now use atomics and std::mutex
#define NANO_DEFINE_THREADSAFE_ALLOCATOR
```

#### IMPORTANT

_Some aspects to consider when using nano-signal-slot._

1. ONLY unique Delegates can be connected to a single Signal instance.
  * _Attempting to connect the same Delegate to a Signal is not an error._  
2. ORDER is not maintained when connecting Delegates to Signals.
  * _Adding "fun_a" followed by "fun_b" could result in either being fired first._  
3. DANGER awaits anyone attempting to remove Delegates from Signals from within a Delegate.
  * _It is not nice to remove a connection as the connections are being iterated._  
  * _Only a no cost mitigation is currently in nano-signal-slot to prevent this issue._  
4. COPYING is weird. That is all.

#### Links

*Benchmarks contain both the old nano-signal-slot v1.x scores as well as the v2.x scores.*

| [Performance](https://github.com/NoAvailableAlias/signal-slot-benchmarks/tree/master/#performance) | [Metrics](https://github.com/NoAvailableAlias/signal-slot-benchmarks/tree/master/#metrics) | [Benchmark Algorithms](https://github.com/NoAvailableAlias/signal-slot-benchmarks/tree/master/#benchmark-algorithms) |
|:-------------------------------------------------------------------------------------------------- |:------------------------------------------------------------------------------------------:|:--------------------------------------------------------------------------------------------------------------------:|

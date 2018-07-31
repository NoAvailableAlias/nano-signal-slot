nano-signal-slot
================

Pure C++17 Signals and Slots

#### Include
```
// #include "nano_function.hpp"         // Nano::Function, Nano::DelegateKey
// #include "nano_pool_allocator.hpp"   // Nano::Pool_Allocator
// #include "nano_noop_mutex.hpp"       // Nano::Noop_Mutex
// #include "nano_observer.hpp"         // Nano::Observer
#include "nano_signal_slot.hpp"         // Nano::Signal
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
signal_one.connect<&Foo::slot_member_one>(foo);
signal_two.connect<&Foo::slot_member_two>(foo);

// Connect overloaded member functions (required template syntax)
signal_one.connect<Foo, &Foo::slot_overloaded_member>(foo);
signal_two.connect<Foo, &Foo::slot_overloaded_member>(foo);

// Connect a static member function
signal_one.connect<&Foo::slot_static_member_one>();

// Connect a free function
signal_two.connect<&slot_free_function_one>();
```

#### Fire / Fire Accumulate
```
// Fire Signals
signal_one.fire("we get signal");
signal_two.fire("main screen turn on", __LINE__);

std::vector<bool> statuses;
auto accumulator = [&](bool srv)
{
    statuses.push_back(srv);
};

// Fire Signals and accumulate SRVs (signal return values)
signal_one.fire_accumulate(accumulator, "how are you gentlemen");
```

#### Disconnect
```
// Disconnect member functions from Nano::Signals
signal_one.disconnect<&Foo::slot_member_one>(foo);
signal_two.disconnect<&Foo::slot_member_two>(foo);

// Connect overloaded member functions (required template syntax)
signal_one.disconnect<Foo, &Foo::slot_overloaded_member>(foo);

// Disconnect a static member function
signal_one.disconnect<&Foo::slot_static_member_one>();

// Disconnect a free function
signal_two.disconnect<&slot_free_function_one>();

// Disconnect all slots
signal_two.remove_all();
```

#### Connection Management

_Automatic connection management requires public inheritance from Nano::Observer<>._

```
struct Foo : public Nano::Observer<>
{
    bool slot_member_one(const char* sl) const
    {
        std::cout << sl << std::endl;
        return true;
    }
	...
```

#### Function Objects

_*Connected function objects must live longer than the connected signal.*_

```
auto fo = [&](const char* sl)
{
    std::cout << sl << std::endl;
    return true;
};

...

// Connect any object that defines a suitable operator()
signal_one.connect(fo);

...

// Disconnect that same functor instance
signal_one.disconnect(fo);
```

#### Preprocessor Definitions
```
// Nano::Observer will now use std::recursive_mutex
#define NANO_DEFINE_THREADSAFE_OBSERVER
// Nano::Pool_Allocator will now use atomics and std::mutex
#define NANO_DEFINE_THREADSAFE_ALLOCATOR
```

### IMPORTANT

1. ONLY unique Slots can be connected to a Signal instance.
  * _Attempting to connect the same Slot to a Signal is not an error._  
2. ORDER of firing is not guaranteed when connecting Delegates to Signals.
  * _Adding "slot_a" followed by "slot_b" could result in either being fired first._  

#### Links

*Benchmarks contain both the old nano-signal-slot v1.x scores as well as the v2.x scores.*

| [Performance](https://github.com/NoAvailableAlias/signal-slot-benchmarks/tree/master/#performance) | [Metrics](https://github.com/NoAvailableAlias/signal-slot-benchmarks/tree/master/#metrics) | [Benchmark Algorithms](https://github.com/NoAvailableAlias/signal-slot-benchmarks/tree/master/#benchmark-algorithms) | [Unit Tests](https://github.com/NoAvailableAlias/nano-signal-slot/tree/master/tests/#unit-tests) |
|:-------------------------------------------------------------------------------------------------- |:------------------------------------------------------------------------------------------:|:--------------------------------------------------------------------------------------------------------------------:|:------------------------------------------------------------------------------------------------:|

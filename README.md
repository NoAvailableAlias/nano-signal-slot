nano-signal-slot
================

Pure C++11 Signals and Slots

#### Include
```
// #include "nano_function.hpp" // Nano::Function, Nano::DelegateKey
// #include "nano_observer.hpp" // Nano::Observer
#include "nano_signal_slot.hpp" // Include all
```

#### Declare
```
// Declare Nano::Signals using function signature syntax
Nano::Signal<bool(const char*)> signal_one;
Nano::Signal<bool(const char*, std::size_t)> signal_two;
```

#### Connect

```
// Connect member functions to Nano::signals;
signal_one.connect<Foo, &Foo::handler_a>(&foo);
signal_two.connect<Foo, &Foo::handler_b>(&foo);

// Connect a static member function
signal_one.connect<Foo::handler_c>();

// Connect a free function
signal_two.connect<handler_d>();
```

#### Emit

_Additionally test signal return value support._

```
// Emit Signals
signal_one("we get signal");
signal_two("main screen turn on", __LINE__);

std::vector<bool> status;

// Emit Signals and accumulate SRVs (signal return values)
signal_one("how are you gentlemen", [&](bool srv)
{
	status.push_back(srv);
});
```

#### Disconnect

_Additionally test convenience overloads for references._

```
// Disconnect member functions from a Nano::Signal
signal_one.disconnect<Foo, &Foo::handler_a>(foo);
signal_two.disconnect<Foo, &Foo::handler_b>(foo);

// Disconnect a static member function
signal_one.disconnect<Foo::handler_c>();

// Disconnect a free function
signal_two.disconnect<handler_c>();
```

#### Connection Management

_To utilize automatic connection management you must inherit from Nano::Observer_

```
struct Foo : public Nano::Observer
{
    bool handler_a(const char* e) const
    {
        std::cout << e << std::endl;
        return true;
    }
	...
```

Performance
-----------

**_Higher score is better._** _N / milliseconds per sample._

```
+ ------------------------------------------------------------------------- +
| Library           | construct | destruct | connect  | emit     | combined |
+ ------------------------------------------------------------------------- +
| Jl_signal         | 21264.33  | 4909.49  | 37937.84 | 38921.39 | 3519.61  |
| Nano-signal-slot  | 1088.15   | 583.51   | 467.42   | 25978.73 | 200.12   |
| Boost Signals     | 749.66    | 145.48   | 42.71    | 3802.59  | 28.96    |
| Boost Signals2    | 476.02    | 215.67   | 126.20   | 3025.00  | 66.19    |
| EvilTwin Observer | 22412.09  | 367.78   | 151.76   | 17171.24 | 94.64    |
| amc522 Signal11   | 26091.52  | 543.57   | 421.58   | 30927.73 | 251.47   |
+ ------------------------------------------------------------------------- +
```

#### Notes

Trying to profile why construction time is so lousy.
Working on refactoring benchmarks and report generation.

nano-signal-slot
================

Pure C++11 Signals and Slots

#### Include
```
// #include "nano_function.hpp" // Nano::Function, Nano::DelegateKey
// #include "nano_observer.hpp" // Nano::Observer
#include "nano_signal_slot.hpp" // Nano::Signal / All the above
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
// Disconnect member functions from Nano::Signals
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

_Jl_signal uses a custom static allocator to achieve high performance._

```
+ --------------------------------------------------------------------------- +
| Library             | construct | destruct | connect  | emission | combined |
+ --------------------------------------------------------------------------- +
| winglot Signals     | 470.81    | 329.68   | 246.02   | 31944.04 | 96.85    |
| supergrover sigslot | 918.46    | 254.33   | 148.30   | 37727.78 | 81.25    |
| pbhogan Signals     | 16399.81  | 553.96   | 269.53   | 29835.24 | 173.83   |
| Jl_signal           | 15296.13  | 4025.32  | 40392.97 | 39481.30 | 3177.38  |
| Nano-signal-slot    | 799.94    | 556.61   | 431.29   | 26245.77 | 174.92   |
| Boost Signals       | 630.12    | 147.00   | 49.39    | 3217.17  | 30.99    |
| Boost Signals2      | 412.91    | 215.09   | 136.11   | 2984.64  | 67.29    |
| EvilTwin Observer   | 17271.50  | 355.72   | 150.76   | 16164.93 | 94.39    |
| EvilTwin Obs Fork   | 19876.86  | 387.07   | 166.73   | 16128.42 | 101.95   |
| EvilTwin Obs Safe   | 15401.88  | 364.32   | 157.25   | 347.11   | 60.92    |
| amc522 Signal11     | 24835.87  | 708.51   | 276.50   | 29602.83 | 178.17   |
+ --------------------------------------------------------------------------- +
```

#### Notes

Trying to profile why construction time is so lousy. Also, it seems odd to me that Nano-signal-slot
performs better in the destruction benchmark than a majority of others considering the complexity.

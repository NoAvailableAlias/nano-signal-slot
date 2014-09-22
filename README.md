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
| winglot Signals     | 439.66    | 271.25   | 226.25   | 28643.18 | 97.85    |
| supergrover sigslot | 916.06    | 252.85   | 162.25   | 41284.94 | 84.25    |
| pbhogan Signals     | 18771.80  | 496.66   | 270.05   | 26793.79 | 176.05   |
| Jl_signal           | 12575.26  | 3862.32  | 31880.96 | 31977.62 | 3185.01  |
| Nano-signal-slot    | 761.69    | 504.66   | 362.85   | 23792.49 | 179.45   |
| Boost Signals       | 513.07    | 152.05   | 51.45    | 4012.94  | 35.65    |
| Boost Signals2      | 434.66    | 225.85   | 94.05    | 3517.20  | 64.45    |
| EvilTwin Observer   | 12198.71  | 318.06   | 156.05   | 16857.98 | 89.85    |
| EvilTwin Obs Fork   | 11107.54  | 288.25   | 158.45   | 15775.01 | 103.85   |
| EvilTwin Obs Safe   | 13377.51  | 302.85   | 162.65   | 271.45   | 62.25    |
| amc522 Signal11     | 19439.56  | 764.87   | 304.05   | 24487.60 | 172.45   |
+ --------------------------------------------------------------------------- +
```

#### Notes

Trying to profile why construction time is so lousy.

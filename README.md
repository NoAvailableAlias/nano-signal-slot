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
| amc522 Signal11     | 24835.87  | 708.51   | 276.50   | 29602.83 | 178.17   |
+ --------------------------------------------------------------------------- +
```

Allocator
---------

To utilize allocators in Nano-signal-slot, the only change needed is the following:

Change
```
    std::map<DelegateKey, Observer*> tracked_connections;
```
To
```
    using Allocator = YourAllocator<std::map<DelegateKey, Observer*>::value_type>;
    std::map<DelegateKey, Observer*, std::less<DelegateKey>, Allocator> tracked_connections;
```
_Note that YourAllocator must support the standard allocator model._

Performance after plugging in a quick non-optimal pool allocator:

```
+ -------------------------------------------------------------------------------- +
| Library             |  construct |  destruct |  connect  |  emission |  combined |
+ -------------------------------------------------------------------------------- +
| Jl_signal           |  9173.90   |  5550.51  |  38883.14 |  41184.04 |  2991.05  |
| Nano-signal-slot    |  7218.18   |  4266.97  |  7709.92  |  26951.28 |  2057.92  |
| supergrover sigslot |  942.15    |  221.36   |  124.91   |  37879.77 |  55.63    |
| amc522 Signal11     |  3782.44   |  571.93   |  240.03   |  32860.31 |  136.79   |
| pbhogan Signals     |  7539.47   |  476.00   |  263.59   |  25958.57 |  131.99   |
| winglot Signals     |  373.53    |  275.41   |  247.12   |  28986.38 |  79.69    |
| EvilTwin Observer   |  6782.33   |  306.73   |  70.25    |  20330.63 |  48.23    |
| EvilTwin Obs Fork   |  3120.83   |  285.91   |  77.89    |  17777.30 |  55.03    |
| Boost Signals       |  266.99    |  106.83   |  28.87    |  3770.69  |  17.95    |
| Boost Signals2      |  260.35    |  167.05   |  72.05    |  3045.77  |  37.19    |
| neosigslot          |  853.81    |  321.31   |  157.54   |  925.89   |  61.95    |
+ -------------------------------------------------------------------------------- +
```

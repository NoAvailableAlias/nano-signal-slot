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

```
+ -------------------------------------------------------------------------------- +
| Library             |  construct |  destruct |  connect  |  emission |  combined |
+ -------------------------------------------------------------------------------- +
| Jl_signal           |  13621.97  |  6267.07  |  43055.01 |  41029.04 |  3560.67  |
| amc522 Signal11     |  26042.47  |  902.47   |  572.37   |  32892.97 |  300.83   |
| EvilTwin Obs Fork   |  24312.01  |  405.96   |  209.87   |  19649.00 |  101.13   |
| pbhogan Signals     |  10563.03  |  529.66   |  261.47   |  29598.50 |  160.63   |
| EvilTwin Observer   |  21806.37  |  358.08   |  151.01   |  18483.56 |  97.41    |
| supergrover sigslot |  812.13    |  219.87   |  99.69    |  36957.01 |  57.67    |
| * winglot Signals   |  330.58    |  291.75   |  203.65   |  29737.04 |  73.87    |
| Nano-signal-slot    |  1026.58   |  482.56   |  278.72   |  24285.34 |  125.63   |
| Boost Signals       |  714.46    |  144.81   |  45.73    |  4208.69  |  31.27    |
| * Boost Signals2    |  800.15    |  268.77   |  129.43   |  2964.32  |  69.09    |
| * neosigslot        |  1064.29   |  373.64   |  164.65   |  736.85   |  75.07    |
+ -------------------------------------------------------------------------------- +
```
_* Library aims to be thread safe._

Allocator
---------

To utilize allocators in Nano-signal-slot, the only change needed is the following:

```
    // Change
    std::map<DelegateKey, Observer*> tracked_connections;
```
```
    // To
    using Allocator = YourAllocator<std::map<DelegateKey, Observer*>::value_type>;
    std::map<DelegateKey, Observer*, std::less<DelegateKey>, Allocator> tracked_connections;
```

#### Performance using an Allocator

_Jl_signal uses a custom static allocator to achieve high performance._

```
+ -------------------------------------------------------------------------------- +
| Library             |  construct |  destruct |  connect  |  emission |  combined |
+ -------------------------------------------------------------------------------- +
| Jl_signal           |  14424.43  |  6711.48  |  43139.61 |  39783.57 |  3940.62  |
| Nano-signal-slot    |  26513.30  |  5376.19  |  8179.32  |  27180.79 |  2597.05  |
| amc522 Signal11     |  27608.88  |  934.51   |  598.57   |  31598.73 |  314.26   |
| EvilTwin Obs Fork   |  24071.42  |  403.41   |  192.20   |  19081.48 |  100.06   |
| EvilTwin Observer   |  22076.84  |  362.93   |  158.25   |  19055.19 |  95.82    |
| pbhogan Signals     |  10091.72  |  539.62   |  266.03   |  28799.46 |  161.86   |
| supergrover sigslot |  825.57    |  218.69   |  105.17   |  38399.19 |  57.70    |
| * winglot Signals   |  336.90    |  295.14   |  201.78   |  31580.88 |  73.60    |
| Boost Signals       |  744.24    |  148.17   |  44.89    |  4365.43  |  31.07    |
| * Boost Signals2    |  790.10    |  233.38   |  139.09   |  3285.30  |  68.57    |
| * neosigslot        |  1251.39   |  379.12   |  158.28   |  756.85   |  77.47    |
+ -------------------------------------------------------------------------------- +
```
_* Library aims to be thread safe._

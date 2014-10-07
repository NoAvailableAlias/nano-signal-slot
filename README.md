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
signal_two.disconnect<handler_d>();
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
| Jl_signal           |  145161.97 |  8828.51  |  47577.79 |  39814.73 |  5997.68  |
| amc522 Signal11     |  149452.90 |  5575.79  |  3640.45  |  32080.56 |  2042.84  |
| pbhogan Signals     |  133265.63 |  4150.35  |  3406.87  |  30616.29 |  1794.07  |
| EvilTwin Obs Fork   |  129236.90 |  3153.31  |  1735.39  |  18904.88 |  1046.02  |
| EvilTwin Observer   |  123673.13 |  2148.93  |  1094.75  |  19242.94 |  708.81   |
| supergrover sigslot |  13330.19  |  1183.43  |  1977.58  |  38502.43 |  668.60   |
| Nano-signal-slot    |  12493.60  |  3899.84  |  3828.53  |  28263.30 |  1631.76  |
| * winglot Signals   |  5564.77   |  1856.80  |  2322.04  |  29981.18 |  853.71   |
| * neosigslot        |  12329.68  |  2294.43  |  2067.94  |  6611.30  |  854.06   |
| Boost Signals       |  8915.83   |  1670.17  |  556.03   |  4226.06  |  363.13   |
| * Boost Signals2    |  6716.64   |  1840.39  |  798.68   |  3174.60  |  439.43   |
+ -------------------------------------------------------------------------------- +
```

Allocator
---------

To utilize allocators in Nano-signal-slot, the only change required is the following:

```
    // Before
    std::map<DelegateKey, Observer*> tracked_connections;
```
```
    // After
    using Allocator = YourAllocator<std::map<DelegateKey, Observer*>::value_type>;
    std::map<DelegateKey, Observer*, std::less<DelegateKey>, Allocator> tracked_connections;
```

#### Performance using an Allocator

_Jl_signal uses a custom static allocator to achieve high performance._

```
+ -------------------------------------------------------------------------------- +
| Library             |  construct |  destruct |  connect  |  emission |  combined |
+ -------------------------------------------------------------------------------- +
| Jl_signal           |  142583.31 |  9441.12  |  47558.71 |  39957.49 |  6207.17  |
| amc522 Signal11     |  149475.55 |  5652.26  |  3684.45  |  32263.83 |  2057.11  |
| pbhogan Signals     |  130972.16 |  4089.34  |  3444.45  |  30025.49 |  1766.30  |
| EvilTwin Obs Fork   |  132207.76 |  3212.45  |  1735.47  |  19003.71 |  1023.43  |
| EvilTwin Observer   |  118959.42 |  2114.94  |  1102.22  |  18918.39 |  704.48   |
| Nano-signal-slot    |  70201.91  |  7999.57  |  8382.54  |  28353.72 |  3605.56  |
| supergrover sigslot |  13067.66  |  1157.52  |  1991.76  |  39613.48 |  663.45   |
| * winglot Signals   |  5811.09   |  1953.51  |  2436.36  |  32844.27 |  895.75   |
| * neosigslot        |  12341.82  |  2282.84  |  2059.40  |  6416.81  |  850.81   |
| Boost Signals       |  9095.11   |  1686.62  |  544.76   |  4341.32  |  359.59   |
| * Boost Signals2    |  6649.52   |  1873.46  |  809.35   |  3194.13  |  444.24   |
+ -------------------------------------------------------------------------------- +
```
_* Library aims to be thread safe._

Release Build Size
------------------

| Library | Object File Size |
| ------- | ---------------- |
| Jl_signal | 846 kb |
| winglot Signals | 904 kb |
| Nano-signal-slot | 908 kb |
| EvilTwin Obs Fork | 955 kb |
| supergrover sigslot | 966 kb |
| pbhogan Signals | 967 kb |
| EvilTwin Observer | 973 kb |
| amc522 Signal11 | 981 kb |
| Boost Signals | 1,375 kb |
| neosigslot | 1,940 kb |
| Boost Signals2 | 2,350 kb |

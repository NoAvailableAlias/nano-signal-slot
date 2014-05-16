nano-signal-slot
================

Pure C++11 Signals and Slots

#### Declare
```
// Declare Nano::Signals using function signature syntax
Nano::Signal<void()> signal_one;
Nano::Signal<long(std::size_t)> signal_two;
```

#### Connect

_The correct overload is selected based on the Nano::Signal template signature._

```
// Connect member functions to Nano::signals;
signal_one.connect<Foo, &Foo::action>(&foo);
signal_two.connect<Foo, &Foo::action>(&foo);

// Connect a free function to a Nano::Signal
signal_two.connect<action>();
```

#### Emit

_The supplied lambda can be much more complex than the example shown._

```
// Emit Signals
signal_one();
signal_two(__LINE__);

// Emit Signals and accumulate SRVs (signal return values)
signal_two.accumulate(__LINE__, [](long srv)
{
	std::cout << srv << ", " << __LINE__ << std::endl;
});
```

#### Disconnect

_Additionally test the convenience overload for references._

```
// Disconnect a member function from a Nano::Signal
signal_two.disconnect<Foo, &Foo::action(foo);

// Disconnect a free function from a Nano::Signal
signal_two.disconnect<action>();
```

#### Connection Management

_To utilize automatic connection management you must inherit from Nano::Observer_

```
struct Foo : public Nano::Observer
{
    void action() const
    {
        std::cout << "Hello, World!" << std::endl;
    }
	...
```

Performance
-----------

_*Higher score is better*_. _N / milliseconds per sample._

```
    Boost Signals: (deprecated)
 ++ ____________________________ construction: 326.21
 ++ _____________________________ destruction: 121.02
 ++ ______________________________ connection: 33.79
 ++ ________________________________ emission: 3780.28
 ++ ________________________________ combined: 23.15

    Boost Signals2:
 ++ ____________________________ construction: 563.19
 ++ _____________________________ destruction: 219.17
 ++ ______________________________ connection: 118.44
 ++ ________________________________ emission: 2871.86
 ++ ________________________________ combined: 63.00

    Nano-signal-slot:
 ++ ____________________________ construction: 1531.24
 ++ _____________________________ destruction: 592.65
 ++ ______________________________ connection: 524.19
 ++ ________________________________ emission: 24807.65
 ++ ________________________________ combined: 202.63

    EvilTwin Observer:
 ++ ____________________________ construction: 8771.42
 ++ _____________________________ destruction: 330.50
 ++ ______________________________ connection: 117.09
 ++ ________________________________ emission: 14300.20
 ++ ________________________________ combined: 78.71

    Jl_signal: (static allocator)
 ++ ____________________________ construction: 28809.69
 ++ _____________________________ destruction: 4958.82
 ++ ______________________________ connection: 29102.97
 ++ ________________________________ emission: 37238.94
 ++ ________________________________ combined: 3601.91

    amc522 Signal11:
 ++ ____________________________ construction: 4108.51
 ++ _____________________________ destruction: 461.25
 ++ ______________________________ connection: 261.12
 ++ ________________________________ emission: 24959.07
 ++ ________________________________ combined: 182.35
 ```

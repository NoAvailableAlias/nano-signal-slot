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

_Additionally use the convenience overload for references._

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

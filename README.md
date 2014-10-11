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

#### Function Objects

_Because of possible misuse, function objects will not be supported by default._

```
... // add the following to Nano::Function<T_rv(Args...)>

    template <typename L>
    static inline Function bind(L* pointer)
    {
        return { pointer, [](void *this_ptr, Args... args) {
        return (static_cast<L*>(this_ptr)->operator()(args...)); }};
    }
```
```
... // add the following to Nano::Signal<T_rv(Args...)>

    template <typename L>
    void connect(L* instance)
    {
        Observer::insert(Function::template bind(instance));
    }
...
    template <typename L>
    void disconnect(L* instance)
    {
        Observer::remove(Function::template bind(instance));
    }
```

#### Notes

| [Performance](https://github.com/NoAvailableAlias/nano-signal-slot/tree/master/benchmark#performance) | [Build Sizes](https://github.com/NoAvailableAlias/nano-signal-slot/tree/master/benchmark#size-metrics) |
|:----------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------:|

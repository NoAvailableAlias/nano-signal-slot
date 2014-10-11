jl_signal
=========

For all your [Observer pattern](http://en.wikipedia.org/wiki/Observer_pattern) needs, a reasonably fast and tidy C++ implementation of signals & slots. For a discussion on my design decisions, [please see this article](http://hoyvinglavin.com/2012/08/06/jl_signal/).

[There](http://doc.trolltech.com/signalsandslots.html) [are](http://www.boost.org/libs/signals/) [lots](http://sigslot.sourceforge.net/) [and](http://libsigc.sourceforge.net/) [lots](https://github.com/pbhogan/Signals) of C++ signals & slots systems out there. This one gives you the following:

#### Simple API

The library has a minimal initialization step, but after that, it's little more than calling `Connect()` and `Emit()` on your signals and observer objects. Use of template syntax and preprocessor macros in the API is kept to a minimum.

#### No heap allocation

By default, he internal allocation system bypasses the heap completely, so you don't need to worry about slow allocations or memory fragmentaton.

#### Automatic signal disconnection

In order to prevent dangling pointers, the system will automatically break connections between signals and observers when the observer objects go out of scope. This saves from you the considerable headache of having to disconnect your observers manually.

#### Fast dispatch from signals to observers

Signals are implemented with Don Clugston's [FastDelegate](http://www.codeproject.com/Articles/7150/Member-Function-Pointers-and-the-Fastest-Possible) library, which can execute arbitrary callbacks as fast as is theoretically possible. The library is famous for its non-standard under-the-hood hacks, but [should work](#compatibility) for most mainstream modern compilers.

#### No external dependencies

The library is mostly self-contained, with minimal dependencies on the standard library. The only external dependency, `FastDelegate.h`, is bundled with the project source.

##### Some caveats:

- Signals can only connect to functions with a `void` return type.
- The classes in this library are not thread-safe.
- This library does require inheritance for observers, but it's almost completely unobtrusive.

### What exactly does this do?

A **signal** is an object that can invoke an arbitrary list of connected functions. These functions are commonly referred to as **slots**, although the `jl_signal` API does not use the term explicitly. Slot functions are **connected** to a signal, and when you **emit** the signal, each connected slot function will be called. Under the hood, it's not much more than iterable containers of function pointers.

Signals aren't very useful unless you can pass arguments when you're triggering them. The nice thing about `jl_signal` is that you can use native function syntax to specify your slot parameters, rather than wrapping the parameters in a specialized, inflexible `struct`, as is the case with many C/C++ message-passing or event listener systems.

Usage example
-------------

```c++
#include <iostream> // for output

#include "Signal.h" // base library
#include "StaticSignalConnectionAllocators.h" // some default allocators for library initialization

// A class whose instances will receive signals.
class Orc : public jl::SignalObserver
{
public:
    // This method will be used as a SLOT.
    void Retort() { std::cout << "GRUMBLE GRUMBLE GRUMBLE...\n"; }
    
    // Another slot, this one taking parameters.
    void TakeDamage( float fDamage )
    {
        if (fDamage >= 20.f) std::cout << "Orc down!\n";
    }
};

// Another class whose instances receive signals.
class HipsterBystander : public jl::SignalObserver
{
public:
    void SmugComment()
    {
        std::cout << "Whatever, I think the first movie was better.\n";
    }
};

// Another class whose instances receive signals.
class Prop : public jl::SignalObserver
{
public:
    void TakeDamage( float fDamage )
    {
        if (fDamage >= 10.f) std::cout << "SMASH!\n";
    }
};

// A class who instances broadcast signals.
class Wizard
{
public:
    // Declare signal that connects to functions that take no args.
    JL_SIGNAL() BattleCrySignal;
    
    // Declare a signal that connects to functions with one float arg.
    JL_SIGNAL( float ) MysticalShotgunSignal;
    
    void BattleCry()
    {
        std::cout << "This is my boomstick!\n";
        BattleCrySignal.Emit();
    };
    
    void FireMysticalShotgun( float fDamage )
    {
        std::cout << "BLAM!\n";
        MysticalShotgunSignal.Emit( fDamage );
    }
};

int main()
{
    // Instantiate some allocators used by the signal system.
    enum { eMaxConnections = 50 };
    jl::StaticSignalConnectionAllocator< eMaxConnections > oSignalConnectionAllocator;
    jl::StaticObserverConnectionAllocator< eMaxConnections > oObserverConnectionAllocator;

    // Initialize the signal system with our allocators
    jl::SignalBase::SetCommonConnectionAllocator( &oSignalConnectionAllocator );
    jl::SignalObserver::SetCommonConnectionAllocator( &oObserverConnectionAllocator );

    // Instantiate our entities.
    Orc rosencrantz, guildenstern;
    HipsterBystander chad;
    Prop chair;
    Wizard merlin;

    // Orcs and hipster bystanders respond to battle cries
    merlin.BattleCrySignal.Connect( &rosencrantz, &Orc::Retort );
    merlin.BattleCrySignal.Connect( &guildenstern, &Orc::Retort );
    merlin.BattleCrySignal.Connect( &chad, &HipsterBystander::SmugComment );

    // Orcs and props take damage
    merlin.MysticalShotgunSignal.Connect( &rosencrantz, &Orc::TakeDamage );
    merlin.MysticalShotgunSignal.Connect( &guildenstern, &Orc::TakeDamage );    
    merlin.MysticalShotgunSignal.Connect( &chair, &Prop::TakeDamage );

    // Emit a signal
    merlin.BattleCry();

    // Output:
    // This is my boomstick! - merlin, Wizard::BattleCry()
    // GRUMBLE GRUMBLE GRUMBLE... - rosencrantz, Orc::Retort()
    // GRUMBLE GRUMBLE GRUMBLE... - guildenstern, Orc::Retort()
    // Whatever, I think the first movie was better. chad, HipsterBystander::SmugComment()

    // Emit another signal
    merlin.FireMysticalShotgun( 20.f );

    // Output:
    // BLAM! - merlin, Wizrd::FireMysticalShotgun()
    // Orc down! - rosencrantz, Orc::TakeDamage()
    // Orc down! - guildenstern, Orc::TakeDamage()
    // SMASH! - chair, Prop::TakeDamage()

    return 0;
}
```

Usage notes
-----------

### Default initialization

Both signal and observer objects need to be initialized with a `jl::ScopedAllocator`. You can set the allocator reference at the class level, or on a per-object basis.

##### Using the built-in allocators

```c++
#include "StaticSignalConnectionAllocators.h"

enum { eMaxConnections = 50 };
jl::StaticSignalConnectionAllocator< eMaxConnections > g_oSignalConnectionAllocator;
jl::StaticObserverConnectionAllocator< eMaxConnections > g_oObserverConnectionAllocator;

void SomeInitializationFunction()
{
    ...
    jl::SignalBase::SetCommonConnectionAllocator( &g_oSignalConnectionAllocator );
    jl::SignalObserver::SetCommonConnectionAllocator( &g_oObserverConnectionAllocator );
    ...
}
```

`StaticSignalConnectionsllocators.h` provides fast, purpose-built allocators that can be declared statically. These allocator classes require you to know the maximum number of connections at compile time, although it's fairly trivial to adapt these pools to use runtime-allocated memory buffers instead.

For many simple applications, you can just declare your connection allocators in the global scope.

### Declaring signals

A signal object must be declared with a list of argument types. Slot functions whose parameter lists match the signal declaration can connect to the signal.

There are three ways to declare a signal:

```c++
// 1. Macro declaration, with a comma-separated list of argument types.
JL_SIGNAL( int, float, const char* ) oSignal;

// 2. Template declaration, with an inferred parameter count. The JL_SIGNAL() macro expands to this form.
jl::Signal< void(int, float, const char*) > oSignal;

// 3. Template declaration, with an explicit parameter count in the typename.
jl::Signal3< int, float, const char* > oSignal;
```

Signals can be declared for signatures with up to eight formal parameters. [Variadic functions](http://en.wikipedia.org/wiki/Variadic_function) are not supported.

### Connecting signals to functions

You can connect signals to any function with a `void` return type, as long as the function's parameter list matches the signal's declared parameter types:

```c++
// OK
JL_SIGNAL( int, float, const char* ) oSignal;
void HandleTransaction( int nId, float fValue, const char* pUsername );
oSignal.Connect( &HandleTransaction );

// NOT OK: doesn't return void
int HandleTransaction( int nId, float fValue, const char* pUsername );

// NOT OK: parameter types don't match signal declaration
void HandleTransaction( int nId, double fValue, const char* pUsername );
```

Note that by default, there is nothing stopping you from connecting the same function twice. If double-connections are a bad thing in your application, then you can uncomment the definition of `JL_SIGNAL_ASSERT_ON_DOUBLE_CONNECT` in `Signal.h` to trigger assertion failures when the same connection is made twice. This is potentially quite slow, so it's best used for debugging purposes only.

### Connecting signals to instance methods

To connect a class's instance methods to a signal, you must derive the class from `jl::SignalObserver`. This guarantees that connections will be broken between signals and connected objects that have gone out of scope.

As with non-instance functions, instance methods that connect to a signal must have a `void` return type. Signals do not discriminate over constness, so both `const` and non-`const` instance methods may be connected to the same signal.

```c++
JL_SIGNAL( int, float, const char* ) oSignal;

class TransactionManager : public jl::SignalObserver
{
public:
    void HandleTransaction( int nId, float fValue, const char* pUsername );
};

class Logger : public jl::SignalObserver
{
public:
    void LogTransaction( int nId, float fValue, const char* pUsername ) const; 
};

TransactionManager* pTransactionManager = new TransactionManager;
const Logger* pLogger = new pLogger;

// Connect an object and non-const instance method to a signal
oSignal.Connect( pTransactionManager, &TransactionManager::HandleTransaction );

// Connect an object and const instance method to a signal
oSignal.Connect( pLogger, &Logger::LogTransaction );
```

### Emitting signals

To emit a signal, simply call the `Emit()` method on the signal with arguments appropriate to the signal's parameter declaration:

```c++
JL_SIGNAL( int, float, const char* ) oSignal;
oSignal.Emit( 5, 40.f, "hello world!" );

// You can also use operator(), which aliases Emit().
oSignal( 5, 40.f, "hello world!" );
```

Connected functions will be called in the order that they were connected.

### Disconnection

You don't need to manage signal disconnection when objects with connected instance methods go out of scope. The `jl::SignalObserver` base class ensures that any pointers to observers will be properly cleaned up.

Several methods are available for manually disconnecting functions and observers from signals:

```c++
// Declare a signal
JL_SIGNAL() oSignal;

// Connect a non-instance function
void DoSomethingGlobally();
oSignal.Connect( &DoSomethingGlobally );

// Disconnect non-instance functions
oSignal.Disconnect( &DoSomethingGlobally );

// Connect instance methods
class Foo : public jlSignalObserver
{
public:
    void Bar();
};

Foo oFoo;
oSignal.Connect( &oFoo, &Foo::Bar );

// Disconnect instance method
oSignal.Disconnect( &oFoo, &Foo::Bar );

// Disconnect all instance methods for an observer
oSignal.Disconnect( &oFoo );

// Disconnect all connected functions and observers
oSignal.DisconnectAll();
```

### Customized allocation schemes

While the default allocators set globally should be enough for most applications, you can write adapters for whatever allocation scheme you like. The *reductio ad absurdum* case would be to wrap `malloc`, which would look something like the following:

```c++
#include <cstdlib>

class MallocAllocator : public jl::ScopedAllocator
{
public:
    virtual void* Alloc( size_t nBytes ) { return malloc( nBytes ); }
    virtual void Free( void* pObject ) { free( pObject ); }
};

MallocAllocator g_oMallocAllocator;

void SomeInitializationFunction()
{
    ...
    jl::SignalBase::SetCommonConnectionAllocator( &g_oMallocAllocator );
    jl::SignalObserver::SetCommonConnectionAllocator( &g_oMallocAllocator );
    ...
}
```

##### Per-object allocators

I've never come upon a use case that required per-object allocation, but the API allows it:

```c++
jl::ScopedAllocator* pSomeCustomAllocator;

// Signals
JL_SIGNAL() oSomeSignal( pSomeCustomAllocator );

// Observers
class SomeObserver : public jl::SignalObserver
{
public:
    SomeObserver( jl::ScopedAllocator* pSignalConnectionAllocator ) :
        jl::SignalObserver( pSignalConnectionAllocator )
    {
        ...
    }
};

SomeObserver oSomeObserver( pSomeCustomAllocator );
```

### Caveat - modifying signals during an `Emit()`

Be careful with logic in connected functions that could lead to modifying or destroying signal objects while they are still processing an `Emit()`.

For example, consider the following code:

```c++
typedef JL_SIGNAL() TSignal;
TSignal* pSignal = new TSignal;

class Foo : public jl::SignalObserver
{
public:
    void OnSignal() { delete pSignal; }
};

Foo* foo = new Foo;
pSignal->Connect( &foo, &Foo::OnSignal );
pSignal->Emit(); // This causes pSignal to be deleted before Emit() returns!
```

Miscellaneous
-------------

### Compatibility

I've used this library and/or similar protoypes with the following compilers:

- LLVM/clang
- GCC
- Visual C++ 9
- SNC Compiler

Most incompatibility issues arise from the fact that [FastDelegate](http://www.codeproject.com/Articles/7150/Member-Function-Pointers-and-the-Fastest-Possible) employs some clever but non-standard hackery to store function pointers in generic structures. It's worth noting that LLVM/clang support isn't documented (indeed, the library was released at least two years before clang), but it seems to work in my admittedly-limited test cases. In general, compiler incompatibility with FastDelegate is very rare.

### License

This code is public domain, with the following exceptions:

- `FastDelegate.h` is released under the [Code Project Open License](http://www.codeproject.com/info/cpol10.aspx).

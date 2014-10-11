signal11
========

Description
-----------

A very simple signal/slot library for C++11.

Installation
------------

Just copy `signal11.h` in your project.
You probably need to enable the C++11 mode of your compiler, `clang++` and `g++` use `-std=c++11`.

Usage
-----

    signal11<A, B, C> signal;

Declares a signal taking three arguments of types `A`, `B` and `C`.
There is no limit on the amount of arguments.

    signal.connect(function);

Connects `function` (anything accepted by `std::function<void(A, B, C)>`) to the signal.
`function(a, b, c)` will be called anytime `signal(a, b, c)` is called.

    auto connection = signal.connect(function);
    // ...
    signal.disconnect(connection);

Prevent `function` from being called anymore.

    signal.connect_oneshot(function);

Like `connect` but `function` will be disconnected as soon as it has been called once.

For a more in-depth example, see `example.cpp`.

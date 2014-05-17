signal11
========

About
-----
C++11 signal slot implementation

This implementation is based on the c++11 signal code written by Tim Janik. http://timj.testbit.eu/2013/01/25/cpp11-signal-system-performance/. Several features were added on top of Tim's signal implementation.

Features
--------

* ```ConnectionRef```. References a connection to allow for easy access to connection specific functions like ```disconnect()```.
* ```ScopedConnectionRef```. Automatically disconnects its referenced connection from the signal upon destruction.
* ```ConnectionScope```. Easy way to keep track of a group of connections and guarentee they will be cleaned up when the ```ConnectionScope``` is destructed.
* Connections have the ability to be enabled and disabled.

Examples
--------

###ConnectionRef
```c++
using namespace Signal11;

void myFunc(int i) {
    printf("myFunc called with %d\n", i);
}

void myOtherFunc(int i) {
    printf("myOtherFunc called with %d\n", i);
}

Signal<int> signal;

ConnectionRef ref1 = signal.connect(myFunc); //creates a connection and returns a reference wrapper to the connection
ConnectionRef ref2 = siganl.connect(myOtherFunc);

signal.emit(3);

ref1.disconnect(); //disconnects the connection from the signal
signal.emit(2);

ref2.disable(); //disables the connection, but it is not removed from the signal
signal.emit(1);

ref2.enable(); //connection is enabled again
signal.emit(0);

ref2.disconnect();

```
Output:

myFunc called with 3<br/>
myOtherFunc called with 3<br/>
myOtherFunc called with 2<br/>
myOtherFunc called with 0<br/>

###ScopedConnectionRef
```c++
{
    ScopedConnectionRef scopedRef = signal.connect(myFunc);
    signal.emit(1337);
}

signal.emit(80085);
```

Output:

myFunc called with 1337<br/>

###ConnectionScope
```c++
{
    ConnectionScope scope;
    ConnectionRef ref;
    scope += signal.connect(myFunc);
    ref = signal.connect(myOtherFunc);
    scope += ref;
    
    signal.emit(808);
}

signal.emit(404);
```

Output:

myFunc called with 808<br/>
myOtherFunc called with 808<br/>

License
-------
http://creativecommons.org/publicdomain/zero/1.0/

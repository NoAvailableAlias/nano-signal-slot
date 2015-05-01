# Nod
Dependency free, header only signals and slot library implemented with C++11.


# Usage

### Simple usage
The following example creates a signal and then connects a lambda as a slot.
```cpp
// Create a signal which accepts slots with no arguments and void return value.
nod::signal<void()> signal;
// Connect a lambda slot that writes "Hello, World!" to stdout
signal.connect([](){
		std::cout << "Hello, World!" << std::endl;
	})
// Call the slots
signal();
```

### Connecting multiple slots
If multiple slots are connected to the same signal, all of the slots will be
called when the signal is invoked. The slots will be called in the same order
as they where connected.
```cpp
void endline() {
	std::cout << std::endl;
}

nod::signal<void()> signal;
signal.connect([](){
		std::count << "Hello, World!";
	});
signal.connect(endline);

// Call the slots
signal();

```

### Slot arguments
When a signal calls it's connected slots, any arguments passed to the signal
are propagated to the slots. To make this work, we do need to specify the 
signature of the signal to accept the arguments.
```cpp
void print_sum( int x, int y ) {
	std::cout << x << "+" << y << "=" << (x+y) << std::endl;
}
void print_product( int x, int y ) {
	std::cout << x << "*" << y << "=" << (x*y) << std::endl;
}


// We create a signal with two integer arguments.
nod::signal<void(int,int)> signal;
// Let's connect our slot
nod.connect( print_sum );
nod.connect( print_product );

// Call the slots
nod( 10, 15 );
nod(-5, 7);

```

### Disconnecting slots
There are many circumstances where the programmer needs to diconnect a slot that
no longer want to recieve events from the signal. This can be really important
if the lifetime of the slots are shorter than the lifetime of the signal. That
could cause the signal to call slots that have been destroyed but not
disconnected, leading to undefined behaviour and probably segmentation faults.

When a slot is connected, the return value from the  `connect` method returns
an instance of the class `nod::connection`, that can be used to disconnect
that slot.
```cpp
// Let's create a signal
nod::signal<void()> signal;
// Connect a slot, and save the connection
nod::connection connection = signal.connect([](){
                                 std::cout << "Hello, World!" << std::endl;
                             });
// Triggering the signal will call the slot
signal();
// Now we disconnect the slot
connection.disconnect();
// Triggering the signal will no longer call the slot
signal();
```	

### Scoped connections
To assist in disconnecting slots, one can use the class `nod::scoped_connection`
to capture a slot connection. A scoped connection will automatically disconnect
the slot when the connection object goes out of scope.
```cpp
nod::signal<void()> signal;

{
	// Let's save the connection in a scoped_connection
	nod::scoped_connection connection =
		signal.connect([](){
			std::cout << "Hello, World!" << std::endl; 
		});
	// If we trigger the signal, the slot will be called
	signal();
} // Our scoped connection is destructed, and disconnects the slot
// Triggering the signal now will not call the slot
signal();
```

### More usage
```cpp
//todo: write some more example usage
```


## Building the tests
The test project uses [premake5](https://premake.github.io/download.html) to 
generate make files or similiar.

### Linux
To build and run the tests, execute the following from the test directory:
```bash
premake5 gmake
make -C build/gmake
bin/gmake/debug/nod_tests
```

### Visual Studio 2013
To build and run the tests, execute the following from the test directory:

```batchfile
REM Adjust paths to suite your environment
c:\path\to\premake\premake5.exe vs2013
"c:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\vsvars32.bat"
msbuild /m build\vs2013\nod_tests.sln
bin\vs2013\debug\nod_tests.exe
```

## The MIT License (MIT)

Copyright (c) 2015 Fredrik Berggren

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

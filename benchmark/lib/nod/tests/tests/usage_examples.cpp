#include <nod/nod.hpp>
#include <catch.hpp>
#include <iostream>

namespace 
{
	// Function to output a endline to stdout
	void endline() {
		std::cout << std::endl;
	}

	// Function that prints the sum calculation of two integers
	void print_sum( int x, int y ) {
		std::cout << x << "+" << y << "=" << (x+y) << std::endl;
	}

	// Function that prints the product calculation of two integers
	void print_product( int x, int y ) {
		std::cout << x << "*" << y << "=" << (x*y) << std::endl;
	}
}

SCENARIO( "Example usage") {
	SECTION( "Simple usage" ) {
		// Create a signal which accepts slots with no arguments and void return value.
		nod::signal<void()> signal;
		// Connect a lambda slot that writes "Hello, World!" to stdout
		signal.connect([](){
				std::cout << "Hello, World!" << std::endl;
			});
		// Call the slots
		signal();
	}

	SECTION( "Connecting multiple slots" ) {
		// Create a signal
		nod::signal<void()> signal;
		// Connect a lambda that prints a message
		signal.connect([](){
				std::cout << "Message without endline!";
			});
		// Connect a function that prints a endline
		signal.connect(endline);

		// Call the slots
		signal();
	}

	SECTION( "Slot arguments" ) {
		// We create a signal with two integer arguments.
		nod::signal<void(int,int)> signal;
		// Let's connect our slot
		signal.connect( print_sum );
		signal.connect( print_product );

		// Call the slots
		signal( 10, 15 );
		signal(-5, 7);
	}

	SECTION( "Disconnecting slots" ) {
		// Let's create a signal
		nod::signal<void()> signal;
		// Connect a slot, and save the connection
		nod::connection connection = signal.connect([](){
										std::cout << "I'm connected!" << std::endl;
		                             });
		// Triggering the signal will call the slot
		signal();
		// Now we disconnect the slot
		connection.disconnect();
		// Triggering the signal will no longer call the slot
		signal();
	}

	SECTION( "Scoped connections" ) {
		// We create a signal
		nod::signal<void()> signal;
		// Let's use a scope to control lifetime
		{ 
			// Let's save the connection in a scoped_connection
			nod::scoped_connection connection =
				signal.connect([](){
					std::cout << "This message should only be emitted once!" << std::endl; 
				});
			// If we trigger the signal, the slot will be called
			signal();
		} // Our scoped connection is destructed, and disconnects the slot
		// Triggering the signal now will not call the slot
		signal();	
	}
}
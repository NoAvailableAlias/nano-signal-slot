#include "../test_helpers.hpp"
#include <nod/nod.hpp>

#include <catch.hpp>

#include <sstream>

SCENARIO( "Slots can get connected and disconnected to signals" ) {
	GIVEN( "A singnal" ) {
		nod::signal<void(void)> signal;
		WHEN( "a slot is connected" ) {
			auto connection = signal.connect( [](){} );
			THEN( "the resulting connection is considered connected" ) {
				REQUIRE( connection.connected() == true );
			}		
			AND_WHEN( "the connection is disconnected" ) {
				connection.disconnect();
				THEN( "the connetion is considered disconnected" ) {
					REQUIRE( connection.connected() == false );
				}
			}
		}
	}
}

SCENARIO( "Signals can be invoked and will trigger connected slots" ) {
	GIVEN( "A signal" ) {
		std::ostringstream ss;
		nod::signal<void(double)> signal;
		WHEN( "A slot gets connected to that signal" ) {
			auto connection = signal.connect(
				[&ss]( double x ){
					ss << "signaled!";
				});
			THEN( "the slot will get called when invoking the signal" ) {
				signal(12.5);
				REQUIRE( ss.str() == "signaled!" );
			}
			AND_WHEN( "we disconnect that slot with the connection object" ) {
				connection.disconnect();
				THEN( "the slot will no longer be called when invoking the signal" ) {
					signal(12.5);				
					REQUIRE( ss.str().empty() ); 
				}
			}
		}
	}
}

SCENARIO( "Signals will disconnect all slots when destroyed" ) {
	GIVEN( "A signal" ) {
		auto signal_ptr = std::unique_ptr<nod::signal<void(void)>>( new nod::signal<void(void)>() );
		WHEN( "Some slots gets connected to the signal" ) {
			auto connection1 = signal_ptr->connect( []() {} );
			auto connection2 = signal_ptr->connect( []() {} );
			auto connection3 = signal_ptr->connect( []() {} );
			THEN( "the connections is considered connected" ) {
				REQUIRE( connection1.connected() == true );
				REQUIRE( connection2.connected() == true );
				REQUIRE( connection3.connected() == true );
			}
			AND_WHEN( "the signal gets destructed" ) {
				signal_ptr.reset();
				THEN( "the connections is no longer considered connected" ) {
					REQUIRE( connection1.connected() == false );
					REQUIRE( connection2.connected() == false );
					REQUIRE( connection3.connected() == false );
				}
			}			
		}
	}
}

SCENARIO( "Scoped connection will disconnect when destroyed" ) {
	GIVEN( "A signal" ) {
		std::ostringstream ss;
		nod::signal<void(void)> signal;
		WHEN( "A slot get's connected and managed by a scoped connection" ) {
			auto connection = test::make_unique<nod::scoped_connection>( signal.connect([&](){ ss << "singaled!"; }) );
			THEN( "the connection is considered connected" ) {
				REQUIRE( connection->connected() == true );
			}
			AND_WHEN( "the scoped_connection is destroyed ") {
				connection.reset();
				THEN( "it is disconnected so no signaling is done" ) {
					signal();
					REQUIRE( ss.str().empty() );
				}
			}
		}
	}
}
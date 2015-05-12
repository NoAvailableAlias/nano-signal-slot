#include "../test_helpers.hpp"
#include <nod/nod.hpp>
#include <catch.hpp>
#include <numeric>

SCENARIO( "It's possible to accumulate the return values of slots" ) {
	GIVEN( "a signal with three slots that return a double" ) {
		nod::signal<double(double,double)> signal;
		signal.connect( std::multiplies<double>{} );
		signal.connect( std::plus<double>{} );
		signal.connect( std::minus<double>{} );
		WHEN( "we create a signal accumulator with the initial value 0.0 and the std::plus<double> function, and trigger the signal through the accumulator" ) {
			auto accumulator = signal.accumulate(0.0, std::plus<double>{});
			auto result = accumulator(42,12);
			THEN( "the result is the sum of all the slots return value" ) {
				REQUIRE( result == (std::multiplies<double>{}(42,12) + std::plus<double>{}(42,12) + std::minus<double>{}(42,12)) );
			}
		}
		AND_WHEN( "we use a lambda to accumulate values into a vector" ) {
			auto result = signal.accumulate( std::vector<double>{}, []( std::vector<double> partial, double slot_result ) {
					partial.push_back( slot_result );
					return partial;
				})(42.0,12.0);
			THEN( "the resulting vector contains all the return values of the slots" ) {
				REQUIRE( result == (std::vector<double>{std::multiplies<double>{}(42,12), std::plus<double>{}(42,12), std::minus<double>{}(42,12)}) );
			}
		}
	}
}

SCENARIO( "It's possible to aggregate the return values of slots into a container" ) {
	GIVEN( "a signal with three slots that return a double" ) {
		nod::signal<double(double,double)> signal;
		signal.connect( std::multiplies<double>{} );
		signal.connect( std::plus<double>{} );
		signal.connect( std::minus<double>{} );
		WHEN( "we trigger the signal through the aggregate method to aggregate into a vector<double>" ) {
			auto result = signal.aggregate<std::vector<double>>(42.0,12.0);
			THEN( "the result is a vector with all slot return values" ) {
				REQUIRE( result == (std::vector<double>{std::multiplies<double>{}(42,12), std::plus<double>{}(42,12), std::minus<double>{}(42,12) }));
			}
		}
	}
}
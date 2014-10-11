#ifndef _JL_SIGNAL_H_
#define _JL_SIGNAL_H_

#include "FastDelegate.h"

// Uncomment this to see verbose console messages about Signal/SignalObserver
// connections and disconnections. Recommended for debug only.
//#define JL_SIGNAL_ENABLE_LOGSPAM

// Uncomment this to force assertion failures when a SignalObserver tries
// to connect the same slot to the same signal twice. Recommended for debug only.
//#define JL_SIGNAL_ASSERT_ON_DOUBLE_CONNECT

/**
 * Quick usage guide:
 *
 * Use Signal to call an arbitrary number of functions with a single compile-time function call.
 * Currently, Signal only supports calls to object-method pairs, i.e., it is not possible to call
 * a static, non-instance function using Signal.
 *
 * There are three ways of declaring a signal:
 *
 * - Base signal class, with parameter count in the type name:
 *        Signal2< int, int > mySignal;
 *
 * - Template wrapper, using function signature syntax:
 *        Signal< void(int, int) > mySignal;
 *
 * - Template wrapper + macro (easiest, slightly hacky):
 *        JL_SIGNAL( int, int ) mySignal;
 */

#include "SignalDefinitions.h"

/**
 * The following macro will allow you to ignore the argument
 * count in the signal typename. Thus, instead of writing this:
 *
 *    Signal1< int, int > SignalA;
 *    Signal3< int, char, float > SignalB;
 *
 * you can write this:
 *
 *    JL_SIGNAL( int, int ) SignalA;
 *    JL_SIGNAL( int, char, float ) SignalB;
 *
 * Note: this macro evaluates to a TYPE, so it can be used in typedefs.
 */
#ifdef FASTDELEGATE_ALLOW_FUNCTION_TYPE_SYNTAX
#define JL_SIGNAL( ... ) jl::Signal< void( __VA_ARGS__ ) >
#else
#define JL_SIGNAL( ... ) JL_COMPILER_ASSERT( false, Your_Compiler_Does_Not_Support_This_Syntax )
#endif // ! defined( FASTDELEGATE_ALLOW_FUNCTION_TYPE_SYNTAX )

#endif // ! defined( _JL_SIGNAL_H_ )
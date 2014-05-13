#ifndef _JL_UTILS_H_
#define _JL_UTILS_H_

/////////////
// I. Asserts

#include <assert.h>

// To disable runtime asserts, either:
// 1. Define JL_DISABLE_ASSERT as a global compiler macro,
// 2. Comment out JL_ENABLE_ASSERT below
#define JL_ENABLE_ASSERT

#if defined( JL_ENABLE_ASSERT ) && ! defined ( JL_DISABLE_ASSERT )
#define JL_ASSERT( _Expr ) assert( _Expr )
#else
#define JL_ASSERT( _Expr )
#endif

// A handy compile-time assert
#define JL_COMPILER_ASSERT( _Expr, _Message ) typedef int JL_COMPILER_ASSERT_##_Message[ _Expr ? 0 : -1 ]

//////////////////////////////
// II. Miscellaneous utilities

#define JL_ARRAY_SIZE( _a ) ( sizeof(_a) / sizeof((_a)[0]) )
#define JL_UNUSED( _a ) (void)( _a )

namespace jl {
    
template< typename _to, typename _from >
_to BruteForceCast( _from p )
{
    union
    {
        _from from;
        _to to;
    } conversion;
    conversion.from = p;
    return conversion.to;
}
    
} // namespace jl

#endif // ! defined( _JL_UTILS_H_ )
#ifndef _JL_STATIC_SIGNAL_CONNECTION_ALLOCATORS_H_
#define _JL_STATIC_SIGNAL_CONNECTION_ALLOCATORS_H_

#include "ObjectPoolScopedAllocator.h"
#include "SignalDefinitions.h"

namespace jl {

typedef Signal0<void> TDummySignal;

template< unsigned _Size >
class StaticSignalConnectionAllocator : public StaticObjectPoolAllocator< TDummySignal::eAllocationSize, _Size >
{
};

template< unsigned _Size >
class StaticObserverConnectionAllocator : public StaticObjectPoolAllocator< SignalObserver::eAllocationSize, _Size >
{
};
    
} // namespace jl

#endif // ! defined( _JL_STATIC_SIGNAL_CONNECTION_ALLOCATORS_H_ )
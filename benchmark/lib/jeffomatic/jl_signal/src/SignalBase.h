#ifndef _JL_SIGNAL_BASE_H_
#define _JL_SIGNAL_BASE_H_

#include "Utils.h"
#include "DoublyLinkedList.h"

namespace jl {

// Forward declarations
class SignalBase;

// Derive from this class to receive signals
class SignalObserver
{
    // Public interface
public:
    virtual ~SignalObserver();
    
    void DisconnectAllSignals();
    void DisconnectSignal( SignalBase* pSignal );
    
    void SetConnectionAllocator( ScopedAllocator* pAllocator ) { m_oSignals.Init( pAllocator ); }
    unsigned CountSignalConnections() const { return m_oSignals.Count(); }
    
    // Interface for child classes
protected:
    // Disallow instances of this class
    SignalObserver() { SetConnectionAllocator( s_pCommonAllocator ); }
    SignalObserver( ScopedAllocator* pAllocator ) { SetConnectionAllocator( pAllocator ); }
    
    // Hmm, a bit of a hack, but if a derived type caches pointers to signals,
    // we may need this
    virtual void OnSignalDisconnectInternal( SignalBase* pSignal ) { JL_UNUSED(pSignal); }
    
    // Private interface (to SignalBase)
private:
    friend class SignalBase;
    
    void OnSignalConnect( SignalBase* pSignal );
    void OnSignalDisconnect( SignalBase* pSignal );
    
    // Signal list
public:
    typedef DoublyLinkedList<SignalBase*> SignalList;
    enum { eAllocationSize = sizeof(SignalList::Node) };
    
private:
    SignalList m_oSignals;
    
    // Global allocator
public:
    static void SetCommonConnectionAllocator( ScopedAllocator* pAllocator ) { s_pCommonAllocator = pAllocator; }
    
private:
    static ScopedAllocator* s_pCommonAllocator;
};

class SignalBase
{
public:
    virtual ~SignalBase() {};
    
    virtual unsigned CountConnections() const = 0;
    
    // Interface for derived signal classes
protected:
    // Disallow instances of this class
    SignalBase() {}
    
    // Called on any connection to the observer.
    void NotifyObserverConnect( SignalObserver* pObserver ) { pObserver->OnSignalConnect(this); }
    
    // Called when no more connections exist to the observer.
    void NotifyObserverDisconnect( SignalObserver* pObserver ) { pObserver->OnSignalDisconnect(this); }
    
    // Private interface (for SignalObserver)
private:
    friend class SignalObserver;
    virtual void OnObserverDisconnect( SignalObserver* pObserver ) = 0;
    
    // Global allocator
public:
    static void SetCommonConnectionAllocator( ScopedAllocator* pAllocator ) { s_pCommonAllocator = pAllocator; }
    
protected:
    static ScopedAllocator* s_pCommonAllocator;
};
    
} // namespace jl

#endif // ! defined( _JL_SIGNAL_BASE_H_ )
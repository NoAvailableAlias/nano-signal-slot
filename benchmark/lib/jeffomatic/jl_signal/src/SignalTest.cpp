#include <stdio.h>
#include <assert.h>

#include "Signal.h"
#include "StaticSignalConnectionAllocators.h"

using namespace jl;

namespace
{
    enum { eSignalMaxArity = 8 };    
    
    class TestObserver : public SignalObserver
    {
    private:
        static int s_id;      
        static unsigned s_pInstanceMethodCallsByArity[ eSignalMaxArity + 1 ];
        static unsigned s_pStaticMethodCallsByArity[ eSignalMaxArity + 1 ];
        
        int m_nId;

    public:    
        static void ResetCallsByArity()
        {
            for ( unsigned i = 0; i < JL_ARRAY_SIZE(s_pInstanceMethodCallsByArity); ++i )
            {
                s_pInstanceMethodCallsByArity[i] = 0;
                s_pStaticMethodCallsByArity[i] = 0;
            }
        }
        
        static unsigned CountCallsByArity( unsigned arity )
        {
            return s_pInstanceMethodCallsByArity[arity] + s_pStaticMethodCallsByArity[arity];
        }
        
        TestObserver()
        {
            m_nId = s_id++;
        }
        
        void M0()
        {
            s_pInstanceMethodCallsByArity[0] += 1;
            printf( "TestObserver %d: instance method 0!\n", m_nId );
        }
        
        void M1( int p1 )
        {
            s_pInstanceMethodCallsByArity[1] += 1;
            printf( "TestObserver %d: instance method 1! %d\n", m_nId, p1 );
        }
        
        void M2( int p1, float p2 )
        {
            s_pInstanceMethodCallsByArity[2] += 1;
            printf( "TestObserver %d: instance method 2! %d %f\n", m_nId, p1, p2 );
        }
        
        void M3( int p1, float p2, char p3 )
        {
            s_pInstanceMethodCallsByArity[3] += 1;            
            printf( "TestObserver %d: instance method 3! %d %f %c\n", m_nId, p1, p2, p3 );
        }
        
        void M4( int p1, float p2, char p3, const char* p4 )
        {
            s_pInstanceMethodCallsByArity[4] += 1;
            printf( "TestObserver %d: instance method 4! %d %f %c %s\n", m_nId, p1, p2, p3, p4 );
        }
        
        void M5( int p1, float p2, char p3, const char* p4, int p5 )
        {
            s_pInstanceMethodCallsByArity[5] += 1;
            printf( "TestObserver %d: instance method 5! %d %f %c %s %d\n", m_nId, p1, p2, p3, p4, p5 );
        }
        
        void M6( int p1, float p2, char p3, const char* p4, int p5, float p6 )
        {
            s_pInstanceMethodCallsByArity[6] += 1;
            printf( "TestObserver %d: instance method 6! %d %f %c %s %d %f\n", m_nId, p1, p2, p3, p4, p5, p6 );
        }
        
        void M7( int p1, float p2, char p3, const char* p4, int p5, float p6, char p7 )
        {
            s_pInstanceMethodCallsByArity[7] += 1;
            printf( "TestObserver %d: instance method 7! %d %f %c %s %d %f %c\n", m_nId, p1, p2, p3, p4, p5, p6, p7 );
        }
        
        void M8( int p1, float p2, char p3, const char* p4, int p5, float p6, char p7, const char* p8 )
        {
            s_pInstanceMethodCallsByArity[8] += 1;
            printf( "TestObserver %d: instance method 8! %d %f %c %s %d %f %c %s\n", m_nId, p1, p2, p3, p4, p5, p6, p7, p8 );
        }
        
        void CM0() const
        {
            s_pInstanceMethodCallsByArity[0] += 1;
            printf( "TestObserver %d: const instance method 0!\n", m_nId );
        }
        
        void CM1( int p1 ) const
        {
            s_pInstanceMethodCallsByArity[1] += 1;
            printf( "TestObserver %d: const instance method 1! %d\n", m_nId, p1 );
        }
        
        void CM2( int p1, float p2 ) const
        {
            s_pInstanceMethodCallsByArity[2] += 1;
            printf( "TestObserver %d: const instance method 2! %d %f\n", m_nId, p1, p2 );
        }
        
        void CM3( int p1, float p2, char p3 ) const
        {
            s_pInstanceMethodCallsByArity[3] += 1;
            printf( "TestObserver %d: const instance method 3! %d %f %c\n", m_nId, p1, p2, p3 );
        }
        
        void CM4( int p1, float p2, char p3, const char* p4 ) const
        {
            s_pInstanceMethodCallsByArity[4] += 1;
            printf( "TestObserver %d: const instance method 4! %d %f %c %s\n", m_nId, p1, p2, p3, p4 );
        }
        
        void CM5( int p1, float p2, char p3, const char* p4, int p5 ) const
        {
            s_pInstanceMethodCallsByArity[5] += 1;
            printf( "TestObserver %d: const instance method 5! %d %f %c %s %d\n", m_nId, p1, p2, p3, p4, p5 );
        }
        
        void CM6( int p1, float p2, char p3, const char* p4, int p5, float p6 ) const
        {
            s_pInstanceMethodCallsByArity[6] += 1;
            printf( "TestObserver %d: const instance method 6! %d %f %c %s %d %f\n", m_nId, p1, p2, p3, p4, p5, p6 );
        }
        
        void CM7( int p1, float p2, char p3, const char* p4, int p5, float p6, char p7 ) const
        {
            s_pInstanceMethodCallsByArity[7] += 1;
            printf( "TestObserver %d: const instance method 7! %d %f %c %s %d %f %c\n", m_nId, p1, p2, p3, p4, p5, p6, p7 );
        }
        
        void CM8( int p1, float p2, char p3, const char* p4, int p5, float p6, char p7, const char* p8 ) const
        {
            s_pInstanceMethodCallsByArity[8] += 1;
            printf( "TestObserver %d: const instance method 8! %d %f %c %s %d %f %c %s\n", m_nId, p1, p2, p3, p4, p5, p6, p7, p8 );
        }
        
        static void SM0()
        {
            s_pStaticMethodCallsByArity[0] += 1;
            printf( "TestObserver: static instance method 0!\n" );
        }
        
        static void SM1( int p1 )
        {
            s_pStaticMethodCallsByArity[1] += 1;
            printf( "TestObserver: static instance method 1! %d\n", p1 );
        }
        
        static void SM2( int p1, float p2 )
        {
            s_pStaticMethodCallsByArity[2] += 1;
            printf( "TestObserver: static instance method 2! %d %f\n", p1, p2 );
        }
        
        static void SM3( int p1, float p2, char p3 )
        {
            s_pStaticMethodCallsByArity[3] += 1;
            printf( "TestObserver: static instance method 3! %d %f %c\n", p1, p2, p3 );
        }
        
        static void SM4( int p1, float p2, char p3, const char* p4 )
        {
            s_pStaticMethodCallsByArity[4] += 1;
            printf( "TestObserver: static instance method 4! %d %f %c %s\n", p1, p2, p3, p4 );
        }
        
        static void SM5( int p1, float p2, char p3, const char* p4, int p5 )
        {
            s_pStaticMethodCallsByArity[5] += 1;
            printf( "TestObserver: static instance method 5! %d %f %c %s %d\n", p1, p2, p3, p4, p5 );
        }
        
        static void SM6( int p1, float p2, char p3, const char* p4, int p5, float p6 )
        {
            s_pStaticMethodCallsByArity[6] += 1;
            printf( "TestObserver: static instance method 6! %d %f %c %s %d %f\n", p1, p2, p3, p4, p5, p6 );
        }
        
        static void SM7( int p1, float p2, char p3, const char* p4, int p5, float p6, char p7 )
        {
            s_pStaticMethodCallsByArity[7] += 1;
            printf( "TestObserver: static instance method 7! %d %f %c %s %d %f %c\n", p1, p2, p3, p4, p5, p6, p7 );
        }
        
        static void SM8( int p1, float p2, char p3, const char* p4, int p5, float p6, char p7, const char* p8 )
        {
            s_pStaticMethodCallsByArity[8] += 1;
            printf( "TestObserver: static instance method 8! %d %f %c %s %d %f %c %s\n", p1, p2, p3, p4, p5, p6, p7, p8 );
        }        
    };
        
    int TestObserver::s_id = 1;
    unsigned TestObserver::s_pInstanceMethodCallsByArity[ eSignalMaxArity + 1 ];
    unsigned TestObserver::s_pStaticMethodCallsByArity[ eSignalMaxArity + 1 ];    
} // anonymous namespace

void SignalTest()
{
    // Allocators
    enum { eMaxConnections = 500, eSignalMaxArgs = 8 };
    StaticSignalConnectionAllocator< eMaxConnections > oSignalConnectionAllocator;
    StaticObserverConnectionAllocator< eMaxConnections > oObserverConnectionAllocator;
    
    jl::SignalBase::SetCommonConnectionAllocator( &oSignalConnectionAllocator );
    jl::SignalObserver::SetCommonConnectionAllocator( &oObserverConnectionAllocator );
    
    // Signals
    JL_SIGNAL() Sig0;
    JL_SIGNAL( int ) Sig1;
    JL_SIGNAL( int, float ) Sig2;
    JL_SIGNAL( int, float, char ) Sig3;
    JL_SIGNAL( int, float, char, const char* ) Sig4;
    JL_SIGNAL( int, float, char, const char*, int ) Sig5;
    JL_SIGNAL( int, float, char, const char*, int, float ) Sig6;
    JL_SIGNAL( int, float, char, const char*, int, float, char ) Sig7;
    JL_SIGNAL( int, float, char, const char*, int, float, char, const char* ) Sig8;
    
    const SignalBase* const ppSignalsByArity[] = {
        & Sig0, & Sig1, & Sig2, & Sig3, & Sig4, & Sig5, & Sig6, & Sig7, & Sig8
    };
    
    // Observers
    TestObserver pObservers[ 16 ];
    
    // Test connections
    printf( "Connection non-const instance methods to signals...\n" );
    
    for ( int i = 0; i < JL_ARRAY_SIZE(pObservers); ++i )
    {      
        Sig0.Connect( & pObservers[i], & TestObserver::M0 );
        assert( Sig0.IsConnected(& pObservers[i], & TestObserver::M0) );
        
        Sig1.Connect( & pObservers[i], & TestObserver::M1 );
        assert( Sig1.IsConnected(& pObservers[i], & TestObserver::M1) );
        
        const int nIndex = i + 1;
        
        if ( nIndex % 2 == 0 )
        {
            Sig2.Connect( & pObservers[i], & TestObserver::M2 );
            assert( Sig2.IsConnected(& pObservers[i], & TestObserver::M2) );
        }
        
        if ( nIndex % 3 == 0 )
        {
            Sig3.Connect( & pObservers[i], & TestObserver::M3 );
            assert( Sig3.IsConnected(& pObservers[i], & TestObserver::M3) );
        }
        
        if ( nIndex % 4 == 0 )
        {
            Sig4.Connect( & pObservers[i], & TestObserver::M4 );
            assert( Sig4.IsConnected(& pObservers[i], & TestObserver::M4) );
        }
        
        if ( nIndex % 5 == 0 )
        {
            Sig5.Connect( & pObservers[i], & TestObserver::M5 );
            assert( Sig5.IsConnected(& pObservers[i], & TestObserver::M5) );
        }
        
        if ( nIndex % 6 == 0 )
        {
            Sig6.Connect( & pObservers[i], & TestObserver::M6 );
            assert( Sig6.IsConnected(& pObservers[i], & TestObserver::M6) );
        }
        
        if ( nIndex % 7 == 0 )
        {
            Sig7.Connect( & pObservers[i], & TestObserver::M7 );
            assert( Sig7.IsConnected(& pObservers[i], & TestObserver::M7) );
        }
        
        if ( nIndex % 8 == 0 )
        {
            Sig8.Connect( & pObservers[i], & TestObserver::M8 );
            assert( Sig8.IsConnected(& pObservers[i], & TestObserver::M8) );
        }
    }
    
    printf( "Firing signals...\n" );
    
    // Zero the received call count
    TestObserver::ResetCallsByArity();
    
    // Emit signals
    Sig0();
    Sig1( 1 );
    Sig2( 1, 2.0f );
    Sig3( 1, 2.0f, 'T' );
    Sig4( 1, 2.0f, 'T', "Four" );
    Sig5( 1, 2.0f, 'T', "Four", 5 );
    Sig6( 1, 2.0f, 'T', "Four", 5, 6.0f );
    Sig7( 1, 2.0f, 'T', "Four", 5, 6.0f, 'S' );
    Sig8( 1, 2.0f, 'T', "Four", 5, 6.0f, 'S', "Eight" );
    
    // Verify that the observer count is equal to the received call count
    for ( unsigned i = 0; i < JL_ARRAY_SIZE(ppSignalsByArity); ++i )
    {
        const unsigned nConnections = ppSignalsByArity[i]->CountConnections();
        const unsigned nCalls = TestObserver::CountCallsByArity( i );
        printf( "Arity %d, Connections: %d, Calls: %d\n", i, nConnections, nCalls);
        assert( ppSignalsByArity[i]->CountConnections() == TestObserver::CountCallsByArity(i) );
    }  
    
    // Test const connections
    printf( "Connecting const instance methods to signals...\n" );
    
    for ( int i = 0; i < JL_ARRAY_SIZE(pObservers); ++i )
    {
        Sig0.Connect( & pObservers[i], & TestObserver::CM0 );
        assert( Sig0.IsConnected(& pObservers[i], & TestObserver::CM0) );
        
        Sig1.Connect( & pObservers[i], & TestObserver::CM1 );
        assert( Sig1.IsConnected(& pObservers[i], & TestObserver::CM1) );
        
        const int nIndex = (JL_ARRAY_SIZE(pObservers) + 1 - i);
        
        if ( nIndex % 2 == 0 )
        {           
            Sig2.Connect( & pObservers[i], & TestObserver::CM2 );
            assert( Sig2.IsConnected(& pObservers[i], & TestObserver::CM2) );
        }
        
        if ( nIndex % 3 == 0 )
        {
            Sig3.Connect( & pObservers[i], & TestObserver::CM3 );
            assert( Sig3.IsConnected(& pObservers[i], & TestObserver::CM3) );
        }
        
        if ( nIndex % 4 == 0 )
        {
            Sig4.Connect( & pObservers[i], & TestObserver::CM4 );
            assert( Sig4.IsConnected(& pObservers[i], & TestObserver::CM4) );
        }
        
        if ( nIndex % 5 == 0 )
        {      
            Sig5.Connect( & pObservers[i], & TestObserver::CM5 );
            assert( Sig5.IsConnected(& pObservers[i], & TestObserver::CM5) );
        }
        
        if ( nIndex % 6 == 0 )
        {      
            Sig6.Connect( & pObservers[i], & TestObserver::CM6 );
            assert( Sig6.IsConnected(& pObservers[i], & TestObserver::CM6) );
        }
        
        if ( nIndex % 7 == 0 )
        {      
            Sig7.Connect( & pObservers[i], & TestObserver::CM7 );
            assert( Sig7.IsConnected(& pObservers[i], & TestObserver::CM7) );
        }
        
        if ( nIndex % 8 == 0 )
        {         
            Sig8.Connect( & pObservers[i], & TestObserver::CM8 );
            assert( Sig8.IsConnected(& pObservers[i], & TestObserver::CM8) );
        }
    }
    
    printf( "Firing signals...\n" );
    
    // Zero the received call count
    TestObserver::ResetCallsByArity();
    
    // Emit signals
    Sig0();
    Sig1( 1 );
    Sig2( 1, 2.0f );
    Sig3( 1, 2.0f, 'T' );
    Sig4( 1, 2.0f, 'T', "Four" );
    Sig5( 1, 2.0f, 'T', "Four", 5 );
    Sig6( 1, 2.0f, 'T', "Four", 5, 6.0f );
    Sig7( 1, 2.0f, 'T', "Four", 5, 6.0f, 'S' );
    Sig8( 1, 2.0f, 'T', "Four", 5, 6.0f, 'S', "Eight" );
    
    // Verify that the observer count is equal to the received call count
    for ( unsigned i = 0; i < JL_ARRAY_SIZE(ppSignalsByArity); ++i )
    {
        const unsigned nConnections = ppSignalsByArity[i]->CountConnections();
        const unsigned nCalls = TestObserver::CountCallsByArity( i );
        printf( "Arity %d, Connections: %d, Calls: %d\n", i, nConnections, nCalls);
        assert( ppSignalsByArity[i]->CountConnections() == TestObserver::CountCallsByArity(i) );
    }
    
    // Test static connections
    printf( "Connecting static methods to signals...\n" );
    
    Sig0.Connect( & TestObserver::SM0 );
    assert( Sig0.IsConnected(& TestObserver::SM0) );
    
    Sig1.Connect( & TestObserver::SM1 );       
    assert( Sig1.IsConnected(& TestObserver::SM1) );
    
    Sig2.Connect( & TestObserver::SM2 );
    assert( Sig2.IsConnected(& TestObserver::SM2) );
    
    Sig3.Connect( & TestObserver::SM3 );
    assert( Sig3.IsConnected(& TestObserver::SM3) );
    
    Sig4.Connect( & TestObserver::SM4 );
    assert( Sig4.IsConnected(& TestObserver::SM4) );
    
    Sig5.Connect( & TestObserver::SM5 );
    assert( Sig5.IsConnected(& TestObserver::SM5) );
    
    Sig6.Connect( & TestObserver::SM6 );
    assert( Sig6.IsConnected(& TestObserver::SM6) );
    
    Sig7.Connect( & TestObserver::SM7 );
    assert( Sig7.IsConnected(& TestObserver::SM7) );
    
    Sig8.Connect( & TestObserver::SM8 );
    assert( Sig8.IsConnected(& TestObserver::SM8) );
    
    printf( "Firing signals...\n" );
    
    // Zero the received call count
    TestObserver::ResetCallsByArity();
    
    // Emit signals
    Sig0();
    Sig1( 1 );
    Sig2( 1, 2.0f );
    Sig3( 1, 2.0f, 'T' );
    Sig4( 1, 2.0f, 'T', "Four" );
    Sig5( 1, 2.0f, 'T', "Four", 5 );
    Sig6( 1, 2.0f, 'T', "Four", 5, 6.0f );
    Sig7( 1, 2.0f, 'T', "Four", 5, 6.0f, 'S' );
    Sig8( 1, 2.0f, 'T', "Four", 5, 6.0f, 'S', "Eight" );
    
    // Verify that the observer count is equal to the received call count
    for ( unsigned i = 0; i < JL_ARRAY_SIZE(ppSignalsByArity); ++i )
    {
        const unsigned nConnections = ppSignalsByArity[i]->CountConnections();
        const unsigned nCalls = TestObserver::CountCallsByArity( i );
        printf( "Arity %d, Connections: %d, Calls: %d\n", i, nConnections, nCalls);
        assert( ppSignalsByArity[i]->CountConnections() == TestObserver::CountCallsByArity(i) );
    }    
    
    // Test disconnections
    printf( "Disconnecting non-const instance methods to signals...\n" );
    
    for ( int i = 0; i < JL_ARRAY_SIZE(pObservers); ++i )
    {
        Sig0.Disconnect( & pObservers[i], & TestObserver::M0 );
        assert( ! Sig0.IsConnected(& pObservers[i], & TestObserver::M0) );
        
        Sig1.Disconnect( & pObservers[i], & TestObserver::M1 );
        assert( ! Sig1.IsConnected(& pObservers[i], & TestObserver::M1) );
        
        const int nIndex = i + 1;
        
        if ( nIndex % 2 == 0 )
        {
            Sig2.Disconnect( & pObservers[i], & TestObserver::M2 );
            assert( ! Sig2.IsConnected(& pObservers[i], & TestObserver::M2) );            
        }
        
        if ( nIndex % 3 == 0 )
        {
            Sig3.Disconnect( & pObservers[i], & TestObserver::M3  );
            assert( ! Sig3.IsConnected(& pObservers[i], & TestObserver::M3) );            
        }
        
        if ( nIndex % 4 == 0 )
        {
            Sig4.Disconnect( & pObservers[i], & TestObserver::M4  );
            assert( ! Sig4.IsConnected(& pObservers[i], & TestObserver::M4) );                        
        }
        
        if ( nIndex % 5 == 0 )
        {
            Sig5.Disconnect( & pObservers[i], & TestObserver::M5  );
            assert( ! Sig5.IsConnected(& pObservers[i], & TestObserver::M5) );                        
        }
        
        if ( nIndex % 6 == 0 )
        {
            Sig6.Disconnect( & pObservers[i], & TestObserver::M6  );
            assert( ! Sig6.IsConnected(& pObservers[i], & TestObserver::M6) );                        
        }
        
        if ( nIndex % 7 == 0 )
        {
            Sig7.Disconnect( & pObservers[i], & TestObserver::M7  );
            assert( ! Sig7.IsConnected(& pObservers[i], & TestObserver::M7) );                        
        }
        
        if ( nIndex % 8 == 0 )
        {
            Sig8.Disconnect( & pObservers[i], & TestObserver::M8  );
            assert( ! Sig8.IsConnected(& pObservers[i], & TestObserver::M8) );                        
        }
    }
    
    printf( "Firing signals...\n" );
    
    // Zero the received call count
    TestObserver::ResetCallsByArity();    
    
    // Emit signals
    Sig0();
    Sig1( 1 );
    Sig2( 1, 2.0f );
    Sig3( 1, 2.0f, 'T' );
    Sig4( 1, 2.0f, 'T', "Four" );
    Sig5( 1, 2.0f, 'T', "Four", 5 );
    Sig6( 1, 2.0f, 'T', "Four", 5, 6.0f );
    Sig7( 1, 2.0f, 'T', "Four", 5, 6.0f, 'S' );
    Sig8( 1, 2.0f, 'T', "Four", 5, 6.0f, 'S', "Eight" );
    
    // Verify that the observer count is equal to the received call count
    for ( unsigned i = 0; i < JL_ARRAY_SIZE(ppSignalsByArity); ++i )
    {
        const unsigned nConnections = ppSignalsByArity[i]->CountConnections();
        const unsigned nCalls = TestObserver::CountCallsByArity( i );
        printf( "Arity %d, Connections: %d, Calls: %d\n", i, nConnections, nCalls);
        assert( ppSignalsByArity[i]->CountConnections() == TestObserver::CountCallsByArity(i) );
    }      
    
    printf( "Disconnecting const instance methods to signals...\n" );
    
    for ( int i = 0; i < JL_ARRAY_SIZE(pObservers); ++i )
    {
        Sig0.Disconnect( & pObservers[i], & TestObserver::CM0 );
        assert( ! Sig0.IsConnected(& pObservers[i], & TestObserver::CM0) );
        Sig1.Disconnect( & pObservers[i], & TestObserver::CM1 );
        assert( ! Sig1.IsConnected(& pObservers[i], & TestObserver::CM1) );
        
        const int nIndex = (JL_ARRAY_SIZE(pObservers) + 1 - i);
        
        if ( nIndex % 2 == 0 )
        {
            Sig2.Disconnect( & pObservers[i], & TestObserver::CM2 );
            assert( ! Sig2.IsConnected(& pObservers[i], & TestObserver::CM2) );            
        }
        
        if ( nIndex % 3 == 0 )
        {
            Sig3.Disconnect( & pObservers[i], & TestObserver::CM3  );
            assert( ! Sig3.IsConnected(& pObservers[i], & TestObserver::CM3) );            
        }
        
        if ( nIndex % 4 == 0 )
        {
            Sig4.Disconnect( & pObservers[i], & TestObserver::CM4  );
            assert( ! Sig4.IsConnected(& pObservers[i], & TestObserver::CM4) );                        
        }
        
        if ( nIndex % 5 == 0 )
        {
            Sig5.Disconnect( & pObservers[i], & TestObserver::CM5  );
            assert( ! Sig5.IsConnected(& pObservers[i], & TestObserver::CM5) );                        
        }
        
        if ( nIndex % 6 == 0 )
        {
            Sig6.Disconnect( & pObservers[i], & TestObserver::CM6  );
            assert( ! Sig6.IsConnected(& pObservers[i], & TestObserver::CM6) );                        
        }
        
        if ( nIndex % 7 == 0 )
        {
            Sig7.Disconnect( & pObservers[i], & TestObserver::CM7  );
            assert( ! Sig7.IsConnected(& pObservers[i], & TestObserver::CM7) );                        
        }
        
        if ( nIndex % 8 == 0 )
        {
            Sig8.Disconnect( & pObservers[i], & TestObserver::CM8  );
            assert( ! Sig8.IsConnected(& pObservers[i], & TestObserver::CM8) );                        
        }
    }
    
    printf( "Firing signals...\n" );

    // Zero the received call count
    TestObserver::ResetCallsByArity();      
    
    // Emit signals
    Sig0();
    Sig1( 1 );
    Sig2( 1, 2.0f );
    Sig3( 1, 2.0f, 'T' );
    Sig4( 1, 2.0f, 'T', "Four" );
    Sig5( 1, 2.0f, 'T', "Four", 5 );
    Sig6( 1, 2.0f, 'T', "Four", 5, 6.0f );
    Sig7( 1, 2.0f, 'T', "Four", 5, 6.0f, 'S' );
    Sig8( 1, 2.0f, 'T', "Four", 5, 6.0f, 'S', "Eight" );
    
    // Verify that the observer count is equal to the received call count
    for ( unsigned i = 0; i < JL_ARRAY_SIZE(ppSignalsByArity); ++i )
    {
        const unsigned nConnections = ppSignalsByArity[i]->CountConnections();
        const unsigned nCalls = TestObserver::CountCallsByArity( i );
        printf( "Arity %d, Connections: %d, Calls: %d\n", i, nConnections, nCalls);
        assert( ppSignalsByArity[i]->CountConnections() == TestObserver::CountCallsByArity(i) );
    }
    
    // Test static method disconnections
    printf( "Disconnecting static methods...\n" );
    
    Sig0.Disconnect( & TestObserver::SM0 );
    assert( ! Sig0.IsConnected(& TestObserver::SM0) );
    
    Sig1.Disconnect( & TestObserver::SM1 );       
    assert( ! Sig1.IsConnected(& TestObserver::SM1) );
    
    Sig2.Disconnect( & TestObserver::SM2 );
    assert( ! Sig2.IsConnected(& TestObserver::SM2) );
    
    Sig3.Disconnect( & TestObserver::SM3 );
    assert( ! Sig3.IsConnected(& TestObserver::SM3) );
    
    Sig4.Disconnect( & TestObserver::SM4 );
    assert( ! Sig4.IsConnected(& TestObserver::SM4) );
    
    Sig5.Disconnect( & TestObserver::SM5 );
    assert( ! Sig5.IsConnected(& TestObserver::SM5) );
    
    Sig6.Disconnect( & TestObserver::SM6 );
    assert( ! Sig6.IsConnected(& TestObserver::SM6) );
    
    Sig7.Disconnect( & TestObserver::SM7 );
    assert( ! Sig7.IsConnected(& TestObserver::SM7) );
    
    Sig8.Disconnect( & TestObserver::SM8 );
    assert( ! Sig8.IsConnected(& TestObserver::SM8) );
    
    printf( "Firing signals...\n" );
    
    // Zero the received call count
    TestObserver::ResetCallsByArity();
    
    // Emit signals
    Sig0();
    Sig1( 1 );
    Sig2( 1, 2.0f );
    Sig3( 1, 2.0f, 'T' );
    Sig4( 1, 2.0f, 'T', "Four" );
    Sig5( 1, 2.0f, 'T', "Four", 5 );
    Sig6( 1, 2.0f, 'T', "Four", 5, 6.0f );
    Sig7( 1, 2.0f, 'T', "Four", 5, 6.0f, 'S' );
    Sig8( 1, 2.0f, 'T', "Four", 5, 6.0f, 'S', "Eight" );
    
    // Verify that the observer count is equal to the received call count
    for ( unsigned i = 0; i < JL_ARRAY_SIZE(ppSignalsByArity); ++i )
    {
        const unsigned nConnections = ppSignalsByArity[i]->CountConnections();
        const unsigned nCalls = TestObserver::CountCallsByArity( i );
        printf( "Arity %d, Connections: %d, Calls: %d\n", i, nConnections, nCalls);
        assert( ppSignalsByArity[i]->CountConnections() == TestObserver::CountCallsByArity(i) );
    }    
}
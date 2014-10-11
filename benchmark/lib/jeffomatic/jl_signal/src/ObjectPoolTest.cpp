#include <stdio.h>
#include <stdlib.h>
#include <new>
#include <assert.h>

#include "ObjectPool.h"

using namespace jl;

// This is a unit test of the various object pool classes.
namespace
{   
    // TestObject instances will be allocated by our object pool allocators.
    // These are simple wrappers around string pointers. We'll test to make sure
    // the string pointers are the same after subsequent allocations to the same
    // pool.
    class TestObject
    {
    public:       
        TestObject( const char* p = NULL ) : m_pContents(p) {}
        const char* GetContents() const { return m_pContents; }

    private:
        const char* m_pContents;
    };

    // TestState instances wrap around allocated TestObject instances. They take
    // a newly-allocated TestObject and memoize its contents. After the
    // allocator state changes, use IsValid() to ensure that allocated object
    // state has not been clobbered.
    class TestState
    {
    public:
        TestState() : m_pContentsCopy(NULL), m_pObject(NULL) {}

        void SetObject( TestObject* pObject = NULL )
        {
            m_pObject = pObject;
            m_pContentsCopy = m_pObject ? m_pObject->GetContents() : NULL;
        }

        TestObject* GetObject() const { return m_pObject; }

        bool IsValid() const
        {
            return m_pObject == NULL || m_pObject->GetContents() == m_pContentsCopy;
        }        
        
        template <size_t _ArraySize>
        static bool IsValidArray( const TestState(&pArray)[_ArraySize] )
        {
            for ( unsigned i = 0; i < _ArraySize; ++i )
            {
                if ( ! pArray[i].IsValid() )
                {
                    return false;
                }
            }
            
            return true;
        }

    private:
        const char* m_pContentsCopy;
        TestObject* m_pObject;
    };

    // Allocator for PreallocatedObjectPool
    template<unsigned _Stride, unsigned _Capacity>
    class PreallocatedPoolFactory
    {
    public:
        enum
        {
            eStride = _Stride,
            eCapacity = _Capacity
        };
        
        typedef PreallocatedObjectPool InternalObjectPool;
        
        static InternalObjectPool* Create()
        {
            void* pBuffer = new unsigned char[ _Stride * _Capacity ];
            InternalObjectPool* pPool = new InternalObjectPool();
            pPool->Init( pBuffer, _Capacity, _Stride, true );
            return pPool;
        }
    };

    // Allocator for StaticObjectPool
    template<unsigned _Stride, unsigned _Capacity>
    class StaticPoolFactory
    {
    public:
        enum
        {
            eStride = _Stride,
            eCapacity = _Capacity
        };        
        
        typedef StaticObjectPool<_Stride, _Capacity> InternalObjectPool;
        
        static InternalObjectPool* Create()
        {
            return new InternalObjectPool();
        }
    };
    
    const char* g_ppSampleContents[] = {
        "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n",
        "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"
    };

    template<typename _TAllocatorFactory, unsigned _PoolSize>
    void PoolTest()
    {
        typedef typename _TAllocatorFactory::InternalObjectPool PoolType;
        PoolType* pPool;
        TestState pTestState[_PoolSize];

        printf( "Creating pool...\n" );
        pPool = _TAllocatorFactory::Create();
        
        for ( unsigned nTrialSize = 1; nTrialSize <= _PoolSize; ++nTrialSize )
        {
            printf( "Testing %u allocations on a pool with size %u\n", nTrialSize, _PoolSize );

            // Ensure that the internal allocation count is zero
            assert( 0 == pPool->CountAllocations() );
            
            // Ensure that the internal allocation count corresponds to the free list size
            assert( pPool->CountAllocations() == pPool->GetCapacity() - ObjectPool::FreeListSize(pPool->GetFreeListHead()) );            
            
            // Ensure that allocations haven't been corrupted
            assert( TestState::IsValidArray(pTestState) );
            
            // Initial allocations
            for ( unsigned i = 0; i < nTrialSize; ++i )
            {
                pTestState[i].SetObject(
                    new( pPool->Alloc() ) TestObject( g_ppSampleContents[i] )
                );
                
                // Validations
                {
                    // Ensure that internal allocation count is accurate
                    assert( i + 1 == pPool->CountAllocations() );
                    
                    // Ensure that free list size corresponds to current number of allocations
                    assert( pPool->CountAllocations() == pPool->GetCapacity() - ObjectPool::FreeListSize(pPool->GetFreeListHead()) );
                    
                    // Ensure that allocations haven't been corrupted
                    assert( TestState::IsValidArray(pTestState) );                    
                }
            }
            
            // Random replacements
            const unsigned nReplacements = nTrialSize;
            for ( unsigned i = 0; i < nReplacements; ++i )
            {                 
                const int nReplacementIndex = rand() % nTrialSize;
                
                // Free the object
                pTestState[nReplacementIndex].GetObject()->~TestObject();
                pPool->Free( static_cast<void*>(pTestState[nReplacementIndex].GetObject()) );
                pTestState[nReplacementIndex].SetObject();
                
                // Validations
                {
                    // Ensure that the old internal allocation count corresponds to the current test size, minus one
                    assert( nTrialSize - 1 == pPool->CountAllocations() );
                    
                    // Ensure that the free list size corresponds to the number of allocations
                    assert( pPool->CountAllocations() == pPool->GetCapacity() - ObjectPool::FreeListSize(pPool->GetFreeListHead()) );
                    
                    // Ensure that allocations haven't been corrupted
                    assert( TestState::IsValidArray(pTestState) );
                }
                
                // Create a new object
                const char* pReplacementValue = g_ppSampleContents[ rand() % JL_ARRAY_SIZE(g_ppSampleContents) ];
                pTestState[nReplacementIndex].SetObject(
                    new( pPool->Alloc() ) TestObject( pReplacementValue )
                );

                // Validations
                {
                    // Ensure that the internal allocation count corresponds to the current test size
                    assert( nTrialSize == pPool->CountAllocations() );
                    
                    // Ensure that the free list size corresponds to the number of allocations
                    assert( pPool->CountAllocations() == pPool->GetCapacity() - ObjectPool::FreeListSize(pPool->GetFreeListHead()) );
                    
                    // Ensure that allocations haven't been corrupted
                    assert( TestState::IsValidArray(pTestState) );               
                }
            }
            
            // Free all allocations
            for ( unsigned i = 0; i < nTrialSize; ++i )
            {
                // Cache the old internal allocation count
                const int nPreFreeAllocations = pPool->CountAllocations();
                
                // Free the object
                pTestState[i].GetObject()->~TestObject();
                pPool->Free( static_cast<void*>(pTestState[i].GetObject()) );
                pTestState[i].SetObject();
                
                // Validations
                {
                    // Ensure that the old internal allocation count corresponds to the current allocation count
                    assert( nPreFreeAllocations - 1 == pPool->CountAllocations() );
                    
                    // Ensure that the free list size corresponds to the number of allocations
                    assert( pPool->CountAllocations() == pPool->GetCapacity() - ObjectPool::FreeListSize(pPool->GetFreeListHead()) );
                    
                    // Ensure that allocations haven't been corrupted                
                    assert( TestState::IsValidArray(pTestState) );
                }
            }
            
            // Final validations
            {
                // Ensure that the internal allocation count is zero
                assert( 0 == pPool->CountAllocations() );
                
                // Ensure that the internal allocation count corresponds to the free list size
                assert( pPool->CountAllocations() == pPool->GetCapacity() - ObjectPool::FreeListSize(pPool->GetFreeListHead()) );
                
                // Ensure that allocations haven't been corrupted
                assert( TestState::IsValidArray(pTestState) );            
            }
        } // Increment trial size
    }
}

void ObjectPoolTest()
{
    enum { ePoolCapacity = 1000 };
    
    printf("Testing PreallocatedObjectPool...\n");
    typedef PreallocatedPoolFactory<sizeof(TestObject), ePoolCapacity> PreallocatedPoolTestFactory;
    PoolTest<PreallocatedPoolTestFactory, PreallocatedPoolTestFactory::eCapacity>();

    printf("\nTesting StaticObjectPool...\n");    
    typedef StaticPoolFactory<sizeof(TestObject), ePoolCapacity> StaticPoolTestFactory;
    PoolTest<StaticPoolTestFactory, StaticPoolTestFactory::eCapacity>();
}

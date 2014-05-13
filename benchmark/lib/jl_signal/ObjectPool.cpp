#include <stddef.h>

#include "ObjectPool.h"

namespace jl {
    // Some helper routines
    namespace
    {
        bool IsBounded( const void* pObject, const unsigned char* pObjectBuffer, unsigned nCapacity, unsigned nStride )
        {
            const unsigned char* const pFirst = pObjectBuffer;
            const unsigned char* const pLast = pObjectBuffer + nStride * (nCapacity - 1);
            return pFirst <= pObject && pObject <= pLast;
        }

        bool IsAligned( const void* pObject, const unsigned char* pObjectBuffer, unsigned nStride )
        {
            const ptrdiff_t nDiff = reinterpret_cast<const unsigned char*>(pObject) - pObjectBuffer;
            return ( nDiff % nStride == 0 );
        }

        // Populate a sorted array with pointers to every free node, and return the number of free nodes.
        unsigned GetSortedFreeNodeList( ObjectPool::FreeNode* ppSortedFreeNodes[], ObjectPool::FreeNode* pFreeListHead )
        {
            unsigned nFreeCount = 0;

            for ( ObjectPool::FreeNode* n = pFreeListHead; n != NULL; n = n->pNextFree )
            {
                ppSortedFreeNodes[ nFreeCount ] = n;
                nFreeCount += 1;
            }

            // Insertion sort
            for ( unsigned i = 1; i < nFreeCount; ++i )
            {
                ObjectPool::FreeNode* pCurrent = ppSortedFreeNodes[ i ];
                unsigned j = i;

                // Insert pCurrent into the appropriate spot in the span [0, i]
                while ( j > 0 && ppSortedFreeNodes[j - 1] > pCurrent )
                {
                    ppSortedFreeNodes[ j ] = ppSortedFreeNodes[ j - 1 ];
                    --j;
                }

                ppSortedFreeNodes[ j ] = pCurrent;
            }

            return nFreeCount;
        }
    } // anon namespace
} // namespace jl

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

// Initializes an object buffer as a free list and returns the head of the list
jl::ObjectPool::FreeNode* jl::ObjectPool::InitFreeList( unsigned char* pObjectBuffer, unsigned nCapacity, unsigned nStride )
{
    // Setup free list links
    unsigned char* const pLast = pObjectBuffer + nStride * (nCapacity - 1);

    for ( unsigned char* pCurrent = pObjectBuffer; pCurrent < pLast; pCurrent += nStride )
    {
        FreeNode::Cast( pCurrent )->pNextFree = FreeNode::Cast( pCurrent + nStride );
    }

    // End free list
    FreeNode::Cast( pLast )->pNextFree = NULL;

    // Return start of free list
    return FreeNode::Cast( pObjectBuffer );
}

unsigned jl::ObjectPool::FreeListSize( ObjectPool::FreeNode* pFreeListHead )
{
    // Early out for degenerate case
    if ( ! pFreeListHead ) return 0;
    
    unsigned n = 0;
    for ( ; pFreeListHead; pFreeListHead = pFreeListHead->pNextFree )
    {
        ++n;
    }
    
    return n;
}

bool jl::ObjectPool::IsBoundedAndAligned( const void* pObject, const unsigned char* pObjectBuffer, unsigned nCapacity, unsigned nStride )
{
    return IsBounded( pObject, pObjectBuffer, nCapacity, nStride )
        && IsAligned( pObject, pObjectBuffer, nStride );
}

bool jl::ObjectPool::IsFree( const void* pObject, const FreeNode* pFreeListHead )
{
    // Scrub through free list and make sure this object hasn't been freed already
    for ( const ObjectPool::FreeNode* n = pFreeListHead; n != NULL; n = n->pNextFree )
    {
        if ( n == pObject )
        {
            return true;
        }
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

jl::PreallocatedObjectPool::PreallocatedObjectPool()
{
    Reset();
}

jl::PreallocatedObjectPool::PreallocatedObjectPool( void* pBuffer, unsigned nCapacity, unsigned nStride, unsigned nFlags /*= eFlag_Default */ )
{
    m_pObjectBuffer = NULL; // prevent assertion failure in Init()
    Init( pBuffer, nCapacity, nStride, nFlags );
}

jl::PreallocatedObjectPool::~PreallocatedObjectPool()
{
    if ( m_nFlags & eFlag_ManageBuffer )
    {
        delete[] m_pObjectBuffer;
    }
}

void jl::PreallocatedObjectPool::Init( void* pBuffer, unsigned nCapacity, unsigned nStride, unsigned nFlags /*= eFlag_Default */ )
{
    JL_ASSERT( m_pObjectBuffer == NULL );

    m_pObjectBuffer = (unsigned char*)pBuffer;
    m_pFreeListHead = ObjectPool::InitFreeList( m_pObjectBuffer, nCapacity, nStride );

    m_nCapacity = nCapacity;
    m_nStride = nStride;
    m_nFlags = nFlags;
}

void jl::PreallocatedObjectPool::Deinit()
{
    if ( m_nFlags & eFlag_ManageBuffer )
    {
        delete[] m_pObjectBuffer;
    }

    Reset();
}

void jl::PreallocatedObjectPool::Reset()
{
    m_pObjectBuffer = NULL;
    m_pFreeListHead = NULL;

    m_nCapacity = 0;
    m_nAllocations = 0;
    m_nStride = 0;
    m_nFlags = 0;
}

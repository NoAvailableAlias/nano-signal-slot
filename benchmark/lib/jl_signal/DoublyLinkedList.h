#ifndef _JL_DOUBLY_LINKED_LIST_H_
#define _JL_DOUBLY_LINKED_LIST_H_

#include "Utils.h"
#include "ScopedAllocator.h"

namespace jl {

/**
 * Your basic doubly-linked list, with link nodes allocated outside of the
 * contained type.
 * Requires a ScopedAllocator for node allocation.
 */
template<typename _T>
class DoublyLinkedList
{
public:
    
    //////////////////
    // Data structures
    //////////////////
    
    typedef _T TObject;
    
    struct Node
    {
        TObject object;
        Node* prev;
        Node* next;
    };
    
    class iterator
    {
    public:
        TObject& operator*()
        {
            return m_pCurrent->object;
        }
        
        TObject& operator->()
        {
            return m_pCurrent->object;
        }
        
        iterator& operator--()
        {
            JL_ASSERT( m_pCurrent );
            
            if ( m_pCurrent )
            {
                m_pCurrent = m_pCurrent->prev;
            }
            
            return *this;
        }
        
        iterator& operator++()
        {
            JL_ASSERT( m_pCurrent );
            
            if ( m_pCurrent )
            {
                m_pCurrent = m_pCurrent->next;
            }
            
            return *this;
        }
        
        bool operator==( const iterator& other ) const
        {
            return m_pList == other.m_pList && m_pCurrent == other.m_pCurrent;
        }
        
        bool isValid() const
        {
            return m_pList != NULL && m_pCurrent != NULL;
        }
        
    private:
        friend class DoublyLinkedList<TObject>;
        DoublyLinkedList<TObject>* m_pList;
        Node* m_pCurrent;
    };
    
    class const_iterator
    {
    public:
        const TObject& operator*()
        {
            return m_pCurrent->object;
        }
        
        const TObject& operator->()
        {
            return m_pCurrent->object;
        }

        const_iterator& operator--()
        {
            JL_ASSERT( m_pCurrent );
            
            if ( m_pCurrent )
            {
                m_pCurrent = m_pCurrent->prev;
            }
            
            return *this;
        }        
        
        const_iterator& operator++()
        {
            JL_ASSERT( m_pCurrent );
            
            if ( m_pCurrent )
            {
                m_pCurrent = m_pCurrent->next;
            }
            
            return *this;
        }
        
        bool operator==( const const_iterator& other ) const
        {
            return m_pList == other.m_pList && m_pCurrent == other.m_pCurrent;
        }
        
        bool isValid() const
        {
            return m_pList != NULL && m_pCurrent != NULL;
        }
        
    private:
        friend class DoublyLinkedList<TObject>;
        const DoublyLinkedList<TObject>* m_pList;
        const Node* m_pCurrent;
    };
    
    /////////////////////
    // Internal interface
    /////////////////////
    
private:
    Node* CreateNode()
    {
        Node* pNode = (Node*)m_pNodeAllocator->Alloc( sizeof(Node) );
        
        if ( ! pNode )
        {
            return NULL;
        }
        
        // Initialize node pointers
        pNode->next = NULL;
        pNode->prev = NULL;  
        
        return pNode;
    }
    
    ///////////////////
    // Public interface
    ///////////////////
    
public:
    
    DoublyLinkedList()
    {
        m_pHead = NULL;
        m_pTail = NULL;
        m_nObjectCount = 0;
        m_pNodeAllocator = NULL;
    }
    
    ~DoublyLinkedList()
    {
        Clear();
    }
    
    void Init( ScopedAllocator* pNodeAllocator )
    {
        m_pNodeAllocator = pNodeAllocator;
    }
    
    // Returns true if the object was successfully added
    Node* Add( const TObject& object )
    {
        // Create a node to contain the object.
        Node* pNode = CreateNode();
        JL_ASSERT( pNode );
        
        if ( ! pNode )
        {
            return NULL;
        }
        
        // Place the object in the node.
        pNode->object = object;
        
        // Add node to the end of the list.
        if ( m_pTail )
        {
            m_pTail->next = pNode;
            pNode->prev = m_pTail;
            m_pTail = pNode;
        }
        else
        {
            JL_ASSERT( ! m_pHead );
            m_pHead = pNode;
            m_pTail = pNode;
        }
        
        // Update object count
        m_nObjectCount += 1;
        
        return pNode;
    }
    
    // Returns true if the object was successfully removed. This will only
    // remove the first instance of the object.
    bool Remove( const TObject& object )
    {
        for ( Node* pNode = m_pHead; pNode != NULL; pNode = pNode->next )
        {
            if ( pNode->object == object )
            {
                return RemoveNode( pNode );
            }
        }
        
        return false;
    }

    // Returns true if the object at the iterator position was successfully removed
    // This will advance the iterator if the removal was successful.
    bool Remove( iterator& i )
    {
        JL_ASSERT( i.m_pList == this );
        if ( i.m_pList != this )
        {
            return false;
        }
        
        Node* pNext = i.m_pCurrent->next;
        if ( RemoveNode(i.m_pCurrent) )
        {
            i.m_pCurrent = pNext;
            return true;
        }
        
        return false;
    }    
    
    // Returns the number of nodes removed from the list.
    unsigned RemoveAll( const TObject& object )
    {
        const unsigned nSizeBefore = m_nObjectCount;
        
        Node* pCurrent = NULL;
        Node* pNext = m_pHead;
        
        while ( pNext )
        {
            pCurrent = pNext;
            pNext = pCurrent->next;
            
            if ( pCurrent->object == object )
            {
                RemoveNode( pCurrent );
            }
        }
    
        JL_ASSERT( nSizeBefore >= m_nObjectCount );
        return nSizeBefore - m_nObjectCount;
    }    
    
    unsigned Count() const
    {
        return m_nObjectCount;
    }
    
    unsigned IsEmpty() const
    {
        return m_nObjectCount == 0;
    }
    
    void Clear()
    {
        Node* pCurrent = NULL;
        Node* pNext = m_pHead;
        
        while ( pNext )
        {
            pCurrent = pNext;
            pNext = pCurrent->next;
            
            m_pNodeAllocator->Free( pCurrent );
        }
        
        m_pHead = NULL;
        m_pTail = NULL;
        m_nObjectCount = 0;
    }
    
    // O(N) random access
    TObject& operator[]( unsigned n )
    {
        JL_ASSERT( n < m_nObjectCount );
        
        unsigned i = 0;
        Node* pNode = m_pHead;
        
        for (;;)
        {
            JL_ASSERT( pNode );
            
            if ( i == n )
            {
                return pNode->object;
            }
            
            i += 1;
            pNode = pNode->next;
        }
    }
    
    const TObject& operator[]( unsigned n ) const
    {
        JL_ASSERT( n < m_nObjectCount );
        
        unsigned i = 0;
        const Node* pNode = m_pHead;
        
        for (;;)
        {
            JL_ASSERT( pNode );
            
            if ( i == n )
            {
                return pNode->object;
            }
            
            i += 1;
            pNode = pNode->next;
        }
    }
    
    // Iterator interface
    iterator begin()
    {
        iterator i;
        i.m_pList = this;
        i.m_pCurrent = m_pHead;
        return i;
    }
    
    const_iterator const_begin() const
    {
        const_iterator i;
        i.m_pList = this;
        i.m_pCurrent = m_pHead;
        return i;
    }
    
private:
    bool RemoveNode( Node* pNode )
    {
        JL_ASSERT( m_nObjectCount );
        if ( ! m_nObjectCount )
        {
            return false;
        }
        
        // Re-assign head/tail pointers, if necessary
        if ( m_pHead == pNode )
        {
            m_pHead = pNode->next;
        }
        
        if ( m_pTail == pNode )
        {
            m_pTail = pNode->prev;
        }
        
        // Reassign links between previous/next buckets
        if ( pNode->prev )
        {
            pNode->prev->next = pNode->next;
        }
        
        if ( pNode->next )
        {
            pNode->next->prev = pNode->prev;
        }
        
        // Update object count
        m_nObjectCount -= 1;
        
        // Free node object
        m_pNodeAllocator->Free( pNode );
        
        return true;
    }
    
    Node* m_pHead;
    Node* m_pTail;
    unsigned m_nObjectCount;
    ScopedAllocator* m_pNodeAllocator;
};

} // namespace jl

#endif // ! defined( _JL_DOUBLY_LINKED_LIST_H_ )
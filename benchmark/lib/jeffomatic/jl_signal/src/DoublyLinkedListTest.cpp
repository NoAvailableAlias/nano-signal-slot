#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "DoublyLinkedList.h"
#include "ObjectPoolScopedAllocator.h"

using namespace jl;

namespace
{
    typedef DoublyLinkedList<const char*> StringList;
    typedef StaticObjectPoolAllocator< sizeof(StringList::Node), 100 > StringNodeAllocator;
}

void DoublyLinkedListTest()
{   
    const char* pTestStrings[] =
    {
        "Test 1",
        "Test 2",
        "Test 3",
        "Test 4",
        "Test 5",
        "Test 6",
        "Test 7",
        "Test 8",
        "Test 9",
        "Test 10",
        "Test 11",
        "Test 12",
        "Test 13",
        "Test 14",
        "Test 15",
        "Test 16",
        "Test 17",
        "Test 18",
        "Test 19",
        "Test 20",
        "Test 21",
        "Test 22",
        "Test 23",
        "Test 24",
        "Test 25",
        "Test 26",
        "Test 27",
        "Test 28",
        "Test 29",
        "Test 30",
        "Test 31",
        "Test 32",
    };
    
    StringNodeAllocator oAllocator;
    StringList oList;
    
    oList.Init( & oAllocator );
    
    // Insertion test
    printf( "Inserting objects...\n" );
    for ( unsigned i = 0; i < JL_ARRAY_SIZE(pTestStrings); ++i )
    {
        assert( oList.Add(pTestStrings[i]) );
    }
    
    // Test object count
    assert( oList.Count() == JL_ARRAY_SIZE(pTestStrings) );
    
    // Iterator test
    printf( "Iterating through list...\n" );
    for ( StringList::iterator i = oList.begin(); i.isValid(); ++i )
    {
        printf( "\tObject: %s\n", *i );
    }
    
    // Value-based removal
    printf( "Value-based removal...\n" );
    
    for ( unsigned i = 0; i < JL_ARRAY_SIZE(pTestStrings); ++i )
    {
        printf( "\tRemoving: %s\n", pTestStrings[i] );
        assert( oList.Remove(pTestStrings[i]) );
    }
    
    // Test object count
    assert( oList.Count() == 0 );
    assert( oAllocator.CountAllocations() == 0 );
    
    // Value-based reverse removal
    printf( "Value-based reverse removal...\n" );
    
    for ( unsigned i = 0; i < JL_ARRAY_SIZE(pTestStrings); ++i )
    {
        assert( oList.Add(pTestStrings[i]) );
    }
    
    for ( unsigned i = JL_ARRAY_SIZE(pTestStrings); i > 0; --i )
    {
        printf( "\tRemoving: %s\n", pTestStrings[i - 1] );
        assert( oList.Remove(pTestStrings[i - 1]) );
    }
    
    // Test object count
    assert( oList.Count() == 0 );
    assert( oAllocator.CountAllocations() == 0 );
    
    // Iterator-based removal
    printf( "Iterator-based removal...\n" );
    
    for ( unsigned i = 0; i < JL_ARRAY_SIZE(pTestStrings); ++i )
    {
        assert( oList.Add(pTestStrings[i]) );
    }
    
    for ( StringList::iterator i = oList.begin(); i.isValid(); )
    {
        printf( "\tRemoving: %s\n", *i );
        assert( oList.Remove(i) );
    }
    
    // Test object count
    assert( oList.Count() == 0 );
    assert( oAllocator.CountAllocations() == 0 );
    
    // Random removal
    enum { eRandomTrials = 16 };
    printf( "\nStarting %d random removal tests\n", eRandomTrials );
    
    for ( unsigned i = 0; i < 5; ++i )
    {
        const unsigned nInsert = ( rand() % JL_ARRAY_SIZE(pTestStrings) ) + 1;
        const unsigned nRemove = ( rand() % nInsert ) + 1;
        
        printf( "\tTrial %d: inserting %d objects and removing %d objects\n", i + 1, nInsert, nRemove );
        
        // Insert objects
        printf( "\t\tInserting %d objects\n", nInsert );
        for ( unsigned j = 0; j < nInsert; ++j )
        {
            printf( "\t\t\tInserting %s\n", pTestStrings[j] );
            assert( oList.Add(pTestStrings[j]) );
        }
        
        // Remove objects
        printf( "\t\tRemoving %d objects\n", nRemove );
        for ( unsigned j = 0; j < nRemove; ++j )
        {
            // Create iterator and seek to random position
            const unsigned nSeek = rand() % oList.Count();
            StringList::iterator iter = oList.begin();
            
            for ( unsigned k = 0; k < nSeek; ++k )
            {
                ++iter;
            }
            
            // Remove object from list
            printf( "\t\t\tRemoving item ID%d: %s\n", nSeek, *iter );
            assert( oList.Remove(iter) );
        }
        
        // Display leftovers
        printf( "\t\tRemaining objects (%d):\n", oList.Count() );
        for ( StringList::iterator i = oList.begin(); i.isValid(); ++i )
        {
            printf( "\t\t\tObject: %s\n", *i );
        }
        
        // Test object count
        const unsigned nCount = nInsert - nRemove;
        assert( oList.Count() == nCount );
        assert( oAllocator.CountAllocations() == nCount );
        
        // Clear
        oList.Clear();
        
        // Test object count
        assert( oList.Count() == 0 );
        assert( oAllocator.CountAllocations() == 0 );
    }
}
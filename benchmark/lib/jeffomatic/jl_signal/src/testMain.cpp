#include <stdlib.h>
#include <stdio.h>
#include <time.h>

extern void ObjectPoolTest();
extern void DoublyLinkedListTest();
extern void SignalTest();

int main(int argc, char** argv)
{
    srand( (unsigned)time(NULL) );    
    
    ObjectPoolTest();
    DoublyLinkedListTest();
    SignalTest();
    
    printf("\nDone! Press enter to continue...\n");
    getchar();    
}

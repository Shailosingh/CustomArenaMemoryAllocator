#include <iostream>
#include "ShailoHeap.h"

void HeapStatus(ShailoHeap& heapObject);

int main()
{
    //Testing heap without free------------------------------------------------------------------------------------------------------
    //Build heap
    int max = 4000;
    ShailoHeap heapObject = ShailoHeap(max*sizeof(int) + sizeof(size_t));
    HeapStatus(heapObject);

    //Insert 2 timestables into the heap
    int* tester = (int*)heapObject.ShailoHeapAlloc(sizeof(int) * max);
    if (tester == nullptr)
    {
        std::cout << "MEMORY ALLOCATION UNSUCCESSFUL\n";
        exit(-1);
    }
    for (int index = 0; index < max; index++)
    {
        tester[index] = 2 * index;
    }

    //Print out saved integers
    for (int index = 0; index < max; index++)
    {
        std::cout << "2x" << index << " = " << tester[index] << "\n";
    }

    HeapStatus(heapObject);

    //Testing free------------------------------------------------------------------------------------------------------------------
    //Create heap and load with three integers
    ShailoHeap freeTestHeap = ShailoHeap(3 * PaddedMemoryAllocation(sizeof(int)));
    int* firstInteger = (int*)freeTestHeap.ShailoHeapAlloc(sizeof(int));
    *firstInteger = 1;
    int* secondInteger = (int*)freeTestHeap.ShailoHeapAlloc(sizeof(int));
    *secondInteger = 2;
    int* thirdInteger = (int*)freeTestHeap.ShailoHeapAlloc(sizeof(int));
    *thirdInteger = 3;
    HeapStatus(freeTestHeap);

    //Test if heap can handle freeing and merging an object between two free spaces consecutively (SUCCESS)
    //Free first and third integers to leave a space in between
    freeTestHeap.ShailoHeapFree(firstInteger);
    freeTestHeap.ShailoHeapFree(thirdInteger);

    //Free second integer to test if it merges it in the middle and thus all as one free space
    freeTestHeap.ShailoHeapFree(secondInteger);
    

}

void HeapStatus(ShailoHeap& heapObject)
{
    std::cout << "Remaining Capacity: " << heapObject.ShailoHeapFreeCapacity() << " bytes\n";
    std::cout << "Max Capacity: " << heapObject.ShailoHeapMaxCapacity() << " bytes\n\n";
}

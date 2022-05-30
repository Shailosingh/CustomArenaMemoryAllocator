#include <iostream>
#include "ShailoHeap.h"

void HeapStatus(ShailoHeap& heapObject);

int main()
{
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
}

void HeapStatus(ShailoHeap& heapObject)
{
    std::cout << "Remaining Capacity: " << heapObject.ShailoHeapFreeCapacity() << " bytes\n";
    std::cout << "Max Capacity: " << heapObject.ShailoHeapMaxCapacity() << " bytes\n\n";
}

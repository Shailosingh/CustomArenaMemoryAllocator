#include <iostream>
#include "ShailoHeap.h"

void HeapStatus(ShailoHeap& heapObject);

int main()
{
    std::cout << "Testing basic heap allocation without free------------------------------------------------------------------------------------------\n"; //SUCCESS
    //Build heap
    int max = 4000;
    ShailoHeap heapObject = ShailoHeap(PaddedMemoryAllocation(max*sizeof(int)));
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
    heapObject.ShailoHeapDestroy();

    std::cout << "Testing coalescing------------------------------------------------------------------------------------------------------------------\n"; //SUCCESS
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
    
    HeapStatus(freeTestHeap);
    freeTestHeap.ShailoHeapDestroy();

    std::cout << "Testing reuse of freed blocks-------------------------------------------------------------------------------------------------------\n"; //SUCCESS
    //Create heap and load with three integers
    ShailoHeap reuseTestHeap = ShailoHeap(3*PaddedMemoryAllocation(sizeof(int)));
    firstInteger = (int*)reuseTestHeap.ShailoHeapAlloc(sizeof(int));
    *firstInteger = 1;
    secondInteger = (int*)reuseTestHeap.ShailoHeapAlloc(sizeof(int));
    *secondInteger = 2;
    thirdInteger = (int*)reuseTestHeap.ShailoHeapAlloc(sizeof(int));
    *thirdInteger = 3;
    HeapStatus(reuseTestHeap);

    //Free the middle integer and ensure this space can be reused
    reuseTestHeap.ShailoHeapFree(secondInteger);

    //Try to put an array of chars in the middle space that was just freed
    char* characterFill = (char*)reuseTestHeap.ShailoHeapAlloc(sizeof(int));
    for (size_t index = 0; index < sizeof(int); index++)
    {
        characterFill[index] = 'A';
    }
    HeapStatus(reuseTestHeap);
    reuseTestHeap.ShailoHeapDestroy();

    std::cout << "Testing 'Best-Fit' Allocation-------------------------------------------------------------------------------------------------------\n"; //SUCCESS
    //Create heap with 100 chars
    ShailoHeap bestFitTestHeap = ShailoHeap(100*PaddedMemoryAllocation(sizeof(char)));
    char* charPointers[100];
    for (size_t index = 0; index < 100; index++)
    {
        charPointers[index] = (char*)bestFitTestHeap.ShailoHeapAlloc(sizeof(char));
        *charPointers[index] = 'A';
    }

    //Remove characters [1,30], [35,46], [51,90] (freespaces of 30*(sizeof(size_t)+sizeof(char))=270, 12*(sizeof(size_t)+sizeof(char))=108, 40*(sizeof(size_t)+sizeof(char))=360)
    for (size_t index = 1; index <= 30; index++)
    {
        bestFitTestHeap.ShailoHeapFree(charPointers[index]);
    }
    for (size_t index = 35; index <= 46; index++)
    {
        bestFitTestHeap.ShailoHeapFree(charPointers[index]);
    }
    for (size_t index = 51; index <= 90; index++)
    {
        bestFitTestHeap.ShailoHeapFree(charPointers[index]);
    }

    //Insert 12 chars one by one into the heap. Should all go into [35,46] (108) space
    char* tempChar;
    for (size_t index = 0; index < 12; index++)
    {
        tempChar = (char*)bestFitTestHeap.ShailoHeapAlloc(sizeof(char));
        *tempChar = 'a';
    }

    //Insert 30 characters which should go into [1,30] (270) space
    for (size_t index = 0; index < 30; index++)
    {
        tempChar = (char*)bestFitTestHeap.ShailoHeapAlloc(sizeof(char));
        *tempChar = 'a';
    }

    //Insert 50 characters which should go into [51,90] (360) and totally fill the heap
    for (size_t index = 0; index < 40; index++)
    {
        tempChar = (char*)bestFitTestHeap.ShailoHeapAlloc(sizeof(char));
        *tempChar = 'a';
    }

    std::cout << "Testing allocation of single integer------------------------------------------------------------------------------------------------\n"; //SUCCESS
    ShailoHeap heap = ShailoHeap(1000);
    int* integerPointer = (int*)heap.ShailoHeapAlloc(sizeof(int));
    *integerPointer = 255;
}

void HeapStatus(ShailoHeap& heapObject)
{
    std::cout << "Remaining Capacity: " << heapObject.ShailoHeapFreeCapacity() << " bytes\n";
    std::cout << "Max Capacity: " << heapObject.ShailoHeapMaxCapacity() << " bytes\n\n";
}

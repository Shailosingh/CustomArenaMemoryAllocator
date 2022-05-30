#include "ShailoHeap.h"
#include <iostream>

//Constructor and Deconstructors-----------------------------------------------------------------------------------------------------
ShailoHeap::ShailoHeap(size_t maxHeapCapacity)
{
	//Setup capacities
	FreeHeapCapacity = maxHeapCapacity;
	MaxHeapCapacity = maxHeapCapacity;

	//Reserve memory for heap capacity and retrieve a char pointer to the chunk
	InitialAllocatedHeapSpacePointer = (char*)malloc(maxHeapCapacity);
	if (InitialAllocatedHeapSpacePointer == nullptr)
	{
		std::cout << "UNABLE TO RESERVE " << maxHeapCapacity << " BYTES FOR HEAP!";
		exit(1);
	}

	//Setup free space vector with heap space and enough space to have default number of disjointed free spaces
	FreeSpaceList.reserve(DEFAULT_FREESPACE_MAX);
	FreeSpaceList.emplace_back(FreeNode(InitialAllocatedHeapSpacePointer, maxHeapCapacity));
}

ShailoHeap::~ShailoHeap()
{
	ShailoHeapDestroy();
}

//Public functions-------------------------------------------------------------------------------------------------------------------
void* ShailoHeap::ShailoHeapAlloc(size_t blockSize)
{
	//If the requested allocation is 0, return a nullptr
	if (blockSize == 0)
	{
		return nullptr;
	}

	//Ensure there is enough space in the heap, nullptr if not
	if (!IsEnoughSpace(blockSize))
	{
		return nullptr;
	}

	//Loop through all the free spaces searching for the smallest node large enough (Best-fit)
	void* allocationRequest;
	size_t smallestFreeSpaceIndex = 0;
	bool foundSpace = false;
	for (size_t index = 0; index < FreeSpaceList.size(); index++)
	{
		if (FreeSpaceList[index].SizeOfFreeSpace() <= FreeSpaceList[smallestFreeSpaceIndex].SizeOfFreeSpace() && FreeSpaceList[index].FreeSpaceLargeEnough(blockSize))
		{
			smallestFreeSpaceIndex = index;
			foundSpace = true;
		}
	}

	//If a free space is found, fulfill the allocation request on it
	if (foundSpace)
	{
		allocationRequest = FreeSpaceList[smallestFreeSpaceIndex].AllocateToFreeSpace(blockSize);

		//Update free heap capacity
		FreeHeapCapacity -= PaddedMemoryAllocation(blockSize);

		//Check if the allocation request left the free space empty and remove it from the vector if so
		if (FreeSpaceList[smallestFreeSpaceIndex].SizeOfFreeSpace() == 0)
		{
			FreeSpaceList.erase(FreeSpaceList.begin() + smallestFreeSpaceIndex);
		}

		return allocationRequest;
	}

	//If a free space is not found, return a nullptr as an error
	return nullptr;
}

void ShailoHeap::ShailoHeapFree(void* blockPointer)
{
	//Get the size of the block to be freed
	//size_t blockSize = ShailoHeapSize(blockPointer);

	//
}

size_t ShailoHeap::ShailoHeapSize(void* blockPointer) const
{
	//Get the size of the block allocation by converting the pointer to a size_t pointer and find the previous one
	size_t blockSize = *(((size_t*)blockPointer) - 1);
	return blockSize;
}

size_t ShailoHeap::ShailoHeapFreeCapacity() const
{
	return this->FreeHeapCapacity;
}

size_t ShailoHeap::ShailoHeapMaxCapacity() const
{
	return this->MaxHeapCapacity;
}

void ShailoHeap::ShailoHeapDestroy()
{
	//Set the capacities to 0, to disable the heap
	MaxHeapCapacity = 0;
	FreeHeapCapacity = 0;

	//Free all memory allocated in the heap
	free(InitialAllocatedHeapSpacePointer);

	//Destroy free space list
	FreeSpaceList.clear();
}

//Helper Functions-------------------------------------------------------------------------------------------------------------------
bool ShailoHeap::IsEnoughSpace(size_t blockSize)
{
	size_t totalAllocationSize = PaddedMemoryAllocation(blockSize);
	if (totalAllocationSize <= FreeHeapCapacity)
	{
		return true;
	}

	return false;
}
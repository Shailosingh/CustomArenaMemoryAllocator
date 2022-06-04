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
	size_t blockSize = ShailoHeapSize(blockPointer);

	//Check if the blockSize is invalid and terminate the program if so. Invalid means that the blockSize is 0 or is larger than the amount of bytes allocated
	size_t bytesAllocated = MaxHeapCapacity - FreeHeapCapacity;
	if (blockSize == 0 || blockSize > bytesAllocated)
	{
		std::cout << "Error in free(): " << blockSize << " bytes\n";
		exit(1);
	}

	//Update free heap capacity
	FreeHeapCapacity += PaddedMemoryAllocation(blockSize);

	//Create free node for given block
	char* charBlockPointer = ((char*)blockPointer) - sizeof(size_t); //Adjust for blockSize metadata
	FreeNode blockToBeFreed = FreeNode(charBlockPointer, PaddedMemoryAllocation(blockSize));

	//Cycle through every free node in the free list and try to merge them with the blockToBeFreed
	size_t mergedBlockIndex = FreeSpaceList.size();
	size_t priorSize = 0;
	size_t afterSize = 0;
	for (size_t index = 0; index < FreeSpaceList.size(); index++)
	{
		priorSize = FreeSpaceList[index].SizeOfFreeSpace();
		FreeSpaceList[index] = FreeSpaceList[index] + blockToBeFreed;
		afterSize = FreeSpaceList[index].SizeOfFreeSpace();

		if (priorSize != afterSize)
		{
			mergedBlockIndex = index;
			break;
		}
	}

	//If the blockToBeFreed was not merged into at least one free space, then it must be a disjoint free space and thus be appended to the list
	if (mergedBlockIndex == FreeSpaceList.size())
	{
		FreeSpaceList.emplace_back(blockToBeFreed);
		return;
	}

	//If the block was merged into one of the free spaces, it must be checked if that space can be merged into another.
	//If this happens, it means that blockToBeFreed is in between two free spaces consecutively
	FreeNode currentSpace = FreeSpaceList[mergedBlockIndex];
	FreeSpaceList.erase(FreeSpaceList.begin() + mergedBlockIndex);
	priorSize = 0;
	afterSize = 0;
	for (size_t index = mergedBlockIndex; index < FreeSpaceList.size(); index++)
	{
		priorSize = FreeSpaceList[index].SizeOfFreeSpace();
		FreeSpaceList[index] = FreeSpaceList[index] + currentSpace;
		afterSize = FreeSpaceList[index].SizeOfFreeSpace();

		//Second merge complete
		if (priorSize != afterSize)
		{
			return;
		}
	}

	//If the second merge couldn't be complete, put the removed space back in
	FreeSpaceList.emplace_back(currentSpace);
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
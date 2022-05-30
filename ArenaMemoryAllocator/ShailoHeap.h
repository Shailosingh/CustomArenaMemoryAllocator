#pragma once

#include <vector>
#include "FreeNode.h"

/*
The default max number of free spaces in the FreeSpaceList.
This number is important because, after there is this many free
spaces, the vector will have to be reallocated. It is set pretty high
so, only if there is A LOT of fragmentation, will it need to reallocate.

This should be set using FreeSpaceList.reserve(DEFAULT_FREESPACE_MAX)
*/
#define DEFAULT_FREESPACE_MAX 128

/*
* A macro to get the total size of an allocation taking into account all headers to
* memory allocation
*/
#define PaddedMemoryAllocation(allocation) (sizeof(size_t) + (allocation))

/// <summary>
/// Shailendra's custom heap object that manages memory using "Best-Fit Allocation"
/// </summary>
class ShailoHeap
{
private:
	//Datafields-----------------------------------------------------------------------------------------------------------------
	std::vector<FreeNode> FreeSpaceList;
	size_t FreeHeapCapacity;
	size_t MaxHeapCapacity;
	char* InitialAllocatedHeapSpacePointer;

	//Helper Functions-----------------------------------------------------------------------------------------------------------
	/// <summary>
	/// Check if there is enough space in the heap to store both the size of the block and the block.
	/// </summary>
	/// <param name="blockSize">The requested allocation size</param>
	/// <returns>True if there is enough space in the heap</returns>
	bool IsEnoughSpace(size_t blockSize);


public:
	//Constructors and deconstructors--------------------------------------------------------------------------------------------
	void operator=(const ShailoHeap&) = delete; //Disable copy constructor to ensure only one instance of the heap exists
	ShailoHeap(size_t maxHeapCapacity);
	~ShailoHeap();

	//Public functions-----------------------------------------------------------------------------------------------------------
	/// <summary>
	/// Allocates a block of memory to the heap using "Best-Fit Allocation."
	/// </summary>
	/// <param name="blockSize">The desired size of the block in bytes</param>
	/// <returns>The pointer to the allocated block. nullptr if anything goes wrong</returns>
	void* ShailoHeapAlloc(size_t blockSize);

	/// <summary>
	/// Frees a block of memory allocated to the heap.
	/// </summary>
	/// <param name="blockPointer">The pointer to the block to be freed</param>
	void ShailoHeapFree(void* blockPointer);

	/// <summary>
	/// Gets the size of an allocated block to this block in bytes
	/// </summary>
	/// <param name="blockPointer">The pointer to the block whose size will be checked</param>
	/// <returns>The size of the specified block</returns>
	size_t ShailoHeapSize(void* blockPointer) const;

	/// <summary>
	/// Gets the number of unallocated bytes in the heap
	/// </summary>
	/// <returns>The remaining capacity of the heap</returns>
	size_t ShailoHeapFreeCapacity() const;

	/// <summary>
	/// Gets the total size of the heap in bytes
	/// </summary>
	/// <returns>Total heap size in bytes</returns>
	size_t ShailoHeapMaxCapacity() const;

	/// <summary>
	/// All allocated pages are returned back to the OS and the heap is ready to be discarded. It should never
	/// be used again.
	/// </summary>
	void ShailoHeapDestroy();
};
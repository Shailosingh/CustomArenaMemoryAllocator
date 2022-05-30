#pragma once

/// <summary>
/// Keeps track of the location and size of a free space in memory
/// </summary>
class FreeNode
{
private:
	//Datafields-----------------------------------------------------------------------------------------------------------------
	char* FreeMemoryStartPointer;
	size_t FreeSpaceSize;

public:
	//Constructors and destructors-----------------------------------------------------------------------------------------------
	FreeNode() = delete; //Disable default constructor
	FreeNode(char* freeMemoryStartPointer, size_t freeSpaceSize);

	//Operator overloads---------------------------------------------------------------------------------------------------------
	/// <summary>
	/// Addition override which will compact two consecutive free spaces together. If the free
	/// spaces are not consecutive, return a copy of "this" object
	/// </summary>
	/// <param name="other">The other node being added</param>
	/// <returns>Node object that combines the two consecutive nodes given.</returns>
	FreeNode operator+(const FreeNode& other) const;

	/// <summary>
	/// Compares two nodes based on the size of the free space
	/// </summary>
	/// <param name="other"></param>
	/// <returns>True if "this" is a smaller free space than the "other"</returns>
	bool operator<(const FreeNode& other) const;

	//Public functions-----------------------------------------------------------------------------------------------------------
	/// <summary>
	/// Allocates a block of memory within the free space block and returns a pointer
	/// to it. Ensure the free space is large enough first using FreeSpaceLargeEnough().
	/// </summary>
	/// <param name="allocationSize">The desired size of the memory allocation</param>
	/// <returns>A pointer to the allocated memory block.</returns>
	void* AllocateToFreeSpace(size_t allocationSize);

	/// <summary>
	/// Ensures there is enough space in the free space block to accomodate the desired
	/// memory allocation AND the size_t at the beginning which keeps track of the size
	/// of the block for ShailoHeapFree
	/// </summary>
	/// <param name="blockSize">The allocation size given by the user</param>
	/// <returns>True if the allocation will fit in the block, false if not</returns>
	bool FreeSpaceLargeEnough(size_t blockSize) const;

	/// <summary>
	/// Gets the size of this free space in bytes
	/// </summary>
	/// <returns>The free space size in bytes</returns>
	size_t SizeOfFreeSpace() const;
};

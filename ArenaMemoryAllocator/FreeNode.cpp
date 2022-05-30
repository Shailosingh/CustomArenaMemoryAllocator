#include "FreeNode.h"

//Constructor and Deconstructors-----------------------------------------------------------------------------------------------------
FreeNode::FreeNode(char* freeMemoryStartPointer, size_t freeSpaceSize)
{
	FreeMemoryStartPointer = freeMemoryStartPointer;
	FreeSpaceSize = freeSpaceSize;
}

//Operator overloads-----------------------------------------------------------------------------------------------------------------
FreeNode FreeNode::operator+(const FreeNode& other) const
{
	//Check if "other" is consecutively before "this" in memory
	if ((other.FreeMemoryStartPointer + other.FreeSpaceSize) == this->FreeMemoryStartPointer)
	{
		//Join other+this as one consecutive free node
		return FreeNode(other.FreeMemoryStartPointer, other.FreeSpaceSize + this->FreeSpaceSize);
	}

	//Check if "other" is consecutively after "this" in memory
	if ((this->FreeMemoryStartPointer + this->FreeSpaceSize) == other.FreeMemoryStartPointer)
	{
		//Join this+other as one consecutive free node
		return FreeNode(this->FreeMemoryStartPointer, this->FreeSpaceSize + other.FreeSpaceSize);
	}

	//If the nodes are not consecutive, simply return "this" node unaltered
	return *this;
}

bool FreeNode::operator<(const FreeNode& other) const
{
	return (this->FreeSpaceSize < other.FreeSpaceSize);
}

//Public functions-------------------------------------------------------------------------------------------------------------------
void* FreeNode::AllocateToFreeSpace(size_t allocationSize)
{
	//Record the size of the allocation into the free space
	*((size_t*)this->FreeMemoryStartPointer) = allocationSize;

	//Increment the pointer to conceal the recorded allocationSize in memory and only give the user the position they may write to
	this->FreeMemoryStartPointer += sizeof(size_t);
	this->FreeSpaceSize -= sizeof(size_t);

	//Record pointer to the allocation and then adjust the free node
	char* allocationPointer = this->FreeMemoryStartPointer;
	this->FreeMemoryStartPointer += allocationSize;
	this->FreeSpaceSize -= allocationSize;

	return (void*)allocationPointer;
}

bool FreeNode::FreeSpaceLargeEnough(size_t blockSize) const
{
	//Check if the allocation is too large for both the size metadata and allocation request
	if (!(sizeof(size_t) + blockSize <= this->FreeSpaceSize))
	{
		return false;
	}

	//If free space is large enough for potential allocation
	return true;
}

size_t FreeNode::SizeOfFreeSpace() const
{
	return this->FreeSpaceSize;
}
#pragma once
#include <malloc.h>
class PoolAllocator {
	//pool allocator is a singly linked list with memory allocation based on a multiple of the type of element residing within the list
	typedef unsigned char U8;
	struct Node{
		Node* next;
	};
	Node* head = nullptr;
private:
	U8* poolAddress;
	size_t pool_size;
	size_t element_size;
	size_t element_number;
public:
	PoolAllocator(size_t elNumber, size_t elSize);
	~PoolAllocator();
	void* allocate(size_t size);
	void deAllocate(void* node);
	void reset();
};
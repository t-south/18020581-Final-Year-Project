#include "PoolAllocator.h"


PoolAllocator::PoolAllocator(size_t elNumber, size_t elSize) : element_number(elNumber), element_size(elSize), pool_size(elSize* elNumber), poolAddress(nullptr) {
	poolAddress = reinterpret_cast<U8*>(malloc(pool_size));	
	Node* nodes = reinterpret_cast<Node*>(poolAddress);
	head = nodes;
	//allocate the next pointers for each of the elements creating a pool of free elements
	for (size_t i = 0; i < element_number; i++) {
		nodes->next = reinterpret_cast<Node*>(reinterpret_cast<char*>(nodes) + element_size);
		nodes = nodes->next;
	}
	nodes->next = nullptr;
}

PoolAllocator::~PoolAllocator() {
}
void* PoolAllocator::allocate(size_t size) {
	if (head->next != nullptr) {
		//got to next pointer and return the previous address
		Node* elem_address = head;
		head = head->next;
		return reinterpret_cast<void*>(elem_address);
	}
	return nullptr;
}

void PoolAllocator::deAllocate(void* node) {
	//set the next pointer for the passed through node to the current head
	Node* previous_node = reinterpret_cast<Node*>(node);
	previous_node->next = head;
	//set the current head pointer to the node that has been passed through
	head = reinterpret_cast<Node*>(node);
}


void PoolAllocator::reset() {	
	Node* nodes = reinterpret_cast<Node*>(poolAddress);
	head = nodes;
	//allocate the next pointers for each of the elements creating a pool of free elements
	for (size_t i = 0; i < element_number; i++) {
		nodes->next = reinterpret_cast<Node*>(reinterpret_cast<char*>(nodes) + element_size);
		nodes = nodes->next;
	}
	nodes->next = nullptr;
	//allocate the next pointers for each of the elements creating a pool of free elements
}
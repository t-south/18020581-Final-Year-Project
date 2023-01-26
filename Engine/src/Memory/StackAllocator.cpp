#include "StackAllocator.h"


StackAllocator::StackAllocator(size_t size): stack_size(size), offSet(0){ 	
	stack_top = reinterpret_cast<U8*>(malloc(size));
	current_pos = stack_top;	
}

StackAllocator::~StackAllocator() {
	free(stack_top);
}

void* StackAllocator::alloc(size_t size, size_t align) {	
	//size = AlignAlloc::AlignAddress(size, align);
	U8* previousAddress = current_pos;	
	offSet = size;
	current_pos += size;
	return reinterpret_cast<void*>(previousAddress);
}

void StackAllocator::dealloc() {
	current_pos -= offSet;
}

void StackAllocator::reset() {
	current_pos = stack_top;
}


void StackAllocator::freeToMarker(Marker old_address) {
	current_pos = old_address.markerAddress;
}

StackAllocator::Marker StackAllocator::getMarker() {	
	return Marker{current_pos};
}
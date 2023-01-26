#include "DeStackAllocator.h"


DeStackAllocator::DeStackAllocator(size_t size) : stack_size(size) {
	stack_top[0] = reinterpret_cast<U8*>(malloc(size));
	current_pos[0] = stack_top[0];
	stack_top[1] = stack_top[0] + stack_size;
	current_pos[0] = stack_top[1];
}

DeStackAllocator::~DeStackAllocator() {
	free(stack_top);
}

void* DeStackAllocator::alloc(size_t size, uint8_t id, size_t align) {
	size = AlignAlloc::AlignAddress(size, align);
	U8* previousAddress = current_pos[id];
	offSet[id] = size;
	current_pos[id] += size;
	return reinterpret_cast<void*>(previousAddress);
}

void DeStackAllocator::reset() {
	current_pos[0] = stack_top[0];
	current_pos[1] = stack_top[1];
}

void* DeStackAllocator::deAlloc(uint8_t id) {
	current_pos[id] -= offSet[id];
	return reinterpret_cast<void*>(current_pos[id]);
}

void DeStackAllocator::freeToMarker(Marker old_address) {
	current_pos[old_address.id] = old_address.markerAddress;
}

DeStackAllocator::Marker DeStackAllocator::getMarker(uint8_t id) {
	return Marker{ current_pos[id], id};
}
#pragma once
#include <malloc.h>
#include "AlignAlloc.h"
#include <cstdint>

class DeStackAllocator {
public:
	typedef unsigned char U8;
	struct Marker { U8* markerAddress; uint8_t id; };
	explicit DeStackAllocator(size_t size);
	~DeStackAllocator();
	void* alloc(size_t size, uint8_t id, size_t align);
	void* deAlloc(uint8_t id);
	void freeToMarker(Marker old_address);
	Marker getMarker(uint8_t id);
	void reset();
private:
	U8* current_pos[2];
	size_t stack_size;
	U8* stack_top[2];
	size_t offSet[2];
};
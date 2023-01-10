#pragma once
#include <malloc.h>
#include "AlignAlloc.h"
class StackAllocator {
public:
	typedef unsigned char U8;
	struct Marker { U8* markerAddress; };
	explicit StackAllocator(size_t size);
	~StackAllocator();
	void* alloc(size_t size, size_t align);
	void freeToMarker(Marker old_address);
	Marker getMarker();	
	void reset();
private:	
	U8* current_pos;
	size_t stack_size;
	U8* stack_top;
	size_t offSet;

};
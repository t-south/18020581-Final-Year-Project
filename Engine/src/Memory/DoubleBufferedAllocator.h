#pragma once
#include "StackAllocator.h"
class DoubleBufferedAllocator {
	typedef unsigned int U32;
	U32 m_curStack;
	StackAllocator m_stack[2];
public:
	void swapBuffers();
	void clearCurrentBuffer();
	void* alloc(U32 nBytes, size_t align);
};
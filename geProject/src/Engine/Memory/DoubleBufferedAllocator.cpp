#include "DoubleBufferedAllocator.h"
void DoubleBufferedAllocator::swapBuffers() {
	m_curStack = (U32)!m_curStack;
}
void DoubleBufferedAllocator::clearCurrentBuffer() {
	m_stack[m_curStack].reset();
}
void* DoubleBufferedAllocator::alloc(U32 nBytes, size_t align) {
	return m_stack[m_curStack].alloc(nBytes, align);
}
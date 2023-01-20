#pragma once
#include <cstddef>
#include <cassert>
typedef unsigned char U8;
namespace AlignAlloc {
	inline uintptr_t AlignAddress(uintptr_t address, size_t align) {
		const size_t mask = align - 1;
		assert(align && mask == 0);
		return (address + mask) & ~mask;
	}
	template<typename T> inline T* AlignPointer(T* ptr, size_t align) {
		const uintptr_t address = reinterpret_cast<uintptr_t>(ptr);
		const uintptr_t aligned = AlignAddress(address, align);
		return reinterpret_cast<T*>(aligned);
	}
	void FreeAligned(void* pMem);
	void* AllocAligned(size_t bytes, size_t align);
}
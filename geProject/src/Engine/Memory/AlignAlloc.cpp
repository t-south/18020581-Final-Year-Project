#include "AlignAlloc.h"
//credit to game engine architecture by Jason Gregory and "are we out of memory" 2008 by christian gyrling



void AlignAlloc::FreeAligned(void* pMem) {
	if (pMem) {
		U8* alignedMem = reinterpret_cast<U8*>(pMem);
		ptrdiff_t shift = alignedMem[-1];
		if (shift == 0)
			shift = 256;
		U8* pRawMem = alignedMem - shift;
		delete[] pRawMem;
	}
}


void* AlignAlloc::AllocAligned(size_t bytes, size_t align)
{
	size_t actualBytes = bytes + align;
	U8* pRawMem = new U8[actualBytes];
	U8* alignedMem = AlignPointer(pRawMem, align);
	if (pRawMem == alignedMem)
		alignedMem += align;
	ptrdiff_t shift = alignedMem - pRawMem;
	assert(shift > 0 && shift <= 256);
	alignedMem[-1] = static_cast<U8>(shift & 0xFF);
	return alignedMem;
}

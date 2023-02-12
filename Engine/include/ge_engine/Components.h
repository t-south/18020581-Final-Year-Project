#pragma once
namespace geProject {
	struct Transform {
		static const unsigned int id = 0x00000001;
		float position{ 1.0f };
		float rotation{ 2.0f };
		static PoolAllocator transformallocator;
		static void* operator new(size_t size) {
			return transformallocator.allocate(size);
		}
		static void operator delete(void* ptr) {
			return transformallocator.deAllocate(ptr);
		}
	};

	struct TestData {
		static const unsigned int id = 0x00000002;
		int x;
		int y;
		int z;
		static PoolAllocator testallocator;
		static void* operator new(size_t size) {
			return testallocator.allocate(size);
		}
		static void operator delete(void* ptr) {
			return testallocator.deAllocate(ptr);
		}
	};

	
}





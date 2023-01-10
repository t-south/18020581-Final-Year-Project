#include <iostream>
#include "memory/DoubleBufferedAllocator.h"
#include "memory/StackAllocator.h"
#include "memory/PoolAllocator.h"

using std::cout;
using std::endl;

class MemoryManager {
public:
	MemoryManager() {

	}
	~MemoryManager() {
	}
	void startup() {
		std::cout << "startup" << std::endl;

	}
	void shutdown() {
		std::cout << "shutdown" << std::endl;
	}


};

//MemoryManager mManager;
//StackAllocator g_singleFrameAllocator;
//DoubleBufferedAllocator g_doubleBuffAllocator;


struct Object {
	uint64_t data[2];
	static PoolAllocator allocator;
	static void* operator new(size_t size) {
		return allocator.allocate(size);
	}
	static void operator delete(void* ptr) {
		return allocator.deAllocate(ptr);
	}
};
constexpr int arraySize = 10;
PoolAllocator Object::allocator{ arraySize, 32 };

int main() {

	Object* objects[arraySize];
	cout << "size(object) = " << sizeof(Object) << endl << endl;
	cout << "About to allocate " << arraySize << " objects" << endl;
	for (int i = 0; i < arraySize; ++i) {
		objects[i] = new Object();
		cout << "new [" << i << "] = " << objects[i] << endl;
	}
	cout << endl;

	for (int i = arraySize - 1; i >= 0; --i) {
		cout << "delete [" << i << "] = " << objects[i] << endl;
		delete objects[i];
	}
	cout << endl;

	objects[0] = new Object();
	cout << "new [0] = " << objects[0] << endl << endl;


	/*
	mManager.startup();
	mManager.shutdown();
	size_t nBytes = 50;
	void* p = g_singleFrameAllocator.alloc(nBytes);
	*/
	//while (true) {
		//g_singleFrameAllocator.reset();
		//void* p = g_singleFrameAllocator.alloc(nBytes);
		//g_doubleBuffAllocator.swapBuffers();
		//g_doubleBuffAllocator.clearCurrentBuffer();
		//void* m = g_doubleBuffAllocator.alloc(nBytes);
	//}
	return 0;
}
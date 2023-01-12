#include "Resource.h"

void Resource::increaseRefCount() {
	refCount++;
}

void Resource::decreaseRefCount() {
	refCount--;
	if (refCount == 0) {
		unload();
	}
}

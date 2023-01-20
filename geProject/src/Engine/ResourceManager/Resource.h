#pragma once
#include <string>

class Resource {
public:
	Resource();
	std::string gUid;
	uint16_t refCount;
	virtual void load();
	virtual void unload();
	uint16_t getRefCount();
	void increaseRefCount();
	void decreaseRefCount();
private:

};
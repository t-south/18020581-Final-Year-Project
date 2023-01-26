#pragma once
#include <unordered_map>
#include <string>
#include <memory>
#include "../Memory/StackAllocator.h"
#include "Resource.h"
//ensure only on copy of each resource in loaded into memory


class ResourceManager {
	ResourceManager();
	~ResourceManager();
	static StackAllocator gameResources;
	std::unordered_map<std::string, std::unique_ptr<Resource>> registry;

	void load(std::string guId);
	void unload(std::string guId);
	void* requestResource(std::string guId);
	void loadLevel();
	void loadGlobalResources();
};
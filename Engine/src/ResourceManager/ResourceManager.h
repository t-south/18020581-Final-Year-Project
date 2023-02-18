#pragma once
#include <unordered_map>
#include <string>
#include <regex>
#include <memory>
#include "../Memory/StackAllocator.h"
#include "Resource.h"
#include "../Render/Shader.h"
#include "../Render/Texture.h"
//ensure only on copy of each resource in loaded into memory

namespace geProject {
	class ResourceManager {
	public:
		ResourceManager();
		~ResourceManager();
		static StackAllocator gameResources;
		std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
		std::unordered_map<std::string, std::shared_ptr<Texture>> textures;

		void loadTexture(std::string guId);
		void loadShader(std::string vert, std::string frag);
		void unloadTexture(std::string guId);
		void unloadShader(std::string guId);
		std::shared_ptr<geProject::Shader> requestShader(std::string guid);
		std::shared_ptr<geProject::Texture> requestTexture(std::string guid);
		void loadLevel();
		void loadGlobalResources();
	};
}
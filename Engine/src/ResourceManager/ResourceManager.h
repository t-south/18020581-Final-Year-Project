#pragma once
#include <unordered_map>
#include <string>
#include <regex>
#include <memory>
#include "../Memory/StackAllocator.h"
#include "../Render/SpriteSheet.h"
#include "../Render/Shader.h"
#include "../Render/Texture.h"
//ensure only on copy of each resource in loaded into memory

namespace geProject {
	class ResourceManager {
	public:
		ResourceManager();
		~ResourceManager();		

		void loadShader(std::string vert, std::string frag);
		void unloadShader(std::string guId);
		std::shared_ptr<geProject::Shader> requestShader(std::string guid);


		unsigned int loadTexture(std::string guId);
		void unloadTexture(unsigned int guId);
		std::shared_ptr<geProject::Texture> requestTexture(unsigned int guid);

		unsigned int loadSpriteSheet(std::string guId, unsigned int spriteNum, float spriteWidth, float spriteHeight, float borderspacing, int zIndex);
		unsigned int loadMap(std::string guId, unsigned int spriteNum, float spriteWidth, float spriteHeight, float borderspacing, int zIndex);
		void unloadSpriteSheet(unsigned int guId);
		std::shared_ptr<geProject::SpriteSheet> requestSpriteSheet(unsigned int guid);
		std::shared_ptr<geProject::SpriteSheet> requestLevelMap(unsigned int guid);

	private:
		static StackAllocator gameResources;
		std::unordered_map<std::string, std::shared_ptr<Shader>> shaders;
		std::unordered_map<unsigned int, std::shared_ptr<Texture>> textures;
		std::unordered_map<unsigned int, std::shared_ptr<SpriteSheet>> spritesheets;
		std::unordered_map<unsigned int, std::shared_ptr<SpriteSheet>> maps;
	};
}
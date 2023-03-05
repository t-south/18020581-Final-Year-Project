#include "ResourceManager.h"

geProject::ResourceManager::ResourceManager(){}

geProject::ResourceManager::~ResourceManager() {}

void geProject::ResourceManager::loadLevel() {
	//traverse all resources
	//increment reference count for all resources for that level
	//decrement ref count for all resource not in level
	//ref count at 0 are unloaded

	//allocate to top of stack
}

void geProject::ResourceManager::loadGlobalResources() {
	//allocate global resources to a stack
	//mark top of stack

}

//SHADER RESOURCES

void geProject::ResourceManager::loadShader(std::string vert, std::string frag) {
	std::shared_ptr<Shader> shader = std::make_shared<Shader>(vert.c_str(), frag.c_str());
	shaders.emplace(vert, shader);
}

void geProject::ResourceManager::unloadShader(std::string guId) {}

std::shared_ptr<geProject::Shader> geProject::ResourceManager::requestShader(std::string guId) {
	if (shaders.contains(guId)) {		
		return shaders.at(guId);
	}
	else {
		std::string frag = guId;
		frag = std::regex_replace(frag, std::regex("Vertex"), "Fragment");
		std::shared_ptr<Shader> shader = std::make_shared<Shader>(guId.c_str(), frag.c_str());
		shaders.emplace(guId, shader);
		return shader;
	}
}

//TEXTURE RESOURCES

unsigned int geProject::ResourceManager::loadTexture(std::string guId) {
	std::shared_ptr<Texture> texture = std::make_shared<Texture>(guId.c_str());
	unsigned int id = texture->getTextureId();
	auto it = textures.find(id);
	if (it != textures.end()) {
		it->second = texture;
	}
	else {
		textures.insert(std::make_pair(id, texture));
	}
	return id;
}

void geProject::ResourceManager::unloadTexture(unsigned int guId) {}


std::shared_ptr<geProject::Texture> geProject::ResourceManager::requestTexture(unsigned int guId) {
	if (textures.contains(guId)) {
		return textures.at(guId);
	}
}


//SPRITESHEET RESOURCES

unsigned int geProject::ResourceManager::loadSpriteSheet(std::string guId, unsigned int spriteNum, float spriteWidth, float spriteHeight, float borderspacing, int zIndex) {
	std::shared_ptr<SpriteSheet> spritesheet = std::make_shared<SpriteSheet>(guId.c_str(), spriteNum, spriteWidth, spriteHeight, borderspacing, zIndex);
	unsigned int id = spritesheet->getTextureId();
	auto it = spritesheets.find(id);
	if (it != spritesheets.end()) {
		it->second = spritesheet;
	}
	else {
		spritesheets.insert(std::make_pair(id, spritesheet));
	}
	return id;
}

void geProject::ResourceManager::unloadSpriteSheet(unsigned int guId) {}


std::shared_ptr<geProject::SpriteSheet> geProject::ResourceManager::requestSpriteSheet(unsigned int guId) {
	if (spritesheets.contains(guId)) {
		return spritesheets.at(guId);
	}
}
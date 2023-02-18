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

void geProject::ResourceManager::unloadShader(std::string guId) {

}
void geProject::ResourceManager::unloadTexture(std::string guId) {

}
void geProject::ResourceManager::loadShader(std::string vert, std::string frag) {
	std::shared_ptr<Shader> shader = std::make_shared<Shader>(vert.c_str(), frag.c_str());
	shaders.emplace(vert, shader);
}
void geProject::ResourceManager::loadTexture(std::string guId) {
	std::shared_ptr<Texture> texture = std::make_shared<Texture>(guId.c_str());
	textures.emplace(guId, texture);
}

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

std::shared_ptr<geProject::Texture> geProject::ResourceManager::requestTexture(std::string guId) {
	if (textures.contains(guId)) {
		return textures.at(guId);
	}
	else {
		std::shared_ptr<Texture> texture = std::make_shared<Texture>(guId.c_str());
		textures.emplace(guId, texture);
		return texture;
	}
}
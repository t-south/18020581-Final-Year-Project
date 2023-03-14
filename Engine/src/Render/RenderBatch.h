#pragma once
#include "Shader.h"
#include <ge_engine/Components.h>
#include <glm/glm.hpp>
#include "../Cameras/Camera.h"
#include "../ResourceManager/ResourceManager.h"
#define PI 3.14159265
namespace geProject {
	//credit to https: //stackoverflow.com/questions/23031674/3d-vertices-class-or-struct
	/*
	struct Vertex {
		float pos[2];
		float color[4];
		
	};
	
	struct Quad {
		Vertex* quad[4];
	};
	*/

	class RenderBatch {
	public:
		RenderBatch(int maxSize, unsigned int zIndex, ResourceManager& resources);
		~RenderBatch();
		bool isBatchFull();
		bool isTextureFull(unsigned int id);
		void addSprite(SpriteRender* sprite, Transform* transform);
		void updateSprite(SpriteRender* sprite, Transform* transform);
		void render(Camera& camera, std::string shaderPath);
		unsigned int getSpriteNum();
		unsigned int getZindex();
	private:
		unsigned int vao, vbo, maxBatch, vertSize, spriteNum, zIndex;
		bool hasUpdate{ true };
		ResourceManager* resourceManager;
		std::vector<float> vertices;
		std::shared_ptr<geProject::Texture> textures[8]{nullptr};
		std::vector<unsigned int> createIndexes();
		void createVertices(SpriteRender* sprite, Transform* transform, unsigned int index);
		void init();
		glm::vec2 rotate(glm::vec2 vert, glm::vec2 centre, float rotation);
		glm::vec2 getCentre(glm::vec2 bLeft, glm::vec2 tRight);
	};
}
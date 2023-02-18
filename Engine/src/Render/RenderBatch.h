#pragma once
#include "Shader.h"
#include <ge_engine/Components.h>
#include <glm/glm.hpp>
#include "Camera.h"
#include "../ResourceManager/ResourceManager.h"

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
		RenderBatch(int maxSize, ResourceManager& resources);
		~RenderBatch();
		bool isBatchFull();
		void addSprite(SpriteRender* sprite, Transform* transform);
		void render(Camera& camera);
	private:
		unsigned int vao, vbo, maxBatch, vertSize, spriteNum;
		
		ResourceManager* resourceManager;
		std::vector<float> vertices;
		
		std::vector<unsigned int> createIndexes();
		void init();
		
	};
}
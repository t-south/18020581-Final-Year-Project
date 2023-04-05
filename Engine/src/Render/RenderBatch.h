#pragma once
#include <ge_engine/Components.h>
#include <ge_engine/Core.h>
#include "Shader.h"
#include <glm/glm.hpp>
#include "../Cameras/Camera.h"



namespace geProject {
	class RenderBatch {
	public:
		RenderBatch(int maxSize, unsigned int zIndex);
		~RenderBatch();
		bool isBatchFull();
		bool isTextureFull(unsigned int id);
		void addSprite(SpriteRender* sprite, Transform* transform);
		void addMapTile(SpriteRender sprite, int ssId, float x, float y, int layer);
		void updateSprite(SpriteRender* sprite, Transform* transform);
		void render(Camera& camera, std::string shaderPath);
		unsigned int getSpriteNum();
		unsigned int getZindex();
		void removeVertices(unsigned int index);		
		int getUnusedRenderSection();
	private:
		unsigned int vao, vbo, maxBatch, vertSize, spriteNum, zIndex;
		bool hasUpdate{ true };
		bool buffer{ false };	
		std::vector<float> vertices;
		std::shared_ptr<geProject::Texture> textures[8]{nullptr};
		std::vector<unsigned int> createIndexes();
		void createVertices(SpriteRender* sprite, float x, float y, float scaleX, float scaleY, float rotation, unsigned int index);
		void init();
		glm::vec2 rotate(glm::vec2 vert, glm::vec2 centre, float rotation);			
	};
}
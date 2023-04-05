#pragma once

#include <ge_engine/Core.h>
#include <vector>
#include <memory>
#include "RenderBatch.h"



namespace geProject {
	class Renderer {
	public:		
		Renderer();
		~Renderer();

		//static void BeginScene(const OrthographicCamera& camera);
		void addSpriteToBatch(SpriteRender* sprite, Transform* transform);
		void updateSprite(SpriteRender* sprite, Transform* transform);
		static void render(Camera& camera, std::string shaderPath);
		unsigned int getZindexBatch(unsigned int zIndex);
		void renderMap(int mapId);
		void clear();
	private:
		static std::vector<RenderBatch> renderList;
		static const unsigned int maxBatch{ 500 };
		void deleteEntity(DeleteEntityEvent* e);
	};
}
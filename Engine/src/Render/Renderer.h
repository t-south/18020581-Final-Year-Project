#pragma once
#include <vector>
#include "RenderBatch.h"
#include "../EntityManager/EntityManager.h"
#include <memory>
#include "Camera.h"
#include "../ResourceManager/ResourceManager.h"


namespace geProject {
	class Renderer {
	public:
		Renderer(ResourceManager& resources);
		~Renderer();
		static void init();
		static void shutdown();
		//static void BeginScene(const OrthographicCamera& camera);
		void addSpriteToBatch(SpriteRender* sprite, Transform* transform);
		static void render(Camera& camera);
	private:
		EntityManager* manager;		
		ResourceManager* resourceManager;
		static std::vector<RenderBatch> renderList;
		static const unsigned int maxBatch{ 500 };
	};
}
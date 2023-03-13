#pragma once
#include <vector>
#include <memory>
#include "RenderBatch.h"
#include "../EntityManager/EntityManager.h"
#include "../ResourceManager/ResourceManager.h"
#include "Camera.h"


namespace geProject {
	class Renderer {
	public:
		Renderer(ResourceManager& resources);
		~Renderer();
		static void init();
		static void shutdown();
		//static void BeginScene(const OrthographicCamera& camera);
		void addSpriteToBatch(SpriteRender* sprite, Transform* transform);
		void updateSprite(SpriteRender* sprite, Transform* transform);
		static void render(Camera& camera, std::string shaderPath);

		void clear();
	private:
		ResourceManager* resourceManager;	
		static std::vector<RenderBatch> renderList;
		static const unsigned int maxBatch{ 500 };
	};
}
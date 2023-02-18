#pragma once
#include "Scene.h"
#include "../EntityManager/EntityManager.h"
#include "../ResourceManager/ResourceManager.h"

namespace geProject {
	class LevelEditorScene : public Scene {
	public:
		LevelEditorScene();
		~LevelEditorScene();		
		virtual void update(float deltaTime) override;	
		virtual void addEntityToScene(unsigned int entityId) override;
		virtual Camera* getCamera() override;
	private:
		std::vector<float> vertexArray;
		std::vector<unsigned int> elementOrder;		
		unsigned int vao, vbo, ebo;
		Camera* camera;
		ResourceManager* resourceManager;
		virtual void init() override;
	};
}
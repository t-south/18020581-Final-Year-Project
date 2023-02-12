#pragma once
#include "Scene.h"
#include "../EntityManager/EntityManager.h"



namespace geProject {
	class LevelEditorScene : public Scene {
	public:
		LevelEditorScene();
		~LevelEditorScene();		
		void update(float deltaTime);	
	private:
		std::vector<float> vertexArray;
		std::vector<unsigned int> elementOrder;
		unsigned int vao, vbo, ebo;
		geProject::Camera* camera;
		geProject::Shader* shader;
		geProject::Texture* testTexture;
		virtual void init() override;
	};
}
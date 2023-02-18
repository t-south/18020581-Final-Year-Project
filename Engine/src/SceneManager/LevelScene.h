#pragma once
#include "Scene.h"
namespace geProject {
	class LevelScene : public Scene {
	public:
		LevelScene();
		~LevelScene();
		void update(float deltaTime);
		void addEntityToScene(unsigned int entityId);
		Camera* getCamera();
	private:
		geProject::Camera* camera;
		void init();
	};
}
#pragma once
#include "Scene.h"
namespace geProject {
	class LevelScene : public Scene {
	public:
		LevelScene();
		~LevelScene();
		void update(float deltaTime);
	private:
		void init();
	};
}
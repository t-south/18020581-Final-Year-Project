#pragma once
#include "Scene.h"
#include "../Cameras/LevelCamera.h"

namespace geProject {
	class LevelScene : public Scene {
	public:
		LevelScene();
		~LevelScene();
		void update(float deltaTime);

		virtual void setPicking() {};
	private:
		virtual void init() override;
		virtual void saveGame(GameSaveEvent* save) {};
		PlayerController* player;
		Camera* camera;		
	};
}
#pragma once
#include "Scene.h"
#include "../Cameras/LevelCamera.h"

namespace geProject {
	class MenuScene : public Scene {
	public:
		MenuScene();
		~MenuScene();
		virtual void update(float deltaTime);
		virtual void setPicking();


	private:
		virtual void init();
		virtual void updateImgui();
		virtual void updateSceneImgui();
		virtual void setActiveEntity(int entityId);
		virtual unsigned int getActiveEntity();
		Camera* camera;
		FrameBuffer* selectionTextures;
		virtual void saveGame(GameSaveEvent* save);
	};
}
#pragma once
#include "Scene.h"
#include "../Cameras/LevelCamera.h"

namespace geProject {
	class LevelScene : public Scene {
	public:
		LevelScene();
		~LevelScene();
		void update(float deltaTime);
		virtual void updateImgui() override;
		virtual void updateSceneImgui() override;
		virtual void render(std::string shaderPath) override;
		virtual void setActiveEntity(int entityId) override;
		virtual unsigned int getActiveEntity() override;
		virtual void setEntityDrag(bool drag) override;
		virtual bool getEntityDrag() override;
		virtual void setPicking() override;
	private:
		void init();
		virtual void saveGame(GameSaveEvent* save) override;
		PlayerController* player;
		Camera* camera;		
	};
}
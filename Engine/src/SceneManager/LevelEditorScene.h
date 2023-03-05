#pragma once
#include "Scene.h"


namespace geProject {
	class LevelEditorScene : public Scene {
	public:
		LevelEditorScene();
		~LevelEditorScene();		
		virtual void update(float deltaTime) override;	
		virtual size_t addEntityToScene(unsigned int entityId) override;
		virtual void reAssignEntityToScene(unsigned int entitySceneId, unsigned int entityId) override;
		virtual Camera* getCamera() override;
		virtual void updateImgui() override;
		virtual void updateSceneImgui() override;
		virtual std::vector<Entity*> getEntities() override;

	private:
		Camera* camera{ nullptr };		
		unsigned int sprites;
		unsigned int testSpritesheet{ 14 };
		unsigned int loopcount{ 0 };
		unsigned int entity;
		virtual void init() override;

	};
}
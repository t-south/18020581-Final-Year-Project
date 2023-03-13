#pragma once
#include "Scene.h"
namespace geProject {
	class LevelScene : public Scene {
	public:
		LevelScene();
		~LevelScene();
		void update(float deltaTime);
		size_t addEntityToScene(unsigned int entityId);
		void reAssignEntityToScene(unsigned int entitySceneId, unsigned int entityId);
		Camera* getCamera();
		virtual void updateImgui() override;
		virtual void updateSceneImgui() override;
		virtual std::vector<Entity*> getEntities() override;
		virtual void render(std::string shaderPath) override;
		virtual void setActiveEntity(int entityId) override;
		virtual void setCameraControlLayout() override;
		virtual unsigned int getActiveEntity() override;
		virtual void setEntityDrag(bool drag) override;
		virtual bool getEntityDrag() override;
	private:
		geProject::Camera* camera{nullptr};
		void init();

	};
}
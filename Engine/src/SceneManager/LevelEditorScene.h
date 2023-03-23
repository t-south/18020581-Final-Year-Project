#pragma once
#include "Scene.h"
#include "../Render/EditorRender.h"
#include "../Cameras/EditorCamera.h"

namespace geProject {
	class LevelEditorScene : public Scene {
	public:
		LevelEditorScene();
		~LevelEditorScene();		
		virtual void update(float deltaTime) override;	
		virtual size_t addEntityToScene(unsigned int entityId) override;
		virtual void reAssignEntityToScene(unsigned int entitySceneId, unsigned int entityId) override;	
		virtual void updateImgui() override;
		virtual void updateSceneImgui() override;
		virtual void render(std::string shaderPath) override;
		virtual void setActiveEntity(int entityId) override;
		virtual unsigned int getActiveEntity() override;
		virtual void setEntityDrag(bool drag) override;
		virtual bool getEntityDrag() override;
		
		virtual void setPicking() override;		
	private:
		int activatedEntity{ -1 };
		float gridWidth, gridHeight;
		bool entityDrag{ false };	
		bool entityClicked{ false };
	
		EditorCamera* editorCam;
		EditorRender* editor;
		std::vector<unsigned int> sprites;
		unsigned int testSpritesheet{ 14 };
		unsigned int loopcount{ 0 };
		float x{ 0.0f };
		float y{ 0.0f };
		unsigned int entity;
		virtual void init() override;
		unsigned int createEditorBlock(SpriteRender* sprite, float sizeX, float sizeY);
		void setGridLines();
		virtual void saveGame(GameSaveEvent* save) override;
		
	};
}
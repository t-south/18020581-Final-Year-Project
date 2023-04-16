#pragma once
#include "Scene.h"
#include "../Render/EditorRender.h"
#include "../Cameras/EditorCamera.h"
#include "../Windows/HierarchyWindow.h"
#include "../AI/Enemy.h"

namespace geProject {
	class LevelEditorScene : public Scene {
	public:
		LevelEditorScene();
		~LevelEditorScene();		
		virtual void update(float deltaTime) override;	
		virtual void setPicking() override;		
		void updateImgui();
		void updateSceneImgui();
		void setActiveEntity(int entityId);
		unsigned int getActiveEntity();
		void setEntityDrag(bool drag);
		bool getEntityDrag() ;		
	private:
		int activatedEntity{ -1 };
		int previousEntity{ -1 };
		float gridWidth, gridHeight;
		bool entityDrag{ false };	
		bool entityClicked{ false };
		bool gridSelected{ false };	
		bool displayColliders{ false };
		EditorCamera* editorCam;
		EditorRender* editor;
		HierarchyWindow* sceneHierarchy;
		PlayerController* player;
		Camera* camera;
		unsigned int testSpritesheet{ 14 };
		unsigned int loopcount{ 0 };
		unsigned int entity;

		virtual void init() override;
		unsigned int createEnvironmentBlock(SpriteRender* sprite, float sizeX, float sizeY, entityTypes type);
		unsigned int createCharacterBlock(SpriteRender* sprite, float sizeX, float sizeY, entityTypes type);
		void setGridLines();
		virtual void saveGame(GameSaveEvent* save) override;			
		void deleteEntity(DeleteEntityEvent* e);
		void updateCopy(CopyEntityEvent* e);
		void editorKeyEvent(KeyPressedEvent* e);
		
	};
}
#pragma once
#include <ge_engine/Components.h>
#include <fstream>
#include <iostream>
#include "json.hpp"
#include "../Inputs/KeyboardListener.h"
#include "../Inputs/MouseListener.h"
#include "../Windows/Window.h"
#include "../Render/Renderer.h"
#include "../Render/FrameBuffer.h"
#include "../Physics/Physics.h"





using json = nlohmann::json;
namespace geProject {
	class Scene {
	public:
		virtual void update(float deltaTime) = 0;
		virtual void init() = 0;
		virtual size_t addEntityToScene(unsigned int entityId) = 0;
		virtual void reAssignEntityToScene(unsigned int entitySceneId, unsigned int entityId) = 0;
		virtual Camera* getCamera() = 0;
		virtual void updateImgui() = 0;
		virtual void updateSceneImgui() = 0;
		virtual void render(std::string shaderPath) = 0;
		virtual std::vector<Entity*> getEntities() = 0;
		virtual void setActiveEntity(int entityId) = 0;
		virtual unsigned int getActiveEntity() = 0;
		virtual void setCameraControlLayout() = 0;
		virtual void setEntityDrag(bool drag) = 0;
		virtual bool getEntityDrag() = 0;
		virtual void setPicking() = 0;
		void setWindow(Window* window);
		void setMouseListener();
		void setKeyboardListener();
		float getMouseX();
		float getMouseY();
		MouseListener* getMouseListener();
		void setViewPos(float x, float y);
		void setViewSize(float x, float y);
		void reloadLevel(std::string filePath);
		//friend class SceneSerialize;
	protected:
		std::vector<Entity*> entities;
		Window* gameWindow;
		MouseListener* mouse;
		KeyboardListener* keyboard;
		ResourceManager* resourceManager{ nullptr };
		EntityManager* manager{ nullptr };
		Renderer* renderer{ nullptr };
		FrameBuffer* selectionTextures{ nullptr };
		Physics* physicsManager{ nullptr };

		//(de)serialisation
		std::string filePath;
		std::ofstream oFile;

		void serialize(std::string filepath);
		json serializeEntity(Entity& entity);
		void deserialize(std::string filepath);

		//overrides for serialising to/from json
		void to_json(json& data, FontRender& comp);		
		void to_json(json& data, SpriteRender& comp);
		void to_json(json& data, Transform& comp);
		void to_json(json& data, Rigidbody& comp);
		void to_json(json& data, CircleCollider& comp);
		void to_json(json& data, BoxCollider& comp);

		void from_json(json& data, FontRender& comp);
		void from_json(json& data, SpriteRender& comp);
		void from_json(json& data, Transform& comp);
		void from_json(json& data, Rigidbody& comp);
		void from_json(json& data, CircleCollider& comp);
		void from_json(json& data, BoxCollider& comp);

		//event listeners
		virtual void startGamePlay(GameStartEvent* start) = 0;
		virtual void stopGamePlay(GameStopEvent* stop) = 0;
		virtual void saveGame(GameSaveEvent* save) = 0;

	};
}
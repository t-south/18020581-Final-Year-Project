#pragma once
#include <ge_engine/Components.h>
#include <fstream>
#include <iostream>
#include <ge_engine/json.hpp>
#include "../Inputs/KeyboardListener.h"
#include "../Inputs/MouseListener.h"
#include "../Windows/Window.h"
#include "../Render/Renderer.h"
#include "../Render/FrameBuffer.h"
#include "../Physics/Physics.h"
#include "../Animation/AnimationManager.h"




using json = nlohmann::json;
namespace geProject {
	class Scene {
	public:
		virtual void update(float deltaTime) = 0;
		virtual void init() = 0;
		virtual size_t addEntityToScene(unsigned int entityId) = 0;
		virtual void reAssignEntityToScene(unsigned int entitySceneId, unsigned int entityId) = 0;
		Camera* getCamera();
		virtual void updateImgui() = 0;
		virtual void updateSceneImgui() = 0;
		virtual void render(std::string shaderPath) = 0;	
		virtual void setActiveEntity(int entityId) = 0;
		virtual unsigned int getActiveEntity() = 0;
		virtual void setEntityDrag(bool drag) = 0;
		virtual bool getEntityDrag() = 0;
		virtual void setPicking() = 0;
		void setWindow(Window* window);
		void setMouseListener();
		void setKeyboardListener();
		float getMouseX();
		float getMouseY();
		MouseListener* getMouseListener();
		void reloadLevel(std::string filePath);
		std::string getFilePath();
		void serialize(std::string filepath);
		void deserialize(std::string filepath);
		void setPhysics(bool check);
		//friend class SceneSerialize;
	protected:
		std::unordered_map<int, Entity*> entities;		
		static Window* gameWindow;
		MouseListener* mouse;
		KeyboardListener* keyboard;
		//MANAGERS
		static ResourceManager* resourceManager;
		static Physics* physicsManager;
		static EntityManager* manager;
		static AnimationManager* animationManager;

		Camera* camera;
		Renderer* renderer{ nullptr };
		FrameBuffer* selectionTextures{ nullptr };
		//(de)serialisation
		std::string filePath;
		std::ofstream oFile;		
		bool physicsEnabled{ false };



		json serializeEntity(Entity& entity);
		//overrides for serialising to/from json
		void to_json(json& data, Animation& comp);		
		void to_json(json& data, SpriteRender& comp);
		void to_json(json& data, Transform& comp);
		void to_json(json& data, Rigidbody& comp);
		void to_json(json& data, CircleCollider& comp);
		void to_json(json& data, BoxCollider& comp);
		void to_json(json& data, Controls& comp);

		void from_json(json& data, Animation& comp);
		void from_json(json& data, SpriteRender& comp);
		void from_json(json& data, Transform& comp);
		void from_json(json& data, Rigidbody& comp);
		void from_json(json& data, CircleCollider& comp);
		void from_json(json& data, BoxCollider& comp);
		void from_json(json& data, Controls& comp);

		//event listeners
		virtual void saveGame(GameSaveEvent* save) = 0;

	};
}
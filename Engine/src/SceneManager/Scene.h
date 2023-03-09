#pragma once
#include <ge_engine/Components.h>
#include <fstream>
#include <iostream>
#include "json.hpp"
#include "../Inputs/KeyboardListener.h"
#include "../Inputs/MouseListener.h"
#include "../Render/Window.h"
#include "../Render/Renderer.h"



using json = nlohmann::json;
namespace geProject {
//abstract scene class
	class Scene {
	public:
		virtual void update(float deltaTime) = 0;
		virtual void init() = 0;
		virtual size_t addEntityToScene(unsigned int entityId) = 0;
		virtual void reAssignEntityToScene(unsigned int entitySceneId, unsigned int entityId) = 0;
		virtual Camera* getCamera() = 0;
		virtual void updateImgui() = 0 ;
		virtual void updateSceneImgui() = 0;
		float getMouseX();
		float getMouseY();
		virtual std::vector<Entity*> getEntities() = 0;
		void setWindow(Window* window);
		//friend class SceneSerialize;
	protected:		
		void setMouseListener();
		void setKeyboardListener();
		std::vector<Entity*> entities;
		Window* gameWindow;
		MouseListener* mouse;
		KeyboardListener* keyboard;
		ResourceManager* resourceManager{ nullptr };
		EntityManager* manager{ nullptr };
		Renderer* renderer{ nullptr };	
		//(de)serialisation
		std::string filePath;
		std::ofstream oFile;
		void serialize(std::string filepath);
		void deserialize(std::string filepath);
		json serializeEntity(unsigned int entityId);
		//overrides for serialising to/from json
		void to_json(json& data, FontRender& comp);
		void to_json(json& data, SpriteRender& comp);
		void to_json(json& data, Transform& comp);
		void to_json(json& data, Rigidbody& comp);
		void from_json(json& data, FontRender& comp);
		void from_json(json& data, SpriteRender& comp);
		void from_json(json& data, Transform& comp);
		void from_json(json& data, Rigidbody& comp);
		

	};
}
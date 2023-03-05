#pragma once
#include <iostream>
#include <imgui.h>
#include <vector>
#include <string>
#include <fstream>
#include <glm/glm.hpp>
#include <ge_engine/Entity.h>
#include "../Render/Shader.h"
#include "../Render/Camera.h"
#include "../Render/Texture.h"
#include "../Render/Renderer.h"
#include "../EntityManager/EntityManager.h"
#include "../ResourceManager/ResourceManager.h"
//#include "SceneSerialize.h"
#include "json.hpp"
//#include "SceneSerialize.h"

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
		virtual std::vector<Entity*> getEntities() = 0;
		//friend class SceneSerialize;
	protected:		
		int activatedEntity{-1};
		std::vector<Entity*> entities;
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
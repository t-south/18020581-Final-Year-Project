/*#pragma once
#include "Scene.h"
#include <string>
#include <fstream>
#include <iostream>
#include "json.hpp"
#include <ge_engine/Components.h>


using json = nlohmann::json;
namespace geProject {
	class SceneSerialize {
	public:
		SceneSerialize(std::shared_ptr<Scene> currentScene);
		~SceneSerialize();
		void serialize(const std::string& filePath);
		void deserialize(const std::string& filePath);
		json serializeEntity(unsigned int entityId);
	private:
		std::shared_ptr<Scene> scene{ nullptr };
		std::ofstream oFile;
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
	
}*/
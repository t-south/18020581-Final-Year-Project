#pragma once
#include <vector>
#include <unordered_map>
#include <ge_engine/Components.h>
#include <ge_engine/Core.h>
#include "../EntityManager/EntityManager.h"
#include "../ResourceManager/ResourceManager.h"
#include <ge_engine/json.hpp>

using json = nlohmann::json;
namespace geProject {
	enum AnimationState {
		Default, Running, Dash
	};

	struct Frame {
		float time;
		int sprite;
	};
	class AnimationManager {
	public:
		AnimationManager(EntityManager& manager, ResourceManager& resources);
		~AnimationManager();
		void assignEntityAnimation(int entityId, std::string state);
		void removeEntity();
		void update(float deltaTime);
		void changeState(int entityId, std::string newState);
		void serializeAnimations();
	private:
		std::unordered_map<std::string, std::vector<Frame>> animationList; // animation state -- key,  vector of frames containing the sprite and how much time to keep that sprite on screen
		std::unordered_map<std::string, std::vector<std::string>> availableStates;    // a mapping of states to all available other states
		EntityManager* entityManager;
		ResourceManager* resources;
		void deserializeAnimations();
		void to_json(json& data, std::string state);
		void from_json(json& data, Frame& comp);
		std::string filePath{ "../../../../Game/assets/levels/animations.json" };
		std::ofstream oFile;
	};
}
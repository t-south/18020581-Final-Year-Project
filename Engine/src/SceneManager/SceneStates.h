#pragma once
#include "Scene.h"
#include <unordered_map>
#include <memory>
namespace geProject {
	class SceneStates {
	public:
		SceneStates();
		~SceneStates();
		void switchScene(int id);
		int addScene(std::shared_ptr<Scene> scene);
		void removeScene(int id);
		void update(float deltaTime);
	private:
		//dictionary of scenes with key value int id's
		std::unordered_map<int, std::shared_ptr<Scene>> scenes;
		int currentSceneId;
		int sceneCount;
	};
}
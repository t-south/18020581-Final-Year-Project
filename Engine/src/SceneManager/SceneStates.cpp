#include "SceneStates.h"


geProject::SceneStates::SceneStates(): currentSceneId(0), sceneCount(0){
}

int geProject::SceneStates::addScene(std::shared_ptr<Scene> scene) {
	sceneCount++;
	scenes.emplace(sceneCount, scene);
	//switch to first scene added to scenemanager as default
	if (sceneCount == 1) {
		switchScene(sceneCount);
	}
	return sceneCount;
}

void geProject::SceneStates::switchScene(int id) {
	if (id <= sceneCount && id > 0) 
		currentSceneId = id;
}

void geProject::SceneStates::removeScene(int id) {
	if (id <= sceneCount && id > 0) {
		scenes.erase(id);
		sceneCount--;
	}
}

std::shared_ptr<geProject::Scene> geProject::SceneStates::getCurrentScene() {
	return scenes.at(currentSceneId);
}

void geProject::SceneStates::update(float deltaTime) {

}
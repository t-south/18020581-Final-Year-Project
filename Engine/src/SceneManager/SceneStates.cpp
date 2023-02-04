#include "SceneStates.h"

geProject::SceneStates::SceneStates(): currentSceneId(0){
}

geProject::SceneStates::addScene(std::shared_ptr<Scene> scene) {
	sceneCount++;
	scenes.emplace(sceneCount, scene);
}

geProject::SceneStates::switchScene(int id) {
	if (id <= sceneCount && id > 0) 
		currentSceneId = id;
}

geProject::SceneStates::removeScene(int id) {
	if (id <= sceneCount && id > 0) {
		scenes.erase(id);
		sceneCount--;
	}
}

geProject::SceneStates::update(float deltaTime) {

}
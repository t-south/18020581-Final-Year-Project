#include "LevelScene.h"

geProject::LevelScene::LevelScene(){
	std::cout << "Level Scene!" << std::endl;
}

geProject::LevelScene::~LevelScene(){}

void geProject::LevelScene::init() {}

void geProject::LevelScene::addEntityToScene(unsigned int entityId) {}

geProject::Camera* geProject::LevelScene::getCamera() {
	return camera;
}

void geProject::LevelScene::update(float deltaTime){}
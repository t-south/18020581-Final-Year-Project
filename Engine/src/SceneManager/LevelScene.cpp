#include "LevelScene.h"

geProject::LevelScene::LevelScene(){	
	std::cout << "Level Scene!" << std::endl;
}

geProject::LevelScene::~LevelScene(){}

void geProject::LevelScene::init() {}

size_t geProject::LevelScene::addEntityToScene(unsigned int entityId) { return 1; }
void geProject::LevelScene::reAssignEntityToScene(unsigned int entitySceneId, unsigned int entityId) {

}
geProject::Camera* geProject::LevelScene::getCamera() {
	return camera;
}

void geProject::LevelScene::update(float deltaTime){}
void geProject::LevelScene::render(std::string shaderPath){}
void geProject::LevelScene::updateImgui() {}
void geProject::LevelScene::updateSceneImgui() {}
std::vector<geProject::Entity*> geProject::LevelScene::getEntities() { return entities; }
void geProject::LevelScene::setActiveEntity(int entityId){}
void geProject::LevelScene::setCameraControlLayout() {}
unsigned int geProject::LevelScene::getActiveEntity() { return 0; }
void geProject::LevelScene::setEntityDrag(bool drag) {}
bool geProject::LevelScene::getEntityDrag() { return false; }

void geProject::LevelScene::setPicking(){}


void geProject::LevelScene::startGamePlay(GameStartEvent* start) {
	if (start->getType() == Type::gameStart) {
		std::cout << "level scene starting play" << std::endl;
	}
}

void geProject::LevelScene::stopGamePlay(GameStopEvent* stop) {
	if (stop->getType() == Type::gameStop) {
		std::cout << "level scene stopping play" << std::endl;
	}
}

void geProject::LevelScene::saveGame(GameSaveEvent* save) {
	if (save->getType() == Type::gameSave) {

	}
}

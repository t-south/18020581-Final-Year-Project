#include "LevelScene.h"


geProject::LevelScene::LevelScene(){	
	std::cout << "Level Scene!" << std::endl;
	camera = new LevelCamera(glm::vec2(-250.0f, 0.0f));
	filePath = "../../../../Game/assets/levels/level1.json";
}

geProject::LevelScene::~LevelScene(){}

void geProject::LevelScene::init() {
	manager->assignUpdate();	
}

size_t geProject::LevelScene::addEntityToScene(unsigned int entityId) { return 1; }
void geProject::LevelScene::reAssignEntityToScene(unsigned int entitySceneId, unsigned int entityId) {

}


void geProject::LevelScene::update(float deltaTime){
	physicsManager->update(deltaTime);
	
	//UPDATES TO RENDERING
	if (manager->hasUpdate()) {
		entities.clear();
		for (int i = 0; i < manager->getEntityNum(); i++) {
			entities.push_back(manager->getEntity(i));
			// only sprites that have not been added to the renderer previously will be set to 0
			auto trans = manager->getTransformComponent(i);
			auto sprite = manager->getSpriteComponent(i);
			//transform dirtyflag for render index is by default set to -1 when first created
			if (trans->dirtyFlag[2] == -1) {
				renderer->addSpriteToBatch(sprite, trans);
				trans->dirtyFlag[0] = 0;
			}
			//if there has been any updates the dirty flag in transform component will be set to 1
			else if (trans->dirtyFlag[0] == 1) {
				renderer->updateSprite(sprite, trans);
			}
		}
		manager->endFrame();
	}
	mouse->endFrame();
	render("../../../../Game/assets/shaders/VertexShaderDefault.glsl");
}




void geProject::LevelScene::render(std::string shaderPath){
	renderer->render(*camera, shaderPath);
}
void geProject::LevelScene::updateImgui() {}
void geProject::LevelScene::updateSceneImgui() {}
std::vector<geProject::Entity*> geProject::LevelScene::getEntities() { return entities; }
void geProject::LevelScene::setActiveEntity(int entityId){}

unsigned int geProject::LevelScene::getActiveEntity() { return 0; }
void geProject::LevelScene::setEntityDrag(bool drag) {}
bool geProject::LevelScene::getEntityDrag() { return false; }

void geProject::LevelScene::setPicking(){}


void geProject::LevelScene::saveGame(GameSaveEvent* save) {
	if (save->getType() == Type::gameSave) {

	}
}

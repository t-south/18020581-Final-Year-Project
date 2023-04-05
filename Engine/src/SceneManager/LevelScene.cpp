#include "LevelScene.h"


geProject::LevelScene::LevelScene(){	
	std::cout << "Level Scene!" << std::endl;
	camera = new LevelCamera(glm::vec2(0.0f, 0.0f));
	filePath = "../../../../Game/assets/levels/level1.json";
	player = new PlayerController(*entitymanager, *physicsmanager, *camera, entitymanager->getPlayerId());
}

geProject::LevelScene::~LevelScene(){}

void geProject::LevelScene::init() {
	entitymanager->assignUpdate();
	camera->setPosition(glm::vec2(0, 0));
}

size_t geProject::LevelScene::addEntityToScene(unsigned int entityId) { return 1; }
void geProject::LevelScene::reAssignEntityToScene(unsigned int entitySceneId, unsigned int entityId) {

}


void geProject::LevelScene::update(float deltaTime){

	camera->update(deltaTime);
	physicsmanager->update(deltaTime);
	if (player != nullptr) {
		Command* command = controlManager->action();
		player->update(deltaTime);
		if (command)
		{
			command->execute(*player);
		}
	}
	
	//UPDATES TO RENDERING
	if (entitymanager->hasUpdate()) {
		entities.clear();
		for (int i = 0; i < entitymanager->getEntityNum(); i++) {
			auto ent = entitymanager->getEntity(i);
			if (ent->compMask > 0 && ent->id > -1) {
				entities[ent->id] = ent;
				// only sprites that have not been added to the renderer previously will be set to 0
				auto trans = entitymanager->getTransformComponent(ent->id);
				auto sprite = entitymanager->getSpriteComponent(ent->id);
				//transform dirtyflag for render index is by default set to -1 when first created
				if (trans->dirtyFlag[2] == -1) {
					rendermanager->addSpriteToBatch(sprite, trans);
					trans->dirtyFlag[0] = 0;
				}
				//if there has been any updates the dirty flag in transform component will be set to 1
				else if (trans->dirtyFlag[0] == 1) {
					rendermanager->updateSprite(sprite, trans);
				}
			}
		}
		entitymanager->endFrame();
	}


	animationManager->update(deltaTime);
	keyboard->endFrame();
	entitymanager->endFrame();
	mouse->endFrame();
	render("../../../../Game/assets/shaders/VertexShaderDefault.glsl");
}




void geProject::LevelScene::render(std::string shaderPath){
	rendermanager->render(*camera, shaderPath);
}
void geProject::LevelScene::updateImgui() {}
void geProject::LevelScene::updateSceneImgui() {}

void geProject::LevelScene::setActiveEntity(int entityId){}

unsigned int geProject::LevelScene::getActiveEntity() { return 0; }
void geProject::LevelScene::setEntityDrag(bool drag) {}
bool geProject::LevelScene::getEntityDrag() { return false; }

void geProject::LevelScene::setPicking(){}


void geProject::LevelScene::saveGame(GameSaveEvent* save) {
	if (save->getType() == Type::gameSave) {

	}
}

#include "LevelScene.h"


geProject::LevelScene::LevelScene(){	
	std::cout << "Level Scene!" << std::endl;
	camera = new LevelCamera(glm::vec2(0.0f, 0.0f));
	filePath = "../../../../Game/assets/levels/level1.json";
	player = new PlayerController(*camera);
}

geProject::LevelScene::~LevelScene(){}

void geProject::LevelScene::init() {
	entitymanager.assignUpdate();
	camera->setPosition(glm::vec2(0, 0));
	for (auto& i : entitymanager.getEnemyIds()) {
		enemies[i] = new Enemy(i);
	}
}

void geProject::LevelScene::update(float deltaTime){

	camera->update(deltaTime);
	physicsmanager.update(deltaTime);
	if (player != nullptr) {
		Command* command = controlManager->action(deltaTime);
		player->update();
		if (command)
		{
			command->execute(*player);
		}
	}
	for (auto& enemy : enemies) {
		enemy.second->update(deltaTime);
	}

	//UPDATES TO RENDERING
	if (entitymanager.hasUpdate()) {
		for (int i = 0; i < entitymanager.getEntityNum(); i++) {
			Entity ent = entitymanager.getEntity(i);
			if (ent.compMask > 0 && ent.id > -1) {			
				// only sprites that have not been added to the renderer previously will be set to 0
				//transform dirtyflag for render index is by default set to -1 when first created
				if (entitymanager.getBatchStatus(ent.id) == -1) {
					rendermanager->addSpriteToBatch(ent.id);				
				}
				//if there has been any updates the dirty flag in transform component will be set to 1
				else if (entitymanager.getUpdateStatus(ent.id) == 1) {
					rendermanager->updateSprite(ent.id);
				}
			}
		}
		entitymanager.endFrame();
	}


	animationManager->update(deltaTime);
	keyboard->endFrame();
	entitymanager.endFrame();
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



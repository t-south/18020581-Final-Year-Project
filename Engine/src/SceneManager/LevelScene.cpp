#include "LevelScene.h"


geProject::LevelScene::LevelScene(){}

geProject::LevelScene::~LevelScene(){}

void geProject::LevelScene::init() {
	camera = new LevelCamera(glm::vec2(0.0f, 0.0f));
	filePath = "../../../../Game/assets/levels/level1.json";
	std::shared_ptr<SpriteSheet> mapDims = resourcemanager.requestLevelMap(1);
	int worldWidth = mapDims->getSpriteSheetWidth();
	int worldHeight = mapDims->getSpriteSheetHeight();
	worldstate.generateWorldMap(worldWidth, worldHeight);
	physicsmanager.startUp();
	controlManager = new Receiver();
	geProject::Scene::deserialize(filePath);
	for (int i = 0; i < entitymanager.getEntityNum(); i++) {
		Entity ent = entitymanager.getEntity(i);
		if ((ent.compMask & 4) == 4) {//check for rigidbody
			if ((ent.compMask & 8) == 8 || (ent.compMask & 16) == 16) {//check for boxcollider or circlecollider
				physicsmanager.addEntity(ent.id);
			}
		}
		if (entitymanager.getVertexStatus(ent.id) == -1) {
			rendermanager->addSpriteToBatch(ent.id);
		}
	}

	int playerId = entitymanager.getPlayerId();
	if (playerId > -1) {
		player = new PlayerController(*camera);
		entitymanager.assignHealth(playerId, Health());
	}

	for (auto& i : entitymanager.getEnemyIds()) {
		enemies[i] = new Enemy(i, goapEnabled);
	}

	rendermanager->renderMap(1);
	eventSystem.setContext(GameplayContext);
}

void geProject::LevelScene::update(float deltaTime){
	glClearColor(0, 0, 0, 1); 
	glClear(GL_COLOR_BUFFER_BIT);
	camera->update(deltaTime);
	if (player != nullptr) {
		Command* command = controlManager->action(deltaTime);
		int playerId = entitymanager.getPlayerId();
		Health health = entitymanager.getHealthComponent(playerId);
		if (health.currentHealth > 65) {
			entitymanager.updateAnimationState(playerId, "PlayerIdle");
		}
		if (health.currentHealth > 35 && health.currentHealth < 65) {
			entitymanager.updateAnimationState(playerId, "PlayerDamaged");
		}
		else if (health.currentHealth < 35) {
			entitymanager.updateAnimationState(playerId, "PlayerCritical");
		}
		if (health.currentHealth == 0) {
			//GAME OVER
			eventSystem.publish(new GameLoadEvent(GameplayContext, 2, true));
		}
		player->update();
		if (command)
		{
			command->execute(*player);
		}
	}
	std::vector<int> deleteEnemies;
	for (auto& enemy : enemies) {		
		enemy.second->update(deltaTime);
		Health health = entitymanager.getHealthComponent(enemy.second->getEnemyId());
		if (health.currentHealth <= 0) {
			entitymanager.deleteEntity(enemy.second->getEnemyId());
			deleteEnemies.push_back(enemy.first);
		}
	}
	for (auto& enemyId : deleteEnemies) {
		enemies.erase(enemyId);
	}
	physicsmanager.update(deltaTime);
	renderScene();
	animationManager->update(deltaTime);
	mouse->endFrame();
	keyboard->endFrame();
	entitymanager.endFrame();
	render(*(camera), "../../../../Game/assets/shaders/VertexShaderDefault.glsl");
}






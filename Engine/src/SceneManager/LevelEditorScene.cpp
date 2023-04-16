#include "LevelEditorScene.h"

geProject::LevelEditorScene::LevelEditorScene() {

	
}


geProject::LevelEditorScene::~LevelEditorScene(){}


void geProject::LevelEditorScene::init() {
	player = nullptr;
	gridWidth = 0.25f;
	gridHeight = 0.25f;
	eventSystem.subscribe(this, &LevelEditorScene::saveGame);
	eventSystem.subscribe(this, &LevelEditorScene::deleteEntity);
	eventSystem.subscribe(this, &LevelEditorScene::updateCopy);
	eventSystem.subscribe(this, &LevelEditorScene::editorKeyEvent);
	std::cout << "Editor Scene!" << std::endl;
	//entitymanager = new EntityManager(10000);
	entitymanager.startUp();
	animationManager = new AnimationManager(/**entitymanager*/);
	rendermanager = new Renderer();
	camera = new EditorCamera(glm::vec2(0.0f, 0.0f));
	mouse->setInverses(camera->getProjectionInverse(), camera->getViewMatrixInverse());
	filePath = "../../../../Game/assets/levels/levelEditor.json";
	editor = new EditorRender();

	std::shared_ptr<SpriteSheet> mapDims = resourcemanager.requestLevelMap(1);	
	int worldWidth = mapDims->getSpriteSheetWidth();
	int worldHeight = mapDims->getSpriteSheetHeight();
	worldstate.generateWorldMap(worldWidth, worldHeight);
	selectionTextures = new FrameBuffer(1920, 1080, true);
	//physicsmanager = new Physics(/**entitymanager*/);
	physicsmanager.startUp();
	controlManager = new Receiver();
	sceneHierarchy = new HierarchyWindow();
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
		
	
	rendermanager->renderMap(1);
	
	int playerId = entitymanager.getPlayerId();
	if (playerId > -1) {
		player = new PlayerController(*camera);
	}
	
	for (auto& i : entitymanager.getEnemyIds()) {
		enemies[i] = new Enemy(i, goapEnabled);
	}
	eventSystem.setContext(EditorContext);
}


void geProject::LevelEditorScene::update(float deltaTime) {

	camera->update(deltaTime);
	//if mouse is out of game view then set the event system to only take imgui callbacks
	if (loopcount > 1) {
		if ((mouse->getScreenXpos() <= 1920.0f && mouse->getScreenXpos() >= 0.0f) && (mouse->getScreenYpos() <= 1080.0f && mouse->getScreenYpos() >= 0.0f)) {
			if (physicsEnabled) {

				eventSystem.setContext(GameplayContext);
			}
			else {

				eventSystem.setContext(EditorContext);
			}
		}
		else {

			eventSystem.setContext(ImGuiContext);
		}
	}

	if (gridSelected) {
		setGridLines();
	}

	//DRAG AND DROP
	if (entityDrag == true && activatedEntity > -1) {
		Transform transform = entitymanager.getTransformComponent(activatedEntity);
		float scroll = camera->getScroll();
		float viewWidth = mouse->getViewXsize();
		float viewHeight = mouse->getViewYsize();
		if (gridSelected) {
			transform.position[0] = (int)(mouse->getCameraMouseX() / gridWidth) * gridWidth;
			transform.position[1] = (int)(mouse->getCameraMouseY() / gridHeight) * gridHeight;
		}
		else {
			transform.position[0] = mouse->getCameraMouseX();
			transform.position[1] = mouse->getCameraMouseY();
		}

		//std::cout << "Pos x: " << mouse->getCameraMouseX() << " Pos Y: " << mouse->getCameraMouseY() << " scroll: " << scroll << " gridwidth: " << gridWidth << " gridheight: " << gridHeight << std::endl;
		entitymanager.assignTransform(activatedEntity, transform);
		if (mouse->mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) && entityDrag == true) {
			entityDrag = false;
			mouse->releaseMouseButton(GLFW_MOUSE_BUTTON_LEFT);
		}
	}
	//std::cout << activatedEntity << std::endl;
	if (physicsEnabled == true) {
		if (player != nullptr) {
			Command* command = controlManager->action(deltaTime);
			player->update();
			if (command)
			{
				command->execute(*player);
			}
		}
		std::vector<int> deleteEnemies;
		for (auto& enemy : enemies) {
			enemy.second->update(deltaTime);
			ViewCollider view = entitymanager.getViewComponent(enemy.second->getEnemyId());
			Transform transform = entitymanager.getTransformComponent(enemy.second->getEnemyId());
			Health health = entitymanager.getHealthComponent(enemy.second->getEnemyId());
			if (health.currentHealth <= 0) {
				entitymanager.deleteEntity(enemy.second->getEnemyId());
				deleteEnemies.push_back(enemy.first);
			}
			else {
				editor->addSensor(transform.position, glm::vec3(0, 0, 1), view.radius, transform.rotation + 50, 1);
				if (enemy.second->getPathSize() > 0) {
					for (auto& path : enemy.second->getPath()) {
						editor->addBox(glm::vec2(path.x, path.y), glm::vec2(0.2f, 0.2f), glm::vec3(1, 0, 0), 0, 1);
					}
				}
			}
		}
		for (auto& enemyId : deleteEnemies) {
			enemies.erase(enemyId);
		}
		physicsmanager.update(deltaTime);
	}
	renderScene();
	animationManager->update(deltaTime);
	mouse->endFrame();
	keyboard->endFrame();
	entitymanager.endFrame();
	render(*(camera), "../../../../Game/assets/shaders/VertexShaderDefault.glsl");
	editor->render(*(camera));


	//DEBUG DRAWING FOR PHYSICS
	/*if (displayColliders && activatedEntity > -1) {
		Entity colliderEntity = entitymanager.getEntity(activatedEntity);
		Transform trans = entitymanager.getTransformComponent(colliderEntity.id);
		if ((colliderEntity.compMask & 8) == 8) {
			for (auto& circle : entitymanager.getCircleColliderComponents(colliderEntity.id)) {
				if (circle.id > 0) {
					//multiply by 2 since box size is set to half width / height
					editor->addCircle(trans.position + circle.offset, glm::vec3(0.0f, 1.0f, 0.0f), circle.radius, 16, 1);
				}
			}
		}
		if ((colliderEntity.compMask & 16) == 16) {
			for (auto& box : entitymanager.getBoxColliderComponents(colliderEntity.id)) {
				if (box.id > 0) {
					//multiply by 2 since box size is set to half width / height
					editor->addBox(trans.position + box.offset, box.boxSize, glm::vec3(0.0f, 1.0f, 0.0f), (float)trans.rotation, 1);
				}
			}
		}

	}
	*/
	if (displayColliders) {
		for (int i = 0; i < entitymanager.getEntityNum(); i++) {
			Entity ent = entitymanager.getEntity(i);
			if (ent.id > -1) {
				Transform trans = entitymanager.getTransformComponent(ent.id);
				for (auto& circle : entitymanager.getCircleColliderComponents(ent.id)) {
					if (circle.id > 0) {
						//multiply by 2 since box size is set to half width / height
						editor->addCircle(trans.position + circle.offset, glm::vec3(0.0f, 1.0f, 0.0f), circle.radius, 16, 1);
					}
				}
				for (auto& box : entitymanager.getBoxColliderComponents(ent.id)) {
					if (box.id > 0) {
						//multiply by 2 since box size is set to half width / height
						editor->addBox(trans.position + box.offset, box.boxSize, glm::vec3(0.0f, 1.0f, 0.0f), (float)trans.rotation, 1);
					}
				}
			}
		}
	}
	

	//Enemy* newEnemy = new Enemy();
	//auto test = newEnemy->planPath(1.291f, 0.544f, 3.582f, 2.721f);
	//worldstate.testWorldMap();
	loopcount++;
}




void geProject::LevelEditorScene::setActiveEntity(int entityId) {
	activatedEntity = entityId;
	previousEntity = activatedEntity;
	if (entityId == -1) {
		entityDrag = false;
	}
}

void geProject::LevelEditorScene::updateSceneImgui() {
	ImGui::Begin("Inspector");
	entitymanager.updateImgui(activatedEntity);
	ImGui::End();
	ImGui::Begin("Config");
	ImGui::Checkbox("grid", &gridSelected);
	ImGui::Checkbox("colliders", &displayColliders);
	ImGui::End();
	sceneHierarchy->imguiUpdate();
	updateImgui();

}

void geProject::LevelEditorScene::updateImgui() {
	ImGui::Begin("Editor Window");
	if (ImGui::BeginTabBar("Tab")) {
		if (ImGui::BeginTabItem("Environment")) {
			//credit to https ://github.com/ocornut/imgui/issues/1977		
			ImGuiStyle& style = ImGui::GetStyle();
			float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
			std::shared_ptr<SpriteSheet> spriteSheet = resourcemanager.requestSpriteSheet(1);
			int spriteSize = spriteSheet->getSpriteSize();
			for (int i = 0; i < spriteSize; i++) {
				SpriteRender newSprite = spriteSheet->getSprite(i);
				// https: //github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples
				ImVec2 spriteDimensions(32, 32);
				ImVec2 uv0(newSprite.texturePos[2].x, newSprite.texturePos[0].y);
				ImVec2 uv1(newSprite.texturePos[0].x, newSprite.texturePos[2].y);
				ImGui::PushID(i);
				float spriteDim = 0.25f;
				if (ImGui::ImageButton((ImTextureID)newSprite.textureId, spriteDimensions, uv0, uv1, 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f))) {
					if (entityDrag == false) {
						createEnvironmentBlock(&newSprite, spriteDim, spriteDim, entityTypes::environment);
					}
				}
				ImGui::PopID();
				float last_button_x2 = ImGui::GetItemRectMax().x;
				float next_button_x2 = last_button_x2 + style.ItemSpacing.x;
				if (i < spriteSize && next_button_x2 < window_visible_x2)
					ImGui::SameLine();
			}
			ImGui::EndTabItem();
		}
		if (ImGui::BeginTabItem("Player")) {
			ImVec2 spriteDimensions(32, 32);
			float spriteDim = 0.25f;
			if (entitymanager.getPlayerId() == -1) {
				std::shared_ptr<SpriteSheet> playerSprites = resourcemanager.requestSpriteSheet(2);
				SpriteRender newSprite = playerSprites->getSprite(0);
				// https: //github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples
				ImVec2 uv0(newSprite.texturePos[2].x, newSprite.texturePos[0].y);
				ImVec2 uv1(newSprite.texturePos[0].x, newSprite.texturePos[2].y);
				if (ImGui::ImageButton((ImTextureID)newSprite.textureId, spriteDimensions, uv0, uv1, 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f))) {
					if (entityDrag == false) {
						createCharacterBlock(&newSprite, spriteDim, spriteDim, entityTypes::player);
					}
				}
			}
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Enemies")) {
			//credit to https ://github.com/ocornut/imgui/issues/1977				
			ImGuiStyle& style = ImGui::GetStyle();
			float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
			std::shared_ptr<SpriteSheet> spriteSheet = resourcemanager.requestSpriteSheet(3);
			int spriteSize = spriteSheet->getSpriteSize();
			for (int i = 0; i < spriteSize; i++) {
				SpriteRender enemies = spriteSheet->getSprite(i);
				// https: //github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples
				ImVec2 spriteDimensions(32, 32);
				ImVec2 uv0(enemies.texturePos[2].x, enemies.texturePos[0].y);
				ImVec2 uv1(enemies.texturePos[0].x, enemies.texturePos[2].y);
				ImGui::PushID(i);
				float spriteDim = 0.25f;
				if (ImGui::ImageButton((ImTextureID)enemies.textureId, spriteDimensions, uv0, uv1, 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f))) {
					if (entityDrag == false) {
						createCharacterBlock(&enemies, spriteDim, spriteDim, entityTypes::enemy);
					}
				}
				ImGui::PopID();
				float last_button_x2 = ImGui::GetItemRectMax().x;
				float next_button_x2 = last_button_x2 + style.ItemSpacing.x;
				if (i < spriteSize && next_button_x2 < window_visible_x2)
					ImGui::SameLine();
			}
			ImGui::EndTabItem();
		}

		
		ImGui::EndTabBar();
	}
	ImGui::End();
}



unsigned int geProject::LevelEditorScene::createEnvironmentBlock(SpriteRender* sprite, float sizeX, float sizeY, entityTypes type) {
	unsigned int entity = entitymanager.addEntity(type);
	//float y = camera->getCameraY();		
	mouse->setInverses(camera->getProjectionInverse(), camera->getViewMatrixInverse());
	entitymanager.assignTransform(entity, Transform{ .position = {mouse->getCameraMouseX(), mouse->getCameraMouseY()}, .scale = {sizeX, sizeY}});
	entitymanager.assignSpriteRender(entity, *sprite);
	//entities.push_back(manager->getEntity(entity));
	entityDrag = true;	
	setActiveEntity(entity);
	return entity;
}

unsigned int geProject::LevelEditorScene::createCharacterBlock(SpriteRender* sprite, float sizeX, float sizeY, entityTypes type){
	unsigned int entity = entitymanager.addEntity(type);
	
	if (type == entityTypes::player) {
		player = new PlayerController(*camera);
		entitymanager.assignController(entity, Controls());
	}	
	if (type == entityTypes::enemy) {
		animationManager->assignEntityAnimation(entity, "Idle");
	}
	mouse->setInverses(camera->getProjectionInverse(), camera->getViewMatrixInverse());
	entitymanager.assignTransform(entity, Transform{ .position = {mouse->getCameraMouseX(), mouse->getCameraMouseY()}, .scale = {sizeX, sizeY} });
	entitymanager.assignSpriteRender(entity, *sprite);
	
	entityDrag = true;
	setActiveEntity(entity);
	return entity;
}


void geProject::LevelEditorScene::setGridLines() {
	glm::vec2 pos = camera->getPosition();
	float scroll = camera->getScroll();
	glm::vec2 projSize = camera->getProjSize() * scroll;
	float x, y, verticalLine, horizontalLine, maxLine;
	x = ((pos.x / gridWidth) - 1) * gridWidth;
	y = ((pos.y / gridHeight) - 1) * gridHeight;
	verticalLine = (projSize.x / gridWidth) + 2;
	horizontalLine = (projSize.y / gridHeight) + 2;
	maxLine = 0;
	if (horizontalLine >= verticalLine) {
		maxLine = horizontalLine;
	}
	else {
		maxLine = verticalLine;
	}
	for (int i = 0; i < maxLine; i++) {
		float newX = x + (gridWidth * i);
		float newY = y + (gridHeight * i);
		if (i < verticalLine) {
			editor->addLine(glm::vec2(newX, y), glm::vec2(newX, y + projSize.y + gridHeight), glm::vec3(0.2f, 0.2f, 0.2f), 1);
		}
		if (i < horizontalLine) {
			editor->addLine(glm::vec2(x, newY), glm::vec2(x + projSize.x + gridWidth, newY), glm::vec3(0.2f, 0.2f, 0.2f), 1);
		}
	}
}

unsigned int geProject::LevelEditorScene::getActiveEntity() { 
	return activatedEntity; 
}
void geProject::LevelEditorScene::setEntityDrag(bool drag) { 
	entityDrag = drag; 
}

bool geProject::LevelEditorScene::getEntityDrag() { return entityDrag; }



void geProject::LevelEditorScene::setPicking() {	
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);	
	glDisable(GL_BLEND);
	glClear(GL_COLOR_BUFFER_BIT);
	selectionTextures->bindPicking();	
	render(*(camera), "../../../../Game/assets/shaders/SelectionVertexShader.glsl");	
	if (mouse->mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
		int x = (int)mouse->getScreenXpos();
		int y = (int)mouse->getScreenYpos();
		int entityId = selectionTextures->getPixel(x, y);		
		//std::cout << "entity: " << entityId << std::endl;	
		if (entityId == -1 && mouse->checkMouseBoundaries()) {
			entityDrag = false;
			entityClicked = false;
			setActiveEntity(-1);
			mouse->releaseMouseButton(GLFW_MOUSE_BUTTON_LEFT);
		}
		else {
			if (activatedEntity == entityId && entityClicked == true && entityDrag == false) {
				entityDrag = true;
				entityClicked = false;
				mouse->releaseMouseButton(GLFW_MOUSE_BUTTON_LEFT);
			}
			else if (entityId > -1 && entityClicked == false && entityDrag == false) {
				setActiveEntity(entityId);
				editor->addBox(entitymanager.getTransformComponent(entityId).position, glm::vec2(0.26f, 0.26f), glm::vec3(1.0f, 0.0f, 0.0f), 0, 5);
				entityClicked = true;
				mouse->releaseMouseButton(GLFW_MOUSE_BUTTON_LEFT);
			}
			else if (entityId > -1 && entityId != activatedEntity) {
				setActiveEntity(entityId);
				entityClicked = true;
				editor->addBox(entitymanager.getTransformComponent(entityId).position, glm::vec2(0.26f, 0.26f), glm::vec3(1.0f, 0.0f, 0.0f), 0, 5);
				mouse->releaseMouseButton(GLFW_MOUSE_BUTTON_LEFT);
			}
		}

		
		//scene->updateImgui();
	}
	selectionTextures->unBindPicking();	
	glEnable(GL_BLEND);
}




void geProject::LevelEditorScene::saveGame(GameSaveEvent* save) { 
	if (save->getType() == Type::gameSave) {
		std::cout << "GAME SAVED" << std::endl;
		Scene::serialize(filePath);
		animationManager->serializeAnimations();
	}
	
}



void geProject::LevelEditorScene::deleteEntity(DeleteEntityEvent* e){
	if (activatedEntity == previousEntity) {
		previousEntity = -1;
	}
	setActiveEntity(-1);
	
}

void geProject::LevelEditorScene::updateCopy(CopyEntityEvent* e){
	setActiveEntity(e->newId);	
	entityClicked = false;
	entityDrag = true;
	
}

void geProject::LevelEditorScene::editorKeyEvent(KeyPressedEvent* e){
	
	if (eventSystem.getContext() == (EditorContext) && e->keycode == GLFW_KEY_C) {
		std::cout << "copied " << std::endl;
		if (previousEntity > -1) {
			entitymanager.copyEntity(previousEntity);
		}
	}
	if (eventSystem.getContext() == (EditorContext) && e->keycode == GLFW_KEY_DELETE) {
		if (previousEntity > -1) {
			entitymanager.deleteEntity(previousEntity);
		}
	}
	
}







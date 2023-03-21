#include "LevelEditorScene.h"

geProject::LevelEditorScene::LevelEditorScene() {
	gridWidth = 0.25f;
	gridHeight = 0.25f;
	resourceManager = new ResourceManager();
	//resourceManager->loadLevel();
	eventSystem.subscribe(this, &LevelEditorScene::saveGame);
	
	std::cout << "Editor Scene!" << std::endl;	
	manager = new EntityManager(10000);
	renderer = new Renderer(*(resourceManager));
	//shader = new geProject::Shader("../../../../Game/assets/shaders/defaultVertexShader.glsl", );
	camera = new EditorCamera(glm::vec2(-250.0f, 0.0f));	
	mouse->setInverses(getCamera()->getProjectionInverse(), getCamera()->getViewMatrixInverse());
	//testTexture = new Texture("../../../../Game/assets/images/container.jpg");
	resourceManager->loadShader("../../../../Game/assets/shaders/VertexShaderDefault.glsl", "../../../../Game/assets/shaders/FragmentShaderDefault.glsl");
	resourceManager->loadShader("../../../../Game/assets/shaders/LineVertexShader.glsl", "../../../../Game/assets/shaders/LineFragmentShader.glsl");
	resourceManager->loadShader("../../../../Game/assets/shaders/SelectionVertexShader.glsl", "../../../../Game/assets/shaders/SelectionFragmentShader.glsl");
	filePath = "../../../../Game/assets/levels/levelEditor.json";	
	editor = new EditorRender(*(resourceManager));
	//init();	
	//unsigned int red = resourceManager->loadTexture("../../../../Game/assets/images/red.jpg");
	sprites.push_back(resourceManager->loadSpriteSheet("../../../../Game/assets/images/spritesheets/decorationsAndBlocks.png", 81, 16, 16, 0, 0));
	sprites.push_back(resourceManager->loadSpriteSheet("../../../../Game/assets/images/spritesheets/icons.png", 16, 32, 32, 0, 0));
	sprites.push_back(resourceManager->loadSpriteSheet("../../../../Game/assets/images/spritesheets/spritesheet.png", 26, 16, 16, 0, 0));
	sprites.push_back(resourceManager->loadSpriteSheet("../../../../Game/assets/images/spritesheets/pipes.png", 6, 16, 16, 0, 0));
	selectionTextures = new FrameBuffer(1920, 1080, true);
	physicsManager = new Physics(*manager);
}


geProject::LevelEditorScene::~LevelEditorScene(){}


void geProject::LevelEditorScene::init() {}

size_t geProject::LevelEditorScene::addEntityToScene(unsigned int entityId){
	auto entity = manager->getEntity(entityId);
	entities.push_back(entity);
	physicsManager->addEntity(*entity);		
	
	//renderer->addSpriteToBatch(manager->getSpriteComponent(entityId), manager->getTransformComponent(entityId));
	return entities.size();
}

void geProject::LevelEditorScene::reAssignEntityToScene(unsigned int entitySceneId, unsigned int entityId) {
	//entities[entitySceneId] = manager->getEntity(entityId);
}

void geProject::LevelEditorScene::update(float deltaTime) {		
	setGridLines();		
	if (physicsEnabled == true) {
		physicsManager->update(deltaTime);
	}
	if (loopcount % 150 == 0) {		
		std::cout << "View X: " << mouse->getViewXsize() << " View Y: " << mouse->getViewYsize() << std::endl;
		std::cout << "Mouse X: " << mouse->getXpos() << " Mouse Y: " << mouse->getYpos() << std::endl;
		std::cout << "World X: " << mouse->getScreenXpos() << " World Y: " << mouse->getScreenYpos() << std::endl;
		std::cout << "Camera X: " << mouse->getCameraMouseX() << " Camera Y: " << mouse->getCameraMouseY() << std::endl;
	}
	
	
	camera->update(deltaTime);
	
	
	//float x = ((float)sin(t) * 200.0f) + 600;
	//float y = ((float)cos(t) * 200.0f) + 400;
	//t += 0.05f;
	//editor->addLine(glm::vec2(600, 400), glm::vec2(x, y), glm::vec3(0.0f, 0.0f, 1.0f), 1);
	
	if (deltaTime == 0) {
		geProject::Scene::deserialize(filePath);
	}
	//editor->addBox(glm::vec2(400.0f, 200.0f), glm::vec2(64.0f, 32.0f), glm::vec3(0.0f, 1.0f, 0.0f), 0, 1);
	
	//DEBUG DRAWING FOR PHYSICS
	auto ent = manager->getEntities();	
	for (int i = 0; i < ent.size(); i++) {
		auto box = manager->getBoxColliderComponent(i);
		auto trans = manager->getTransformComponent(i);
		if (box->id > 0) {
			editor->addBox(trans->centre + box->offset, box->boxSize, glm::vec3(0.0f, 1.0f, 0.0f), 0, 1);
		}
	}
	/*
	
	editor->addCircle(glm::vec2(x, y), glm::vec3(0.0f, 1.0f, 0.0f), 64.0f, 20, 1);
	x += 50.0f * deltaTime;
	y += 50.0f * deltaTime;
	*/


	/*if (testSpritesheet == 18) {
		testSpritesheet = 15;
	}

	
	auto tst = resourceManager->requestSpriteSheet(sprites);
	auto cratesprite = tst->getSprite(testSpritesheet);

	loopcount++;
	
	for (int i = 1; i < manager->getEntityNum(); i++) {
		manager->assignSpriteRender(i, cratesprite);
	}
	*/
	
	//DRAG AND DROP
	if (entityDrag == true && activatedEntity > -1) {		
		auto transform = manager->getTransformComponent(activatedEntity);
		float scroll = camera->getScroll();		
		camera->projectionUpdate();
		transform->position[0] = (int)(((float)mouse->getCameraMouseX() * scroll) / (int)gridWidth) * gridWidth ;
		transform->position[1] = (int)(((float)mouse->getCameraMouseY() * scroll) / (int)gridHeight) * gridHeight;
		//std::cout << "Pos x: " << mouse->getCameraMouseX() << " Pos Y: " << mouse->getCameraMouseY() << " scroll: " << scroll << " gridwidth: " << gridWidth << " gridheight: " << gridHeight << std::endl;
		manager->assignTransform(activatedEntity, *transform);		
		if (mouse->mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT) && entityDrag == true) {
			entityDrag = false;	
			mouse->releaseMouseButton(GLFW_MOUSE_BUTTON_LEFT);
		}		
	}

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
	editor->render(*(camera));
	render("../../../../Game/assets/shaders/VertexShaderDefault.glsl");
	loopcount++;
}


void geProject::LevelEditorScene::render(std::string shaderPath) {	
	renderer->render(*(camera), shaderPath);
}

void geProject::LevelEditorScene::setActiveEntity(int entityId) {
	activatedEntity = entityId;
}

void geProject::LevelEditorScene::updateSceneImgui() {
	if (activatedEntity > -1) {
		ImGui::Begin("Inspector");
		manager->updateImgui(activatedEntity);
		ImGui::End();
	}
	updateImgui();
}

void geProject::LevelEditorScene::updateImgui() {
	ImGui::Begin("Editor Window");
	//credit to https ://github.com/ocornut/imgui/issues/1977
	ImGui::Text("Manually wrapping:");
	ImGuiStyle& style = ImGui::GetStyle();	
	float window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
	auto spriteSheet = resourceManager->requestSpriteSheet(sprites[0]);
	unsigned int spriteSize = spriteSheet->getSpriteSize();
	for (int i = 1; i < spriteSize + 1; i++){
		auto newSprite = spriteSheet->getSprite(i);
		// https: //github.com/ocornut/imgui/wiki/Image-Loading-and-Displaying-Examples
		ImVec2 spriteDimensions(32, 32);
		ImVec2 uv0(newSprite.texturePos[2].x, newSprite.texturePos[0].y);
		ImVec2 uv1(newSprite.texturePos[0].x, newSprite.texturePos[2].y);
		ImGui::PushID(i);
		float spriteDim = 0.25f;
		
		if (ImGui::ImageButton((ImTextureID)newSprite.textureId, spriteDimensions, uv0, uv1, 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f))) {
			if (entityDrag == false) {				
				createEditorBlock(&newSprite, spriteDim, spriteDim);
			}
		}
		ImGui::PopID();
		float last_button_x2 = ImGui::GetItemRectMax().x;
		float next_button_x2 = last_button_x2 + style.ItemSpacing.x; 
		if (i < spriteSize && next_button_x2 < window_visible_x2)
			ImGui::SameLine();
	}
	ImGui::End();
}

std::vector<geProject::Entity*> geProject::LevelEditorScene::getEntities() {
	return entities;
}

unsigned int geProject::LevelEditorScene::createEditorBlock(SpriteRender* sprite, float sizeX, float sizeY) {
	unsigned int entity = manager->addEntity();
	//float y = camera->getCameraY();
	
	mouse->setInverses(camera->getProjectionInverse(), camera->getViewMatrixInverse());
	manager->assignTransform(entity, Transform{ .position = {mouse->getScreenXpos(), mouse->getScreenYpos()/*mouse->getCameraMouseX(), mouse->getCameraMouseY()}*/}, .scale = {sizeX, sizeY} });
	manager->assignSpriteRender(entity, *sprite);
	//entities.push_back(manager->getEntity(entity));
	entityDrag = true;	
	activatedEntity = entity;	
	return entity;
}


void geProject::LevelEditorScene::setGridLines() {
	glm::vec2 pos = camera->getPosition();
	float scroll = camera->getScroll();
	glm::vec2 projSize = camera->getProjSize() * scroll;
	float x, y, vLine, hLine, maxLine;
	x = ((pos.x / gridWidth) - 1) * gridWidth;
	y = ((pos.y / gridHeight) - 1) * gridHeight;
	vLine = (projSize.x / gridWidth) + 2;
	hLine = (projSize.y / gridHeight) + 2;
	maxLine = 0;
	if (hLine >= vLine) {
		maxLine = hLine;
	}
	else {
		maxLine = vLine;
	}

	for (int i = 0; i < maxLine; i++) {
		float newX = x + (gridWidth * i);
		float newY = y + (gridHeight * i);
		if (i < vLine) {
			editor->addLine(glm::vec2(newX, y), glm::vec2(newX, y + (int)projSize.y + gridHeight), glm::vec3(0.2f, 0.2f, 0.2f), 1);
		}
		if (i < hLine) {
			editor->addLine(glm::vec2(x, newY), glm::vec2(x + (int)projSize.x + gridWidth, newY), glm::vec3(0.2f, 0.2f, 0.2f), 1);
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
	selectionTextures->bindPicking();	
	render("../../../../Game/assets/shaders/SelectionVertexShader.glsl");	
	if (mouse->mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
		int x = (int)mouse->getScreenXpos();
		int y = (int)mouse->getScreenYpos();
		int entityId = selectionTextures->getPixel(x, y);
		std::cout << "entity: " << entityId << std::endl;	
		if (activatedEntity == entityId && entityClicked == true && entityDrag == false) {			
			entityDrag = true;			
			entityClicked = false;			
			mouse->releaseMouseButton(GLFW_MOUSE_BUTTON_LEFT);
		}
		else if (entityId > -1 && entityClicked == false && entityDrag == false) {			
			activatedEntity = entityId;	
			editor->addBox(manager->getTransformComponent(entityId)->centre, glm::vec2(300, 300), glm::vec3(0.0f, 1.0f, 0.0f), 0, 250);
			entityClicked = true;
			mouse->releaseMouseButton(GLFW_MOUSE_BUTTON_LEFT);		
		}
		else if (entityId > -1 && entityId != activatedEntity) {
			activatedEntity = entityId;
			entityClicked = true;
			editor->addBox(manager->getTransformComponent(entityId)->centre, glm::vec2(300, 300), glm::vec3(0.0f, 1.0f, 0.0f), 0, 250);
			mouse->releaseMouseButton(GLFW_MOUSE_BUTTON_LEFT);
		}
		//scene->updateImgui();
	}
	selectionTextures->unBindPicking();	
}




void geProject::LevelEditorScene::saveGame(GameSaveEvent* save) { 
	if (save->getType() == Type::gameSave) {
		std::cout << "GAME SAVED" << std::endl;
		Scene::serialize(filePath);
	}
}



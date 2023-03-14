#include "LevelEditorScene.h"



geProject::LevelEditorScene::LevelEditorScene() {
	gridWidth = 32;
	gridHeight = 32;
	resourceManager = new ResourceManager();
	//resourceManager->loadLevel();

	std::cout << "Editor Scene!" << std::endl;	
	manager = new EntityManager(10000);
	renderer = new Renderer(*(resourceManager));
	//shader = new geProject::Shader("../../../../Game/assets/shaders/defaultVertexShader.glsl", );

	auto camera = new Camera(glm::vec2(-250.0f, 0.0f));	
	editorCam = new EditorCamera(*camera);
	mouse->setInverses(getCamera()->getProjectionInverse(), getCamera()->getViewMatrixInverse());
	//testTexture = new Texture("../../../../Game/assets/images/container.jpg");
	resourceManager->loadShader("../../../../Game/assets/shaders/VertexShaderDefault.glsl", "../../../../Game/assets/shaders/FragmentShaderDefault.glsl");
	resourceManager->loadShader("../../../../Game/assets/shaders/LineVertexShader.glsl", "../../../../Game/assets/shaders/LineFragmentShader.glsl");
	resourceManager->loadShader("../../../../Game/assets/shaders/SelectionVertexShader.glsl", "../../../../Game/assets/shaders/SelectionFragmentShader.glsl");
	filePath = "../../../../Game/assets/levels/levelEditor.json";	
	editor = new EditorRender(*(resourceManager));
	//init();	
	unsigned int crate = resourceManager->loadTexture("../../../../Game/assets/images/container2.jpg");
	unsigned int mario = resourceManager->loadTexture("../../../../Game/assets/images/testImage.png");
	unsigned int goomba = resourceManager->loadTexture("../../../../Game/assets/images/testImage2.png");
	unsigned int blend1 = resourceManager->loadTexture("../../../../Game/assets/images/blendImage1.png");
	unsigned int blend2 = resourceManager->loadTexture("../../../../Game/assets/images/blendImage2.png");
	//unsigned int red = resourceManager->loadTexture("../../../../Game/assets/images/red.jpg");
	sprites.push_back(resourceManager->loadSpriteSheet("../../../../Game/assets/images/spritesheets/decorationsAndBlocks.png", 81, 16, 16, 0, 3));
	sprites.push_back(resourceManager->loadSpriteSheet("../../../../Game/assets/images/spritesheets/icons.png", 16, 32, 32, 0, 3));
	sprites.push_back(resourceManager->loadSpriteSheet("../../../../Game/assets/images/spritesheets/spritesheet.png", 26, 16, 16, 0, 3));
	sprites.push_back(resourceManager->loadSpriteSheet("../../../../Game/assets/images/spritesheets/pipes.png", 6, 16, 16, 0, 3));
}


geProject::LevelEditorScene::~LevelEditorScene(){}


void geProject::LevelEditorScene::init() {	

	
	/*
	unsigned int blen1 = manager->addEntity();	
	manager->assignTransform(blen1, Transform{ .position = {100, 100}, .scale = {200, 200} });
	manager->assignSpriteRender(blen1, SpriteRender{ .textureId{blend1}, .zIndex = 2 });
	manager->assignRigidBody(manager->getEntity(0)->id, Rigidbody { .collider = 0 });
	addEntityToScene(blen1);

	unsigned int blen2 = manager->addEntity();
	manager->assignTransform(blen2, Transform{ .position = {400, 100}, .scale = {200, 200} });
	manager->assignSpriteRender(blen2, SpriteRender{ .textureId{blend2}, .zIndex = 3 });
	addEntityToScene(blen2);

	unsigned int blen3 = manager->addEntity();
	manager->assignTransform(blen3, Transform{ .position = {700, 100}, .scale = {200, 200} });
	manager->assignSpriteRender(blen3, SpriteRender{ .textureId{blend1}, .zIndex = 1 });
	addEntityToScene(blen3);
	
	
	*/
	

	/*
	
	unsigned int blen4 = manager->addEntity();
	manager->assignTransform(blen4, Transform{ .position = {400, 100}, .scale = {200, 200} });
	manager->assignSpriteRender(blen4, SpriteRender{ .color = {0.2, 0.3, 0.4, 1.0}, .zIndex = 1 });
	addEntityToScene(blen4);
	
	
	
	


	
	unsigned int blen5 = manager->addEntity();
	manager->assignTransform(blen5, Transform{ .position = {100, 100}, .scale = {200, 200} });
	manager->assignSpriteRender(blen5, SpriteRender{ .color = {0.2, 0.3, 0.4, 1.0}, .zIndex = 3 });
	addEntityToScene(blen5);
	
	*/
	/*
	int x = 100;
	
	for (int i = 0; i < 25; i++) {
		unsigned int entity = manager->addEntity();
		addEntityToScene(entity);
		auto cratetransform = Transform{ .position = {x, 200}, .scale = {50, 50} };
		manager->assignTransform(entity, cratetransform);
		x += 100;
	}
	*/

	
	/*
	int xpos = 40;
	int ypos = 50;
	int scalex = 50;
	int scaley = 50;
	float r = 0.2;
	float g = 0.2;
	float b = 0.2;
	for (int i = 0; i < 25; i++) {
		
		unsigned int entityId = manager->addEntity();
		auto transform = Transform{ .position = {xpos, ypos}, .scale = {scalex, scaley} };
		manager->assignTransform(entityId, transform);
		auto sprite = SpriteRender{ .color = {r, g, b, 1.0} };
		manager->assignSpriteRender(entityId, sprite);
		addEntityToScene(entityId);
		r += 0.05;
		g += 0.08;
		b += 0.02;
		xpos += 60;
		ypos += 20;
		scalex++;
		scaley++;

	}
	*/
	
	
	//auto tst = resourceManager->requestSpriteSheet(sprites);
	
	//entity = manager->addEntity();
	
	//auto cratesprite =  tst->getSprite(1);
	//manager->assignSpriteRender(entity, cratesprite);
	//unsigned int crateId = addEntityToScene(entity);
	//auto cratetestsprite = SpriteRender{.textureId{crate} };
	/*
	unsigned int marioEntity = manager->addEntity();
	auto mariotransform = Transform{ .position = {400, 100}, .scale = {256, 256} };
	manager->assignTransform(marioEntity, mariotransform);
	manager->assignSpriteRender(marioEntity, cratetestsprite);
	//unsigned int marioId = addEntityToScene(marioEntity);
	
	unsigned int goombaEntity = manager->addEntity();
	auto goombatransform = Transform{ .position = {800, 400}, .scale = {256, 256} };
	manager->assignTransform(goombaEntity, goombatransform);
	auto goombasprite = SpriteRender{.textureId{goomba} };
	manager->assignSpriteRender(goombaEntity, goombasprite);
	//unsigned int goombaId = addEntityToScene(goombaEntity);
	*/
	//manager->assignSpriteRender(entity, goombasprite);
	//manager->assignSpriteRender(marioEntity, goombasprite);
	//reAssignEntityToScene(crateId, entity);
	
	

}

size_t geProject::LevelEditorScene::addEntityToScene(unsigned int entityId){
	entities.push_back(manager->getEntity(entityId));
	//renderer->addSpriteToBatch(manager->getSpriteComponent(entityId), manager->getTransformComponent(entityId));
	return entities.size();
}

void geProject::LevelEditorScene::reAssignEntityToScene(unsigned int entitySceneId, unsigned int entityId) {
	//entities[entitySceneId] = manager->getEntity(entityId);
}

geProject::Camera* geProject::LevelEditorScene::getCamera() {
	return editorCam->getCamera();
}

void geProject::LevelEditorScene::update(float deltaTime) {		
	
	setGridLines();
	editorCam->update(deltaTime);
	editorCam->getCamera()->projectionUpdate();
	/*
	if (loopcount % 150 == 0) {
		std::cout << "Viewsize X: " << mouse->getViewXsize() << " Viewsize Y: " << mouse->getViewYsize() << std::endl;
		std::cout << "Mouse X: " << mouse->getXpos() << " Mouse Y: " << mouse->getYpos() << std::endl;
		std::cout << "World X: " << mouse->getScreenXpos() << " World Y: " << mouse->getScreenYpos() << std::endl;
		std::cout << "Camera X: " << mouse->getCameraXpos() << " Camera Y: " << mouse->getCameraYpos() << std::endl;
	}
	*/
	/*float x = ((float)sin(t) * 200.0f) + 600;
	float y = ((float)cos(t) * 200.0f) + 400;
	t += 0.05f;
	editor->addLine(glm::vec2(600, 400), glm::vec2(x, y), glm::vec3(0.0f, 0.0f, 1.0f), 1);
	*/
	if (deltaTime == 0) {
		geProject::Scene::deserialize(filePath);
	}
	//editor->addBox(glm::vec2(400.0f, 200.0f), glm::vec2(64.0f, 32.0f), glm::vec3(0.0f, 1.0f, 0.0f), t, 1);
	editor->addCircle(glm::vec2(x, y), glm::vec3(0.0f, 1.0f, 0.0f), 64.0f, 20, 1);
	x += 50.0f * deltaTime;
	y += 50.0f * deltaTime;
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

	


	if (entityDrag == true) {
		auto transform = manager->getTransformComponent(activatedEntity);		
		transform->position[0] = (int)(mouse->getCameraXpos() / gridWidth) * gridWidth;		
		transform->position[1] = (int)(mouse->getCameraYpos() / gridHeight) * gridHeight;
		manager->assignTransform(activatedEntity, *transform);		
		if (mouse->mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
			entityDrag = false;			
		}
		
	}
	
	
	if (manager->hasUpdate()) {
		entities.clear();
		for (int i = 0; i < manager->getEntityNum(); i++) {
			entities.push_back(manager->getEntity(i));			
			// only sprites that have not been added to the renderer previously will be set to 0
			auto trans = manager->getTransformComponent(i);
			if (trans->dirtyFlag[2] == -1) {				
				renderer->addSpriteToBatch(manager->getSpriteComponent(i), manager->getTransformComponent(i));				
				trans->dirtyFlag[0] = 0;
			}
			else if (trans->dirtyFlag[0] > 0) {
				renderer->updateSprite(manager->getSpriteComponent(i), manager->getTransformComponent(i));
			}
		}
		manager->endFrame();
		
		
		
		geProject::Scene::serialize(filePath);
	}	
	
	editor->render(*(editorCam->getCamera()));
	
	
	loopcount++;
	
	//camera->position.x -= deltaTime * 2.0f;
	//camera->position.y -= deltaTime * 2.0f;
	//editorCam->getCamera()->setPosition(glm::vec2(deltaTime *5.0f, deltaTime * 5.0f));
	/*shader->setTexture("textureSampler", 0);
	glActiveTexture(GL_TEXTURE0);
	testTexture->bindTexture();
	shader->setMat4f("uProjMat", camera->getProjection());
	shader->setMat4f("uViewMat", camera->getViewMatrix());
	glBindVertexArray(vao);
	glDrawElements(GL_TRIANGLES, 9 * sizeof(float), GL_UNSIGNED_INT, 0);

	*/

	/*glUseProgram(shaderProgram);
	glBindVertexArray(vao);
	//glEnableVertexAttribArray(1);
	//glDrawElements(GL_TRIANGLES, sizeof(vertexArray), GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	//glDeleteShader(vertexShader);
	//glDeleteShader(fragmentShader);
	glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);
	glBindVertexArray(0);
	glUseProgram(0);
	*/
}


void geProject::LevelEditorScene::render(std::string shaderPath) {	
	renderer->render(*(editorCam->getCamera()), shaderPath);

}


void geProject::LevelEditorScene::setCameraControlLayout() {
	editorCam->setMouseListener(*mouse);
	editorCam->setKeyboardListener(*keyboard);
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
		if (ImGui::ImageButton((ImTextureID)newSprite.textureId, spriteDimensions, uv0, uv1, 0, ImVec4(0.0f, 0.0f, 0.0f, 0.0f), ImVec4(1.0f, 1.0f, 1.0f, 1.0f))) {
			if (entityDrag == false) {				
				createEditorBlock(&newSprite, spriteDimensions[0], spriteDimensions[1]);
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
	mouse->setInverses(getCamera()->getProjectionInverse(), getCamera()->getViewMatrixInverse());
	manager->assignTransform(entity, Transform{ .position = {mouse->getCameraXpos(), mouse->getCameraYpos()}, .scale = {sizeX, sizeY}});
	manager->assignSpriteRender(entity, *sprite);
	//entities.push_back(manager->getEntity(entity));
	entityDrag = true;
	activatedEntity = entity;	
	return entity;
}


void geProject::LevelEditorScene::setGridLines() {
	glm::vec2 pos = editorCam->getCamera()->getPosition();
	float scroll = editorCam->getCamera()->getScroll();
	glm::vec2 projSize = editorCam->getCamera()->getProjSize() * scroll;
	int x, y, vLine, hLine, maxLine;
	x = ((int)(pos.x / gridWidth) - 1) * gridWidth;
	y = ((int)(pos.y / gridHeight) - 1) * gridHeight;
	vLine = (int)(projSize.x / gridWidth) + 2;
	hLine = (int)(projSize.y / gridHeight) + 2;
	maxLine = 0;
	if (hLine >= vLine) {
		maxLine = hLine;
	}
	else {
		maxLine = vLine;
	}
	
	for (int i = 0; i < maxLine; i++) {
		int newX = x +(gridWidth * i);
		int newY = y + (gridHeight * i);
		if (i < vLine) {
			editor->addLine(glm::vec2(newX, y), glm::vec2(newX, y +(int)projSize.y + gridHeight),glm::vec3(0.2f, 0.2f, 0.2f), 1);
		}
		if (i < hLine) {
			editor->addLine(glm::vec2(x, newY), glm::vec2(x + (int)projSize.x + gridWidth , newY), glm::vec3(0.2f, 0.2f, 0.2f), 1);
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
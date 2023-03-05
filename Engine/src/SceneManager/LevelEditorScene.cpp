#include "LevelEditorScene.h"



geProject::LevelEditorScene::LevelEditorScene() {
	resourceManager = new ResourceManager();
	resourceManager->loadLevel();
	std::cout << "Editor Scene!" << std::endl;	
	/*vertexArray = {  //vertices x,y,z           colors r,g,b,a             texture x,y
					 100.5f,  100.5f, 0.0f,		1.0f, 0.0f, 0.0f, 1.0f,		1, 0, //top right
					 100.5f, -100.5f, 0.0f,		0.0f, 1.0f, 0.0f, 1.0f,		1, 1, //bottom right
					-100.5f, -100.5f, 0.0f,		1.0f, 0.0f, 1.0f, 1.0f,		0, 0, //bottom left
					-100.5f,  100.5f, 0.0f,		1.0f, 1.0f, 0.0f, 1.0f,		0, 1  //top left
	};*/
	/*vertexArray = {
					0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   // 1.0f, 1.0f,   // top right
		            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   // 1.0f, 0.0f,   // bottom right
				   -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   // 0.0f, 0.0f,   // bottom left
				   -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f   // 0.0f, 1.0f    // top left 
	};*/
	//elementOrder = {0,1,3,1,2,3};
	manager = new EntityManager(10000);
	renderer = new Renderer(*(resourceManager));
	//shader = new geProject::Shader("../../../../Game/assets/shaders/defaultVertexShader.glsl", );
	camera = new geProject::Camera(glm::vec2(0.0f, 0.0f));
	//testTexture = new Texture("../../../../Game/assets/images/container.jpg");
	resourceManager->loadShader("../../../../Game/assets/shaders/VertexShaderDefault.glsl", "../../../../Game/assets/shaders/FragmentShaderDefault.glsl");
	filePath = "../../../../Game/assets/levels/levelEditor.json";
	
	init();
	
	
}

geProject::LevelEditorScene::~LevelEditorScene(){
	
}



void geProject::LevelEditorScene::init() {	
	/*//generate buffers for VAO, VBO and EBO
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexArray.size() * sizeof(float), &vertexArray[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elementOrder.size() * sizeof(unsigned int), &elementOrder[0], GL_STATIC_DRAW);
	//set vertices pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//set color pointers
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//set texture pointers
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
	glEnableVertexAttribArray(2);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	manager = EntityManager();
	
	
	//instantiate game objects and add to level editor list
	for (int i = 0; i < 32; i++) {
		int entityId = manager.addEntity();
		manager.assignComponent(entityId, 1);
		manager.assignComponent(entityId, 2);
	}
	entities = manager.getEntities();
	std::vector<Transform*> test;
	test = manager.getTransformComponents();*/
	/*
	for (int i = 31; i >= 0; i--) {
		manager.deleteComponent(i, 2);
		manager.deleteComponent(i, 1);
	}
	for (int i = 0; i < 32; i++) {		
		manager.assignComponent(i, 1);
		manager.assignComponent(i, 2);
	}
	

	*/
	unsigned int crate = resourceManager->loadTexture("../../../../Game/assets/images/container2.jpg");
	unsigned int mario = resourceManager->loadTexture("../../../../Game/assets/images/testImage.png");
	unsigned int goomba = resourceManager->loadTexture("../../../../Game/assets/images/testImage2.png");
	unsigned int blend1 = resourceManager->loadTexture("../../../../Game/assets/images/blendImage1.png");
	unsigned int blend2 = resourceManager->loadTexture("../../../../Game/assets/images/blendImage2.png");
	//unsigned int red = resourceManager->loadTexture("../../../../Game/assets/images/red.jpg");
	sprites = resourceManager->loadSpriteSheet("../../../../Game/assets/images/spritesheet.png", 26, 16, 16, 0, 3);
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
	return camera;
}

void geProject::LevelEditorScene::update(float deltaTime) {	
	if (loopcount == 0) {
		geProject::Scene::deserialize(filePath);
	}
	/*if (testSpritesheet == 18) {
		testSpritesheet = 15;
	}

	
	auto tst = resourceManager->requestSpriteSheet(sprites);
	auto cratesprite = tst->getSprite(testSpritesheet);
	if (loopcount % 25 == 0) {
		testSpritesheet++;
	}
	loopcount++;
	
	for (int i = 1; i < manager->getEntityNum(); i++) {
		manager->assignSpriteRender(i, cratesprite);
	}
	*/
	if (manager->hasUpdate()) {
		entities.clear();
		for (int i = 0; i < manager->getEntityNum(); i++) {
			entities.push_back(manager->getEntity(i));			
			// only sprites that have not been added to the renderer previously will be set to 0
			if (manager->getSpriteComponent(i)->dirtyFlag[2] == 0) {
				renderer->addSpriteToBatch(manager->getSpriteComponent(i), manager->getTransformComponent(i));
			}
			else if (manager->getSpriteComponent(i)->dirtyFlag[0] > 0 || manager->getTransformComponent(i)->dirtyFlag[0] > 0) {
				renderer->updateSprite(manager->getSpriteComponent(i), manager->getTransformComponent(i));
			}

		}
		
		
		activatedEntity = manager->getEntity(0)->id;
		geProject::Scene::serialize(filePath);
	}	
	
	renderer->render(*(camera));
	
	manager->endFrame();
	loopcount++;
	/*
	camera->position.x -= deltaTime * 50.0f;
	camera->position.y -= deltaTime * 20.0f;
	shader->setTexture("textureSampler", 0);
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
	ImGui::Text("Some random text");
	ImGui::End();
}

std::vector<geProject::Entity*> geProject::LevelEditorScene::getEntities() {
	return entities;
}


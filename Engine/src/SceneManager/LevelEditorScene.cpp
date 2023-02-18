#include "LevelEditorScene.h"



geProject::LevelEditorScene::LevelEditorScene() {
	resourceManager = new ResourceManager();
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
	resourceManager->loadTexture("../../../../Game/assets/images/container.jpg");
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
	
}

void geProject::LevelEditorScene::addEntityToScene(unsigned int entityId){
	entities.push_back(manager->getEntity(entityId));
	renderer->addSpriteToBatch(manager->getSpriteComponent(entityId), manager->getTransformComponent(entityId));

}

geProject::Camera* geProject::LevelEditorScene::getCamera() {
	return camera;
}

void geProject::LevelEditorScene::update(float deltaTime) {	
	if (manager->hasUpdate()) {
		entities.clear();
		for (int i = 0; i < manager->getEntityNum(); i++) {
			entities.push_back(manager->getEntity(i));
		}		
	}	

	renderer->render(*(camera));
	manager->endFrame();
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
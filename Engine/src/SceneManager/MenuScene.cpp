#include "MenuScene.h"

geProject::MenuScene::MenuScene(){
	init();
}

geProject::MenuScene::~MenuScene()
{
}

void geProject::MenuScene::update(float deltaTime){	
	camera->update(deltaTime);
	renderScene();
	animationManager->update(deltaTime);
	mouse->endFrame();
	keyboard->endFrame();
	entitymanager.endFrame();
	render(*(camera), "../../../../Game/assets/shaders/VertexShaderDefault.glsl");
}

void geProject::MenuScene::init()
{
	std::cout << "Editor Scene!" << std::endl;
	//entitymanager = new EntityManager(10000);
	camera = new LevelCamera(glm::vec2(0.0f, 0.0f));
	entitymanager.startUp();
	animationManager = new AnimationManager(/**entitymanager*/);
	rendermanager = new Renderer();
	mouse->setInverses(camera->getProjectionInverse(), camera->getViewMatrixInverse());
	resourcemanager.loadShader("../../../../Game/assets/shaders/VertexShaderDefault.glsl", "../../../../Game/assets/shaders/FragmentShaderDefault.glsl");
	resourcemanager.loadShader("../../../../Game/assets/shaders/LineVertexShader.glsl", "../../../../Game/assets/shaders/LineFragmentShader.glsl");
	resourcemanager.loadShader("../../../../Game/assets/shaders/SelectionVertexShader.glsl", "../../../../Game/assets/shaders/SelectionFragmentShader.glsl");
	filePath = "../../../../Game/assets/levels/menu.json";
	resourcemanager.loadSpriteSheet("../../../../Game/assets/images/spritesheets/environment.png", 15, 16, 16, 0, 0);
	resourcemanager.loadSpriteSheet("../../../../Game/assets/images/spritesheets/player.png", 26, 16, 16, 0, 0);
	resourcemanager.loadSpriteSheet("../../../../Game/assets/images/spritesheets/enemies.png", 26, 16, 16, 0, 0);
	resourcemanager.loadMap("../../../../Game/assets/images/spritesheets/level1.png", 9476, 16, 16, 0, 0);
	resourcemanager.loadSpriteSheet("../../../../Game/assets/images/spritesheets/menu1.png", 4, 64, 32, 0, 0);
	selectionTextures = new FrameBuffer(1920, 1080, true);
	eventSystem.setContext(MenuContext);	
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
	geProject::Scene::serialize(filePath);

}

void geProject::MenuScene::updateImgui()
{
}

void geProject::MenuScene::updateSceneImgui()
{
}



void geProject::MenuScene::setActiveEntity(int entityId)
{
}

unsigned int geProject::MenuScene::getActiveEntity()
{
	return 0;
}



void geProject::MenuScene::setPicking()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glDisable(GL_BLEND);
	glClear(GL_COLOR_BUFFER_BIT);
	selectionTextures->bindPicking();
	render(*(camera),"../../../../Game/assets/shaders/SelectionVertexShader.glsl");
	if (mouse->mouseButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
		int x = (int)mouse->getScreenXpos();
		int y = (int)mouse->getScreenYpos();
		int entityId = selectionTextures->getPixel(x, y);	
		std::cout << entityId << std::endl;
		
		switch (entityId) {
		case 0:
			eventSystem.publish(new GameLoadEvent(MenuContext, 2, true));
			break;
		case 1:
			eventSystem.publish(new GameLoadEvent(MenuContext, 2, false));
			
			break;
		case 2:
			eventSystem.publish(new GameLoadEvent(MenuContext, 3, true));
			break;
		case 3:
			eventSystem.publish(new GameLoadEvent(MenuContext, -1 , true));
			break;
		default:
			break;
		}
	}

	selectionTextures->unBindPicking();
	glEnable(GL_BLEND);
}

void geProject::MenuScene::saveGame(GameSaveEvent* save)
{
}

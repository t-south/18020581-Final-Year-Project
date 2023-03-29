#include "Scene.h"

geProject::Window* geProject::Scene::gameWindow;

//MANAGERS
geProject::ResourceManager* geProject::Scene::resourceManager;
geProject::Physics* geProject::Scene::physicsManager;
geProject::EntityManager* geProject::Scene::manager;
geProject::AnimationManager* geProject::Scene::animationManager;

std::string geProject::Scene::getFilePath()
{
	return filePath;
}

void geProject::Scene::serialize(std::string filepath) {
	oFile.open(filepath);
	if (oFile.is_open(), std::ofstream::out | std::ofstream::trunc) {
		//std::cout << "File is open" << std::endl;
		//serialize each entity into json
		json sceneData;
		auto ent = manager->getEntities();
		for (auto & i : ent) {
			json entity = serializeEntity(*i);
			if (entity != NULL) {
				sceneData.push_back(entity);
			}
		}
		oFile << sceneData << std::endl;
		oFile.close();
	}
	else {
		std::cout << "Unable to open file" << std::endl;
	}
	
}


json geProject::Scene::serializeEntity(Entity& entity) {
	//go through each of the components 
	if (oFile.is_open() && entity.id > -1) {
		json entityjson = json::object();
		json entityType = json{"entityType", entity.type};
		json transformData = json::object();
		json rigidData = json::object();
		json circleColliderData = json::object();
		json boxColliderData = json::object();
		json animateData = json::object();
		Transform transform = *manager->getTransformComponent(entity.id);
		to_json(transformData, transform);
		json spriteData = json::object();
		SpriteRender sprite = *manager->getSpriteComponent(entity.id);
		to_json(spriteData, sprite);
		if ((entity.compMask & 1 << 2) == 1 << 2) {			
			Rigidbody rigid = *manager->getRigidBodyComponent(entity.id);
			to_json(rigidData, rigid);
		}
		else {			
			rigidData = json{
				"Rigidbody", 0
			};
		}
		if ((entity.compMask & 1 << 3) == 1 << 3) {					
			for (auto& circle : manager->getCircleColliderComponents(entity.id)){
				json circleData = json::object();
				to_json(circleData, circle);
				circleColliderData["EntityCircleCollider"].push_back(circleData);
			}
		}


		
		if ((entity.compMask & 1 << 4) == 1 << 4) {				
			for (auto& box : manager->getBoxColliderComponents(entity.id)) {
				json boxData = json::object();
				to_json(boxData, box);
				boxColliderData["EntityBoxCollider"].push_back(boxData);
			}
		}

		
		if ((entity.compMask & 1 << 5) == 1 << 5) {
			Animation animate = *manager->getAnimationComponent(entity.id);
			to_json(animateData, animate);
		}
		else {
			animateData = json{
				"Animation", 0
			};
		}
		entityjson["entity"].push_back(entityType);
		entityjson["entity"].push_back(transformData);
		entityjson["entity"].push_back(spriteData);
		entityjson["entity"].push_back(rigidData);
		entityjson["entity"].push_back(circleColliderData);
		entityjson["entity"].push_back(boxColliderData);
		entityjson["entity"].push_back(animateData);
		return entityjson;
	}

	else {
		std::cerr << "file is not open" << std::endl;
		return NULL;
	}




	//auto font = scene->manager->getFontRenderComponent(entityId);
}



void geProject::Scene::deserialize(std::string filepath) {
	std::ifstream iFile(filepath);
	try {
		if (iFile.is_open()) {
			json data = json::parse(iFile);
			for (const auto& i : data.items()) {
				entityTypes entityType = i.value().at("entity")[0][1];
				json transData = i.value().at("entity")[1];
				json spritedata = i.value().at("entity")[2];		
				json rigidData = i.value().at("entity")[3];
				json circleData = i.value().at("entity")[4];
				json boxData = i.value().at("entity")[5];
				json animateData = i.value().at("entity")[6];
				Transform trans = Transform();
				SpriteRender sprite = SpriteRender();				
				from_json(transData, trans);
				from_json(spritedata, sprite);
				int entityId = manager->addEntity(entityType);
				manager->assignTransform(entityId, trans);
				manager->assignSpriteRender(entityId, sprite);		
				if (rigidData[1] != 0) {
					Rigidbody rigid = Rigidbody();
					from_json(rigidData, rigid);
					manager->assignRigidBody(entityId, rigid);
				}
				if (circleData.empty() == 0) {
					for (const auto& i : circleData.find("EntityCircleCollider").value().items()) {
						CircleCollider circle = CircleCollider();
						from_json(i.value(), circle);
						manager->assignCircleCollider(entityId, circle);
					}
				}
				if (boxData.empty() == 0) {
					
					for (const auto& i : boxData.find("EntityBoxCollider").value().items()) {
						
						BoxCollider box = BoxCollider();
						from_json(i.value(), box);
						manager->assignBoxCollider(entityId, box);
					}
				}
				if (animateData[1] != 0) {
					Animation animation = Animation();
					from_json(animateData, animation);
					manager->assignAnimation(entityId, animation);
				}
				addEntityToScene(entityId);		
			}
			iFile.close();
		}
		else if (iFile.fail()) {
			serialize(filepath);
		}
	}
	catch (json::parse_error& err)
	{
		std::cerr << "parse error at byte " << err.byte << std::endl;
	}
}


void geProject::Scene::to_json(json& data, Animation& comp) {	
	data = json{
		"Animation", {
			{"speed", comp.speed},
			{"currentFrame", comp.currentFrame},
			{"animationState", comp.state},
			{"loopAnimation", comp.loopAnimation}
		}
	};
}

void geProject::Scene::to_json(json& data, SpriteRender& comp) {
	data = json{
		"SpriteRender", {		
			{"colorR", comp.color[0]}, {"colorG", comp.color[1]}, {"colorB", comp.color[2]}, {"colorA", comp.color[3]},
			{"texturePosX1",comp.texturePos[0][0]}, {"texturePosY1", comp.texturePos[0][1]},
			{"texturePosX2",comp.texturePos[1][0]}, {"texturePosY2", comp.texturePos[1][1]},
			{"texturePosX3",comp.texturePos[2][0]}, {"texturePosY3", comp.texturePos[2][1]},
			{"texturePosX4",comp.texturePos[3][0]}, {"texturePosY4", comp.texturePos[3][1]},
			{"textureId", comp.textureId},
			{"spriteSheetId", comp.spriteSheetId},
			{"zIndex", comp.zIndex}
		}

	};
}
void geProject::Scene::to_json(json& data, Transform& comp) {
	data = json{
		"Transform", {
			{"transformName", comp.name},
			{"positionX", comp.position[0]}, {"positionY", comp.position[1]},
			{"scaleX", comp.scale[0]}, {"scaleY", comp.scale[1]},
			{"rotation", comp.rotation},		
			{"dirtyFlag", comp.dirtyFlag[0]}, {"renderBuffer", comp.dirtyFlag[1]}, {"bufferIndex", comp.dirtyFlag[2]}
		}
	};
}


void geProject::Scene::to_json(json& data, Rigidbody& comp) {	
	data = json{
		"Rigidbody", {
			{"collider", comp.collider},
			{"friction" , comp.friction},
			{"angularDamping", comp.angularDamping},
			{"linearDamping", comp.linearDamping},
			{"density", comp.density},
			{"fixedRotate", comp.fixedRotate},
			{"bullet", comp.bullet},
			{"bodyType", comp.bodyType},
			{"velocityX", comp.velocity[0]}, {"velocityY", comp.velocity[1]}, {"velocityZ", comp.velocity[2]}
		}
	};	
}


void geProject::Scene::to_json(json& data, CircleCollider& comp) {
	data = json{
		"CircleCollider", {		
			{"entityAssigned", comp.entityAssigned},
			{"circleRadius", comp.radius},
			{"circleoffsetX", comp.offset[0]}, {"circleoffsetY", comp.offset[1]},
			{"circleSensor", comp.sensor}
		}
	};	
}


void geProject::Scene::to_json(json& data, BoxCollider& comp) {
	data = json{
		"BoxCollider", {	
			{"entityAssigned", comp.entityAssigned},
			{"boxoffsetX", comp.offset[0]}, {"boxoffsetY", comp.offset[1]},
			{"boxSizeX", comp.boxSize[0]}, {"boxSizeY", comp.boxSize[1]},
			{"boxOriginX", comp.origin[0]}, {"boxOriginY", comp.origin[1]},
			{"boxSensor", comp.sensor}
	}
	};	
}



void geProject::Scene::from_json(json& data, Animation& comp) {
	data[1].at("speed").get_to(comp.speed);
	data[1].at("currentFrame").get_to(comp.currentFrame);
	data[1].at("animationState").get_to(comp.state);
	data[1].at("loopAnimation").get_to(comp.loopAnimation);

}


void geProject::Scene::from_json(json& data, SpriteRender& comp) {
	data[1].at("colorR").get_to(comp.color[0]);
	data[1].at("colorG").get_to(comp.color[1]);
	data[1].at("colorB").get_to(comp.color[2]);
	data[1].at("colorA").get_to(comp.color[3]);
	data[1].at("texturePosX1").get_to(comp.texturePos[0][0]);
	data[1].at("texturePosY1").get_to(comp.texturePos[0][1]);
	data[1].at("texturePosX2").get_to(comp.texturePos[1][0]);
	data[1].at("texturePosY2").get_to(comp.texturePos[1][1]);
	data[1].at("texturePosX3").get_to(comp.texturePos[2][0]);
	data[1].at("texturePosY3").get_to(comp.texturePos[2][1]);
	data[1].at("texturePosX4").get_to(comp.texturePos[3][0]);
	data[1].at("texturePosY4").get_to(comp.texturePos[3][1]);
	data[1].at("textureId").get_to(comp.textureId);
	data[1].at("spriteSheetId").get_to(comp.spriteSheetId);
	data[1].at("zIndex").get_to(comp.zIndex);
}


void geProject::Scene::from_json(json& data, Transform& comp) {
	data[1].at("transformName").get_to(comp.name);
	data[1].at("positionX").get_to(comp.position[0]);
	data[1].at("positionY").get_to(comp.position[1]);
	data[1].at("scaleX").get_to(comp.scale[0]);
	data[1].at("scaleY").get_to(comp.scale[1]);
	data[1].at("rotation").get_to(comp.rotation);
	data[1].at("dirtyFlag").get_to(comp.dirtyFlag[0]);
	data[1].at("renderBuffer").get_to(comp.dirtyFlag[1]);
	data[1].at("bufferIndex").get_to(comp.dirtyFlag[2]);
}


void geProject::Scene::from_json(json& data, CircleCollider& comp) {
	if (data[1] == 0) {
		comp.id = 0;
	}
	else {	
		data[1].at("entityAssigned").get_to(comp.entityAssigned);
		data[1].at("circleRadius").get_to(comp.radius);
		data[1].at("circleoffsetX").get_to(comp.offset[0]);
		data[1].at("circleoffsetY").get_to(comp.offset[1]);
		data[1].at("circleSensor").get_to(comp.sensor);
	}
}

void geProject::Scene::from_json(json& data, BoxCollider& comp) {	
	if (data[1] == 0) {
		comp.id = 0;
	}
	else {
		data[1].at("entityAssigned").get_to(comp.entityAssigned);
		data[1].at("boxoffsetX").get_to(comp.offset[0]);
		data[1].at("boxoffsetY").get_to(comp.offset[1]);
		data[1].at("boxSizeX").get_to(comp.boxSize[0]);
		data[1].at("boxSizeY").get_to(comp.boxSize[1]);
		data[1].at("boxOriginX").get_to(comp.origin[0]);
		data[1].at("boxOriginY").get_to(comp.origin[1]);
		data[1].at("boxSensor").get_to(comp.sensor);
	}
}



void geProject::Scene::from_json(json& data, Rigidbody& comp) {

	if (data[1] == 0) {
		comp.id = 0;
	}
	else {
		data[1].at("collider").get_to(comp.collider);
		data[1].at("friction").get_to(comp.friction);
		data[1].at("angularDamping").get_to(comp.angularDamping);
		data[1].at("linearDamping").get_to(comp.linearDamping);
		data[1].at("density").get_to(comp.density);
		data[1].at("fixedRotate").get_to(comp.fixedRotate);
		data[1].at("bullet").get_to(comp.bullet);
		data[1].at("bodyType").get_to(comp.bodyType);
		data[1].at("velocityX").get_to(comp.velocity[0]);
		data[1].at("velocityY").get_to(comp.velocity[1]);
		data[1].at("velocityZ").get_to(comp.velocity[2]);
	}
}




void geProject::Scene::setWindow(Window* window){
	gameWindow = window;
	setKeyboardListener();
	setMouseListener();
}

void geProject::Scene::setKeyboardListener(){
	keyboard = KeyboardListener::getInstance();
	glfwSetKeyCallback(gameWindow->getWindow(), keyboard->key_callback);	
}

void geProject::Scene::setMouseListener(){
	mouse = MouseListener::getInstance();
	mouse->setWindowDimensions(gameWindow->getWidth(), gameWindow->getHeight());
	glfwSetCursorPosCallback(gameWindow->getWindow(), mouse->cursor_position_callback);
	glfwSetMouseButtonCallback(gameWindow->getWindow(), mouse->mouse_button_callback);
	glfwSetScrollCallback(gameWindow->getWindow(), mouse->scroll_callback);
}

float geProject::Scene::getMouseX() { return mouse->getYpos(); }

float geProject::Scene::getMouseY() { return mouse->getYpos(); }


geProject::MouseListener* geProject::Scene::getMouseListener() {
	return mouse;
}

void geProject::Scene::reloadLevel(std::string filepath) {
	physicsManager->clear();
	renderer->clear();
	manager->reloadManager();	
}

geProject::Camera* geProject::Scene::getCamera(){
	return camera;
}


void geProject::Scene::setPhysics(bool check) {
	physicsEnabled = check;
}
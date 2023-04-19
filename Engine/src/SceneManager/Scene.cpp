#include "Scene.h"

geProject::Window* geProject::Scene::gameWindow;

//MANAGERS
geProject::AnimationManager* geProject::Scene::animationManager;
geProject::Receiver* geProject::Scene::controlManager;
geProject::MouseListener* geProject::Scene::mouse;
geProject::KeyboardListener* geProject::Scene::keyboard;


//geProject::Physics* geProject::Scene::physicsmanager;
//geProject::EntityManager* geProject::Scene::entitymanager;
geProject::Renderer* geProject::Scene::rendermanager;
std::string geProject::Scene::getFilePath()
{
	return filePath;
}

void geProject::Scene::serialize(std::string filepath) {
	oFile.open(filepath.c_str());
	if (oFile.is_open(), std::ofstream::out | std::ofstream::trunc) {
		//serialize each entity into json
		json sceneData;
		auto ent = entitymanager.getEntities();
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


void geProject::Scene::renderScene()
{
	//UPDATES TO RENDERING
	if (entitymanager.hasUpdate()) {
		for (int i = 0; i < entitymanager.getEntityNum(); i++) {
			Entity ent = entitymanager.getEntity(i);
			if (ent.compMask > 0 && ent.id > -1) {
				if ((ent.compMask & 4) == 4) {//check for rigidbody
					if ((ent.compMask & 8) == 8 || (ent.compMask & 16) == 16) {//check for boxcollider or circlecollider			
						physicsmanager.addEntity(ent.id);
					}
				}
				// only sprites that have not been added to the renderer previously will be set to 0		
				//transform dirtyflag for render index is by default set to -1 when first created
				if (entitymanager.getVertexStatus(ent.id) == -1) {
					rendermanager->addSpriteToBatch(ent.id);
				}
				//if there has been any updates the dirty flag in transform component will be set to 1
				else if (entitymanager.getUpdateStatus(ent.id) == 1) {
					rendermanager->updateSprite(ent.id);
				}
			}
		}
		//manager->endFrame();			
	}

}

json geProject::Scene::serializeEntity(Entity& entity) {
	//go through each of the components 

	if (oFile.is_open() && entity.id > -1) {
		json entityjson = json::object();
		json entityType = json{ "entityType", entity.type };
		json transformData = json::object();
		json rigidData = json::object();
		json circleColliderData = json::object();
		json boxColliderData = json::object();
		json animateData = json::object();
		json controllerData = json::object();
		json healthData = json::object();
		json damageData = json::object();
		json viewData = json::object();
		Transform transform = entitymanager.getTransformComponent(entity.id);
		to_json(transformData, transform);
		json spriteData = json::object();
		SpriteRender sprite = entitymanager.getSpriteComponent(entity.id);
		to_json(spriteData, sprite);
		if ((entity.compMask & 1 << 2) == 1 << 2) {
			Rigidbody rigid = entitymanager.getRigidBodyComponent(entity.id);
			to_json(rigidData, rigid);
		}
		else {	
		rigidData = json{
			"Rigidbody", 0
		};
		}
		if ((entity.compMask & 1 << 3) == 1 << 3) {
			for (auto& circle : entitymanager.getCircleColliderComponents(entity.id)) {
				json circleData = json::object();
				to_json(circleData, circle);
				circleColliderData["EntityCircleCollider"].push_back(circleData);
			}
		}
	

		
		if ((entity.compMask & 1 << 4) == 1 << 4) {				
			for (auto& box : entitymanager.getBoxColliderComponents(entity.id)) {
				json boxData = json::object();
				to_json(boxData, box);
				boxColliderData["EntityBoxCollider"].push_back(boxData);
			}
		}

		
		if ((entity.compMask & 1 << 5) == 1 << 5) {
			Animation animate = entitymanager.getAnimationComponent(entity.id);
			to_json(animateData, animate);
		}

		else {
			animateData = json{
				"Animation", 0
			};
		}

		if ((entity.compMask & 1 << 6) == 1 << 6) {
			Controls control = entitymanager.getControllerComponent(entity.id);
			to_json(controllerData, control);
		}
		else {
			controllerData = json{
				"Controller", 0
			};
		}

		if ((entity.compMask & 1 << 7) == 1 << 7) {
			Health health = entitymanager.getHealthComponent(entity.id);
			to_json(healthData, health);
		}
		else {
			healthData = json{
				"Health", 0
			};
		}


		if ((entity.compMask & 1 << 8) == 1 << 8) {
			Damage damage = entitymanager.getDamageComponent(entity.id);
			to_json(damageData, damage);
		}
		else {
			damageData = json{
				"Damage", 0
			};
		}

		if ((entity.compMask & 1 << 9) == 1 << 9) {
			ViewCollider view = entitymanager.getViewComponent(entity.id);
			to_json(viewData, view);
		}
		else {
			viewData = json{
				"ViewCollider", 0

			};
		}

		entityjson["entity"].push_back(entityType);
		entityjson["entity"].push_back(transformData);
		entityjson["entity"].push_back(spriteData);
		entityjson["entity"].push_back(rigidData);
		entityjson["entity"].push_back(circleColliderData);
		entityjson["entity"].push_back(boxColliderData);
		entityjson["entity"].push_back(animateData);
		entityjson["entity"].push_back(controllerData);
		entityjson["entity"].push_back(healthData);
		entityjson["entity"].push_back(damageData);
		entityjson["entity"].push_back(viewData);
		return entityjson;
	}

	else {
		std::cerr << "file is not open" << std::endl;
		return NULL;
	}




	//auto font = scene->manager->getFontRenderComponent(entityId);
}



void geProject::Scene::deserialize(std::string filepath) {
	std::ifstream iFile(filepath.c_str());
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
				json controllerData = i.value().at("entity")[7];
				json healthData = i.value().at("entity")[8];
				json damageData = i.value().at("entity")[9];
				json viewData = i.value().at("entity")[10];
				Transform trans = Transform();
				SpriteRender sprite = SpriteRender();				
				from_json(transData, trans);
				from_json(spritedata, sprite);
				int entityId = entitymanager.addEntity(entityType);
				entitymanager.assignTransform(entityId, trans);
				entitymanager.assignSpriteRender(entityId, sprite);
				if (rigidData[1] != 0) {
					Rigidbody rigid = Rigidbody();
					from_json(rigidData, rigid);
					entitymanager.assignRigidBody(entityId, rigid);
				}
				if (circleData.empty() == 0) {
					for (const auto& i : circleData.find("EntityCircleCollider").value().items()) {
						CircleCollider circle = CircleCollider();
						from_json(i.value(), circle);
						entitymanager.assignCircleCollider(entityId, circle);
					}
				}
				if (boxData.empty() == 0) {					
					for (const auto& i : boxData.find("EntityBoxCollider").value().items()) {
						
						BoxCollider box = BoxCollider();
						from_json(i.value(), box);
						entitymanager.assignBoxCollider(entityId, box);
					}
				}
				if (animateData[1] != 0) {
					Animation animation = Animation();
					from_json(animateData, animation);
					entitymanager.assignAnimation(entityId, animation);
				}
				if (controllerData[1] != 0) {
					Controls control = Controls();
					from_json(controllerData, control);
					entitymanager.assignController(entityId, control);
				}
				if (healthData[1] != 0) {
					Health health = Health();
					from_json(healthData, health);
					entitymanager.assignHealth(entityId, health);
				}
				
				if (damageData[1] != 0) {
					Damage dmg = Damage();
					from_json(damageData, dmg);
					entitymanager.assignDamage(entityId, dmg);
				}	
				if (viewData[1] != 0) {
					ViewCollider view = ViewCollider();
					from_json(viewData, view);
					entitymanager.assignView(entityId, view);
				}
	
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
			{"circleentityType", comp.entityType},
			{"circlecolliders", comp.colliders},
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
			{"boxentityType", comp.entityType},
			{"boxcolliders", comp.colliders},
			{"boxSensor", comp.sensor}
	}
	};	
}


void geProject::Scene::to_json(json& data, Controls& comp) {
	data = json{
		"Controller", {
			{"runspeed", comp.runspeed},
			{"speedMod", comp.speedModifier},
			{"controldirection", comp.direction}
		}
	};
}

void geProject::Scene::to_json(json& data, Health& comp){
	data = json{
	"Health", {
		{"maxHealth", comp.maxHealth},
		{"currHealth", comp.currentHealth},
		{"invincible", comp.invincible}
	}
	};

}

void geProject::Scene::to_json(json& data, Damage& comp){
	data = json{
	"Damage", {
		{"dmgAtk", comp.dmgAtk},
		{"dmgModifier", comp.dmgModifier},
		{"dmgType", comp.dmgType}
	}
	};

}


void geProject::Scene::to_json(json& data, ViewCollider& comp) {
	data = json{
		"ViewCollider", {
			{"entityAssigned", comp.entityAssigned},
			{"viewRadius", comp.radius},
			{"viewoffsetX", comp.offset[0]}, {"viewoffsetY", comp.offset[1]},			
			{"viewentityType", comp.entityType},
			{"viewcolliders", comp.colliders},
			{"viewSensor", comp.sensor}
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
		data[1].at("circleentityType").get_to(comp.entityType);
		data[1].at("circlecolliders").get_to(comp.colliders);
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
		data[1].at("boxentityType").get_to(comp.entityType);
		data[1].at("boxcolliders").get_to(comp.colliders);
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

void geProject::Scene::from_json(json& data, Controls& comp) {
	if (data[1] == 0) {
		comp.id = 0;
	}
	else {
		data[1].at("runspeed").get_to(comp.runspeed);
		data[1].at("speedMod").get_to(comp.speedModifier);
		data[1].at("controldirection").get_to(comp.direction);		
	}
}

void geProject::Scene::from_json(json& data, Health& comp){
	if (data[1] == 0) {
		comp.id = 0;
	}
	else {
		data[1].at("maxHealth").get_to(comp.maxHealth);
		data[1].at("currHealth").get_to(comp.currentHealth);
		data[1].at("invincible").get_to(comp.invincible);
	}
}

void geProject::Scene::from_json(json& data, Damage& comp){
	if (data[1] == 0) {
		comp.id = 0;
	}
	else {
		data[1].at("dmgAtk").get_to(comp.dmgAtk);
		data[1].at("dmgModifier").get_to(comp.dmgModifier);
		data[1].at("dmgType").get_to(comp.dmgType);	
	}
}


void geProject::Scene::from_json(json& data, ViewCollider& comp) {
	if (data[1] == 0) {
		comp.id = 0;
	}
	else {
		data[1].at("entityAssigned").get_to(comp.entityAssigned);
		data[1].at("viewRadius").get_to(comp.radius);
		data[1].at("viewoffsetX").get_to(comp.offset[0]);
		data[1].at("viewoffsetY").get_to(comp.offset[1]);	
		data[1].at("viewentityType").get_to(comp.entityType);
		data[1].at("viewcolliders").get_to(comp.colliders);
		data[1].at("viewSensor").get_to(comp.sensor);
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

void geProject::Scene::reloadLevel() {

	physicsmanager.clear();
	enemies.clear();
	rendermanager->clear();	
	entitymanager.reloadManager();
}




void geProject::Scene::setPhysics(bool check) {
	physicsEnabled = check;
}

void geProject::Scene::setGoapStatus(bool goap)
{
	goapEnabled = goap;
}



void geProject::Scene::render(Camera& camera, std::string shaderPath)
{
	rendermanager->render(camera, shaderPath);
}

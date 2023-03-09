#include "Scene.h"



void geProject::Scene::serialize(std::string filepath) {
	oFile.open(filepath);
	if (oFile.is_open(), std::ofstream::out | std::ofstream::trunc) {
		//std::cout << "File is open" << std::endl;
		//serialize each entity into json
		json sceneData;
		for (auto const& i : entities) {
			json entity = serializeEntity(i->id);
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


json geProject::Scene::serializeEntity(unsigned int entityId) {
	//go through each of the components 
	if (oFile.is_open()) {
		json entity = json::object();
		json transformData = json::object();
		auto transform = *manager->getTransformComponent(entityId);
		to_json(transformData, transform);
		json spriteData = json::object();
		auto sprite = *manager->getSpriteComponent(entityId);
		to_json(spriteData, sprite);

		json rigidData = json::object();
		auto rigid = *manager->getRigidBodyComponent(entityId);
		to_json(rigidData, rigid);
		//json fontData = json::object();
		//auto font = *manager->getFontRenderComponent(entityId);
		//to_json(spriteData, font);


		entity["entity"].push_back(transformData);
		entity["entity"].push_back(spriteData);
		entity["entity"].push_back(rigidData);
		return entity;
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
				json transData = i.value().at("entity")[0];
				json spritedata = i.value().at("entity")[1];
		
				json rigidData = i.value().at("entity")[2];
				Transform trans = Transform();
				SpriteRender sprite = SpriteRender();
				Rigidbody rigid = Rigidbody();
				from_json(transData, trans);
				from_json(spritedata, sprite);
				from_json(rigidData, rigid);

				unsigned int entityId = manager->addEntity();
				manager->assignTransform(entityId, trans);
				manager->assignSpriteRender(entityId, sprite);
				if (rigid.id > 0) {
					manager->assignRigidBody(entityId, rigid);
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


void geProject::Scene::to_json(json& data, FontRender& comp) {}

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
		{"zIndex", comp.zIndex},
		{"dirtyFlag", comp.dirtyFlag[0]}, {"renderBuffer", comp.dirtyFlag[1]}, {"bufferIndex", comp.dirtyFlag[2]}
		}

	};
}
void geProject::Scene::to_json(json& data, Transform& comp) {
	data = json{
		"Transform", {
			{"positionX", comp.position[0]}, {"positionY", comp.position[1]},
			{"scaleX", comp.scale[0]}, {"scaleY", comp.scale[1]},
			{"dirtyFlag", comp.dirtyFlag[0]}, {"renderBuffer", comp.dirtyFlag[1]}, {"bufferIndex", comp.dirtyFlag[2]}
		}
	};
}


void geProject::Scene::to_json(json& data, Rigidbody& comp) {
	if (comp.id == 0) {
		data = json{
			"Rigidbody", 0
		};
	}
	else {
		data = json{
			"Rigidbody", {
				{"collider", comp.collider},
				{"friction" , comp.friction},
				{"velocityX", comp.velocity[0]}, {"velocityY", comp.velocity[1]}, {"velocityZ", comp.velocity[2]},
				{"dirtyFlag", comp.dirtyFlag[0]}, {"renderBuffer", comp.dirtyFlag[1]}, {"bufferIndex", comp.dirtyFlag[2]}
			}
		};
	}
}


void geProject::Scene::from_json(json& data, FontRender& comp) {}


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
	data[1].at("dirtyFlag").get_to(comp.dirtyFlag[0]);
	data[1].at("renderBuffer").get_to(comp.dirtyFlag[1]);
	data[1].at("bufferIndex").get_to(comp.dirtyFlag[2]);
}


void geProject::Scene::from_json(json& data, Transform& comp) {
	data[1].at("positionX").get_to(comp.position[0]);
	data[1].at("positionY").get_to(comp.position[1]);
	data[1].at("scaleX").get_to(comp.scale[0]);
	data[1].at("scaleY").get_to(comp.scale[1]);
	data[1].at("dirtyFlag").get_to(comp.dirtyFlag[0]);
	data[1].at("renderBuffer").get_to(comp.dirtyFlag[1]);
	data[1].at("bufferIndex").get_to(comp.dirtyFlag[2]);
}

void geProject::Scene::from_json(json& data, Rigidbody& comp) {

	if (data[1] == 0) {
		comp.id = 0;
	}
	else {
		data[1].at("collider").get_to(comp.collider);
		data[1].at("friction").get_to(comp.friction);
		data[1].at("velocityX").get_to(comp.velocity[0]);
		data[1].at("velocityY").get_to(comp.velocity[1]);
		data[1].at("velocityZ").get_to(comp.velocity[2]);
		data[1].at("dirtyFlag").get_to(comp.dirtyFlag[0]);
		data[1].at("renderBuffer").get_to(comp.dirtyFlag[1]);
		data[1].at("bufferIndex").get_to(comp.dirtyFlag[2]);
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
/*#include "SceneSerialize.h"

geProject::SceneSerialize::SceneSerialize(std::shared_ptr<geProject::Scene> currentScene): scene(currentScene){}
geProject::SceneSerialize::~SceneSerialize() { oFile.close(); }


void geProject::SceneSerialize::serialize(const std::string& filePath) {
	if (scene != nullptr) {
		oFile.open("../../../../Game/assets/levels/jsonTest.json");
		if (oFile.is_open()) {
			std::cout << "File is open" << std::endl;
			//serialize each entity into json
			json sceneData;
			for (auto const& i : scene->entities) {
				json entity = serializeEntity(i->id);				
				if (entity != NULL) {
					sceneData.push_back(entity);
				}
			}
			oFile << sceneData << std::endl;
		}
		else {
			std::cout << std::filesystem::current_path().string() << std::endl;
			std::cout << "Unable to open file" << std::endl;
		}	
	}
}


json geProject::SceneSerialize::serializeEntity(unsigned int entityId) {
	//go through each of the components 
	if (oFile.is_open()) {
		json entity = json::object();
		json transformData = json::object();
		auto transform = *scene->manager->getTransformComponent(entityId);
		to_json(transformData, transform);
		json spriteData = json::object();
		auto sprite = *scene->manager->getSpriteComponent(entityId);
		to_json(spriteData, sprite);
		entity["entity"].push_back(transformData);
		entity["entity"].push_back(spriteData);
		return entity;
	}

	else {
		std::cerr << "file is not open" << std::endl;
		return NULL;
	}


	
	
	//auto font = scene->manager->getFontRenderComponent(entityId);
}



void geProject::SceneSerialize::deserialize(const std::string& filePath) {
	if (scene != nullptr) {

	}
	std::ifstream iFile ("../../../../Game/assets/levels/jsonTest.json");
	try {
		if (iFile.is_open()) {
			json data = json::parse(iFile);			
			for (const auto& i : data.items() ){
				json transData = i.value().at("entity")[0];
				json spritedata = i.value().at("entity")[1];				
				Transform trans = Transform();				
				SpriteRender sprite = SpriteRender();
				from_json(transData, trans);
				from_json(spritedata, sprite);
				unsigned int entityId = scene->manager->addEntity();
				scene->manager->assignTransform(entityId, trans);
				scene->manager->assignSpriteRender(entityId, sprite);
				scene->addEntityToScene(entityId);
			}
		}		
	}
	catch (json::parse_error& err)
	{
		std::cerr << "parse error at byte " << err.byte << std::endl;
	}


}



void geProject::SceneSerialize::to_json(json& data, FontRender& comp) {}

void geProject::SceneSerialize::to_json(json& data, SpriteRender& comp) {
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
void geProject::SceneSerialize::to_json(json& data, Transform& comp) {
	data = json {
		"Transform", {
			{"positionX", comp.position[0]}, {"positionY", comp.position[1]},
			{"scaleX", comp.scale[0]}, {"scaleY", comp.scale[1]},
			{"dirtyFlag", comp.dirtyFlag[0]}, {"renderBuffer", comp.dirtyFlag[1]}, {"bufferIndex", comp.dirtyFlag[2]}
		}		
	};
}


void geProject::SceneSerialize::to_json(json& data, Rigidbody& comp) {
	data = json{
		"Rigidbody", {
			{"collider", comp.collider}, 
			{"friction" , comp.friction},
			{"velocityX", comp.velocity[0]}, {"velocityY", comp.velocity[1]}, {"velocityZ", comp.velocity[2]},
			{"testX" , comp.test[0]}, {"testY" , comp.test[1]}, {"testZ" , comp.test[2]}, {"testW" , comp.test[3]},
			{"dirtyFlag", comp.dirtyFlag[0]}, {"renderBuffer", comp.dirtyFlag[1]}, {"bufferIndex", comp.dirtyFlag[2]}
		}
	};
}


void geProject::SceneSerialize::from_json(json& data, FontRender& comp) {}


void geProject::SceneSerialize::from_json(json& data, SpriteRender& comp) {
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


void geProject::SceneSerialize::from_json(json& data, Transform& comp) {
	data[1].at("positionX").get_to(comp.position[0]);
	data[1].at("positionY").get_to(comp.position[1]);
	data[1].at("scaleX").get_to(comp.scale[0]);
	data[1].at("scaleY").get_to(comp.scale[1]);
	data[1].at("dirtyFlag").get_to(comp.dirtyFlag[0]);
	data[1].at("renderBuffer").get_to(comp.dirtyFlag[1]);
	data[1].at("bufferIndex").get_to(comp.dirtyFlag[2]);
}

void geProject::SceneSerialize::from_json(json& data, Rigidbody& comp) {
	data[2].at("collider").get_to(comp.collider);
	data[2].at("friction").get_to(comp.friction);
	data[2].at("velocityX").get_to(comp.velocity[0]);
	data[2].at("velocityY").get_to(comp.velocity[1]);
	data[2].at("velocityZ").get_to(comp.velocity[2]);
	data[1].at("testX").get_to(comp.test[0]);
	data[1].at("testY").get_to(comp.test[1]);
	data[1].at("testZ").get_to(comp.test[2]);
	data[1].at("testW").get_to(comp.test[3]);
	data[2].at("dirtyFlag").get_to(comp.dirtyFlag[0]);
	data[2].at("renderBuffer").get_to(comp.dirtyFlag[1]);
	data[2].at("bufferIndex").get_to(comp.dirtyFlag[2]);

}*/
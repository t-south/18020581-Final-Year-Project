#include "AnimationManager.h"



geProject::AnimationManager::AnimationManager(/*EntityManager& emanager*/) /* : entitymanager(&emanager)*/ {
	deserializeAnimations();	
	
}

geProject::AnimationManager::~AnimationManager(){
}

void geProject::AnimationManager::update(float deltaTime){
	for (const auto& i : entitymanager.getEntities()) {
		if ((i->compMask & 32) == 32 && i->id > -1) {					//check there is an animation assigned to entity, if so retrieve sprite and animation data
			SpriteRender sprite = entitymanager.getSpriteComponent(i->id);
			Animation animation = entitymanager.getAnimationComponent(i->id);			
			auto& frameList = animationList[animation.state];
			if (animation.currentFrame >= frameList.size()) {
				animation.currentFrame = 0;
			}			
			//std::cout << deltaTime << std::endl;
			Frame& currentFrame = frameList[animation.currentFrame];
			auto playerSprites = resourcemanager.requestSpriteSheet(sprite.textureId);
			auto& newSprite = playerSprites->getSprite(frameList[animation.currentFrame].sprite);
			entitymanager.assignSpriteRender(i->id, newSprite);
			currentFrame.time += deltaTime;
			//entitymanager.assignSpriteRender(i->id, newSprite);
			if (currentFrame.time >= animation.speed) {
				animation.currentFrame++;
				currentFrame.time = 0.0f;
			}
			entitymanager.assignAnimation(i->id, animation);
		}
	}
}

void geProject::AnimationManager::addFrame(std::string state, int sprite){
	Frame newFrame = Frame{ .sprite = {sprite} };
	animationList[state].push_back(newFrame);
}


void geProject::AnimationManager::changeState(int entityId, std::string newState){

}

void geProject::AnimationManager::serializeAnimations(){
	oFile.open(filePath);
	if (oFile.is_open(), std::ofstream::out | std::ofstream::trunc) {
		//std::cout << "File is open" << std::endl;
		//serialize each entity into json
		json animationData;
		for (auto& state : animationList) {			
			to_json(animationData, state.first);
		}
		oFile << animationData << std::endl;
		oFile.close();
	}
	else {
		std::cout << "Unable to open file" << std::endl;
	}
}

void geProject::AnimationManager::deserializeAnimations(){
	std::ifstream iFile(filePath);
	try {
		if (iFile.is_open()) {
			json data = json::parse(iFile);
			for (const auto& i : data.items()) {
				for (const auto& j : i.value().items()) {
					Frame newFrame = Frame();
					from_json(j.value(), newFrame);					
					animationList[i.key()].push_back(newFrame);
				}
			}
		}
	}
	catch (json::parse_error& err)
	{
		std::cerr << "parse error at byte " << err.byte << std::endl;
	}
}

void geProject::AnimationManager::to_json(json& data, std::string state){
	for (auto& frame : animationList[state]) {
		json FrameData = json::object();
		FrameData = json{
			"Frame", {				
				{"sprite", frame.sprite}
			}
		};
		data[state].push_back(FrameData);
	}
}

void geProject::AnimationManager::from_json(json& data, Frame& comp){		
	data[1].at("sprite").get_to(comp.sprite);

}

void geProject::AnimationManager::assignEntityAnimation(int entityId, std::string state){
	Entity entity = entitymanager.getEntity(entityId);
	if ((entity.compMask & 32) != 32) {
		entitymanager.assignAnimation(entity.id, Animation{.state = {state}});
	}
}

void geProject::AnimationManager::removeEntity(){

}



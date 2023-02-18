#include "EntityManager.h"

//PoolAllocator geProject::Transform::pool{ 1000000, sizeof(Transform) };
//PoolAllocator geProject::SpriteRender::pool{ 1000000, sizeof(SpriteRender) };
//PoolAllocator geProject::FontRender::pool{ 1000000, sizeof(FontRender) };

geProject::EntityManager::EntityManager(uInt maxEntities): transformpool(maxEntities, sizeof(Transform)), spritepool(maxEntities, sizeof(SpriteRender)),
															fontpool(maxEntities, sizeof(FontRender)), maxEntities(maxEntities), entityUpdated(false) 
{

	for (int i = 0; i < maxEntities - 1; ++i) {
		componentTransforms.push_back(nullptr);
		componentFontRender.push_back(nullptr);
		componentSpriteRender.push_back(nullptr);
	}

	

	std::cout << "Transform size : " << sizeof(Transform) << std::endl;
	std::cout << "SpriteRender size : " << sizeof(SpriteRender) << std::endl;
	std::cout << "FontRender size : " << sizeof(FontRender) << std::endl;
}

geProject::EntityManager::~EntityManager(){}

geProject::EntityManager::uInt geProject::EntityManager::addEntity() {
	//add to component vector if new component
	Entity entity = Entity();
	entity.id = entities.size();
	entity.compMask = 0;
	entities.push_back(entity);
	return entities.back().id;
}

bool geProject::EntityManager::hasUpdate() {
	return entityUpdated;
}

void geProject::EntityManager::assignTransform(uInt entityId, Transform transform) {
	if (entityId < maxEntities && entityId >= 0) {
		auto trans = (Transform*)transformpool.allocate(sizeof(Transform));
		std::memcpy(trans, &transform, sizeof(Transform));
		componentTransforms[entityId] = trans;
		//std::cout << "new [entity " << entityId << "][TransformArray] = " << componentTransforms[entityId] << std::endl;
	}
	else {
		//std::cout << "unable to assign transform" << std::endl;
		delete(&transform);
	}
}

void geProject::EntityManager::assignSpriteRender(uInt entityId, SpriteRender sprite) {
	if (entityId < maxEntities && entityId >= 0) {
		auto spr = (SpriteRender*)spritepool.allocate(sizeof(SpriteRender));
		std::memcpy(spr, &sprite, sizeof(SpriteRender));
		componentSpriteRender[entityId] = spr;		
		//std::cout << "new [entity " << entityId << "][componentSpriteRender] = " << componentSpriteRender[entityId] << std::endl;
	}
	else {
		//std::cout << "unable to assign sprite" << std::endl;
		delete(&sprite);
	}
}

void geProject::EntityManager::assignFontRender(uInt entityId, FontRender font) {
	if (entityId < maxEntities && entityId >= 0) {
		auto fonts = (FontRender*)fontpool.allocate(sizeof(FontRender));
		std::memcpy(fonts, &font, sizeof(FontRender));
		componentFontRender[entityId] = fonts;
		//std::cout << "new [entity " << entityId << "][componentFontRender] = " << componentFontRender[entityId] << std::endl;
	}
	else {
		//std::cout << "unable to assign fontrender" << std::endl;
		delete(&font);
	}
}

void geProject::EntityManager::deleteComponent(uInt entityId, uInt componentId) {
	if(entityId < maxEntities && entityId >= 0){
		switch (componentId) {
		case 1:
			delete componentTransforms[entityId];
			componentTransforms[entityId] = nullptr;
			//std::cout << "delete [entity " << entityId << "][TransformArray] = " << componentTransforms[entityId] << std::endl;
			break;
		case 2:
			delete componentSpriteRender[entityId];
			componentSpriteRender[entityId] = nullptr;
			//std::cout << "delete [entity " << entityId << "][componentSpriteRender] = " << componentSpriteRender[entityId] << std::endl;
			break;
		case 4:
			delete componentFontRender[entityId];
			componentFontRender[entityId] = nullptr;
			//std::cout << "delete [entity " << entityId << "][componentFontRender] = " << componentFontRender[entityId] << std::endl;
			break;
		default:
			break;
		}
		entities[entityId].compMask = entities[entityId].compMask & ~componentId;
	}
}


/*
template <class T>
std::vector<T> geProject::EntityManager::getComponents(int componentId){	
	switch (componentId) {
	case 1:
		return componentTransforms;
	case 2:
		return componentSpriteRender;
	case 4:
		return componentFontRender;
	default:
		break;
	}
	
}*/


std::vector<geProject::Transform*> geProject::EntityManager::getTransformComponents() {
	return componentTransforms;
}
std::vector<geProject::SpriteRender*> geProject::EntityManager::getSpriteComponents() {
	return componentSpriteRender;
}
std::vector<geProject::FontRender*> geProject::EntityManager::getFontRenderComponents() {
	return componentFontRender;
}

geProject::Transform* geProject::EntityManager::getTransformComponent(uInt entityId) {
	return componentTransforms[entityId];
}
geProject::SpriteRender* geProject::EntityManager::getSpriteComponent(uInt entityId) {
	return componentSpriteRender[entityId];
}
geProject::FontRender* geProject::EntityManager::getFontRenderComponent(uInt entityId) {
	return componentFontRender[entityId];
}

unsigned int geProject::EntityManager::getEntityNum() {
	return entities.size();
}

geProject::Entity* geProject::EntityManager::getEntity(uInt entityId) {
	return &entities[entityId];
}

void geProject::EntityManager::endFrame() {
	entityUpdated = false;
}
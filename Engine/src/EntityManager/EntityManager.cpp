#include "EntityManager.h"
PoolAllocator geProject::Transform::transformallocator{ 32, sizeof(Transform) };
PoolAllocator geProject::TestData::testallocator{ 32, sizeof(TestData) };

geProject::EntityManager::EntityManager() {

	std::cout << "Transform size : " << sizeof(Transform) << std::endl;
	std::cout << "Test Data size : " << sizeof(TestData) << std::endl;
}

geProject::EntityManager::~EntityManager(){}

unsigned int geProject::EntityManager::addEntity() {
	//add to component vector if new component
	Entity entity = Entity();
	entity.id = entities.size();
	entity.compMask = 0;
	entities.push_back(entity);
	return entities.back().id;
}

void geProject::EntityManager::assignComponent(unsigned int entityId, unsigned int componentId) {
	//check that component not already assigned
	if (componentId | entities[entityId].compMask == 0) {
		unsigned int newMask = entities[entityId].compMask | componentId;
		entities[entityId].compMask = newMask;
		addComponent(entityId, componentId);
	}
}


void geProject::EntityManager::deleteComponent(unsigned int entityId, unsigned int componentId) {
	switch (componentId) {
	case 1:
		delete componentTransforms[entityId];
		componentTransforms[entityId] = nullptr;
		//std::cout << "delete [entity " << entityId << "][TransformArray] = " << componentTransforms[entityId] << std::endl;
		break;
	case 2:
		delete componentTestData[entityId];
		componentTestData[entityId] = nullptr;
		//std::cout << "delete [entity " << entityId << "][TestCompArray] = " << componentTestData[entityId] << std::endl;
		break;
	default:
		break;
	}
	entities[entityId].compMask = entities[entityId].compMask & ~componentId;
}

void geProject::EntityManager::addComponent(unsigned int entityId, unsigned int componentId){
	switch (componentId) {
	case 1:
		componentTransforms[entityId] = new Transform;
		std::cout << "new [entity " << entityId << "][TransformArray] = " << componentTransforms[entityId] << std::endl;
		break;
	case 2:
		componentTestData[entityId] = new TestData;
		std::cout << "new [entity " << entityId << "][TestCompArray] = " << componentTestData[entityId] << std::endl;
		break;
	default:
		break;
	}
}
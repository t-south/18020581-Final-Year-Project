#pragma once
#include "../Memory/PoolAllocator.h"
#include <vector>
#include <ge_engine/Entity.h>
#include <ge_engine/Components.h>
#include <iostream>

//data driven aproach. An entity will contain components. Each component will have its own id and be stored in a memory pool of each 
//type of component. The entity ID will be used to access the index of each component to get each entities components.
namespace geProject {
	class EntityManager {
	public:
		EntityManager();
		~EntityManager();
		unsigned int addEntity();
		void assignComponent(unsigned int entityId, unsigned int componentId);
		void deleteComponent(unsigned int entityId, unsigned int componentId);
		

	private:
		unsigned int maxEntities;
		unsigned int maxComponents;
		Transform* componentTransforms[32]{nullptr};
		TestData* componentTestData[32]{ nullptr };
		std::vector<Entity> entities;
		
		void addComponent(unsigned int entityId, unsigned int componentId);
	};
}
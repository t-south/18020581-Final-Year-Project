#pragma once
#include <vector>
#include <ge_engine/Entity.h>
#include <ge_engine/Components.h>
#include <iostream>
#include "../Memory/PoolAllocator.h"
#include <imgui.h>

//data driven aproach. An entity will contain components. Each component will have its own id and be stored in a memory pool of each 
//type of component. The entity ID will be used to access the index of each component to get each entities components.
namespace geProject {
	class EntityManager {
		typedef unsigned int uInt;
	public:
		EntityManager(uInt maxEntities);
		~EntityManager();
		//Entities
		uInt addEntity();
		uInt getEntityNum();
		Entity* getEntity(uInt entityId);
		//Components
		void assignTransform(uInt entityId, Transform transform);
		void assignSpriteRender(uInt entityId, SpriteRender sprite);
		void assignRigidBody(uInt entityId, Rigidbody rBody);
		void assignFontRender(uInt entityId, FontRender font);
		void deleteComponent(uInt entityId, uInt componentId);
		std::vector<Transform*> getTransformComponents();
		std::vector<SpriteRender*> getSpriteComponents();
		std::vector <Rigidbody*> getRigidBodyComponents();
		std::vector<FontRender*> getFontRenderComponents();		
		Transform* getTransformComponent(uInt entityId);
		SpriteRender* getSpriteComponent(uInt entityId);
		Rigidbody* getRigidBodyComponent(uInt entityId);
		FontRender* getFontRenderComponent(uInt entityId);
		void updateImgui(uInt entityId);
		bool hasUpdate();
		void endFrame();
	private:		
		bool entityUpdated;
		uInt maxEntities;
		std::vector<Transform*> componentTransforms;
		std::vector <SpriteRender*> componentSpriteRender;
		std::vector <Rigidbody*> componentRigidBody;
		std::vector <FontRender*> componentFontRender;
		std::vector<Entity> entities;	
	};
}
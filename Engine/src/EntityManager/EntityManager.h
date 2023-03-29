#pragma once

#include <ge_engine/Core.h>
#include <vector>
#include <ge_engine/Entity.h>
#include <ge_engine/Components.h>
#include <iostream>
#include "../Memory/PoolAllocator.h"
#include <imgui.h>

//data driven aproach. An entity will contain components. Each component will have its own id and be stored in a memory pool of each 
//type of component. The entity ID will be used to access the index of each component to get each entities components.
namespace geProject {
	enum ComponentType {	
		TransformType = (1 << 0),		/* 0b0000000000000001 */
		SpriteType = (1 << 1),			/* 0b0000000000000010 */
		RigidType  = (1 << 2),			/* 0b0000000000000100 */
		CircleColliderType = (1 << 3),	/* 0b0000000000001000 */
		BoxColliderType = (1 << 4),		/* 0b0000000000010000 */
		AnimationType = (1 << 5),		/* 0b0000000000100000 */
	};


	class EntityManager {
		typedef unsigned int uInt;
	public:
		EntityManager(uInt maxEntities);
		~EntityManager();
		//Entities
		int addEntity(entityTypes type);
		void deleteEntity(int entityId);
		void copyEntity(int entityId);
		uInt getEntityNum();
		Entity* getEntity(int entityId);
		std::vector<Entity*> getEntities();
		//Components
		void assignTransform(int entityId, Transform transform);
		void assignSpriteRender(int entityId, SpriteRender sprite);
		void assignRigidBody(int entityId, Rigidbody rBody);
		void assignBoxCollider(int entityId, BoxCollider box);
		void assignCircleCollider(int entityId, CircleCollider circle);
		void assignAnimation(int entityId, Animation animate);

		void deleteComponent(int entityId, uInt componentId);
		std::vector<Transform*> getTransformComponents();
		std::vector<SpriteRender*> getSpriteComponents();
		std::vector <Rigidbody*> getRigidBodyComponents();	
		std::vector<Animation*> getAnimationComponents();	


		Transform* getTransformComponent(int entityId);
		SpriteRender* getSpriteComponent(int entityId);
		Rigidbody* getRigidBodyComponent(int entityId);
		std::vector<BoxCollider> getBoxColliderComponents(int entityId);
		std::vector<CircleCollider> getCircleColliderComponents(int entityId);
		Animation* getAnimationComponent(int entityId);

		void updateImgui(int entityId);
		bool hasUpdate();
		void endFrame();
		void reloadManager();
		void assignUpdate();
	private:		
		bool entityUpdated;
		uInt maxEntities;
		std::unordered_map<int, std::vector <BoxCollider>> componentBoxCollider;
		std::unordered_map<int, std::vector <CircleCollider>> componentCircleCollider;
		int playerId, entitiesDeleted;
		//glm::vec2 getCentre(glm::vec2 bLeft, glm::vec2 tRight);
		void updateTransform(TransformEvent* event);
		void updateSprite(SpriteEvent* event);
		void updateRigidBody(RigidEvent* event);
		void updateBoxCollider(BoxColliderEvent* event);
		void updateCircleCollider(CircleColliderEvent* event);
		bool popup{ false };
		bool textEdited{ false };

		std::vector<Entity*> entities;		
		std::vector<Transform*> componentTransforms;
		std::vector <SpriteRender*> componentSpriteRender;
		std::vector <Animation*> componentAnimation;
		std::vector <Rigidbody*> componentRigidBody;

		PoolAllocator entitypool{ maxEntities, sizeof(Entity) };
		PoolAllocator transformpool{ maxEntities, sizeof(Transform) };
		PoolAllocator spritepool{ maxEntities, sizeof(SpriteRender) };
		PoolAllocator animatePool{ maxEntities, sizeof(Animation) };
		PoolAllocator rigidpool{ maxEntities, sizeof(Rigidbody) };
	};
}
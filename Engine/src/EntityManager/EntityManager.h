#pragma once
#include <ge_engine/Entity.h>
#include <ge_engine/Components.h>
#include <ge_engine/Core.h>
#include <vector>
#include <iostream>
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
		EntityManager(/*uInt max*/);
		~EntityManager();
		//Entities
		void startUp();
		void addParentEntity(int parentId, int childId);
		int addEntity(entityTypes type);
		void deleteEntity(int entityId);
		void copyEntity(int entityId);
		int getEntityNum();

		std::vector<Entity*> getEntities();
		//Components
		void assignTransform(int entityId, Transform transform);
		void assignSpriteRender(int entityId, SpriteRender sprite);
		void assignRigidBody(int entityId, Rigidbody rBody);
		void assignBoxCollider(int entityId, BoxCollider box);
		void assignCircleCollider(int entityId, CircleCollider circle);
		void assignAnimation(int entityId, Animation animate);
		void assignController(int entityId, Controls control);
		void assignHealth(int entityId, Health health);
		void assignDamage(int entityId, Damage dmg);
		void assignView(int entityId, ViewCollider view);
		void assignAgent(int entityId, Agent agent);
		void deleteComponent(int entityId, uInt componentId);

		//std::vector<Transform*> getTransformComponents();
		//std::vector<SpriteRender*> getSpriteComponents();
		//std::vector <Rigidbody*> getRigidBodyComponents();	
		//std::vector<Animation*> getAnimationComponents();	
		//std::vector <Controls*> getControllerComponents();
		//std::vector<Health*> getHealthComponents();
		//std::vector <Damage*> getDamageComponents();
		//std::vector <ViewCollider*> getViewComponents();

		int getUpdateStatus(int entityId);
		int getBatchStatus(int entityId);
		int getVertexStatus(int entityId);
		int getZindex(int entityId);

		void updateTransform(int entityId, float x, float y, float rotate);
		void updateAnimationState(int entityId, std::string state);
		Entity getEntity(int entityId);
		Transform getTransformComponent(int entityId);
		SpriteRender getSpriteComponent(int entityId);
		Rigidbody getRigidBodyComponent(int entityId);
		Animation getAnimationComponent(int entityId);
		Controls getControllerComponent(int entityId);
		Health getHealthComponent(int entityId);
		Damage getDamageComponent(int entityId);
		ViewCollider getViewComponent(int entityId);
		Agent getAgentComponent(int entityId);
		std::vector<CircleCollider> getCircleColliderComponents(int entityId);
		std::vector<BoxCollider> getBoxColliderComponents(int entityId);

		void updateImgui(int entityId);
		bool hasUpdate();
		void endFrame();
		void reloadManager();
		void assignUpdate();
		void updateDirtyFlags(int entityId, int update, int batch, int vertex);
		int getPlayerId();
		void updateAgentState(int entityId, int newState);
		int getAgentState(int entityId);
		std::vector<int> getEnemyIds();
	private:		
		bool entityUpdated{ false };
		uInt maxEntities{ 10000 };

		int playerId{ -1 };
		std::vector<int> enemyIds;
		int entitiesDeleted{ 0 };


		
		void updateSprite(SpriteEvent* event);
		void updateRigidBody(RigidEvent* event);
		void updateBoxCollider(BoxColliderEvent* event);
		void updateCircleCollider(CircleColliderEvent* event);



		//BOX2D CALLBACKS
		void BeginContact(BeginContactEvent* event);
		void EndContact(EndContactEvent* event);
		void PreSolve(PresolveEvent* event);
		void PostSolve(PostsolveEvent* event);

		bool popup{ false };
		bool textEdited{ false };

		std::vector<Entity*> entities;		
		std::vector<Transform*> componentTransforms;
		std::vector <SpriteRender*> componentSpriteRender;
		std::vector <Animation*> componentAnimation;
		std::vector <Rigidbody*> componentRigidBody;
		std::vector <Controls*> componentController;
		std::vector <Health*> componentHealth;
		std::vector <Damage*> componentDamage;
		std::vector <ViewCollider*> componentView;
		std::vector <Agent*> componentAgent;

		PoolAllocator entitypool{ maxEntities, sizeof(Entity) };
		PoolAllocator transformpool{ maxEntities, sizeof(Transform) };
		PoolAllocator spritepool{ maxEntities, sizeof(SpriteRender) };
		PoolAllocator animatePool{ maxEntities, sizeof(Animation) };
		PoolAllocator rigidpool{ maxEntities, sizeof(Rigidbody) };
		PoolAllocator controllerpool{ maxEntities, sizeof(Controls) };
		PoolAllocator healthpool{ maxEntities, sizeof(Health) };
		PoolAllocator damagepool{ maxEntities, sizeof(Damage) };
		PoolAllocator viewpool{ maxEntities, sizeof(ViewCollider) };
		PoolAllocator agentpool{ maxEntities, sizeof(Agent) };

		//stored in unordered maps instead of pool allocators, due to varying memory amounts per component
		std::unordered_map<int, std::vector <BoxCollider>> componentBoxCollider;
		std::unordered_map<int, std::vector <CircleCollider>> componentCircleCollider;
	};
}
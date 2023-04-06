#include "EntityManager.h"



geProject::EntityManager::EntityManager() {}
geProject::EntityManager::~EntityManager(){}

void geProject::EntityManager::startUp()
{
	eventSystem.subscribe(this, &EntityManager::updateTransform);
	eventSystem.subscribe(this, &EntityManager::updateSprite);
	eventSystem.subscribe(this, &EntityManager::updateRigidBody);
	eventSystem.subscribe(this, &EntityManager::updateBoxCollider);
	eventSystem.subscribe(this, &EntityManager::updateCircleCollider);
	//BOX2D CALLBACKS
	eventSystem.subscribe(this, &EntityManager::BeginContact);
	eventSystem.subscribe(this, &EntityManager::EndContact);
	eventSystem.subscribe(this, &EntityManager::PreSolve);
	eventSystem.subscribe(this, &EntityManager::PostSolve);
}

int geProject::EntityManager::addEntity(entityTypes type) {
	Entity entity = Entity();
	entity.compMask = 0;
	entity.type = type;
	//check if a previously deleted pool is available
	if (entitiesDeleted > 0 && entities.size() > 0) {
		int index = 0;
		while (entities[index]->id > -1) {
			index++;
		}
		if (entities[index]->id == -1) {
			entity.id = index;
			std::memcpy(entities[index], &entity, sizeof(Entity));
			entitiesDeleted--;
			if (type == 0) {
				playerId = index;
			}
			else if (type == 1) {
				enemyIds.push_back(index);
			}
			return index;
		}
	}
	else {
		//initialise all the pools for that entity
		int index = entities.size();
		entities.push_back(reinterpret_cast<Entity*>(entitypool.allocate(sizeof(Entity))));
		entity.id = index;
		componentTransforms.push_back(reinterpret_cast<Transform*>(transformpool.allocate(sizeof(Transform))));
		componentTransforms[index]->id = 0;
		componentAnimation.push_back(reinterpret_cast<Animation*>(animatePool.allocate(sizeof(Animation))));
		componentAnimation[index]->id = 0;
		componentSpriteRender.push_back(reinterpret_cast<SpriteRender*>(spritepool.allocate(sizeof(SpriteRender))));
		componentSpriteRender[index]->id = 0;
		componentRigidBody.push_back(reinterpret_cast<Rigidbody*>(rigidpool.allocate(sizeof(Rigidbody))));
		componentRigidBody[index]->id = 0;
		componentController.push_back(reinterpret_cast<Controls*>(controllerpool.allocate(sizeof(Controls))));
		componentController[index]->id = 0;
		componentHealth.push_back(reinterpret_cast<Health*>(healthpool.allocate(sizeof(Health))));
 

		componentHealth[index]->id = 0;
		componentDamage.push_back(reinterpret_cast<Damage*>(damagepool.allocate(sizeof(Damage))));
		componentDamage[index]->id = 0;
		std::memcpy(entities[index], &entity, sizeof(Entity));
		if (type == 0) {
			playerId = index;
		}
		else if (type == 1) {
			enemyIds.push_back(index);
		}
		return index;
	}
	return -1;
}


void geProject::EntityManager::deleteEntity(int entityId){
	entities[entityId]->id = -1;
	entities[entityId]->compMask = 0;
	componentTransforms[entityId]->id = 0;
	componentSpriteRender[entityId]->id = 0;
	componentRigidBody[entityId]->id = 0;
	componentCircleCollider[entityId].clear();
	componentBoxCollider[entityId].clear();	
	componentController[entityId] = 0;
	componentHealth[entityId] = 0;
	componentDamage[entityId] = 0;

	entitiesDeleted++;	
}

void geProject::EntityManager::copyEntity(int entityId){
	unsigned int mask = entities[entityId]->compMask;
	int newEntityId = addEntity(entities[entityId]->type);
	Transform newtransform = getTransformComponent(entityId);	
	newtransform.id = 0;
	newtransform.position += 0.1f;
	assignTransform(newEntityId, newtransform);
	assignSpriteRender(newEntityId, getSpriteComponent(entityId));
	for (int i = 4; i <= 64; i = i << 1) {//powers of 2, optional components onwards
		int compmask = i & mask;
		switch (compmask) {
		case 4:
			assignRigidBody(newEntityId, getRigidBodyComponent(entityId));
			break;
		case 8:
			for (auto& circle : componentCircleCollider[entityId]) {
				assignCircleCollider(newEntityId, circle);
			}			
			break;
		case 16:
			for (auto& box : componentBoxCollider[entityId]) {
				assignBoxCollider(newEntityId, box);
			}
			
			break;
		case 32:
			assignAnimation(newEntityId, getAnimationComponent(entityId));
			break;
		
		case 64:
			assignController(newEntityId, getControllerComponent(entityId));
			break;

		case 128:
			assignHealth(newEntityId, getHealthComponent(entityId));
			break;

		case 256:
			assignDamage(newEntityId, getDamageComponent(entityId));
			break;

		default:
			break;
		}

	}
	eventSystem.publishImmediately(new CopyEntityEvent(EditorContext | ImGuiContext, newEntityId));
}

bool geProject::EntityManager::hasUpdate() {
	for (const auto& i : componentTransforms) {
		if (i->dirtyFlag[0] == 1) {
			return true;
		}
	};
	return false;
}

void geProject::EntityManager::assignTransform(int entityId, Transform transform) {

	if (entityId < maxEntities && entityId >= 0) {
		if (componentTransforms[entityId]->id > 0) {
			transform.dirtyFlag[1] = componentTransforms[entityId]->dirtyFlag[1];
			transform.dirtyFlag[2] = componentTransforms[entityId]->dirtyFlag[2];		
		}
		else if (componentTransforms[entityId]->id == 0) {
			transform.id = 1;
			transform.dirtyFlag[1] = 0;
			transform.dirtyFlag[2] = -1;
		}
		transform.dirtyFlag[0] = 1;
		std::memcpy(componentTransforms[entityId], &transform, sizeof(Transform));
		if (entities[entityId]->type == entityTypes::environment) {
			//make that position inaccessible on the map
			worldstate.addToMap((int)(transform.position[0] * 4), (int)(transform.position[1] * 4), 0);
		}
		entities[entityId]->compMask = entities[entityId]->compMask | transform.id;

	}
	else{
		std::cout << "unable to assign transform" << std::endl;
	}
	
}


void geProject::EntityManager::assignSpriteRender(int entityId, SpriteRender sprite) {
	if (entityId < maxEntities && entityId >= 0) {	
		sprite.entityId = entityId;	
		std::memcpy(componentSpriteRender[entityId], &sprite, sizeof(SpriteRender));	
		entities[entityId]->compMask = entities[entityId]->compMask | sprite.id;
		componentTransforms[entityId]->dirtyFlag[0] = 1;		
	}
	else {
		std::cout << "unable to assign sprite" << std::endl;
	}
}


void geProject::EntityManager::assignRigidBody(int entityId, Rigidbody rbody) {
	if (entityId < maxEntities && entityId >= 0) {
		std::memcpy(componentRigidBody[entityId], &rbody, sizeof(Rigidbody));
		entities[entityId]->compMask = entities[entityId]->compMask | rbody.id;
		
	}
	else {
		std::cout << "unable to assign rigidbody" << std::endl;
	}

}

void geProject::EntityManager::assignCircleCollider(int entityId, CircleCollider circle) {
	if (entityId < maxEntities && entityId >= 0) {
		circle.entityAssigned = entityId;
		componentCircleCollider[entityId].push_back(circle);		
		if ((entities[entityId]->compMask & CircleColliderType) != CircleColliderType) {
			//no need to alter component mask if we already know that there is a circle component
			entities[entityId]->compMask = entities[entityId]->compMask | circle.id;
		}
	}
	else{
		std::cout << "unable to assign circle collider" << std::endl;
	}
}

void geProject::EntityManager::assignBoxCollider(int entityId, BoxCollider box) {
	if (entityId < maxEntities && entityId >= 0) {
		box.entityAssigned = entityId;	
		componentBoxCollider[entityId].push_back(box);
		if ((entities[entityId]->compMask & BoxColliderType) != BoxColliderType) {
			entities[entityId]->compMask = entities[entityId]->compMask | box.id;
		}				
	}
	else{
		std::cout << "unable to assign box collider" << std::endl;
	}
}


void geProject::EntityManager::assignAnimation(int entityId, Animation animate) {
	if (entityId < maxEntities && entityId >= 0) {
		std::memcpy(componentAnimation[entityId], &animate, sizeof(Animation));	
		entities[entityId]->compMask = entities[entityId]->compMask | animate.id;
	}
	else {
		std::cout << "unable to assign animation" << std::endl;
	}	
}

void geProject::EntityManager::assignController(int entityId, Controls control){
	if (entityId < maxEntities && entityId >= 0) {
		std::memcpy(componentController[entityId], &control, sizeof(Controls));
		entities[entityId]->compMask = entities[entityId]->compMask | control.id;
	}
	else {
		std::cout << "unable to assign controller" << std::endl;
	}

}

void geProject::EntityManager::assignHealth(int entityId, Health health){
	if (entityId < maxEntities && entityId >= 0) {
		std::memcpy(componentHealth[entityId], &health, sizeof(Health));
		entities[entityId]->compMask = entities[entityId]->compMask | health.id;
	}
	else {
		std::cout << "unable to assign health component" << std::endl;
	}
}

void geProject::EntityManager::assignDamage(int entityId, Damage dmg){
	if (entityId < maxEntities && entityId >= 0) {
		std::memcpy(componentDamage[entityId], &dmg, sizeof(Damage));
		entities[entityId]->compMask = entities[entityId]->compMask | dmg.id;
	}
	else {
		std::cout << "unable to assign damage component" << std::endl;
	}
}


void geProject::EntityManager::deleteComponent(int entityId, uInt componentId) {
	if(entityId < maxEntities && entityId >= 0){
		switch (componentId) {
		case 1:
			componentTransforms[entityId]->id = 0;
			break;
		case 2:
			componentSpriteRender[entityId]->id = 0;
			break;
		case 4:
			componentRigidBody[entityId]->id = 0;			
			break;
		case 8:
			componentCircleCollider[entityId].clear();		
			break;
		case 16:
			componentBoxCollider[entityId].clear();;
			break;
		case 32:
			componentAnimation[entityId]->id = 0;
			break;
		case 64:
			componentController[entityId]->id = 0;
			break;
		case 128:
			componentHealth[entityId]->id = 0;
			break;
		case 256:
			componentDamage[entityId]->id = 0;
			break;
		default:
			break;
		}
		entities[entityId]->compMask = entities[entityId]->compMask & ~componentId;
	
	}
}


std::vector<geProject::Transform*> geProject::EntityManager::getTransformComponents() {
	return componentTransforms;
}
std::vector<geProject::SpriteRender*> geProject::EntityManager::getSpriteComponents() {
	return componentSpriteRender;
}
std::vector<geProject::Rigidbody*> geProject::EntityManager::getRigidBodyComponents() {
	return componentRigidBody;
}
std::vector<geProject::Animation*> geProject::EntityManager::getAnimationComponents() {
	return componentAnimation;
}

std::vector<geProject::Controls*> geProject::EntityManager::getControllerComponents(){
	return componentController;
}

std::vector<geProject::Health*> geProject::EntityManager::getHealthComponents(){
	return componentHealth;
}

std::vector<geProject::Damage*> geProject::EntityManager::getDamageComponents(){
	return componentDamage;
}


int geProject::EntityManager::getUpdateStatus(int entityId) {
	return componentTransforms[entityId]->dirtyFlag[0];
}

int geProject::EntityManager::getBatchStatus(int entityId) {
	return componentTransforms[entityId]->dirtyFlag[1];
}

int geProject::EntityManager::getVertexStatus(int entityId) {
	return componentTransforms[entityId]->dirtyFlag[2];
}

int geProject::EntityManager::getZindex(int entityId){
	return componentSpriteRender[entityId]->zIndex;
}

geProject::Entity geProject::EntityManager::getEntity(int entityId) {	
	if (entities.size() > 0) {			
		return *entities[entityId];
	}
}



geProject::Transform geProject::EntityManager::getTransformComponent(int entityId) {
	return *componentTransforms[entityId];
}
geProject::SpriteRender geProject::EntityManager::getSpriteComponent(int entityId) {
	return *componentSpriteRender[entityId];
}
geProject::Rigidbody geProject::EntityManager::getRigidBodyComponent(int entityId) {
	return *componentRigidBody[entityId];
}
geProject::Animation geProject::EntityManager::getAnimationComponent(int entityId) {
	return *componentAnimation[entityId];
}

geProject::Controls geProject::EntityManager::getControllerComponent(int entityId){
	return *componentController[entityId];
}

geProject::Health geProject::EntityManager::getHealthComponent(int entityId){
	return *componentHealth[entityId];
}

geProject::Damage geProject::EntityManager::getDamageComponent(int entityId){
	return *componentDamage[entityId];
}

std::vector<geProject::CircleCollider> geProject::EntityManager::getCircleColliderComponents(int entityId) {
	return componentCircleCollider[entityId];
}
std::vector <geProject::BoxCollider> geProject::EntityManager::getBoxColliderComponents(int entityId) {
	return componentBoxCollider[entityId];
}


void geProject::EntityManager::reloadManager() {
	entitiesDeleted = 0;
	transformpool.reset();
	componentTransforms.clear();
	spritepool.reset();
	componentSpriteRender.clear();
	rigidpool.reset();
	componentRigidBody.clear();
	animatePool.reset();
	componentAnimation.clear();
	componentCircleCollider.clear();
	componentBoxCollider.clear();
	controllerpool.reset();
	componentController.clear();
	entities.clear();
}

void geProject::EntityManager::assignUpdate() {
	entityUpdated = true;
}

void geProject::EntityManager::updateDirtyFlags(int entityId ,int update, int batch, int vertex) {
	componentTransforms[entityId]->dirtyFlag = glm::vec3(update, batch, vertex);
}

//EVENT LISTENERS
void geProject::EntityManager::updateTransform(TransformEvent* event) {
	Transform* newTransform = componentTransforms[event->entityId];
	newTransform->position[0] = event->posX;
	newTransform->position[1] = event->posY;
	newTransform->rotation = event->rotation;
	//newTransform->centre = getCentre(newTransform->position, glm::vec2(newTransform->position[0] + (1 * newTransform->scale.x), newTransform->position[1] + (1 * newTransform->scale.y)));
	componentTransforms[event->entityId]->dirtyFlag[0] = 1;	
}

void geProject::EntityManager::updateSprite(SpriteEvent* event) {
	assignSpriteRender(event->entityId, *event->sprite);
}

void geProject::EntityManager::updateRigidBody(RigidEvent* event) {
	assignRigidBody(event->entityId, *event->rigidbody);	
}

void geProject::EntityManager::updateBoxCollider(BoxColliderEvent* event) {
	assignBoxCollider(event->entityId, BoxCollider{ .boxSize = {event->boxSizeX, event->boxSizeY} });	
}

void geProject::EntityManager::updateCircleCollider(CircleColliderEvent* event) {
	assignCircleCollider(event->entityId, CircleCollider{});	
}




//PHYSICS EVENT LISTENERS
void geProject::EntityManager::BeginContact(BeginContactEvent* event) {
	//std::cout << "begin contact" << std::endl;
	switch (event->entityA->type) {
	case entityTypes::player:
		break;

	case entityTypes::enemy:
		break;

	case entityTypes::environment:
		if (event->entityB->type == player) {
			componentSpriteRender[event->entityA->id]->color = glm::vec4(1, 0, 0, 1);
			componentTransforms[event->entityA->id]->dirtyFlag[0] = 1;
		}
		if (event->entityB->type == enemy) {
			componentSpriteRender[event->entityA->id]->color = glm::vec4(0, 1, 0, 1);
			componentTransforms[event->entityA->id]->dirtyFlag[0] = 1;
		}
		break;

	default:
		break;
	}


}

void geProject::EntityManager::EndContact(EndContactEvent* event){
	//std::cout << "end contact" << std::endl;
	switch (event->entityA->type) {
	case entityTypes::player:
		break;

	case entityTypes::enemy:
		break;

	case entityTypes::environment:
		if (event->entityB->type == player) {
			componentSpriteRender[event->entityA->id]->color = glm::vec4(1, 1, 1, 1);
			componentTransforms[event->entityA->id]->dirtyFlag[0] = 1;
		}
		if (event->entityB->type == enemy) {
			componentSpriteRender[event->entityA->id]->color = glm::vec4(1, 1, 1, 1);
			componentTransforms[event->entityA->id]->dirtyFlag[0] = 1;
		}
		break;

	default:
		break;
	}
	
}

void geProject::EntityManager::PreSolve(PresolveEvent* event){


}

void geProject::EntityManager::PostSolve(PostsolveEvent* event){
	

}

int geProject::EntityManager::getPlayerId(){
	return playerId;
}

std::vector<int> geProject::EntityManager::getEnemyIds(){
	return enemyIds;
}



int geProject::EntityManager::getEntityNum() {
	return entities.size();
}



std::vector<geProject::Entity*> geProject::EntityManager::getEntities() {
	return entities;
}

void geProject::EntityManager::endFrame() {
	int count = 0;
	for (auto const& i : entities) {
		if (i->id == -1) {
			eventSystem.publishImmediately(new HierarchyEvent(EditorContext | ImGuiContext, count, i->compMask, componentTransforms[count]->name, false));
		}
		else {
			eventSystem.publishImmediately(new HierarchyEvent(EditorContext | ImGuiContext, i->id, i->compMask, componentTransforms[i->id]->name, true));
		}
		count++;
	}	
}


//IMGUI
void geProject::EntityManager::updateImgui(int entityId) {	
	if (entityId > -1) {
		Entity entity = getEntity(entityId);
		SpriteRender* sprite = componentSpriteRender[entityId];
		Transform* trans = componentTransforms[entityId];
		static char newName[256];
		//TRANSFORM IMGUI updates
		float positionX = trans->position[0];
		float positionY = trans->position[1];
		bool transformUpdate = false;
		bool spriteUpdate = false;
		bool rigidUpdate = false;
		bool boxUpdate = false;
		bool circleUpdate = false;
		ImGui::Text(trans->name.c_str());

		if (ImGui::BeginTabBar("Editor Tabs"))
			if (ImGui::BeginTabItem("Shortcuts")) {
				if (ImGui::Button("Copy", ImVec2(100, 50))) {
					copyEntity(entity.id);
				}
				ImGui::SameLine();
				if (ImGui::Button("Delete", ImVec2(100, 50))) {
					deleteEntity(entityId);
					eventSystem.publishImmediately(new DeleteEntityEvent(EditorContext | ImGuiContext, entityId, trans->dirtyFlag[1], trans->dirtyFlag[2]));
				}
				ImGui::EndTabItem();
			}

		if (ImGui::BeginTabItem("Transforms")) {
			if (ImGui::InputText("Entity Name", newName, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll)) {
				trans->name = newName;
			}
			//change contexts in event handler to stop keyboard events while typing into input
			if (ImGui::IsItemActive()) {
				eventSystem.setContext(ImGuiContext);
			}
			else {
				memset(&newName[0], 0, sizeof(newName));				
			}

			if (ImGui::DragFloat("positonX", &positionX), 0.01f, 1.0f, "%.3f") {
				if (positionX != trans->position[0]) {
					trans->position[0] = positionX;
					transformUpdate = true;
					trans->dirtyFlag[0] = 1;
				}
			}
			if (ImGui::DragFloat("positonY", &positionY), 0.01f, 1.0f, "%.3f") {
				if (positionY != trans->position[1]) {
					trans->position[1] = positionY;
					transformUpdate = true;
					trans->dirtyFlag[0] = 1;
				}
			}
			float scaleX = trans->scale[0];
			float scaleY = trans->scale[1];
			if (ImGui::DragFloat("scaleX", &scaleX)) {
				if (scaleX != trans->scale[0]) {
					trans->scale[0] = scaleX;
					transformUpdate = true;
					trans->dirtyFlag[0] = 1;
				}
			}
			if (ImGui::DragFloat("scaleY", &scaleY)) {
				if (scaleY != trans->scale[1]) {
					trans->scale[1] = scaleY;
					transformUpdate = true;
					trans->dirtyFlag[0] = 1;
				}
			}

			int rotation = trans->rotation;
			if (ImGui::DragInt("rotation", &rotation)) {
				if (rotation >= 0) {
					rotation = rotation % 360;
				}
				else if (rotation < 0) {
					rotation = rotation % -360;
				}
				if (rotation != trans->rotation) {
					trans->rotation = rotation;
					transformUpdate = true;
					trans->dirtyFlag[0] = 1;
				}
			}
			if (transformUpdate) {
				eventSystem.publishImmediately(new TransformEvent(EditorContext | ImGuiContext, entityId, trans->position[0], trans->position[1], trans->rotation));
			}
			ImGui::EndTabItem();
		}
		//SPRITE IMGUI updates
		if (ImGui::BeginTabItem("Sprite")) {
			int zIndex = sprite->zIndex;
			if (ImGui::InputInt("zIndex", &zIndex, 1, 0,1)) {
				if (zIndex != sprite->zIndex) {
					sprite->zIndex = zIndex;
					trans->dirtyFlag[0] = 1;
					spriteUpdate = true;
				}
			}

			static ImVec4 color = ImVec4(sprite->color[0], sprite->color[1], sprite->color[2], sprite->color[3]);
			ImGui::Text("MY CUSTOM COLOR PICKER WITH AN AMAZING PALETTE!");
			if (ImGui::ColorPicker4("##picker", (float*)&color)) {
				sprite->color[0] = color.x;
				sprite->color[1] = color.y;
				sprite->color[2] = color.z;
				sprite->color[3] = color.w;
				trans->dirtyFlag[0] = 1;
				spriteUpdate = true;
			}
			ImGui::EndTabItem();
		}
	
		//ANIMATION IMGUI UPDATES

		if ((entity.compMask & 32) == 32) {
			if (ImGui::BeginTabItem("Animation")) {
				bool loop = componentAnimation[entityId]->loopAnimation;
				float speed = componentAnimation[entityId]->speed;
				std::string state = componentAnimation[entityId]->state;


				if (ImGui::DragFloat("Animation Speed", &speed, 0.01f, 0, 1, "%.3f", 1)) {
					if (componentAnimation[entityId]->speed != speed) {
						componentAnimation[entityId]->speed = speed;
					}
				}

				ImGui::EndTabItem();
			}
		}
		ImGui::EndTabBar();
		
		
		//RIGIDBODY IMGUI updates
		if ((entity.compMask & 4) == 4) {
			if (ImGui::BeginTabBar("Editor Tabs")) {
				if (ImGui::BeginTabItem("RigidBody")) {
					auto rigid = getRigidBodyComponent(entityId);
					int collider = rigid.collider;
					float friction = rigid.friction;
					float velocityX = rigid.velocity[0];
					float velocityY = rigid.velocity[1];
					float velocityZ = rigid.velocity[2];
					float aDamp = rigid.angularDamping;
					float lDamp = rigid.linearDamping;
					float density = rigid.density;
					bool fRotate = rigid.fixedRotate;
					bool bullet = rigid.bullet;
					int bType = rigid.bodyType;

					if (ImGui::InputInt("body type", &bType)) {
						if (bType > 2) {
							bType--;
						}
						else if (bType < 0) {
							bType++;
						}
						rigid.bodyType = bType;
						rigidUpdate = true;
					}
					if (ImGui::DragInt("rigidBody", &collider)) {
						rigid.collider = collider;
						rigidUpdate = true;
					}
					if (ImGui::DragFloat("friction", &friction)) {
						rigid.friction = friction;
						rigidUpdate = true;
					}
					if (ImGui::DragFloat("angular damping", &aDamp)) {
						rigid.angularDamping = aDamp;
						rigidUpdate = true;
					}
					if (ImGui::DragFloat("linear damping", &lDamp)) {
						rigid.linearDamping = lDamp;
						rigidUpdate = true;
					}
					if (ImGui::InputFloat("density", &density)) {
						rigid.density = density;
						rigidUpdate = true;
					}
					if (ImGui::Checkbox("fixed rotate", &fRotate)) {
						rigid.fixedRotate = fRotate;
						rigidUpdate = true;
					}
					if (ImGui::Checkbox("bullet", &bullet)) {
						rigid.bullet = bullet;
						rigidUpdate = true;
					}

					if (ImGui::DragFloat("velocityX", &velocityX)) {
						rigid.velocity[0] = velocityX;
						rigidUpdate = true;
					}
					if (ImGui::DragFloat("velocityY", &velocityY)) {
						rigid.velocity[1] = velocityY;
						rigidUpdate = true;
					}
					if (ImGui::DragFloat("velocityZ", &velocityZ)) {
						rigid.velocity[2] = velocityZ;
						rigidUpdate = true;
					}
					if (ImGui::Button("Delete Rigidbody")) {
						deleteComponent(entityId, 4);
					}
					if (rigidUpdate) {
						eventSystem.publishImmediately(new RigidEvent(EditorContext | ImGuiContext, entityId, rigid));
					}
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			
		}
		

		//CIRCLE COLLIDER IMGUI updates
		if ((entity.compMask & 8) == 8) {
			if (ImGui::BeginTabBar("Editor Tabs")) {
				if (ImGui::BeginTabItem("Circle Colliders")) {
					int count = 0;
					for (auto& circleCollider : componentCircleCollider[entityId]) {
						ImGui::PushID(count);
						if (ImGui::CollapsingHeader("Circle Collider")) {
							float radius = circleCollider.radius;
							float oX = circleCollider.offset[0];
							float oY = circleCollider.offset[1];
							bool sensorCheck = circleCollider.sensor;
							if (ImGui::InputFloat("radius", &radius, 0.01f, 0.0f, "%.3f", 1)) {
								circleCollider.radius = radius;
								if (circleCollider.radius != radius) {
									circleUpdate = true;
								}

							}
							if (ImGui::InputFloat("offsetX", &oX, 0.01f, 0.0f, "%.3f", 1)) {
								circleCollider.offset[0] = oX;
								circleUpdate = true;
							}
							if (ImGui::InputFloat("offsetY", &oY, 0.01f, 0.0f, "%.3f", 1)) {
								circleCollider.offset[1] = oY;
								circleUpdate = true;
							}

							if (ImGui::Checkbox("sensor", &sensorCheck)) {
								circleCollider.sensor = sensorCheck;								
							}

							if (ImGui::Button("Delete Circle Collider")) {
								deleteComponent(entityId, 8);
							}
							//if (circleUpdate) {
							//	eventSystem.publishImmediately(new CircleColliderEvent(entityId, circleCollider));
							//}					
						}		
						ImGui::PopID();
						count++;
					}
					
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}

		//BOX COLLIDER IMGUI updates
		if ((entity.compMask & 16) == 16) {
			if (ImGui::BeginTabBar("Editor Tabs")) {

				if (ImGui::BeginTabItem("Box Colliders")) {
					int count = 0;
					for (auto& boxCollider : componentBoxCollider[entityId]) {
						ImGui::PushID(count);
						if (ImGui::CollapsingHeader("Box Collider")) {
							float boxSizeX = boxCollider.boxSize[0];
							float boxSizeY = boxCollider.boxSize[1];
							float offsetX = boxCollider.offset[0];
							float offsetY = boxCollider.offset[1];
							bool sensorCheck = boxCollider.sensor;
							if (ImGui::InputFloat("boxSizeX", &boxSizeX, 0.01f, 0.0f, "%.3f", 1)) {
								boxCollider.boxSize[0] = boxSizeX;
								if (boxSizeX != boxCollider.boxSize[0]) {
									boxUpdate = true;
								}

							}
							if (ImGui::InputFloat("boxSizeY", &boxSizeY, 0.01f, 0.0f, "%.3f", 1)) {
								boxCollider.boxSize[1] = boxSizeY;
								if (boxSizeY != boxCollider.boxSize[1]) {
									boxUpdate = true;
								}

							}

							if (ImGui::InputFloat("offsetX", &offsetX, 0.01f, 0.0f, "%.3f", 1)) {
								boxCollider.offset[0] = offsetX;
								if (offsetX != boxCollider.offset[0]) {
									boxUpdate = true;
								}

							}

							if (ImGui::InputFloat("offsetY", &offsetY, 0.01f, 0.0f, "%.3f", 1)) {
								boxCollider.offset[1] = offsetY;
								if (offsetY != boxCollider.offset[1]) {
									boxUpdate = true;
								}

							}
							if (ImGui::Checkbox("sensor", &sensorCheck)) {
								boxCollider.sensor = sensorCheck;						

							}

							if (ImGui::Button("Delete Box Collider")) {
								deleteComponent(entityId, 16);
							}
							
						}
						if (boxUpdate) {
							eventSystem.publishImmediately(new BoxColliderEvent(EditorContext | ImGuiContext, entityId, boxCollider.offset[0], boxCollider.offset[1],
							boxCollider.boxSize[0], boxCollider.boxSize[1], boxCollider.origin[0], boxCollider.origin[1]));
						}						
						ImGui::PopID();
						count++;
						
					}
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
		}
		componentTransforms[entityId]->dirtyFlag[0] = transformUpdate || spriteUpdate || rigidUpdate || boxUpdate || circleUpdate;


		if (ImGui::BeginPopupContextWindow("UniqueId")) {
			if ((entity.compMask & 4) != 4) {
				if (ImGui::MenuItem("Add Rigidbody")) {
					assignRigidBody(entityId, Rigidbody());
				}
			}			
			if (ImGui::MenuItem("Add CircleCollider")) {
				assignCircleCollider(entityId, CircleCollider());
			}
			
			if (ImGui::MenuItem("Add BoxCollider")) {
				assignBoxCollider(entityId, BoxCollider());
			}
			
			ImGui::EndPopup();
		}

		


	}


	
}
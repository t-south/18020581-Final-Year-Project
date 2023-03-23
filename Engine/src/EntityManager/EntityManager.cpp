#include "EntityManager.h"

//PoolAllocator geProject::Transform::pool{ 1000000, sizeof(Transform) };
//PoolAllocator geProject::SpriteRender::pool{ 1000000, sizeof(SpriteRender) };
//PoolAllocator geProject::FontRender::pool{ 1000000, sizeof(FontRender) };

geProject::EntityManager::EntityManager(uInt maxEntities): maxEntities(maxEntities), entityUpdated(false){
	PoolAllocator transformpool{ maxEntities, sizeof(Transform) };
	PoolAllocator spritepool{ maxEntities, sizeof(SpriteRender) };
	PoolAllocator fontpool{ maxEntities, sizeof(FontRender) };
	PoolAllocator rigidpool{ maxEntities, sizeof(Rigidbody) };
	PoolAllocator circlePool{ maxEntities, sizeof(CircleCollider) };
	PoolAllocator boxPool{ maxEntities, sizeof(BoxCollider) };
	for (int i = 0; i < maxEntities - 1; ++i) {
		componentTransforms.push_back(reinterpret_cast<Transform*>(transformpool.allocate(sizeof(Transform))));
		componentTransforms[i]->id = 0;
		componentFontRender.push_back(reinterpret_cast<FontRender*>(fontpool.allocate(sizeof(FontRender))));
		componentFontRender[i]->id = 0;
		componentSpriteRender.push_back(reinterpret_cast<SpriteRender*>(spritepool.allocate(sizeof(SpriteRender))));
		componentSpriteRender[i]->id = 0;
		componentRigidBody.push_back(reinterpret_cast<Rigidbody*>(rigidpool.allocate(sizeof(Rigidbody))));
		componentRigidBody[i]->id = 0;	
		componentCircleCollider.push_back(reinterpret_cast<CircleCollider*>(circlePool.allocate(sizeof(CircleCollider))));
		componentCircleCollider[i]->id = 0;
		componentBoxCollider.push_back(reinterpret_cast<BoxCollider*>(boxPool.allocate(sizeof(BoxCollider))));
		componentBoxCollider[i]->id = 0;
	}

	eventSystem.subscribe(this, &EntityManager::updateTransform);
	eventSystem.subscribe(this, &EntityManager::updateSprite);
	eventSystem.subscribe(this, &EntityManager::updateRigidBody);
	eventSystem.subscribe(this, &EntityManager::updateBoxCollider);
	eventSystem.subscribe(this, &EntityManager::updateCircleCollider);
}

geProject::EntityManager::~EntityManager(){}

unsigned int geProject::EntityManager::addEntity() {
	//add to component vector if new component
	Entity entity = Entity();
	entity.id = entities.size();
	entity.compMask = 0;
	entities.push_back(entity);
	entityUpdated = true;
	return entities.back().id;
}

bool geProject::EntityManager::hasUpdate() {
	return entityUpdated;
}

void geProject::EntityManager::assignTransform(uInt entityId, Transform transform) {
	if (entityId < maxEntities && entityId >= 0) {
		if (componentTransforms[entityId]->id > 0) {
			transform.dirtyFlag[1] = componentTransforms[entityId]->dirtyFlag[1];
			transform.dirtyFlag[2] = componentTransforms[entityId]->dirtyFlag[2];		
		}
		else if (componentTransforms[entityId]->id == 0) {
			transform.dirtyFlag[1] = 0;
			transform.dirtyFlag[2] = -1;
		}
		transform.dirtyFlag[0] = 1;
		std::memcpy(componentTransforms[entityId], &transform, sizeof(Transform));	
		auto box = componentTransforms[entityId];
		box->centre = getCentre(box->position, glm::vec2(box->position[0] + (1 * box->scale.x), box->position[1] + (1 * box->scale.y)));
		entities[entityId].compMask = entities[entityId].compMask | transform.id;
		entityUpdated = true;
	}
	else{
		std::cout << "unable to assign transform" << std::endl;
		delete(&transform);
	}
	
}


void geProject::EntityManager::assignSpriteRender(uInt entityId, SpriteRender sprite) {
	if (entityId < maxEntities && entityId >= 0) {	
		sprite.entityId = entityId;
		std::memcpy(componentSpriteRender[entityId], &sprite, sizeof(SpriteRender));	
		entities[entityId].compMask = entities[entityId].compMask | sprite.id;
		entityUpdated = true;
	}
	else {
		std::cout << "unable to assign sprite" << std::endl;
		delete(&sprite);
	}
}


void geProject::EntityManager::assignRigidBody(uInt entityId, Rigidbody rbody) {
	if (entityId < maxEntities && entityId >= 0) {
		std::memcpy(componentRigidBody[entityId], &rbody, sizeof(Rigidbody));
		entities[entityId].compMask = entities[entityId].compMask | rbody.id;
		entityUpdated = true;
	}
	else {
		std::cout << "unable to assign rigidbody" << std::endl;
		delete(&rbody);
	}

}

void geProject::EntityManager::assignCircleCollider(uInt entityId, CircleCollider circle) {
	if (entityId < maxEntities && entityId >= 0) {
		std::memcpy(componentCircleCollider[entityId], &circle, sizeof(CircleCollider));
		entities[entityId].compMask = entities[entityId].compMask | circle.id;
		entityUpdated = true;
	}
	else{
		std::cout << "unable to assign circle collider" << std::endl;
		delete(&circle);
	}
}

void geProject::EntityManager::assignBoxCollider(uInt entityId, BoxCollider box) {
	if (entityId < maxEntities && entityId >= 0) {
		std::memcpy(componentBoxCollider[entityId], &box, sizeof(BoxCollider));
		auto transform = componentTransforms[entityId];		
		entities[entityId].compMask = entities[entityId].compMask | box.id;
		entityUpdated = true;	
	}
	else{
		std::cout << "unable to assign box collider" << std::endl;
	}
}




void geProject::EntityManager::assignFontRender(uInt entityId, FontRender font) {
	if (entityId < maxEntities && entityId >= 0) {
		if (componentFontRender[entityId]->id == 0) {
			font.dirtyFlag[1] = 0;
			font.dirtyFlag[2] = 0;
		}
		else {
			font.dirtyFlag[1] = componentFontRender[entityId]->dirtyFlag[1];
			font.dirtyFlag[2] = componentFontRender[entityId]->dirtyFlag[2];
		}

		std::memcpy(componentFontRender[entityId], &font, sizeof(FontRender));
		componentFontRender[entityId]->dirtyFlag[0] = 1;
		entities[entityId].compMask = entities[entityId].compMask | font.id;
		entityUpdated = true;
	}
	else {
		std::cout << "unable to assign font" << std::endl;
		delete(&font);
	}
	
}







void geProject::EntityManager::deleteComponent(uInt entityId, uInt componentId) {
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
			componentCircleCollider[entityId]->id = 0;
			break;
		case 16:
			componentBoxCollider[entityId]->id = 0;
			break;
		case 32:
			componentFontRender[entityId]->id = 0;
			break;
		default:
			break;
		}
		entities[entityId].compMask = entities[entityId].compMask & ~componentId;
		entityUpdated = true;
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
std::vector<geProject::FontRender*> geProject::EntityManager::getFontRenderComponents() {
	return componentFontRender;
}
std::vector<geProject::CircleCollider*> geProject::EntityManager::getCircleColliderComponents() {
	return componentCircleCollider;
}
std::vector<geProject::BoxCollider*> geProject::EntityManager::getBoxColliderComponents() {
	return componentBoxCollider;
}






geProject::Transform* geProject::EntityManager::getTransformComponent(uInt entityId) {
	return componentTransforms[entityId];
}
geProject::SpriteRender* geProject::EntityManager::getSpriteComponent(uInt entityId) {
	return componentSpriteRender[entityId];
}
geProject::Rigidbody* geProject::EntityManager::getRigidBodyComponent(uInt entityId) {
	return componentRigidBody[entityId];
}
geProject::FontRender* geProject::EntityManager::getFontRenderComponent(uInt entityId) {
	return componentFontRender[entityId];
}

geProject::CircleCollider* geProject::EntityManager::getCircleColliderComponent(uInt entityId) {
	return componentCircleCollider[entityId];
}
geProject::BoxCollider* geProject::EntityManager::getBoxColliderComponent(uInt entityId) {
	return componentBoxCollider[entityId];
}


void geProject::EntityManager::reloadManager() {	
	for (int i = 0; i < entities.size(); i++) {
		componentTransforms[i]->id = 0;
		componentSpriteRender[i]->id = 0;
		componentRigidBody[i]->id = 0;
		componentFontRender[i]->id = 0;
		componentCircleCollider[i]->id = 0;
		componentBoxCollider[i]->id = 0;
	}
	entities.clear();
}

void geProject::EntityManager::assignUpdate(){
	entityUpdated = true;
}

glm::vec2 geProject::EntityManager::getCentre(glm::vec2 bLeft, glm::vec2 tRight){
	glm::vec2 centre{};
	centre[0] = (bLeft[0] + tRight[0]) / 2;
	centre[1] = (bLeft[1] + tRight[1]) / 2;
	return centre;
}


//EVENT LISTENERS
void geProject::EntityManager::updateTransform(TransformEvent* event){
	auto newTransform = componentTransforms[event->entityId];
	newTransform->position[0] = event->posX;
	newTransform->position[1] = event->posY;
	newTransform->rotation = event->rotation;
	newTransform->centre = getCentre(newTransform->position, glm::vec2(newTransform->position[0] + (1 * newTransform->scale.x), newTransform->position[1] + (1 * newTransform->scale.y)));
	entityUpdated = true;
	componentTransforms[event->entityId]->dirtyFlag[0] = 1;
}

void geProject::EntityManager::updateSprite(SpriteEvent* event){
	assignSpriteRender(event->entityId, *event->sprite);
}

void geProject::EntityManager::updateRigidBody(RigidEvent* event){
	assignRigidBody(event->entityId, *event->rigidbody);
}

void geProject::EntityManager::updateBoxCollider(BoxColliderEvent* event){
	assignBoxCollider(event->entityId, BoxCollider{ .boxSize = {event->boxSizeX, event->boxSizeY} });
}

void geProject::EntityManager::updateCircleCollider(CircleColliderEvent* event){
	assignCircleCollider(event->entityId, *event->circlecollider);
}



unsigned int geProject::EntityManager::getEntityNum() {
	return entities.size();
}

geProject::Entity* geProject::EntityManager::getEntity(uInt entityId) {
	if (entities.size() > 0) {
		return &entities[entityId];
	}
}

std::vector<geProject::Entity> geProject::EntityManager::getEntities(){
	return entities;
}

void geProject::EntityManager::endFrame() {
	entityUpdated = false;
}


//IMGUI
void geProject::EntityManager::updateImgui(uInt entityId) {
	auto entity = getEntity(entityId);
	auto sprite  = getSpriteComponent(entityId);
	auto trans = getTransformComponent(entityId);
	//TRANSFORM IMGUI updates
	float positionX = trans->position[0];
	float positionY = trans->position[1];
	bool transformUpdate = false;
	bool spriteUpdate = false;
	bool rigidUpdate = false;
	bool boxUpdate = false;
	bool circleUpdate = false;
	if (ImGui::CollapsingHeader("Transform Component", ImGuiTreeNodeFlags_DefaultOpen)) {
		
		if (ImGui::DragFloat("positonX", &positionX)) {
			if (positionX != trans->position[0]) {
				trans->position[0] = positionX;
				transformUpdate = true;
				trans->dirtyFlag[0] = 1;
			}
		}
		if (ImGui::DragFloat("positonY", &positionY)) {
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
		float centreX = trans->centre[0];
		float centreY = trans->centre[1];
		if (ImGui::DragFloat("centreX", &centreX)) {
			trans->centre[0] = centreX;
			transformUpdate = true;
		}
		if (ImGui::DragFloat("centreY", &centreY)) {
			trans->centre[1] = centreY;
			transformUpdate = true;
		}
		if (entityUpdated) {
			eventSystem.publishImmediately(new TransformEvent(entityId, trans->position[0], trans->position[1], trans->rotation));
		}
	}
	//SPRITE IMGUI updates
	if (ImGui::CollapsingHeader("Sprite Component")) {
		int zIndex = sprite->zIndex;
		if (ImGui::DragInt("zIndex", &zIndex, 1, 0, 10, "%d", 1)) {
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
	}

	//RIGIDBODY IMGUI updates
	if ((entity->compMask & 4) == 4) {		
		if (ImGui::CollapsingHeader("RigidBody component")) {
			auto rigid = getRigidBodyComponent(entityId);
			int collider = rigid->collider;
			float friction = rigid->friction;
			float velocityX = rigid->velocity[0];
			float velocityY = rigid->velocity[1];
			float velocityZ = rigid->velocity[2];
			float aDamp = rigid->angularDamping;
			float lDamp = rigid->linearDamping;
			float density = rigid->density;
			bool fRotate = rigid->fixedRotate;
			bool bullet = rigid->bullet;
			int bType = rigid->bodyType;
		
		
			if (ImGui::InputInt("body type", &bType)) {
				if (bType > 2) {
					bType--;
				}
				else if (bType < 0) {
					bType++;
				}
				rigid->bodyType = bType;
				rigidUpdate = true;
			}

		
			if (ImGui::DragInt("rigidBody", &collider)) {
				rigid->collider = collider;
				rigidUpdate = true;
			}
			if (ImGui::DragFloat("friction", &friction)) {
				rigid->friction = friction;
				rigidUpdate = true;
			}
			if (ImGui::DragFloat("angular damping", &aDamp)) {
				rigid->angularDamping = aDamp;
				rigidUpdate = true;
			}
			if (ImGui::DragFloat("linear damping", &lDamp)) {
				rigid->linearDamping = lDamp;
				rigidUpdate = true;
			}
			if (ImGui::InputFloat("density", &density)) {
				rigid->density = density;
				rigidUpdate = true;
			}
			if (ImGui::Checkbox("fixed rotate", &fRotate)) {
				rigid->fixedRotate = fRotate;
				rigidUpdate = true;
			}
			if (ImGui::Checkbox("bullet", &bullet)) {
				rigid->bullet = bullet;
				rigidUpdate = true;
			}

			if (ImGui::DragFloat("velocityX", &velocityX)) {
				rigid->velocity[0] = velocityX;
				rigidUpdate = true;
			}
			if (ImGui::DragFloat("velocityY", &velocityY)) {
				rigid->velocity[1] = velocityY;
				rigidUpdate = true;
			}
			if (ImGui::DragFloat("velocityZ", &velocityZ)) {
				rigid->velocity[2] = velocityZ;
				rigidUpdate = true;
			}
			if (ImGui::Button("Delete Rigidbody")) {
				deleteComponent(entityId, 4);
			}
			if (rigidUpdate) {
				eventSystem.publishImmediately(new RigidEvent(entityId, *rigid));			}

		}
	}

	//CIRCLE COLLIDER IMGUI updates
	if ((entity->compMask & 8) == 8) {		
		if (ImGui::CollapsingHeader("Circle Collider Component")) {
			auto circleCollider = getCircleColliderComponent(entityId);
			float radius = circleCollider->radius;
			float oX = circleCollider->offset[0];
			float oY = circleCollider->offset[1];
			if (ImGui::DragFloat("radius", &radius)) {
				circleCollider->radius = radius;
				circleUpdate = true;
			}
			if (ImGui::DragFloat("offsetX", &oX)) {
				circleCollider->offset[0] = oX;
				circleUpdate = true;
			}
			if (ImGui::DragFloat("offsetY", &oY)) {
				circleCollider->offset[1] = oY;
				circleUpdate = true;
			}
			if (ImGui::Button("Delete Circle Collider")) {
				deleteComponent(entityId, 8);
			}
			if (circleUpdate) {
				eventSystem.publishImmediately(new CircleColliderEvent(entityId, *circleCollider));
			}
		}



	}
	//BOX COLLIDER IMGUI updates
	if ((entity->compMask & 16) == 16) {		
		if (ImGui::CollapsingHeader("Box Collider Component")) {
			auto boxCollider = getBoxColliderComponent(entityId);
			float boxSizeX = boxCollider->boxSize[0];
			float boxSizeY = boxCollider->boxSize[1];

			if (ImGui::InputFloat("boxSizeX", &boxSizeX, 0.01f)) {
				boxCollider->boxSize[0] = boxSizeX;
				if (boxSizeX != boxCollider->boxSize[0]) {
					boxUpdate = true;
				}
				
			}
			
			if (ImGui::InputFloat("boxSizeY", &boxSizeY), 0.01f) {	
				boxCollider->boxSize[1] = boxSizeY;
				if (boxSizeY != boxCollider->boxSize[1]) {
					boxUpdate = true;
				}
				
			}

			if (ImGui::Button("Delete Box Collider")) {
				deleteComponent(entityId, 16);
			}
			if (boxUpdate) {
				eventSystem.publishImmediately(new BoxColliderEvent(entityId, boxCollider->offset[0], boxCollider->offset[1], 
					boxCollider->boxSize[0], boxCollider->boxSize[1], boxCollider->origin[0], boxCollider->origin[1]));
			}
		}
	}
	entityUpdated = transformUpdate || spriteUpdate || rigidUpdate || boxUpdate || circleUpdate;


	if (ImGui::BeginPopupContextWindow("UniqueId")) {
		if ((entity->compMask & 4) != 4) {
			if (ImGui::MenuItem("Add Rigidbody")) {			
				assignRigidBody(entityId, Rigidbody());
			}
		}
		if ((entity->compMask & 8) != 8 && (entity->compMask & 16) != 16) {
			if (ImGui::MenuItem("Add CircleCollider")) {						
				assignCircleCollider(entityId, CircleCollider());
			}
		}

		if ((entity->compMask & 8) != 8 && (entity->compMask & 16) != 16) {
			if (ImGui::MenuItem("Add BoxCollider")) {			
				assignBoxCollider(entityId, BoxCollider());
			}
		}
		ImGui::EndPopup();
	}





	
}
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
	std::cout << "Transform size : " << sizeof(Transform) << std::endl;
	std::cout << "SpriteRender size : " << sizeof(SpriteRender) << std::endl;
	std::cout << "FontRender size : " << sizeof(FontRender) << std::endl;
	std::cout << "Rigidbody size : " << sizeof(Rigidbody) << std::endl;
	std::cout << "CircleCollider size : " << sizeof(CircleCollider) << std::endl;
	std::cout << "BoxCollider size : " << sizeof(BoxCollider) << std::endl;
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
		entities[entityId].compMask = entities[entityId].compMask | box.id;
		entityUpdated = true;	
	}
	else{
		std::cout << "unable to assign box collider" << std::endl;
		delete(&box);
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



unsigned int geProject::EntityManager::getEntityNum() {
	return entities.size();
}

geProject::Entity* geProject::EntityManager::getEntity(uInt entityId) {
	return &entities[entityId];
}

void geProject::EntityManager::endFrame() {
	entityUpdated = false;
}

void geProject::EntityManager::updateImgui(uInt entityId) {
	auto entity = getEntity(entityId);
	auto sprite  = getSpriteComponent(entityId);
	auto transform = getTransformComponent(entityId);
	//TRANSFORM IMGUI updates
	float positionX = transform->position[0];
	float positionY = transform->position[1];

	if (ImGui::CollapsingHeader("Transform Component", ImGuiTreeNodeFlags_DefaultOpen)) {
		if (ImGui::DragFloat("positonX", &positionX)) {
			if (positionX != transform->position[0]) {
				transform->position[0] = positionX;
				entityUpdated = true;
				transform->dirtyFlag[0] = 1;
			}
		}

		if (ImGui::DragFloat("positonY", &positionY)) {
			if (positionY != transform->position[1]) {
				transform->position[1] = positionY;
				entityUpdated = true;
				transform->dirtyFlag[0] = 1;
			}
		}
		float scaleX = transform->scale[0];
		float scaleY = transform->scale[1];
		if (ImGui::DragFloat("scaleX", &scaleX)) {
			if (scaleX != transform->scale[0]) {
				transform->scale[0] = scaleX;
				entityUpdated = true;
				transform->dirtyFlag[0] = 1;
			}
		}
		if (ImGui::DragFloat("scaleY", &scaleY)) {
			if (scaleY != transform->scale[1]) {
				transform->scale[1] = scaleY;
				entityUpdated = true;
				transform->dirtyFlag[0] = 1;
			}
		}

		int rotation = transform->rotation;
		if (ImGui::DragInt("rotation", &rotation)) {
			if (rotation >= 0) {
				rotation = rotation % 360;
			}
			else if (rotation < 0) {
				rotation = rotation % -360;
			}
			if (rotation != transform->rotation) {
				transform->rotation = rotation;
				entityUpdated = true;
				transform->dirtyFlag[0] = 1;
			}
		}
	}
	//SPRITE IMGUI updates
	if (ImGui::CollapsingHeader("Sprite Component", ImGuiTreeNodeFlags_DefaultOpen)) {
		int zIndex = sprite->zIndex;
		if (ImGui::DragInt("zIndex", &zIndex, 1, 0, 10, "%d", 1)) {
			if (zIndex != sprite->zIndex) {
				sprite->zIndex = zIndex;
				transform->dirtyFlag[0] = 1;
				entityUpdated = true;
			}
		}

		static ImVec4 color = ImVec4(sprite->color[0], sprite->color[1], sprite->color[2], sprite->color[3]);
		ImGui::Text("MY CUSTOM COLOR PICKER WITH AN AMAZING PALETTE!");
		if (ImGui::ColorPicker4("##picker", (float*)&color)) {
			sprite->color[0] = color.x;
			sprite->color[1] = color.y;
			sprite->color[2] = color.z;
			sprite->color[3] = color.w;
			transform->dirtyFlag[0] = 1;
			entityUpdated = true;
		}
	}

	//RIGIDBODY IMGUI updates
	if ((entity->compMask & 4) == 4) {
		ImGui::Columns(2);
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
			if (ImGui::DragInt("rigidBody", &collider)) {
				rigid->collider = collider;
				entityUpdated = true;
			}
			if (ImGui::DragFloat("friction", &friction)) {
				rigid->friction = friction;
				entityUpdated = true;
			}
			if (ImGui::DragFloat("angular damping", &aDamp)) {
				rigid->angularDamping = aDamp;
				entityUpdated = true;
			}
			if (ImGui::DragFloat("linear damping", &lDamp)) {
				rigid->linearDamping = lDamp;
				entityUpdated = true;
			}
			if (ImGui::DragFloat("density", &density)) {
				rigid->density = density;
				entityUpdated = true;
			}
			if (ImGui::Checkbox("fixed rotate", &fRotate)) {
				rigid->fixedRotate = fRotate;
				entityUpdated = true;
			}
			if (ImGui::Checkbox("bullet", &bullet)) {
				rigid->bullet = bullet;
				entityUpdated = true;
			}
			if (ImGui::DragInt("bodyType", &bType, 1, 0, 3, "%d", 0)) {
				rigid->bodyType = bType;
				entityUpdated = true;
			}
			if (ImGui::DragFloat("velocityX", &velocityX)) {
				rigid->velocity[0] = velocityX;
				entityUpdated = true;
			}
			if (ImGui::DragFloat("velocityY", &velocityY)) {
				rigid->velocity[1] = velocityY;
				entityUpdated = true;
			}
			if (ImGui::DragFloat("velocityZ", &velocityZ)) {
				rigid->velocity[2] = velocityZ;
				entityUpdated = true;
			}
			if (ImGui::Button("Delete Rigidbody")) {
				deleteComponent(entityId, 4);
			}
		}
	}

	//CIRCLE COLLIDER IMGUI updates
	if ((entity->compMask & 8) == 8) {
		ImGui::NextColumn();
		if (ImGui::CollapsingHeader("Circle Collider Component")) {
			auto circleCollider = getCircleColliderComponent(entityId);
			float centreX = circleCollider->centre[0];
			float centreY = circleCollider->centre[1];
			float radius = circleCollider->radius;
			float oX = circleCollider->offset[0];
			float oY = circleCollider->offset[1];
			if (ImGui::DragFloat("centreX", &centreX)) {
				circleCollider->centre[0] = centreX;
				entityUpdated = true;
			}
			if (ImGui::DragFloat("centreY", &centreY)) {
				circleCollider->centre[1] = centreY;
				entityUpdated = true;
			}
			if (ImGui::DragFloat("radius", &radius)) {
				circleCollider->radius = radius;
				entityUpdated = true;
			}
			if (ImGui::DragFloat("offsetX", &oX)) {
				circleCollider->offset[0] = oX;
				entityUpdated = true;
			}
			if (ImGui::DragFloat("offsetY", &oY)) {
				circleCollider->offset[1] = oY;
				entityUpdated = true;
			}
			if (ImGui::Button("Delete Circle Collider")) {
				deleteComponent(entityId, 8);
			}
		}



	}
	//BOX COLLIDER IMGUI updates
	if ((entity->compMask & 16) == 16) {
		ImGui::NextColumn();
		if (ImGui::CollapsingHeader("Box Collider Component")) {
			auto boxCollider = getBoxColliderComponent(entityId);
			float centreX = boxCollider->centre[0];
			float centreY = boxCollider->centre[1];
			float oX = boxCollider->offset[0];
			float oY = boxCollider->offset[1];
			float originX = boxCollider->origin[0];
			float originY = boxCollider->origin[0];
			if (ImGui::DragFloat("centreX", &centreX)) {
				boxCollider->centre[0] = centreX;
				entityUpdated = true;
			}
			if (ImGui::DragFloat("centreY", &centreY)) {
				boxCollider->centre[1] = centreY;
				entityUpdated = true;
			}
			if (ImGui::DragFloat("offsetX", &oX)) {
				boxCollider->offset[0] = oX;
				entityUpdated = true;
			}
			if (ImGui::DragFloat("offsetY", &oY)) {
				boxCollider->offset[1] = oY;
				entityUpdated = true;
			}
			if (ImGui::DragFloat("originX", &originX)) {
				boxCollider->origin[0] = originX;
				entityUpdated = true;
			}
			if (ImGui::DragFloat("originY", &originY)) {
				boxCollider->origin[1] = originY;
				entityUpdated = true;
			}
			if (ImGui::Button("Delete Box Collider")) {
				deleteComponent(entityId, 16);
			}
		}
	}



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
#include "EntityManager.h"

//PoolAllocator geProject::Transform::pool{ 1000000, sizeof(Transform) };
//PoolAllocator geProject::SpriteRender::pool{ 1000000, sizeof(SpriteRender) };
//PoolAllocator geProject::FontRender::pool{ 1000000, sizeof(FontRender) };

geProject::EntityManager::EntityManager(uInt maxEntities): maxEntities(maxEntities), entityUpdated(false){
	PoolAllocator transformpool{ maxEntities, sizeof(Transform) };
	PoolAllocator spritepool{ maxEntities, sizeof(SpriteRender) };
	PoolAllocator fontpool{ maxEntities, sizeof(FontRender) };
	PoolAllocator rigidpool{ maxEntities, sizeof(Rigidbody) };
	for (int i = 0; i < maxEntities - 1; ++i) {
		componentTransforms.push_back(reinterpret_cast<Transform*>(transformpool.allocate(sizeof(Transform))));
		componentTransforms[i]->id = 0;
		componentFontRender.push_back(reinterpret_cast<FontRender*>(fontpool.allocate(sizeof(FontRender))));
		componentFontRender[i]->id = 0;
		componentSpriteRender.push_back(reinterpret_cast<SpriteRender*>(spritepool.allocate(sizeof(SpriteRender))));
		componentSpriteRender[i]->id = 0;
		componentRigidBody.push_back(reinterpret_cast<Rigidbody*>(rigidpool.allocate(sizeof(Rigidbody))));
		componentRigidBody[i]->id = 0;		
	}
	std::cout << "Transform size : " << sizeof(Transform) << std::endl;
	std::cout << "SpriteRender size : " << sizeof(SpriteRender) << std::endl;
	std::cout << "FontRender size : " << sizeof(FontRender) << std::endl;
	std::cout << "Rigidbody size : " << sizeof(Rigidbody) << std::endl;
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
		entityUpdated = true;
	}
	else{
		std::cout << "unable to assign transform" << std::endl;
		delete(&transform);
	}
	
}


void geProject::EntityManager::assignRigidBody(uInt entityId, Rigidbody rbody) {
	if (entityId < maxEntities && entityId >= 0) {
		if (componentRigidBody[entityId]->id == 0) {
			rbody.dirtyFlag[1] = 0;
			rbody.dirtyFlag[2] = 0;
		}
		else {
			rbody.dirtyFlag[1] = componentRigidBody[entityId]->dirtyFlag[1];
			rbody.dirtyFlag[2] = componentRigidBody[entityId]->dirtyFlag[2];
		}
		rbody.dirtyFlag[1] = componentRigidBody[entityId]->dirtyFlag[1];
		rbody.dirtyFlag[2] = componentRigidBody[entityId]->dirtyFlag[2];
		std::memcpy(componentRigidBody[entityId], &rbody, sizeof(Rigidbody));
		componentRigidBody[entityId]->dirtyFlag[0] = 1;
		entityUpdated = true;
	}
	else {
		std::cout << "unable to assign rigidbody" << std::endl;
		delete(&rbody);
	}

}


void geProject::EntityManager::assignSpriteRender(uInt entityId, SpriteRender sprite) {
	/*if (entityId < maxEntities && entityId >= 0) {
		//set dirty flag so the renderer knows there are updates for rendering
		sprite.dirtyFlag[0] = 1;
		//if a memory address is already assigned to componentsprite render then copy the content into the already 
		//established memory address
		if (componentSpriteRender[entityId] != NULL) {
			//copy render buffer id to new spriteRender component
			sprite.dirtyFlag[1] = componentSpriteRender[entityId]->dirtyFlag[1];
			//copy vertices index id to new spriteRender component
			sprite.dirtyFlag[2] = componentSpriteRender[entityId]->dirtyFlag[2];
			std::memcpy(componentSpriteRender[entityId], &sprite, sizeof(SpriteRender));
		}
		// else allocate a new memory address from the pool allocator to the list of sprite components
		else {
			auto spr = (SpriteRender*)spritepool.allocate(sizeof(SpriteRender));			
			std::memcpy(spr, &sprite, sizeof(SpriteRender));
			componentSpriteRender[entityId] = spr;
		}			
		entityUpdated = true;
		//std::cout << "new [entity " << entityId << "][componentSpriteRender] = " << componentSpriteRender[entityId] << std::endl;
	}
	else {
		//std::cout << "unable to assign sprite" << std::endl;
		delete(&sprite);
	}
	*/
	if (entityId < maxEntities && entityId >= 0) {
		sprite.dirtyFlag[0] = 1;
		sprite.entityId = entityId;
		/*
		if (componentSpriteRender[entityId]->id == 0) {
			sprite.dirtyFlag[1] = 0;
			sprite.dirtyFlag[2] = 0;
			
		}
		else {
			sprite.dirtyFlag[1] = componentSpriteRender[entityId]->dirtyFlag[1];
			sprite.dirtyFlag[2] = componentSpriteRender[entityId]->dirtyFlag[2];
		}
		*/
		std::memcpy(componentSpriteRender[entityId], &sprite, sizeof(SpriteRender));		
		entityUpdated = true;
	}
	else {
		std::cout << "unable to assign sprite" << std::endl;
		delete(&sprite);
	}
}

void geProject::EntityManager::assignFontRender(uInt entityId, FontRender font) {
	/*
	if (entityId < maxEntities && entityId >= 0) {
		auto fonts = (FontRender*)fontpool.allocate(sizeof(FontRender));
		std::memcpy(fonts, &font, sizeof(FontRender));
		if (componentFontRender[entityId] != NULL) {
			fonts->dirtyFlag = true;
		}
		componentFontRender[entityId] = fonts;
		//std::cout << "new [entity " << entityId << "][componentFontRender] = " << componentFontRender[entityId] << std::endl;
	}
	else {
		//std::cout << "unable to assign fontrender" << std::endl;
		delete(&font);
	}
	*/
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
			//std::cout << "delete [entity " << entityId << "][TransformArray] = " << componentTransforms[entityId] << std::endl;
			break;
		case 2:
			componentSpriteRender[entityId]->id = 0;
			//std::cout << "delete [entity " << entityId << "][componentSpriteRender] = " << componentSpriteRender[entityId] << std::endl;
			break;
		case 4:
			componentRigidBody[entityId]->id = 0;
			//std::cout << "delete [entity " << entityId << "][componentFontRender] = " << componentFontRender[entityId] << std::endl;
			break;
		case 8:
			componentFontRender[entityId]->id = 0;
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
std::vector<geProject::Rigidbody*> geProject::EntityManager::getRigidBodyComponents() {
	return componentRigidBody;
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
geProject::Rigidbody* geProject::EntityManager::getRigidBodyComponent(uInt entityId) {
	return componentRigidBody[entityId];
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

void geProject::EntityManager::updateImgui(uInt entityId) {
	auto sprite  = getSpriteComponent(entityId);
	auto transform = getTransformComponent(entityId);
	auto rigid = getRigidBodyComponent(entityId);
	auto font = getFontRenderComponent(entityId);
	//TRANSFORM IMGUI updates
	float positionX = transform->position[0];
	float positionY = transform->position[1];
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

	//SPRITE IMGUI updates
	int textureId = sprite->textureId;
	if (ImGui::DragInt("textureId", &textureId)) {
		if (textureId != sprite->textureId) {
			sprite->textureId = textureId;
			sprite->dirtyFlag[0] = 1;
			entityUpdated = true;
		}
	}

	int zIndex = sprite->zIndex;
	if (ImGui::DragInt("zIndex", &zIndex)) {
		if (zIndex != sprite->zIndex) {
			sprite->zIndex = zIndex;
			sprite->dirtyFlag[0] = 1;
			entityUpdated = true;
		}
	}
	
	//RIGIDBODY IMGUI updates
	if (rigid->id > 0) {		
		int collider = rigid->collider;
		float friction = rigid->friction;
		float velocityX = rigid->velocity[0];
		float velocityY = rigid->velocity[1];
		float velocityZ = rigid->velocity[2];
		if (ImGui::DragInt("rigidBody", &collider)) {
			rigid->collider = collider;
			entityUpdated = true;			
		}
		if (ImGui::DragFloat("friction", &friction)) {
			rigid->friction = friction;
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
	}
	if (font->id > 0) {
		
	}
	static ImVec4 color = ImVec4(sprite->color[0], sprite->color[1], sprite->color[2], sprite->color[3]);
	ImGui::Text("MY CUSTOM COLOR PICKER WITH AN AMAZING PALETTE!");
	if (ImGui::ColorPicker4("##picker", (float*)&color)) {
		sprite->color[0] = color.x;
		sprite->color[1] = color.y;
		sprite->color[2] = color.z;
		sprite->color[3] = color.w;
		sprite->dirtyFlag[0] = 1;
		entityUpdated = true;
	}
	ImGui::SameLine();
}
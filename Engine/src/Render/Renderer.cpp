#include "Renderer.h"

std::vector<geProject::RenderBatch> geProject::Renderer::renderList;

geProject::Renderer::Renderer(geProject::ResourceManager& resources){
	eventSystem.subscribe(this, &Renderer::deleteEntity);
	resourceManager = &resources;
	int zIndexRange = 10;
	//instantiate 10 zindex batches
	for (int i = 0; i <= zIndexRange; i++) {
		renderList.push_back(RenderBatch(maxBatch, i, *resourceManager));
	}
	//renderList.push_back(RenderBatch(maxBatch, 0, *resourceManager));
}

geProject::Renderer::~Renderer(){}
	
void geProject::Renderer::init() {	
	
}
void geProject::Renderer::shutdown() {}

unsigned int geProject::Renderer::getZindexBatch(unsigned int zIndex) {
	int index = 0;	
	int size = renderList.size();
	if (size > -1) {	
		if (size == 0) {
			renderList.push_back(RenderBatch(maxBatch, zIndex, *resourceManager));			
		}
		else {
			while (index <= size && zIndex > renderList[index].getZindex()) {
				index++;
			}
			//if renderbatch is full for that zindex then create a new one
			while (renderList[index].isBatchFull()) {
				index++;
			}
			//create a new batch if one is not already made
			if (index < size && renderList[index].getZindex() != zIndex) {
				renderList.insert(renderList.begin() + index, RenderBatch(maxBatch, zIndex, *resourceManager));
			}
		}
	}
	return index;
}

void geProject::Renderer::addSpriteToBatch(SpriteRender* sprite, Transform* transform) {
	//auto rList = std::make_shared<RenderBatch>(RenderBatch(maxBatch));
	//renderList.push_back(rList);		
	//if there are no batches added a new batch with zindex
	//if (renderList.size() == 0) {
	//	count++;
	//	renderList.push_back(RenderBatch(maxBatch, sprite->zIndex, *resourceManager));
	//}
	//if there are batches go through until batch is found
	//else if (renderList.size() > 0){
		
		//if no batch with zindex found, then insert a new batch in sequential zindex order
		/*
		if (count > (renderList.size() - 1) || sprite->zIndex != renderList[count].getZindex()) {
			renderList.insert(renderList.begin() + count, RenderBatch(maxBatch, sprite->zIndex, *resourceManager));
		}
		//if batch is found check if it is full, if it is full then create a new batch next to old batch
		else if (sprite->zIndex == renderList[count].getZindex()) {
			if (renderList[count].isBatchFull() || renderList[count].isTextureFull(sprite->textureId)) {
				count++;
				
			}
		}
		*/		
	//}	
	int count = getZindexBatch(sprite->zIndex);	
	transform->dirtyFlag[1] = count;	
	renderList[count].addSprite(sprite, transform);	
}

void geProject::Renderer::updateSprite(SpriteRender* sprite, Transform* transform) {	
	if (transform->dirtyFlag[2] > -1) {

		//if zIndex is different from the current entities renderbatch
		if (sprite->zIndex != renderList[transform->dirtyFlag[1]].getZindex()) {
			//remove the entity from the renderbatch and switch the renderbatch to the new z index;
			renderList[transform->dirtyFlag[1]].removeVertices(transform->dirtyFlag[2]);
			transform->dirtyFlag[1] = getZindexBatch(sprite->zIndex);
			//find an unused section of the renderbatch
			int newIndex = renderList[transform->dirtyFlag[1]].getUnusedRenderSection();
			if (newIndex > -1) {
				transform->dirtyFlag[2] = newIndex;
			}		
			renderList[transform->dirtyFlag[1]].addSprite(sprite, transform);
		}
		else if (transform->dirtyFlag[0] == 1) {
			renderList[transform->dirtyFlag[1]].updateSprite(sprite, transform);
		}
	}
}




void geProject::Renderer::render(Camera& camera, std::string shaderPath) {
	//std::cout << "number of batches" << renderList.size() << std::endl;
	//iterate backward through list to allow layers with lower z index to be towards the front
	
	for (int i = renderList.size() - 1; i >= 0 ;i--) {
		renderList[i].render(camera, shaderPath);
	}
}

void geProject::Renderer::clear() {	
	//std::cout << "number of batches" << renderList.size() << std::endl;
	if (renderList[0].getSpriteNum() > 0) {
		renderList.clear();
		//renderList.push_back(RenderBatch(maxBatch, 0, *resourceManager));
	}
}

void geProject::Renderer::deleteEntity(DeleteEntityEvent* e){
	renderList[e->renderBatch].removeVertices(e->renderIndex);
}

#include "Renderer.h"

std::vector<geProject::RenderBatch> geProject::Renderer::renderList;

geProject::Renderer::Renderer(geProject::ResourceManager& resources){
	resourceManager = &resources;
	//renderList.push_back(RenderBatch(maxBatch, 0, *resourceManager));
}

geProject::Renderer::~Renderer(){}
	
void geProject::Renderer::init() {	
	
}
void geProject::Renderer::shutdown() {}

void geProject::Renderer::addSpriteToBatch(SpriteRender* sprite, Transform* transform) {
	//auto rList = std::make_shared<RenderBatch>(RenderBatch(maxBatch));
	//renderList.push_back(rList);	
	
	int count = -1;

	//if there are no batches added a new batch with zindex
	if (renderList.size() == 0) {
		count++;
		renderList.push_back(RenderBatch(maxBatch, sprite->zIndex, *resourceManager));
	}
	//if there are batches go through until batch is found
	else if (renderList.size() > 0){
		do
		{
			count++;
		} while (count <= (renderList.size() - 1) && sprite->zIndex > renderList[count].getZindex());
		

		//if no batch with zindex found, then insert a new batch in sequential zindex order
		if (count > (renderList.size() - 1) || sprite->zIndex != renderList[count].getZindex()) {
			renderList.insert(renderList.begin() + count, RenderBatch(maxBatch, sprite->zIndex, *resourceManager));
		}
		//if batch is found check if it is full, if it is full then create a new batch next to old batch
		else if (sprite->zIndex == renderList[count].getZindex()) {
			if (renderList[count].isBatchFull() || renderList[count].isTextureFull(sprite->textureId)) {
				count++;
				renderList.insert(renderList.begin() + count, RenderBatch(maxBatch, sprite->zIndex, *resourceManager));
			}
		}

		
	}
	sprite->dirtyFlag[1] = count;
	transform->dirtyFlag[1] = count;
	renderList[count].addSprite(sprite, transform);	
}

void geProject::Renderer::updateSprite(SpriteRender* sprite, Transform* transform) {	
	renderList[transform->dirtyFlag[1]].updateSprite(sprite, transform);
}

void geProject::Renderer::render(Camera& camera) {
	//std::cout << "number of batches" << renderList.size() << std::endl;
	for (int i = 0; i < renderList.size();i++) {
		renderList[i].render(camera);
	}
}

void geProject::Renderer::clear() {
	//std::cout << "number of batches" << renderList.size() << std::endl;
	if (renderList[0].getSpriteNum() > 0) {
		renderList.clear();
		//renderList.push_back(RenderBatch(maxBatch, 0, *resourceManager));
	}
}
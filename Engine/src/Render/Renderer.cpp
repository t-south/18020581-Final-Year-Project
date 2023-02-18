#include "Renderer.h"

std::vector<geProject::RenderBatch> geProject::Renderer::renderList;;

geProject::Renderer::Renderer(geProject::ResourceManager& resources){
	resourceManager = &resources;
	renderList.push_back(RenderBatch(maxBatch, *resourceManager));
}

geProject::Renderer::~Renderer(){}
	
void geProject::Renderer::init() {	
	
}
void geProject::Renderer::shutdown() {}

void geProject::Renderer::addSpriteToBatch(SpriteRender* sprite, Transform* transform) {
	//auto rList = std::make_shared<RenderBatch>(RenderBatch(maxBatch));
	//renderList.push_back(rList);
	//if current batch is full start a new batch
	if (renderList.back().isBatchFull()) {
		renderList.push_back(RenderBatch(maxBatch, *resourceManager));
	}
	renderList.back().addSprite(sprite, transform);
	
}



void geProject::Renderer::render(Camera& camera) {
	//std::cout << "number of batches" << renderList.size() << std::endl;
	for (int i = 0; i < renderList.size();i++) {
		renderList[i].render(camera);
	}
}
#include "RenderBatch.h"


geProject::RenderBatch::RenderBatch(int maxSize, ResourceManager& resources) : maxBatch(maxSize), vertSize(6), vao(0), vbo(0), spriteNum(0) {
	resourceManager = &resources;
	
	
	// vector size will be the size of a quad of vertices multiplied by the max size of the batch
	int vectorSize = maxBatch * vertSize * 4;
	
	//initialise vector for later use
	for (int i = 0; i < vectorSize; i++) {		
		vertices.push_back(0);
	}
	init();
}
geProject::RenderBatch::~RenderBatch() {}

bool geProject::RenderBatch::isBatchFull() {
	return spriteNum == maxBatch;
}

void geProject::RenderBatch::init() {
	//generate buffers for VAO, VBO and EBO
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);

	unsigned int ebo;
	glGenBuffers(1, &ebo);
	std::vector<unsigned int> indices = createIndexes();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);


	//set vertices pointers
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, vertSize * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//set color pointers
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, vertSize * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//set texture pointers
	//glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(7 * sizeof(float)));
	//glEnableVertexAttribArray(2);
	
	

}

void geProject::RenderBatch::addSprite(SpriteRender* sprite, Transform* transform) {
	if (spriteNum <= maxBatch) {
		int index = spriteNum * 4 * vertSize;
	
		for (int i = 0; i < 4; i++) {
			switch (i) {
			case 0:
				vertices[index] = transform->position[0] + (1 * transform->scale.x); //x
				vertices[index + 1] = transform->position[1] + (1 * transform->scale.y); //y
				break;
			case 1:
				vertices[index] = transform->position[0] + (1 * transform->scale.x); //x
				vertices[index + 1] = transform->position[1] + (0 * transform->scale.y); //y
				break;

			case 2:
				vertices[index] = transform->position[0] + (0 * transform->scale.x); //x
				vertices[index + 1] = transform->position[1] + (0 * transform->scale.y); //y
				break;

			case 3:
				vertices[index] = transform->position[0] + (0 * transform->scale.x); //x
				vertices[index + 1] = transform->position[1] + (1 * transform->scale.y); //y
				break;

			}
			vertices[index + 2] = sprite->color[0]; //r
			vertices[index + 3] = sprite->color[1]; //g
			vertices[index + 4] = sprite->color[2]; //b
			vertices[index + 5] = sprite->color[3]; //a
			index += vertSize;
		}
		spriteNum++;
	}
}

std::vector<unsigned int> geProject::RenderBatch::createIndexes() {
	std::vector<unsigned int> elementOrder;
	// 0 1 3 -- 1 2 3 indices ordering  4 5 7 -- 5 6 7
	for (int i = 0; i < maxBatch; i++) {
		//First triangle indices order
		unsigned int offset = i * 4;
		elementOrder.push_back(offset);			// 0
		elementOrder.push_back(offset + 1);	// 1
		elementOrder.push_back(offset + 3);	// 3
		//Second triangle indices order
		elementOrder.push_back(offset + 1);	// 1
		elementOrder.push_back(offset + 2);	// 2
		elementOrder.push_back(offset + 3);	// 3
	}
	return elementOrder;
}

void geProject::RenderBatch::render(Camera& camera) {
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), &vertices[0]);
	auto shader = resourceManager->requestShader("../../../../Game/assets/shaders/VertexShaderDefault.glsl");
	shader->setMat4f("uProjMat", camera.getProjection());
	shader->setMat4f("uViewMat", camera.getViewMatrix());
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawElements(GL_TRIANGLES, (spriteNum * vertSize) * sizeof(float), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	shader->detach();
}




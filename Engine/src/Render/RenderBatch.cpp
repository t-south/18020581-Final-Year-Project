#include "RenderBatch.h"


geProject::RenderBatch::RenderBatch(int maxSize, unsigned int zIndex, ResourceManager& resources) : zIndex(zIndex),maxBatch(maxSize), vertSize(10), vao(0), vbo(0), spriteNum(0) {
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

bool geProject::RenderBatch::isTextureFull(unsigned int id) {
	if (textures[7] == nullptr) {
		return false;
	}
	else {
		auto tex = resourceManager->requestTexture(id);
		for (auto const& i : textures) {
			if (i == tex) {
				return false;
			}
		}
		return true;
	}
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
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, vertSize * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	//texture ID
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, vertSize * sizeof(float), (void*)(8 * sizeof(float)));
	glEnableVertexAttribArray(3);
	//entity ID
	glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, vertSize * sizeof(float), (void*)(9 * sizeof(float)));
	glEnableVertexAttribArray(4);
}

void geProject::RenderBatch::addSprite(SpriteRender* sprite, Transform* transform) {	
	if (spriteNum <= maxBatch && sprite->zIndex == zIndex) {
		int index = spriteNum * 4 * vertSize;		
		//load in new texture into texture array if not present, leave 0th element for normal colors		
		if (sprite->spriteSheetId == 0 && sprite->textureId > 0 && textures[sprite->textureId] == nullptr) {
			textures[sprite->textureId] = resourceManager->requestTexture(sprite->textureId);
		}
		else if (sprite->spriteSheetId > 0) {
			textures[sprite->textureId] = resourceManager->requestSpriteSheet(sprite->textureId);
		}		
		transform->dirtyFlag[2] = index;
		sprite->dirtyFlag[2] = index + 2;
		transform->dirtyFlag[0] = 0;
		sprite->dirtyFlag[0] = 0;
		createVertices(sprite, transform, index);
		hasUpdate = true;
		spriteNum++;
		if (spriteNum > 4) {
			std::cout << spriteNum << std::endl;
		}
	}
}

void geProject::RenderBatch::updateSprite(SpriteRender* sprite, Transform* transform) {
	if (sprite->zIndex == zIndex) {
		createVertices(sprite, transform, transform->dirtyFlag[2]);
		transform->dirtyFlag[0] = 0;
		sprite->dirtyFlag[0] = 0;
		hasUpdate = true;
	}
}


void geProject::RenderBatch::createVertices(SpriteRender* sprite, Transform* transform, unsigned int index) {
	glm::vec2 centre = getCentre(transform->position, glm::vec2(transform->position[0] + (1 * transform->scale.x), transform->position[1] + (1 * transform->scale.y)));//getCentre(transform->position, glm::vec2(transform->position[0] + (0 * transform->scale.x), transform->position[1] + (1 * transform->scale.y)));
	for (int i = 0; i < 4; i++) {

		switch (i) {
		case 0:
			vertices[index] = transform->position[0] + (1 * transform->scale.x); //x
			vertices[static_cast<std::vector<float, std::allocator<float>>::size_type>(index) + 1] = transform->position[1] + (1 * transform->scale.y); //y
			break;
		case 1:
			vertices[index] = transform->position[0] + (1 * transform->scale.x); //x
			vertices[static_cast<std::vector<float, std::allocator<float>>::size_type>(index) + 1] = transform->position[1] + (0 * transform->scale.y); //y
			break;

		case 2:
			vertices[index] = transform->position[0] + (0 * transform->scale.x); //x
			vertices[static_cast<std::vector<float, std::allocator<float>>::size_type>(index) + 1] = transform->position[1] + (0 * transform->scale.y); //y
			break;

		case 3:
			vertices[index] = transform->position[0] + (0 * transform->scale.x); //x
			vertices[static_cast<std::vector<float, std::allocator<float>>::size_type>(index) + 1] = transform->position[1] + (1 * transform->scale.y); //y
			break;

		}
		if (transform->rotation > 0 || transform->rotation < 0) {
			glm::vec2 newVerts = rotate(glm::vec2(vertices[index], vertices[index + 1]), centre, transform->rotation);
			vertices[index] = newVerts[0];
			vertices[index + 1] = newVerts[1];
		}
		vertices[static_cast<std::vector<float, std::allocator<float>>::size_type>(index) + 2] = sprite->color[0]; //r
		vertices[static_cast<std::vector<float, std::allocator<float>>::size_type>(index) + 3] = sprite->color[1]; //g
		vertices[static_cast<std::vector<float, std::allocator<float>>::size_type>(index) + 4] = sprite->color[2]; //b
		vertices[static_cast<std::vector<float, std::allocator<float>>::size_type>(index) + 5] = sprite->color[3]; //a
		vertices[static_cast<std::vector<float, std::allocator<float>>::size_type>(index) + 6] = sprite->texturePos[i][0]; //texture coord x
		vertices[static_cast<std::vector<float, std::allocator<float>>::size_type>(index) + 7] = sprite->texturePos[i][1]; //texture coord y
		vertices[static_cast<std::vector<float, std::allocator<float>>::size_type>(index) + 8] = (float)sprite->textureId; // texture id
		vertices[static_cast<std::vector<float, std::allocator<float>>::size_type>(index) + 9] = (float)sprite->entityId + 1;
		index += vertSize;
	}
}


std::vector<unsigned int> geProject::RenderBatch::createIndexes() {
	std::vector<unsigned int> elementOrder;
	// 0 1 3 -- 1 2 3 indices ordering  4 5 7 -- 5 6 7
	for (int i = 0; i < maxBatch; i++) {
		//First triangle indices order
		unsigned int offset = i * 4;
		elementOrder.push_back(offset);		// 0
		elementOrder.push_back(offset + 1);	// 1
		elementOrder.push_back(offset + 3);	// 3
		//Second triangle indices order
		elementOrder.push_back(offset + 1);	// 1
		elementOrder.push_back(offset + 2);	// 2
		elementOrder.push_back(offset + 3);	// 3
	}
	return elementOrder;
}

void geProject::RenderBatch::render(Camera& camera, std::string shaderPath) {	
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), &vertices[0]);
	auto shader = resourceManager->requestShader(shaderPath);
	shader->setMat4f("uProjMat", camera.getProjection());
	shader->setMat4f("uViewMat", camera.getViewMatrix());	
	unsigned int count = 0;
	//go through each texture and bind to a seperate texture unit -- set for up to 8 texture units
	for (auto const& i : textures) {
		if ( i != nullptr) {
			glActiveTexture(GL_TEXTURE0 + count);
			textures[count]->bindTexture();
			//slots[count] = count;
			std::string name = "texture" + std::to_string(count);
			shader->setTexture(name, count);
		}
		count++;
	}

	/*
	int index = 0;
	for (auto const& i : vertices) {
		if (index % 9 == 0) {
			std::cout << std::endl;
		}
		std::cout << i << " ";
		index++;
	
	}
	*/
	//shader->setIntArray("textures", slots);
	
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawElements(GL_TRIANGLES, spriteNum * vertSize * sizeof(float), GL_UNSIGNED_INT, 0);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
	
	count = 1;
	//unbind each texture previously bound
	for (auto const& i : textures) {
		if (i != nullptr) {
			textures[count]->unbindTexture();
			count++;
		}
	}	
	shader->detach();
}



unsigned int geProject::RenderBatch::getSpriteNum() {
	return spriteNum;
}

unsigned int geProject::RenderBatch::getZindex() {
	return zIndex;
}

glm::vec2 geProject::RenderBatch::rotate(glm::vec2 vert, glm::vec2 centre, float rotation) {
	rotation = rotation * PI / 180.0;
	return glm::vec2((cos(rotation) * (vert.x - centre.x) - sin(rotation) * (vert.y - centre.y) + centre.x), (sin(rotation) * (vert.x - centre.x) + cos(rotation) * (vert.y - centre.y) + centre.y));
}

glm::vec2 geProject::RenderBatch::getCentre(glm::vec2 bLeft, glm::vec2 tRight) {
	glm::vec2 centre{};
	centre[0] = (bLeft[0] + tRight[0]) / 2;
	centre[1] = (bLeft[1] + tRight[1]) / 2;
	return centre;
}
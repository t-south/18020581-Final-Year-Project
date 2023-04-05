#include "RenderBatch.h"


geProject::RenderBatch::RenderBatch(int maxSize, unsigned int zIndex) : zIndex(zIndex),maxBatch(maxSize), vertSize(10), vao(0), vbo(0), spriteNum(0) {		
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
		auto tex = resourcemanager.requestTexture(id);
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
		std::cout << index << std::endl;
		if (vertices[static_cast<std::vector<float, std::allocator<float>>::size_type>(index) + 9] != 0.0f) { // get the index where the object id is stored
			index = getUnusedRenderSection();
		}	
		//load in new texture into texture array if not present, leave 0th element for normal colors		
		if (sprite->spriteSheetId == 0 && sprite->textureId > 0 && textures[sprite->textureId] == nullptr) {
			textures[sprite->textureId] = resourcemanager.requestTexture(sprite->textureId);
		}
		else if (sprite->spriteSheetId > 0) {
			textures[sprite->textureId] = resourcemanager.requestSpriteSheet(sprite->textureId);
		}		
		transform->dirtyFlag[2] = index;	
		transform->dirtyFlag[0] = 0;	
		if (index == -1) {
			std::cout << "test error" << std::endl;
		}
		createVertices(sprite, transform->position[0], transform->position[1], transform->scale[0], transform->scale[1], transform->rotation, index);
		hasUpdate = true;
		spriteNum++;
	}
}

void geProject::RenderBatch::addMapTile(SpriteRender sprite, int ssId, float x, float y, int layer)
{
	if (spriteNum <= maxBatch && layer == zIndex) {
		int index = spriteNum * 4 * vertSize;
		if (vertices[static_cast<std::vector<float, std::allocator<float>>::size_type>(index) + 9] != 0.0f) { // get the index where the object id is stored
			index = getUnusedRenderSection();
		}
		//load in new texture into texture array if not present, leave 0th element for normal colors		
		auto map = resourcemanager.requestLevelMap(ssId);
		
		textures[map->getTextureId()] = map;
		createVertices(&sprite, x, y, 0.25f, 0.25f, 0, index);
		spriteNum++;
	}
}

void geProject::RenderBatch::updateSprite(SpriteRender* sprite, Transform* transform) {
	if (sprite->zIndex == zIndex) {
		createVertices(sprite, transform->position[0], transform->position[1], transform->scale[0], transform->scale[1], transform->rotation, transform->dirtyFlag[2]);
		transform->dirtyFlag[0] = 0;	
		hasUpdate = true;
	}
}


void geProject::RenderBatch::createVertices(SpriteRender* sprite, float x, float y, float scaleX, float scaleY, float rotation, unsigned int index) {
	
	for (int i = 0; i < 4; i++) {
		switch (i) {
		case 0:
			vertices[index] = x + (0.5f /* 1*/ * scaleX); //x
			vertices[static_cast<std::vector<float, std::allocator<float>>::size_type>(index) + 1] = y + (0.5f /* 1*/ * scaleY); //y
			break;
		case 1:
			vertices[index] = x + (0.5f /* 1*/ * scaleX); //x
			vertices[static_cast<std::vector<float, std::allocator<float>>::size_type>(index) + 1] = y + (-0.5f /* 0*/ * scaleY); //y
			break;

		case 2:
			vertices[index] = x + ( -0.5f /* 0*/ * scaleX); //x
			vertices[static_cast<std::vector<float, std::allocator<float>>::size_type>(index) + 1] = y + (-0.5f /* 0*/ * scaleY); //y
			break;

		case 3:
			vertices[index] = x + (-0.5f /* 0*/ * scaleX); //x
			vertices[static_cast<std::vector<float, std::allocator<float>>::size_type>(index) + 1] = y + (0.5f /* 1*/ * scaleY); //y
			break;

		}
		if (rotation > 0 || rotation < 0) {
			glm::vec2 newVerts = rotate(glm::vec2(vertices[index], vertices[index + 1]), glm::vec2(x, y), rotation);
			vertices[index] = newVerts[0];
			vertices[static_cast<std::vector<float, std::allocator<float>>::size_type>(index) + 1] = newVerts[1];
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

void geProject::RenderBatch::removeVertices(unsigned int index) {
	for (int i = index; i < index + (4*vertSize); i++) {
		vertices[i] = 0;  //reset vertices in specific index 
	}
	spriteNum--;

}




std::vector<unsigned int> geProject::RenderBatch::createIndexes() {
	std::vector<unsigned int> elementOrder;
	// 0 1 3 -- 1 2 3 indices ordering  4 5 7 -- 5 6 7
	for (unsigned int i = 0; i < maxBatch; i++) {
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
	//if (shaderPath == "../../../../Game/assets/shaders/SelectionVertexShader.glsl") {
	//	glClear(GL_COLOR_BUFFER_BIT);
	//}
	auto shader = resourcemanager.requestShader(shaderPath);
	camera.projectionUpdate();
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


	
	int index = 0;
	//shader->setIntArray("textures", slots);
	
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawElements(GL_TRIANGLES, spriteNum * 10 * sizeof(float), GL_UNSIGNED_INT, 0);
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

int geProject::RenderBatch::getUnusedRenderSection() {
	int count = 0;
	int verticesSize = vertSize * 4;
	while ((static_cast<unsigned long long>(count) * verticesSize) < vertices.size() && vertices[(static_cast<std::vector<float, std::allocator<float>>::size_type>(count) * verticesSize) + 9] != 0) {
		count++;
	}
	return count * verticesSize;
}

unsigned int geProject::RenderBatch::getSpriteNum() {
	return spriteNum;
}

unsigned int geProject::RenderBatch::getZindex() {
	return zIndex;
}

glm::vec2 geProject::RenderBatch::rotate(glm::vec2 vert, glm::vec2 centre, float rotation) {
	double pi = 3.14159265;
	rotation = rotation * pi / 180.0;
	return glm::vec2((cos(rotation) * (vert.x - centre.x) - sin(rotation) * (vert.y - centre.y) + centre.x), (sin(rotation) * (vert.x - centre.x) + cos(rotation) * (vert.y - centre.y) + centre.y));
}



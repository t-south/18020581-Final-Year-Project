#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Line.h"
#include "Shader.h"
#include "../ResourceManager/ResourceManager.h"
#include "Camera.h"

namespace geProject {
	class EditorRender {
	public:
		EditorRender(ResourceManager& resources);
		~EditorRender();
		void render(Camera& camera);
		void lineCheck();
		void createVertices();
		void addLine(glm::vec2 origin, glm::vec2 destination, glm::vec3 color, unsigned int life);
	private:
		unsigned int vao, vbo, vertSize, index;
		std::vector<Line*> lines;
		std::vector<float> vertices;
		ResourceManager* resourceManager;
	};
}
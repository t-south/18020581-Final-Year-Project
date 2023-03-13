#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Line.h"
#include "Shader.h"
#include "../ResourceManager/ResourceManager.h"
#include "../Cameras/Camera.h"


namespace geProject {
	class EditorRender {
	public:
		EditorRender(ResourceManager& resources);
		~EditorRender();
		void render(Camera& camera);
		void lineCheck();
		void createVertices();
		void addLine(glm::vec2 origin, glm::vec2 destination, glm::vec3 color, unsigned int life);
		void addBox(glm::vec2 centre, glm::vec2 dim, glm::vec3 color, float rotation, unsigned int life);
		void addCircle(glm::vec2 centre, glm::vec3 color, float radius, unsigned int segments, unsigned int life);
		glm::vec2 rotate(glm::vec2 vert, glm::vec2 centre, float rotation);
	
	private:
		unsigned int vao, vbo, vertSize, index, renderSize;
		std::vector<Line*> lines;
		std::vector<float> vertices;
		ResourceManager* resourceManager;
	};
}
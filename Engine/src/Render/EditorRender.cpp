#include "EditorRender.h"

geProject::EditorRender::EditorRender(ResourceManager& resources) {
	index = 0;
	vertSize = 6;
	for (int i = 0; i < 500 * vertSize; i++) {
		vertices.push_back(0);
	}
	
	resourceManager = &resources;	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertSize * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertSize * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);
	glLineWidth(4.0f);
}

geProject::EditorRender::~EditorRender(){}

void geProject::EditorRender::lineCheck() {
	for (int i = 0; i < lines.size(); i++) {
		if (lines[i]->changeFrame() <= 0) {
			lines.erase(lines.begin() + i);
			i--;
		}
	}
}

void geProject::EditorRender::addLine(glm::vec2 origin, glm::vec2 destination, glm::vec3 color, unsigned int life) {
	lines.push_back(new Line(origin, destination, color, life));
	createVertices();
}

void geProject::EditorRender::createVertices() {
	if (lines.size() > 0) {		
		int index = 0;
		for (auto const& line : lines) {
			glm::vec2 pos;
			for (int i = 0; i < 2; i++) {				
				switch (i) {
				case 0:
					pos = line->getOrigin();
					vertices[index] = pos[0];
					vertices[index + 1] = pos[1];
					break;
				case 1:
					pos = line->getDestination();
					vertices[index] = pos[0];
					vertices[index + 1] = pos[1];
					break;
				default:
					break;
				}
				glm::vec3 color = line->getColor();
				vertices[index + 2] = 0.0f;
				vertices[index + 3] = color[0];
				vertices[index + 4] = color[1];
				vertices[index + 5] = color[2];
				index += vertSize;
			}

		}
	}
}

void geProject::EditorRender::render(Camera& camera){
	std::fill(vertices.begin(), vertices.end(), 0.0f);
	lineCheck();
	createVertices();
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(float), &vertices[0]);
	auto shader = resourceManager->requestShader("../../../../Game/assets/shaders/LineVertexShader.glsl");
	shader->setMat4f("uProjMat", camera.getProjection());
	shader->setMat4f("uViewMat", camera.getViewMatrix());
	glBindVertexArray(vao);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_LINES, 0, lines.size() * 6 * 2);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glBindVertexArray(0);
	shader->detach();
}
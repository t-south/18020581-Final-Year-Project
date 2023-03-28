#include "EditorRender.h"


geProject::EditorRender::EditorRender(ResourceManager& resources) {
	renderSize = 50000;
	index = 0;
	vertSize = 6;
	for (int i = 0; i < renderSize * vertSize; i++) {
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


void geProject::EditorRender::addBox(glm::vec2 centre, glm::vec2 dim, glm::vec3 color, float rotation, unsigned int life) {
	if (dim[0] > 0 && dim[1] > 0) {
		glm::vec2 min = centre - dim * 0.5f;
		glm::vec2 max = centre + dim * 0.5f;
		std::vector<glm::vec2> verts;
		verts.push_back(glm::vec2(min.x, min.y));
		verts.push_back(glm::vec2(min.x, max.y));
		verts.push_back(glm::vec2(max.x, max.y));
		verts.push_back(glm::vec2(max.x, min.y));
		if (rotation != 0.0f) {
			for (int i = 0; i < verts.size(); i++) {
				verts[i] = rotate(verts[i], centre, rotation);
			}
		}
		addLine(verts[0], verts[1], color, life);
		addLine(verts[0], verts[3], color, life);
		addLine(verts[1], verts[2], color, life);
		addLine(verts[2], verts[3], color, life);
	}
}


glm::vec2 geProject::EditorRender::rotate(glm::vec2 vert, glm::vec2 centre, float rotation) {
	//return glm::vec2(centre.x + (vert.x - centre.x) * cos(rotation) - (vert.y - centre.y) * sin(rotation), centre.y + (vert.x - centre.x) * sin(rotation) + (vert.y - centre.y) * cos(rotation));
	//convert from degrees to radians
	double PI =  3.14159265;
	rotation = rotation * PI / 180.0;
	return glm::vec2((cos(rotation) * (vert.x - centre.x) - sin(rotation) * (vert.y - centre.y) + centre.x), (sin(rotation) * (vert.x - centre.x) + cos(rotation) * (vert.y - centre.y) + centre.y));
}


void geProject::EditorRender::addCircle(glm::vec2 centre, glm::vec3 color, float radius, unsigned int segments, unsigned int life) {
	if (radius > 0) {
		std::vector<glm::vec2> verts;
		int increment = 360 / segments;
		float angle = 0;
		for (int i = 0; i < segments; i++) {
			glm::vec2 point = glm::vec2(radius, 0);
			point = rotate(point, glm::vec2(0, 0), angle);
			verts.push_back(point + centre);
			if (i > 0) {
				addLine(verts[static_cast<std::vector<glm::vec2, std::allocator<glm::vec2>>::size_type>(i) - 1], verts[i], color, life);
			}
			angle += increment;
		}
		addLine(verts[verts.size() - 1], verts[0], color, life);
	}
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
					vertices[static_cast<std::vector<float, std::allocator<float>>::size_type>(index) + 1] = pos[1];//x
					break;
				case 1:
					pos = line->getDestination();
					vertices[index] = pos[0];
					vertices[static_cast<std::vector<float, std::allocator<float>>::size_type>(index) + 1] = pos[1];//y
					break;
				default:
					break;
				}
				glm::vec3 color = line->getColor();
				vertices[static_cast<std::vector<float, std::allocator<float>>::size_type>(index) + 2] = 0.0f;  //z
				vertices[static_cast<std::vector<float, std::allocator<float>>::size_type>(index) + 3] = color[0];
				vertices[static_cast<std::vector<float, std::allocator<float>>::size_type>(index) + 4] = color[1];
				vertices[static_cast<std::vector<float, std::allocator<float>>::size_type>(index) + 5] = color[2];
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


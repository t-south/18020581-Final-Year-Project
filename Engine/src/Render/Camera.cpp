#include "Camera.h"


geProject::Camera::Camera(glm::vec2 pos): direction(glm::vec3(0.0f, 0.0f, -1.0f)), up(glm::vec3(0.0f, 1.0f, 0.0f)), position(glm::vec3(pos, 0.0f)) {
	projectionUpdate();
}

geProject::Camera::~Camera(){}

glm::mat4 geProject::Camera::getViewMatrix()
{	
    return glm::lookAt(position, position + direction, up);
}

void geProject::Camera::projectionUpdate() {
	projection = glm::mat4(1.0f);
	projection = glm::ortho(0.0f, 32.0f * 40.0f, 0.0f, 32 * 21.0f);

}

glm::mat4 geProject::Camera::getProjection() {
	return projection;
}
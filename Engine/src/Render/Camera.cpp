#include "Camera.h"



geProject::Camera::Camera(glm::vec2 pos): direction(glm::vec3(0.0f, 0.0f, -1.0f)), up(glm::vec3(0.0f, 1.0f, 0.0f)), position(glm::vec3(pos, 0.0f)), projSize(glm::vec2(1280.0f, 672.0f)) {	
	projectionUpdate();
}

geProject::Camera::~Camera(){}

glm::mat4 geProject::Camera::getViewMatrix(){
	//view = glm::mat4(0.0f);
	view = glm::lookAt(position, position + direction, up);
	viewInv = glm::inverse(view);
	return view; 
}

void geProject::Camera::projectionUpdate() {
	projection = glm::mat4(1.0f);	
	//NEED TO UPDATE -- CURRENTLY SET TO 1080 BY 672
	projection = glm::ortho(0.0f, projSize[0], 0.0f, projSize[1]);
	projInv = glm::inverse(projection);	
}

glm::mat4 geProject::Camera::getProjection() {
	return projection;
}
glm::mat4 geProject::Camera::getViewMatrixInverse() { 
	glm::mat4 view = getViewMatrix();
	return viewInv; 
}
glm::mat4 geProject::Camera::getProjectionInverse() {
	//projectionUpdate();
	return projInv; 
}

float geProject::Camera::getCameraY() { return position.y; }

glm::vec2 geProject::Camera::getProjSize() { return projSize; }

glm::vec3 geProject::Camera::getPosition() {	return position;}
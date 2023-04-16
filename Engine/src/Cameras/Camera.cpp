#include "Camera.h"

geProject::Camera::Camera() {
	projectionUpdate();
	eventSystem.subscribe(this, &Camera::cameraMouseScrolled);	
}

geProject::Camera::~Camera(){}

glm::mat4 geProject::Camera::getViewMatrix(){
	view = glm::lookAt(position , position + direction, up);
	viewInv = glm::inverse(view);
	return view; 
}

void geProject::Camera::projectionUpdate() {
	projection = glm::mat4(1.0f);	
	projection = glm::ortho(0.0f, projSize[0] * scroll, 0.0f, projSize[1] * scroll);	
	projInv = glm::inverse(projection);	
	eventSystem.publishImmediately(new ProjectionEvent(GameplayContext | EditorContext | GameplayContext, projInv, viewInv));
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




glm::vec2 geProject::Camera::getProjSize() { return projSize; }

glm::vec2 geProject::Camera::getPosition() {
	projectionUpdate();
	return glm::vec2(position[0], position[1]);
}

void geProject::Camera::setPosition(glm::vec2 pos) { 
	position -= glm::vec3(pos , 0);	 
	std::cout <<"positionX: " << position.x << " positionY: " << position.y << std::endl;
	projectionUpdate();
}

void geProject::Camera::setScroll(float scale) { 	
	scroll += scale;	

}


float geProject::Camera::getScroll() { 
	return scroll; 
}

void geProject::Camera::cameraMouseScrolled(MouseScrollEvent* scroll) {
	if (scroll->contextCheck(Context::GameplayContext) || scroll->contextCheck(Context::EditorContext)) {
		//std::cout << "Context: " << scroll->getContexts() << " Type: " << scroll->getType() << std::endl;
		auto scrollval = scroll->yScroll;
		if (scroll->yScroll != 0.0f && (scroll->screenX >= 0.0f && scroll->screenX <= 1920.0f) && (scroll->screenY >= 0.0f && scroll->screenY <= 1080.0f)) {
			int signum = (0 < scrollval) - (scrollval < 0);
			float newScroll = pow(abs(scrollval), 1 / getScroll()) * 0.2f * -signum;			
			setScroll(newScroll);				
		}
	}
}

void geProject::Camera::setCentredPosition(float x, float y){
	x = x - (projSize.x / 2.0f);
	y = y - (projSize.y / 2.0f);
	position = glm::vec3(x, y , 0);

}


glm::vec2 geProject::Camera::getCentredPosition(){
	return glm::vec2(position.x, position.y) + (projSize / 2.0f);
}


void geProject::Camera::update(float dt) {
	deltaTime = dt;
	eventSystem.handleEvents(Type::mousePressed);
	eventSystem.handleEvents(Type::mouseMove);
	eventSystem.handleEvents(Type::mouseScroll);
}


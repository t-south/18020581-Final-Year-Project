#include "EditorCamera.h"
#include <iostream>

geProject::EditorCamera::EditorCamera(glm::vec2 pos){
	position = glm::vec3(pos, 0.0f);
	eventSystem.subscribe(this, &EditorCamera::cameraMouseButton);
	eventSystem.subscribe(this, &EditorCamera::cameraMouseScrolled);
	eventSystem.subscribe(this, &EditorCamera::mouseMoved);
	projectionUpdate();
}

void geProject::EditorCamera::update(float dt) {
	projectionUpdate();
	deltaTime = dt;
	eventSystem.handleEvents(Type::mousePressed);
	eventSystem.handleEvents(Type::mouseMove);
	eventSystem.handleEvents(Type::mouseScroll);
}


void geProject::EditorCamera::cameraMouseButton(MouseButtonEvent* mouse){		
	if (mouse->mouseButton == GLFW_MOUSE_BUTTON_MIDDLE && mouse->mouseButtonDown == GLFW_PRESS) {			
		mouseDown = true;
	}
	else if (GLFW_MOUSE_BUTTON_MIDDLE && mouse->mouseButtonDown == GLFW_RELEASE) {
		mouseDown = false;
	}	
}


void geProject::EditorCamera::mouseMoved(MouseMoveEvent* mouse) {
	//std::cout << "Mouse X: " << drag << std::endl;
	projectionUpdate();
	if (mouseDown) {
		if (prevClick[0] == 0 && prevClick[1] == 0) {
			prevClick[0] = mouse->posX;
			prevClick[1] =  mouse->posY;
			drag -= deltaTime;
		}
		drag -= deltaTime;
		glm::vec2 cameraPos = glm::vec2(mouse->posX, mouse->posY);
		cameraPos = prevClick - cameraPos;
		cameraPos = getPosition() - cameraPos * (deltaTime * 2.0f);
		//interpolate between origin of drag and the new camera position
		prevClick[0] = prevClick[0] * (1.0 - deltaTime) + (mouse->posX * deltaTime);
		prevClick[1] = prevClick[1] * (1.0 - deltaTime) + (mouse->posY * deltaTime);
		setPosition(getPosition() - cameraPos);
		//std::cout << "cameraPosX : " << cameraPos[0] << " cameraPosY: " << cameraPos[1] << std::endl;
	}
	if (drag <= 0 && !mouseDown) {
		drag = 0.1f;
	}	

}

void geProject::EditorCamera::cameraMouseScrolled(MouseScrollEvent* scroll){
	//std::cout << "Context: " << scroll->getContexts() << " Type: " << scroll->getType() << std::endl;
	auto scrollval = scroll->yScroll;
	if (scrollval != 0.0f && (scroll->screenX >= 0.0f && scroll->screenX <= 1920.0f) && (scroll->screenY >= 0.0f && scroll->screenY <= 1080.0f)) {		
		int signum = (0 < scrollval) - (scrollval < 0);
		setScroll(pow(abs(scrollval), 1 / getScroll()) * 0.2f * -signum);
	}

}











#include "EditorCamera.h"
#include <iostream>


geProject::EditorCamera::EditorCamera(Camera& cam) {
	camera = &cam;

}

geProject::EditorCamera::~EditorCamera() {}


void geProject::EditorCamera::update(float deltaTime) {
	if (mouseListen->mouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE) && drag > 0) {
		prevClick = glm::vec2(mouseListen->getCameraXpos(), mouseListen->getCameraYpos());
		drag -= deltaTime;

	}
	else if (mouseListen->mouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE)) {
		glm::vec2 cameraPos = glm::vec2(mouseListen->getCameraXpos(), mouseListen->getCameraYpos());
		cameraPos = (prevClick - cameraPos);
		cameraPos = camera->getPosition() - cameraPos * (deltaTime * 10.0f);
		//interpolate between origin of drag and the new camera position
		prevClick[0] = prevClick[0] * (1.0 - deltaTime) + (mouseListen->getCameraXpos() * deltaTime);
		prevClick[1] = prevClick[1] * (1.0 - deltaTime) + (mouseListen->getCameraYpos() * deltaTime);
		camera->setPosition(camera->getPosition() - cameraPos);
	}
	if (drag <= 0 && !mouseListen->mouseButtonDown(GLFW_MOUSE_BUTTON_MIDDLE)) {
		drag = 0.1f;
	}
	float mouseX = mouseListen->getScreenXpos();
	float mouseY = mouseListen->getScreenYpos();

	if (mouseListen->getYscroll() != 0.0f && (mouseX >= 0.0f && mouseX <= 1920.0f) && (mouseY >= 0.0f && mouseY <= 1080.0f)) {
		auto scrollval = mouseListen->getYscroll();
		int signum = (0 < scrollval) - (scrollval < 0);
		camera->setScroll(pow(abs(mouseListen->getYscroll()), 1 / camera->getScroll()) * 0.2f * -signum);
	}


}

geProject::Camera* geProject::EditorCamera::getCamera() {
	return camera;
}

void geProject::EditorCamera::setKeyboardListener(KeyboardListener& key) { keyboard = &key; }
void geProject::EditorCamera::setMouseListener(MouseListener& mouse) { mouseListen = &mouse; }
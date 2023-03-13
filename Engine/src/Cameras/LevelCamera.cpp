#include "LevelCamera.h"


geProject::LevelCamera::LevelCamera(MouseListener& mouse, Camera& camera) {
	mouseListen = &mouse;	
}

geProject::LevelCamera::~LevelCamera() {}

void geProject::LevelCamera::update(float deltaTime){}
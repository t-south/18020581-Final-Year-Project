#include "LevelCamera.h"


geProject::LevelCamera::LevelCamera(glm::vec2 pos) {
	position = glm::vec3(pos, 0.0f);   
	projectionUpdate();
}




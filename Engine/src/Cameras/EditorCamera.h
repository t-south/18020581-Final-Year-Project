#pragma once

#include "Camera.h"
#include <cmath>

namespace geProject {
	class EditorCamera : public Camera {
	public:	
		EditorCamera(glm::vec2 pos);		
		void cameraMouseButton(MouseButtonEvent* mouse);
		void cameraMouseScrolled(MouseScrollEvent* scroll);
		void mouseMoved(MouseMoveEvent* event);	
		float prevX{ 0 };
		float prevY{ 0 };
		void update(float dt);
		glm::vec2 cameraPos;

	};
}
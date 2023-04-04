#pragma once

#include "Camera.h"
#include <cmath>

namespace geProject {
	class EditorCamera : public Camera {
	public:	
		EditorCamera(glm::vec2 pos);		
		void cameraMouseButton(MouseButtonEvent* mouse);		
		void mouseMoved(MouseMoveEvent* event);	
		float prevX{ 0 };
		float prevY{ 0 };		
		glm::vec2 cameraPos;
		void updateKeyPress(KeyPressedEvent* e);		
	};
}
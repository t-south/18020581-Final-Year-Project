#pragma once
#include "Camera.h"
#include "../Inputs/MouseListener.h"
#include "../Inputs/KeyboardListener.h"
#include <cmath>

namespace geProject {
	class EditorCamera {
	public:
		EditorCamera(Camera& cam);
		~EditorCamera();
		void update(float deltaTime);
		Camera* getCamera();
		void setMouseListener(MouseListener& mouse);
		void setKeyboardListener(KeyboardListener& keyboard);
	private:
		glm::vec2 prevClick{ 0.0f, 0.0f };
		float drag{ 0.1f };
		MouseListener* mouseListen;
		KeyboardListener* keyboard;
		Camera* camera;

	};
}
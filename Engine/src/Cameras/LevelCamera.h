#pragma once
#include "Camera.h"

namespace geProject {
	class LevelCamera {
	public:
		LevelCamera(MouseListener& mouse, Camera& camera);
		~LevelCamera();
		void update(float deltaTime);

	private:
		glm::vec2 prevClick;
		float drag{ 0.1f };
		MouseListener* mouseListen;
		Camera* camera;
	};
}
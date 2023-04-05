#pragma once
//using GLM library --- https: //github.com/g-truc/glm ---
#define GLFW_INCLUDE_NONE
#include <glm/vec2.hpp>
#include <examples/libs/glfw/include/GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"

#include <ge_engine/Core.h>
#include <iostream>
#include <cmath>

//orthographic camera for 2d games
//converts from world coordinates to normalised device coordinates
namespace geProject {
	class Camera {
	public:		
		Camera();
		virtual ~Camera();
		void projectionUpdate();
		glm::mat4 getViewMatrix();
		glm::mat4 getProjection();
		glm::mat4 getViewMatrixInverse();
		glm::mat4 getProjectionInverse();
		//position of objects on screen
		glm::vec2 getProjSize();
		glm::vec2 getPosition();
		void setPosition(glm::vec2 pos);
		void setScroll(float scale);
		float getScroll();	
		void cameraMouseScrolled(MouseScrollEvent* scroll);
		void setCentredPosition(float x, float y);
		virtual void update(float dt);		
		glm::vec2 getCentredPosition();
	protected:		
		//camera facing direction		
		glm::vec3 position;
		glm::vec3 direction{ 0.0f, 0.0f, -1.0f };
		//x axis
		glm::vec3 right;
		//y axis is upward direction
		glm::vec3 up{ 0.0f, 1.0f, 0.0f };
		glm::vec2 projSize{ 6.0f, 3.0f };
		//where is the camera in relation to world
		glm::mat4 view, viewInv;
		//maps to screen size
		glm::mat4 projection, projInv;
		glm::vec2 prevClick{0,0};
		float drag{ 0.0f };
		float scroll{ 1.0f };
		float deltaTime{ 0.0f };
		bool mouseDown{ false };
		bool scrollTest;
	};
}
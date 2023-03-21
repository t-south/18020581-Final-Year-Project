#pragma once
//using GLM library --- https: //github.com/g-truc/glm ---
#define GLFW_INCLUDE_NONE
#include <glm/vec2.hpp>
#include <examples/libs/glfw/include/GLFW/glfw3.h>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include <ge_engine/Core.h>

//orthographic camera for 2d games
//converts from world coordinates to normalised device coordinates
namespace geProject {
	class Camera {
	public:		
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
		virtual void update(float dt) = 0;		
	protected:		
		//camera facing direction		
		static glm::vec3 position;
		static glm::vec3 direction;
		//x axis
		static glm::vec3 right;
		//y axis is upward direction
		static glm::vec3 up;
		static glm::vec2 projSize;
		//where is the camera in relation to world
		static glm::mat4 view, viewInv;
		//maps to screen size
		static glm::mat4 projection, projInv;
		static glm::vec2 prevClick;
		float drag{ 0.0f };
		static float scroll;
		float deltaTime{ 0.0f };
		bool mouseDown{ false };			
	};
}
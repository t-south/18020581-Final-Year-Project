#pragma once
//using GLM library --- https: //github.com/g-truc/glm ---
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "../Inputs/MouseListener.h"

//orthographic camera for 2d games
//converts from world coordinates to normalised device coordinates
namespace geProject {
	class Camera {
	public:
		Camera(glm::vec2 pos);
		~Camera();
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
	private:		
		//camera facing direction		
		glm::vec3 position;
		glm::vec3 direction;
		//x axis
		glm::vec3 right;
		//y axis is upward direction
		glm::vec3 up;
		glm::vec2 projSize;
		//where is the camera in relation to world
		glm::mat4 view, viewInv;
		//maps to screen size
		glm::mat4 projection, projInv;
		float scroll;
	
	};
}
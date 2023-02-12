#pragma once
//using GLM library --- https: //github.com/g-truc/glm ---
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
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
		//position of objects on screen
		glm::vec3 position;
	private:
		//camera facing direction
		glm::vec3 direction;
		//x axis
		glm::vec3 right;
		//y axis is upward direction
		glm::vec3 up;
		
		//where is the camera in relation to world
		glm::mat4 view;
		//maps to screen size
		glm::mat4 projection;
	};
}
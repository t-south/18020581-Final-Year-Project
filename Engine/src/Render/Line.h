#pragma once
#include <glm/glm.hpp>
namespace geProject {
	class Line {
	public:
		Line(glm::vec2 origin, glm::vec2 destination,glm::vec3 color, unsigned int lifeTime);
		~Line();
		glm::vec2 getOrigin();
		glm::vec2 getDestination();
		glm::vec3 getColor();
		unsigned int getLife();
		unsigned int changeFrame();
	private:
		glm::vec2 origin;
		glm::vec2 destination;
		glm::vec3 color;
		unsigned int lifeTime;

	};
}
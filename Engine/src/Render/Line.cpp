#include "Line.h"

geProject::Line::Line(glm::vec2 origin, glm::vec2 destination, glm::vec3 color, unsigned int lifeTime):origin(origin), destination(destination), color(color), lifeTime(lifeTime){}
geProject::Line::~Line(){}

glm::vec2 geProject::Line::getOrigin() {
	return origin;
}
glm::vec2 geProject::Line::getDestination() {
	return destination;
}
glm::vec3 geProject::Line::getColor() {
	return color;
}
unsigned int geProject::Line::getLife() {
	return lifeTime;
}

unsigned int geProject::Line::changeFrame() {
	return lifeTime--;
}
#include "RayCastListener.h"

geProject::RayCastListener::RayCastListener(): point(glm::vec2()), normal(glm::vec2()), fraction(0) {

}

geProject::RayCastListener::~RayCastListener(){

}

float geProject::RayCastListener::ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction)
{
	return 0.0f;
}

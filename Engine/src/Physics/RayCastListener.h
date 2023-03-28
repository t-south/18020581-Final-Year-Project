#pragma once
#include <box2d/box2d.h>
#include <glm/glm.hpp>


namespace geProject {

	class RayCastListener : public b2RayCastCallback {
	public:
		RayCastListener();
		~RayCastListener();
		//b2Fixture fixture;
		glm::vec2 point;
		glm::vec2 normal;
		float fraction;
		bool hit;
		int entityId;
		int targetEntityId;
	
		virtual float ReportFixture(b2Fixture* fixture, const b2Vec2& point,
			const b2Vec2& normal, float fraction) override;
	};
}
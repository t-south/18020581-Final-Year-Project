#pragma once
#include <box2d/box2d.h>
#include <glm/glm.hpp>
#include <ge_engine/Entity.h>


namespace geProject {
	//credit to box2d.org/documentation/md__d_1__git_hub_box2d_docs_dynamics.html#autotoc_md119
	class RayCastListener : public b2RayCastCallback {
	public:
		RayCastListener(int entityId);
		~RayCastListener();
		b2Fixture* rFixture;
		b2Vec2 rPoint;
		b2Vec2 rNormal;
		float rFraction;
		bool rHit;
		int entityId;
		int targetEntityId;
	
		virtual float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction) override;
	};
}
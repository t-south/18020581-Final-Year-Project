#pragma once
#include <box2d/box2d.h>
#include <glm/glm.hpp>
#include <ge_engine/Entity.h>



namespace geProject {
	//credit github.com/behdad/box2d/blob/master/Box2D/Testbed/Tests/RayCast.h
	class RayCastCallback : public b2RayCastCallback
	{
	public:
		RayCastCallback()
		{
			m_hit = false;	
		}

		float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction)
		{
			b2Body* body = fixture->GetBody();
			Entity* userData = (Entity*)body->GetUserData().pointer;

			//enemy projectiles are ignored for the raycast
			if (userData->type == enemyprojectile) {
				return -1.0f;
			}
			if (userData->type == environment) {
				environmentHit = true;
			}
			raycasthit.push_back(*userData);
			if (userData)
			{
				int32 index = *(int32*)userData;
				if (index == 0)
				{
					// By returning -1, we instruct the calling code to ignore this fixture and
					// continue the ray-cast to the next fixture.
					return -1.0f;
				}
			}

			m_hit = true;
			m_point = point;
			m_normal = normal;

			// By returning the current fraction, we instruct the calling code to clip the ray and
			// continue the ray-cast to the next fixture. WARNING: do not assume that fixtures
			// are reported in order. However, by clipping, we can always get the closest fixture.
			if (fraction < minFraction) {
				minFraction = fraction;
			}
			
			return fraction;
		}

		bool m_hit;
		b2Vec2 m_point;
		b2Vec2 m_normal;
		float minFraction{ 1 };
		bool environmentHit{ false };	
		std::vector<Entity> raycasthit;
	};
}
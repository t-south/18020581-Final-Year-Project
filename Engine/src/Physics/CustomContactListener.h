#pragma once
#include <box2d/box2d.h>
#include <glm/vec2.hpp>
#include <ge_engine/Components.h>
#include <ge_engine/Entity.h>
#include <ge_engine/Core.h>

namespace geProject {
	//credit to box2d.org/documentation/md__d_1__git_hub_box2d_docs_dynamics.html#autotoc_md107
	class CustomContactListener : public b2ContactListener {
	public:
		CustomContactListener();
		~CustomContactListener();
		//BOX2D CALLBACKS
		void BeginContact(b2Contact* contact);
		void EndContact(b2Contact* contact);
		void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
		void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
	};
	
	
}
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "../Render/Shader.h"
#include "../Render/Camera.h"
#include "../Render/Texture.h"
#include <ge_engine/Entity.h>
#include "../EntityManager/EntityManager.h"
#include "../Render/Renderer.h"
namespace geProject {
//abstract scene class
	class Scene {
	public:
		virtual void update(float deltaTime) = 0;
		virtual void init() = 0;
		virtual void addEntityToScene(unsigned int entityId) = 0;

		virtual Camera* getCamera() = 0;
	protected:
		std::vector<Entity*> entities;
		EntityManager* manager;
		Renderer* renderer;
	};
}
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "../Render/Shader.h"
#include "../Render/Camera.h"
#include "../Render/Texture.h"
namespace geProject {
//abstract scene class
	class Scene {
	public:
		virtual void update(float deltaTime) = 0;
		virtual void init() = 0;
	};
}
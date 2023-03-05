#pragma once
#include <memory>
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include <imgui.h>
#include "../SceneManager/Scene.h"
#include <GLFW/glfw3.h>
//#include "../Render/Window.h"


namespace geProject {
	class ImguiWindow {
	public:
		ImguiWindow(GLFWwindow* window);
		~ImguiWindow();		
		void start(int display_w, int display_h, float mouse_x, float mouse_y);
		void update(float deltaTime, std::shared_ptr<Scene> scene);
		void destroyWindow();
		void onEvent();
	private:
		GLFWwindow* window;
	
	};
}
#pragma once
#include <memory>
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
		void dockWindow(int width, int height);
	};
}
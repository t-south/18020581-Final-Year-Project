#pragma once
#include "../IMGui/imgui_impl_opengl3.h"
#include "../IMGui/imgui_impl_glfw.h"
#include <imgui.h>
#include <memory>
#include "../SceneManager/Scene.h"
#include "../Render/FrameBuffer.h"
#include <GLFW/glfw3.h>
#include <ge_engine/Core.h>



//#include "../Render/Window.h"


namespace geProject {
	class ImguiWindow {
	public:
		ImguiWindow(GLFWwindow* window, int windowWidth, int windowHeight, FrameBuffer* fBuffer);
		~ImguiWindow();		
		void start(int display_w, int display_h, float mouse_x, float mouse_y);
		void update(float deltaTime, std::shared_ptr<Scene> scene);
		void destroyWindow();
		void onEvent();
		ImVec2 getViewPos();
		ImVec2 getViewSize();		
	private:
		GLFWwindow* window;
		FrameBuffer* frameBuffer;
		
		int windowWidth, windowHeight;
		ImVec2 viewPos, viewSize;
		void dockWindow();
		bool windowRunning{ false };
		void gameViewWindow();
		bool loadLevel{ false };		
		ImVec2 getMaxViewPort();
	};
}
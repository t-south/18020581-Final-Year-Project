#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>
#include <ge_engine/Core.h>

namespace geProject {
	class Window
	{
	public:
		Window(const char* m_title, int m_width, int m_height);
		~Window();
		//void loop();		
		GLFWwindow* getWindow();
		int getWidth();
		int getHeight();
		static void window_size_callback(GLFWwindow* window, int width, int height);
		void startGamePlay(GameStartEvent* start);
		void stopGamePlay(GameStopEvent* stop);
	private:
		int width, height;
		const char* title;
		GLFWwindow* window;			
		//MouseListener* mouse;
		//KeyboardListener* keyboard;
		//SceneStates* sceneManager;
		//std::shared_ptr<geProject::Scene> getScene();
		//Time* clock;
		static void glfwError(int id, const char* description);
	};
}

#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <system_error>
#include <iostream>
#include <memory>
#include "../Inputs/MouseListener.h"
#include "../Inputs/KeyboardListener.h"
#include "../Clock/Clock.h"
#include "../SceneManager/SceneStates.h"
#include "../SceneManager/LevelEditorScene.h"
#include "../SceneManager/LevelScene.h"
#include "../Systems/SpriteSystems.h"
#include "../IMGui/ImguiWindow.h"
#include "../SceneManager/SceneSerialize.h"


namespace geProject {
	class Window
	{
	public:
		Window(const char* m_title, int m_width, int m_height);
		~Window();
		void loop();
		
	private:
		int width, height;
		const char* title;
		GLFWwindow* window;	
		MouseListener* mouse;
		ImguiWindow* imguiwindow;
		KeyboardListener* keyboard;
		SceneStates* sceneManager;
		std::shared_ptr<geProject::Scene> getScene();
		//Time* clock;
		static void glfwError(int id, const char* description);
		static void window_size_callback(GLFWwindow* window, int width, int height);
	};
}

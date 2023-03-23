#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <backends/imgui_impl_glfw.h>
#include <ge_engine/Core.h>

namespace geProject {
	class KeyboardListener {
	public:
		
		static KeyboardListener* getInstance();
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);		
		static bool isKeyPressed(int key);		
		void endFrame();
	private:
		KeyboardListener() {};
		KeyboardListener(const KeyboardListener& obj) = delete;
		std::unordered_map<int, bool> keyPressed;
		static KeyboardListener* instance;
		~KeyboardListener();

	};
}
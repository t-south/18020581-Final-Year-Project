#pragma once
#include <GLFW/glfw3.h>

namespace geProject {
	class KeyboardListener {
	public:
		KeyboardListener();
		static KeyboardListener& getInstance();
		static bool keyPressed[350];
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		bool isKeyPressed(int key);
	private:
		
		~KeyboardListener();
		
	};
}
#pragma once
#include <GLFW/glfw3.h>

namespace geProject {
	class KeyboardListener {
	public:
		
		static KeyboardListener* getInstance();
		static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);		
		static bool isKeyPressed(int key);
	private:
		KeyboardListener() {};
		KeyboardListener(const KeyboardListener& obj) = delete;
		bool keyPressed[350] = { 0 };
		static KeyboardListener* instance;
		~KeyboardListener();
		
	};
}
#pragma once
#include <GLFW/glfw3.h>

namespace geProject {
	class MouseListener {
	public:
		static MouseListener& getInstance();
		static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void endFrame();
		//getters
		static float getXpos();
		static float getYpos();
		static float getXprev();
		static float getYprev();
		static float getXscroll();
		static float getYscroll();
		static float getXdiff();
		static float getYdiff();
		static bool isDrag();
		static bool mouseButtonDown(int button);

	private:
		MouseListener();
		~MouseListener();
		static MouseListener instance;
		static double xPos, yPos, xPrev, yPrev, xScroll, yScroll;
		static bool isDragging;
		static bool mouseButton[3];
	};
}
#pragma once
#include <GLFW/glfw3.h>

namespace geProject {
	class MouseListener {
	public:
		
		static MouseListener* getInstance();
		static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
		static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		void endFrame();
		//getters
		float getXpos();
		float getYpos();
		float getXprev();
		float getYprev();
		float getXscroll();
		float getYscroll();
		float getXdiff();
		float getYdiff();
		bool isDrag();
		bool mouseButtonDown(int button);

	private:
		MouseListener() {};
		MouseListener(const MouseListener& obj) = delete;
		static MouseListener* instance;
		~MouseListener();		
		double xPos, yPos, xPrev, yPrev, xScroll, yScroll = 0;
		bool isDragging = false;
		bool mouseButton[3] = {false, false, false};
	};
}
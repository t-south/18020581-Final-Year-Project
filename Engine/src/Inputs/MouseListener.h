#pragma once
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
//#include <glm/mat4x4.hpp>
#include <memory>

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
		float getCameraXpos();
		float getCameraYpos();
		bool isDrag();
		bool mouseButtonDown(int button);
		void setWindowDimensions(int windowW, int windowH);
		void setInverses(glm::mat4 inverseProj, glm::mat4 inverseView);

	private:
		MouseListener();
		MouseListener(const MouseListener& obj) = delete;
		~MouseListener();		
		static MouseListener* instance;
		double xPos, yPos, xPrev, yPrev, xScroll, yScroll;
		bool isDragging = false;
		int windowWidth, windowHeight;		
		bool mouseButton[3] = {false, false, false};
		glm::mat4 projectionInv, viewInv;
	};
}
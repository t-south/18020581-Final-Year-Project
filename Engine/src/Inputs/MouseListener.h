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
		float getScreenXpos();
		float getScreenYpos();
		float getCameraXpos();
		float getCameraYpos();
		float getViewXsize();
		float getViewYsize();
		bool isMouseButtonDown(int button);
		bool isDrag();
		bool mouseButtonDown(int button);
		void releaseMouseButton(int button);
		void setWindowDimensions(int windowW, int windowH);
		void setInverses(glm::mat4 inverseProj, glm::mat4 inverseView);
		void setViewSize(float x, float y);
		void setViewPos(float x, float y);

	private:
		MouseListener();
		MouseListener(const MouseListener& obj) = delete;
		~MouseListener();
		static MouseListener* instance;
		double xPos, yPos, xPrev, yPrev, xScroll, yScroll;
		bool isDragging = false;
		int windowWidth, windowHeight;
		bool mouseButton[3] = { false, false, false };
		glm::vec2 viewSize, viewPos;
		glm::mat4 projectionInv, viewInv;
	};
}
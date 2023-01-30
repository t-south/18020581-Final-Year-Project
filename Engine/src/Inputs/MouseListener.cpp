#include <ge_engine/MouseListener.h>

geProject::MouseListener::MouseListener() {};

geProject::MouseListener& geProject::MouseListener::getInstance() {
    return instance;
}


/* docs from https://www.glfw.org/docs/3.3/input_guide.html */

void geProject::MouseListener::cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    xPrev = xPos;
    yPrev = yPos;
    xPos = xPos;
    xPos = yPos;
    isDragging = mouseButton[0] || mouseButton[1] || mouseButton[2];
}

void geProject::MouseListener::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS) {
        mouseButton[button] = true;
    }
    else if (action == GLFW_RELEASE) {
        mouseButton[button] = false;
        isDragging = false;
    }
}

void geProject::MouseListener::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    xScroll = xoffset;
    yScroll = yoffset;
}

void geProject::MouseListener::endFrame() {
    xPos = 0;
    xPos = 0;
    xPrev = xPos;
    yPrev = yPos;
}

float geProject::MouseListener::getXpos() { return xPos; };
float geProject::MouseListener::getYpos() { return yPos; };
float geProject::MouseListener::getXprev(){ return xPrev; };
float geProject::MouseListener::getYprev() { return yPrev; };
float geProject::MouseListener::getXscroll() { return xScroll; };
float geProject::MouseListener::getYscroll() { return yScroll; };
float geProject::MouseListener::getXdiff() { return xPos - xPrev; };
float geProject::MouseListener::getYdiff() { return yPos - yPrev; };
bool geProject::MouseListener::isDrag() { return isDragging; };
bool geProject::MouseListener::mouseButtonDown(int button) { return mouseButton[button]; };

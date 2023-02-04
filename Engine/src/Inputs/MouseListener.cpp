#include "MouseListener.h"

geProject::MouseListener* geProject::MouseListener::instance = nullptr;
/* docs from https ://www.glfw.org/docs/3.3/input_guide.html */

geProject::MouseListener* geProject::MouseListener::getInstance() {
    if (instance == NULL)
    {
        instance = new MouseListener();
        return instance;
    }
    else
    {
        return instance;
    }
}

void geProject::MouseListener::cursor_position_callback(GLFWwindow* window, double newX, double newY)
{
    geProject::MouseListener::getInstance()->xPrev = geProject::MouseListener::getInstance()->xPos;
    geProject::MouseListener::getInstance()->yPrev = geProject::MouseListener::getInstance()->yPos;
    geProject::MouseListener::getInstance()->xPos = newX;
    geProject::MouseListener::getInstance()->xPos = newY;
    geProject::MouseListener::getInstance()->isDragging = geProject::MouseListener::getInstance()->mouseButton[0] 
        || geProject::MouseListener::getInstance()->mouseButton[1] || geProject::MouseListener::getInstance()->mouseButton[2];
}

void geProject::MouseListener::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS) {
        geProject::MouseListener::getInstance()->mouseButton[button] = true;
    }
    else if (action == GLFW_RELEASE) {
        geProject::MouseListener::getInstance()->mouseButton[button] = false;
        geProject::MouseListener::getInstance()->isDragging = false;
    }
}

void geProject::MouseListener::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    geProject::MouseListener::getInstance()->xScroll = xoffset;
    geProject::MouseListener::getInstance()->yScroll = yoffset;
}

void geProject::MouseListener::endFrame() {
    xPos = 0;
    xPos = 0;
    xPrev = xPos;
    yPrev = yPos;
}

float geProject::MouseListener::getXpos() { return (float)xPos; };
float geProject::MouseListener::getYpos() { return (float)yPos; };
float geProject::MouseListener::getXprev(){ return (float)xPrev; };
float geProject::MouseListener::getYprev() { return (float)yPrev; };
float geProject::MouseListener::getXscroll() { return (float)xScroll; };
float geProject::MouseListener::getYscroll() { return (float)yScroll; };
float geProject::MouseListener::getXdiff() { return (float)(xPos - xPrev); };
float geProject::MouseListener::getYdiff() { return (float)(yPos - yPrev); };
bool geProject::MouseListener::isDrag() { return isDragging; };
bool geProject::MouseListener::mouseButtonDown(int button) { return mouseButton[button]; };

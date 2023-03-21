#include "MouseListener.h"
#include <iostream>

geProject::MouseListener* geProject::MouseListener::instance = nullptr;
/* docs from https ://www.glfw.org/docs/3.3/input_guide.html */

geProject::MouseListener::MouseListener() : xPos(0), yPos(0), xPrev(0), yPrev(0), xScroll(0), yScroll(0), windowWidth(0), windowHeight(0) {
    eventSystem.subscribe(this, &MouseListener::updateViewPort);
}

geProject::MouseListener::~MouseListener()
{
}

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

void geProject::MouseListener::cursor_position_callback(GLFWwindow* window, double newX, double newY) {
    eventSystem.publish(new MouseMoveEvent(MouseListener::getInstance()->getCameraMouseX(), MouseListener::getInstance()->getCameraMouseY()));
    MouseListener::getInstance()->xPrev = MouseListener::getInstance()->xPos;
    MouseListener::getInstance()->yPrev = MouseListener::getInstance()->yPos;
    MouseListener::getInstance()->xPos = newX;
    MouseListener::getInstance()->yPos = newY;
 
    //std::cout << "x pos: " << geProject::MouseListener::getInstance()->getXpos() << "  y pos:  " << geProject::MouseListener::getInstance()->getYpos() << std::endl;

}

void geProject::MouseListener::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        eventSystem.publish(new MouseButtonEvent(button, GLFW_PRESS, MouseListener::getInstance()->xPos, MouseListener::getInstance()->yPos));
        geProject::MouseListener::getInstance()->mouseButton[button] = true;
    }
    else if (action == GLFW_RELEASE) {
        eventSystem.publish(new MouseButtonEvent(button, GLFW_RELEASE, MouseListener::getInstance()->xPos, MouseListener::getInstance()->yPos));
        geProject::MouseListener::getInstance()->mouseButton[button] = false;
        geProject::MouseListener::getInstance()->isDragging = false;
    }
}

void geProject::MouseListener::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {  
    eventSystem.publish(new MouseScrollEvent(xoffset, yoffset, MouseListener::getInstance()->xPos, MouseListener::getInstance()->yPos));
    MouseListener::getInstance()->xScroll = xoffset;
    MouseListener::getInstance()->yScroll = yoffset;
}

void geProject::MouseListener::endFrame() {
    MouseListener::getInstance()->yScroll = 0;
    MouseListener::getInstance()->xScroll = 0;
    MouseListener::getInstance()->xPrev = MouseListener::getInstance()->xPos;
    MouseListener::getInstance()->yPrev = MouseListener::getInstance()->yPos;
    eventSystem.handleEvents(Type::gameSave);
    eventSystem.handleEvents(Type::gameLoad);
    eventSystem.handleEvents(Type::gameStart);
    eventSystem.handleEvents(Type::gameStop);
}

float geProject::MouseListener::getXpos() { return (float)MouseListener::getInstance()->xPos; };
float geProject::MouseListener::getYpos() { return (float)MouseListener::getInstance()->yPos; };
float geProject::MouseListener::getXprev() { return (float)MouseListener::getInstance()->xPrev; };
float geProject::MouseListener::getYprev() { return (float)MouseListener::getInstance()->yPrev; };
float geProject::MouseListener::getXscroll() { return (float)MouseListener::getInstance()->xScroll; };
float geProject::MouseListener::getYscroll() { return (float)MouseListener::getInstance()->yScroll; };
float geProject::MouseListener::getXdiff() { return (float)(MouseListener::getInstance()->xPos - MouseListener::getInstance()->xPrev); };
float geProject::MouseListener::getYdiff() { return (float)(MouseListener::getInstance()->yPos - MouseListener::getInstance()->yPrev); };
bool geProject::MouseListener::isDrag() { return geProject::MouseListener::getInstance()->isDragging; };



float geProject::MouseListener::getCameraMouseX() {  
    /*
    float x = MouseListener::getInstance()->getXpos() - MouseListener::getInstance()->viewPos[0];
    glm::vec4 worldCoordX = glm::vec4((x / MouseListener::getInstance()->viewSize.x) * 2.0f - 1.0f, 0.0f, 0.0f, 1.0f);
    worldCoordX = MouseListener::getInstance()->viewInv * MouseListener::getInstance()->projectionInv * worldCoordX;
    glm::mat4 tmp = MouseListener::getInstance()->projectionInv;
    return worldCoordX.x;
    */
    
    float currentX = MouseListener::getInstance()->getXpos() - MouseListener::getInstance()->viewPos.x;
    currentX = (currentX / MouseListener::getInstance()->viewSize.x) * 2.0f - 1.0f;
    glm::vec4 tmp = glm::vec4(currentX, 0, 0, 1);  
    glm::vec4 worldCoordX = MouseListener::getInstance()->viewInv * MouseListener::getInstance()->projectionInv * tmp;
    return worldCoordX.x;
    
    
}

float geProject::MouseListener::getCameraMouseY() {   
    /*
    float y = MouseListener::getInstance()->viewPos[1] - MouseListener::getInstance()->getYpos();
    glm::vec4 worldCoordY = glm::vec4(0.0f, (y / MouseListener::getInstance()->viewSize.y) * 2.0f - 1.0f, 0.0f, 1.0f);
    worldCoordY = viewInv * projectionInv * worldCoordY;
    glm::mat4 tmp = MouseListener::getInstance()->projectionInv;
    return worldCoordY.y;
    */
    float currentY = MouseListener::getInstance()->getYpos() - MouseListener::getInstance()->viewPos.y;
    currentY = -((currentY / MouseListener::getInstance()->viewSize.y) * 2.0f - 1.0f);
    glm::vec4 tmp = glm::vec4(0, currentY, 0, 1);       
    glm::vec4  worldCoordY = MouseListener::getInstance()->viewInv * MouseListener::getInstance()->projectionInv * tmp;
    return worldCoordY.y;
    
    //return yPos;
}

float geProject::MouseListener::getScreenXpos() {
    float x = ((getXpos() - viewPos[0]) / getViewXsize()) * 1920.0f;
    //float x = ((xPos - viewPos.x) / viewSize.x) * 1920.0f;   

    /*
    float currentX = MouseListener::getInstance()->getXpos() - MouseListener::getInstance()->viewPos.x;
    currentX = (currentX / MouseListener::getInstance()->viewSize.x) * 1920.0f;
    return currentX;  
    */
    return x;
}


float geProject::MouseListener::getScreenYpos() {
    float y = ((viewPos[1] - getYpos()) / getViewYsize()) * 1080.0f;
    //float y = ((yPos - viewPos.x) / viewSize.y) * 1080.0f;

    /*
    float currentY = MouseListener::getInstance()->getYpos() - MouseListener::getInstance()->viewPos.y;
    currentY = 1080.0f - ((currentY / MouseListener::getInstance()->viewSize.y) * 1080.0f);
    return currentY;*/
    return y;
}




bool geProject::MouseListener::mouseButtonDown(int button) { return MouseListener::getInstance()->mouseButton[button]; };
void geProject::MouseListener::releaseMouseButton(int button) {MouseListener::getInstance()->mouseButton[button] = false; }


void geProject::MouseListener::setWindowDimensions(int windowW, int windowH) {
    MouseListener::getInstance()->windowWidth = windowW;
    MouseListener::getInstance()->windowHeight = windowH;
}


void geProject::MouseListener::setInverses(glm::mat4 inverseProj, glm::mat4 inverseView) {
    MouseListener::getInstance()->projectionInv = inverseProj;
    MouseListener::getInstance()->viewInv = inverseView;
}



bool geProject::MouseListener::isMouseButtonDown(int button) { return mouseButton[button]; }

void geProject::MouseListener::updateViewPort(ViewPortEvent* event) {
    viewPos.x = event->windowPosX;
    viewPos.y = event->windowPosY;
    viewSize.x = event->windowSizeX;
    viewSize.y = event->windowSizeY;
    std::cout << "ViewPos: " << viewPos.x << " " << viewPos.y << " ViewSize: " << viewSize.x << " " << viewSize.y << std::endl;
}

float geProject::MouseListener::getViewXsize() { return (float)geProject::MouseListener::getInstance()->viewSize.x; }
float geProject::MouseListener::getViewYsize() { return (float)geProject::MouseListener::getInstance()->viewSize.y; }
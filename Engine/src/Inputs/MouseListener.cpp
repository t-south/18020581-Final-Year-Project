#include "MouseListener.h"

geProject::MouseListener* geProject::MouseListener::instance = nullptr;
/* docs from https ://www.glfw.org/docs/3.3/input_guide.html */

geProject::MouseListener::MouseListener(): xPos(0), yPos(0), xPrev(0), yPrev(0), xScroll(0), yScroll(0), windowWidth(0), windowHeight(0){}

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

void geProject::MouseListener::cursor_position_callback(GLFWwindow* window, double newX, double newY){
    geProject::MouseListener::getInstance()->xPrev = geProject::MouseListener::getInstance()->xPos;
    geProject::MouseListener::getInstance()->yPrev = geProject::MouseListener::getInstance()->yPos;
    geProject::MouseListener::getInstance()->xPos = newX;
    geProject::MouseListener::getInstance()->yPos = newY;
    geProject::MouseListener::getInstance()->isDragging = geProject::MouseListener::getInstance()->mouseButton[0] 
    || geProject::MouseListener::getInstance()->mouseButton[1] || geProject::MouseListener::getInstance()->mouseButton[2];
    //std::cout << "x pos: " << geProject::MouseListener::getInstance()->getXpos() << "  y pos:  " << geProject::MouseListener::getInstance()->getYpos() << std::endl;
    
}

void geProject::MouseListener::mouse_button_callback(GLFWwindow* window, int button, int action, int mods){
    if (action == GLFW_PRESS) {
        geProject::MouseListener::getInstance()->mouseButton[button] = true;
    }
    else if (action == GLFW_RELEASE) {
        geProject::MouseListener::getInstance()->mouseButton[button] = false;
        geProject::MouseListener::getInstance()->isDragging = false;
    }
}

void geProject::MouseListener::scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    geProject::MouseListener::getInstance()->xScroll = xoffset;
    geProject::MouseListener::getInstance()->yScroll = yoffset;
}

void geProject::MouseListener::endFrame() {
    xPos = 0;
    xPos = 0;
    xPrev = xPos;
    yPrev = yPos;
}

float geProject::MouseListener::getXpos() { return (float)geProject::MouseListener::getInstance()->xPos; };
float geProject::MouseListener::getYpos() { return (float)geProject::MouseListener::getInstance()->yPos; };
float geProject::MouseListener::getXprev(){ return (float)geProject::MouseListener::getInstance()->xPrev; };
float geProject::MouseListener::getYprev() { return (float)geProject::MouseListener::getInstance()->yPrev; };
float geProject::MouseListener::getXscroll() { return (float)geProject::MouseListener::getInstance()->xScroll; };
float geProject::MouseListener::getYscroll() { return (float)geProject::MouseListener::getInstance()->yScroll; };
float geProject::MouseListener::getXdiff() { return (float)(geProject::MouseListener::getInstance()->xPos - geProject::MouseListener::getInstance()->xPrev); };
float geProject::MouseListener::getYdiff() { return (float)(geProject::MouseListener::getInstance()->yPos - geProject::MouseListener::getInstance()->yPrev); };
bool geProject::MouseListener::isDrag() { return geProject::MouseListener::getInstance()->isDragging; };
bool geProject::MouseListener::mouseButtonDown(int button) { return geProject::MouseListener::getInstance()->mouseButton[button]; };
float geProject::MouseListener::getCameraXpos(){
    glm::vec4 worldCoordX = glm::vec4((getXpos() / (float)windowWidth) * 2 - 1, 0.0f, 0.0f, 1.0f);
    //glm matrices are non commutative so order of operation is reversed
    worldCoordX = viewInv * projectionInv *  worldCoordX;
    return worldCoordX.x;
}

float geProject::MouseListener::getCameraYpos(){
    float yPos = windowHeight - getYpos();
    glm::vec4 worldCoordY = glm::vec4(0.0f, (yPos / (float)windowHeight) * 2 - 1, 0.0f, 1.0f);
    worldCoordY = viewInv * projectionInv *  worldCoordY;
    return worldCoordY.y;
    //return yPos;
}

void geProject::MouseListener::setWindowDimensions(int windowW, int windowH) {
    geProject::MouseListener::getInstance()->windowWidth = windowW;
    geProject::MouseListener::getInstance()->windowHeight = windowH;
}


void geProject::MouseListener::setInverses(glm::mat4 inverseProj, glm::mat4 inverseView) {
    projectionInv = inverseProj;
    viewInv = inverseView;
}



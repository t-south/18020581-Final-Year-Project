#include "KeyboardListener.h"

geProject::KeyboardListener* geProject::KeyboardListener::instance = nullptr;


geProject::KeyboardListener* geProject::KeyboardListener::getInstance() {
    if (instance == NULL)
    {
        instance = new KeyboardListener();
        return instance;
    }
    else
    {
        return instance;
    }
}


void geProject::KeyboardListener::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        geProject::KeyboardListener::getInstance()->keyPressed[key] = true;
    }
    else if (action == GLFW_RELEASE) {
        geProject::KeyboardListener::getInstance()->keyPressed[key] = false;
    }
}

bool geProject::KeyboardListener::isKeyPressed(int key) {
    return geProject::KeyboardListener::getInstance()->keyPressed[key];
}

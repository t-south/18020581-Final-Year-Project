#include <ge_engine/KeyboardListener.h>

geProject::KeyboardListener::KeyboardListener() {};


void geProject::KeyboardListener::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        keyPressed[key] = true;
    }
    else if (action == GLFW_RELEASE) {
        keyPressed[key] = false;
    }
}

bool geProject::KeyboardListener::isKeyPressed(int key) {
    return keyPressed[key];
}

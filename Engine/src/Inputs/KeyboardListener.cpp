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


void geProject::KeyboardListener::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    ImGuiIO& io = ImGui::GetIO();
        if (action == GLFW_PRESS) {        
            KeyboardListener::getInstance()->keyPressed[key] = true;
        }
        if (action == GLFW_RELEASE) {
            KeyboardListener::getInstance()->keyPressed[key] = false; 
        }
   
}

bool geProject::KeyboardListener::isKeyPressed(int key) {
    return KeyboardListener::getInstance()->keyPressed[key];
}

geProject::KeyboardListener::~KeyboardListener()
{
}

void geProject::KeyboardListener::endFrame(){
    std::vector<int> removalList;
    for (auto& i : keyPressed) {
        if (i.second) {            
            eventSystem.publish(new KeyPressedEvent(i.first, 0, true));
        }
        removalList.push_back(i.first);
    }  
    for (int i = 0; i < removalList.size(); i++) {
        keyPressed.erase(i);
    }
    
}



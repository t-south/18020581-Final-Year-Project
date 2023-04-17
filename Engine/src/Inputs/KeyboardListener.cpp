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
    Context currentContext = eventSystem.getContext();
    
    if (action == GLFW_PRESS) {            
        if (key == GLFW_KEY_C ) { 
            eventSystem.publishImmediately(new KeyPressedEvent(EditorContext | ImGuiContext, key, 0));           
        }
        else if (key == GLFW_KEY_DELETE) {
            eventSystem.publishImmediately(new KeyPressedEvent(EditorContext | ImGuiContext, key, 0));
        }
        else {
            eventSystem.publishImmediately(new KeyPressedEvent(GameplayContext | ImGuiContext, key, 0));
            KeyboardListener::getInstance()->keyPressed[key] = true;
        }
    }
    if (action == GLFW_RELEASE) {

        eventSystem.publishImmediately(new KeyReleasedEvent(GameplayContext | ImGuiContext, key, 0));
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
    Context currentContext = eventSystem.getContext();
    for (auto& i : keyPressed) {
        if (i.second) {            
            eventSystem.publish(new KeyPressedEvent(EditorContext, i.first, 0));
        }
        removalList.push_back(i.first);
    }  
    for (int i = 0; i < removalList.size(); i++) {
        keyPressed.erase(i);
    }
    
    
}



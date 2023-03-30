#include "Receiver.h"

geProject::Receiver::Receiver(){
	leftClick = new AttackCommand();
	rightClick = new ShieldCommand();
	buttonShift = new DashCommand();
	buttonW = new MoveCommand();;
	buttonA = new MoveCommand();;
	buttonS = new MoveCommand();;
	buttonD = new MoveCommand();
	buttonF = new SpecialAttackCommand();
	buttonTab = new SwitchAbilityCommand();
	eventSystem.subscribe(this, &Receiver::updateKeyButtonPress);
	eventSystem.subscribe(this, &Receiver::updateMouseButtonPress);
}

geProject::Command* geProject::Receiver::action(){	
	if (buttonPressed(GLFW_MOUSE_BUTTON_LEFT)) { return leftClick; }
	if (buttonPressed(GLFW_MOUSE_BUTTON_RIGHT)) { return rightClick; }
	if (buttonPressed(GLFW_KEY_LEFT_SHIFT || GLFW_KEY_RIGHT_SHIFT)) { return buttonShift; }
	if (buttonPressed(GLFW_KEY_W || GLFW_KEY_UP)) { return buttonW; }
	if (buttonPressed(GLFW_KEY_A || GLFW_KEY_LEFT)) { return buttonA; }
	if (buttonPressed(GLFW_KEY_S || GLFW_KEY_DOWN)) { return buttonS; }
	if (buttonPressed(GLFW_KEY_D || GLFW_KEY_RIGHT)) { return buttonD; }
	if (buttonPressed(GLFW_KEY_F)) { return buttonF; }
	if (buttonPressed(GLFW_KEY_TAB)) { return buttonTab; }
	return nullptr;
}

bool geProject::Receiver::buttonPressed(int button){	
	if (buttonevents.size() > 0) {
		if (buttonevents[button] == true) {
			buttonevents[button] = false;
			return true;
		}
	}
	return false;
}

void geProject::Receiver::updateMouseButtonPress(MouseButtonEvent* mouseevent){
	if (mouseevent->contextCheck(GameplayContext)) {
		buttonevents[mouseevent->mouseButton] = true;
	}
}

void geProject::Receiver::updateKeyButtonPress(KeyPressedEvent* keyevent){	
	if (keyevent->contextCheck(GameplayContext)) {
		buttonevents[keyevent->keycode] = true;
	}
}


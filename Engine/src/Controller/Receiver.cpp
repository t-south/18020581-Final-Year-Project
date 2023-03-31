#include "Receiver.h"

geProject::Receiver::Receiver(){
	leftClick = new AttackCommand();
	rightClick = new ShieldCommand();
	buttonShift = new DashCommand();	
	moveTo = new MoveCommand();
	buttonF = new SpecialAttackCommand();
	buttonTab = new SwitchAbilityCommand();
	eventSystem.subscribe(this, &Receiver::updateKeyButtonPress);
	eventSystem.subscribe(this, &Receiver::updateKeyButtonRelease);
	eventSystem.subscribe(this, &Receiver::updateMouseButtonPress);
}

geProject::Command* geProject::Receiver::action(){		
	bool movement = false;
	
	if (buttonPressed(0)) { 		
		return leftClick; 
	}
	if (buttonPressed(1)) { 		
		return rightClick; 
	}
	if (buttonPressed(2)) { 
		return buttonShift; 
	}
	if (buttonPressed(3) && buttonPressed(4)) { // W + A
		moveTo->x = -1;
		moveTo->y = 1;
		return moveTo;
	}
	if (buttonPressed(3) && buttonPressed(6)){ // W + D
		moveTo->x = 1;
		moveTo->y = 1;
		return moveTo;
	}
	if (buttonPressed(6) && buttonPressed(5)) { // D + S
		moveTo->x = 1;
		moveTo->y = -1;
		return moveTo;
	}
	if (buttonPressed(4) && buttonPressed(5)) { // A + S
		moveTo->x = -1;
		moveTo->y = -1;
		return moveTo;
	}
	if (buttonPressed(3)) { // W
		moveTo->x = 0;
		moveTo->y = 1;
		return moveTo;
	}
	if (buttonPressed(4)) { // A
		moveTo->x = -1;
		moveTo->y = 0;
		return moveTo;
	}
	if (buttonPressed(5)) { // S
		moveTo->x = 0;
		moveTo->y = -1;
		return moveTo;
	}
	if (buttonPressed(6)) { // D	
		moveTo->x = 1;
		moveTo->y = 0;
		return moveTo;
	}

	if (buttonPressed(7)) { 
		return buttonF; 
	}
	if (buttonPressed(8)) { 
		return buttonTab; 
	}
	return nullptr;
}

bool geProject::Receiver::buttonPressed(int button){	
	if (buttonevents[button] == true) {			
		return true;
	}	
	return false;
}

void geProject::Receiver::updateMouseButtonPress(MouseButtonEvent* mouseevent) {
	if (mouseevent->contextCheck(GameplayContext)) {
		mouseevent->mouseButtonDown;
		switch (mouseevent->mouseButton) {
		case GLFW_MOUSE_BUTTON_LEFT:
			buttonevents[0] = mouseevent->mouseButtonDown;
			break;
		case GLFW_MOUSE_BUTTON_RIGHT:
			buttonevents[1] = mouseevent->mouseButtonDown;
			break;
		default:
			break;
		}
	}
}

void geProject::Receiver::updateKeyButtonPress(KeyPressedEvent * keyevent) {
	if (keyevent->contextCheck(GameplayContext) || (keyevent->contextCheck(ImGuiContext))) {
		switch (keyevent->keycode) {
			case GLFW_KEY_LEFT_SHIFT:
				buttonevents[2] = true;
				break;
			case GLFW_KEY_W:				
				buttonevents[3] = true;
				break;
			case GLFW_KEY_A:				
				buttonevents[4] = true;
				break;
			case GLFW_KEY_S:	
				buttonevents[5] = true;
				break;
			case GLFW_KEY_D:	
				buttonevents[6] = true;
				break;
			case GLFW_KEY_F:
				buttonevents[7] = true;
				break;
			case GLFW_KEY_TAB:
				buttonevents[8] = true;
				break;
			default:
				break;
		}
	}
}

void geProject::Receiver::updateKeyButtonRelease(KeyReleasedEvent* keyevent){
	if (keyevent->contextCheck(GameplayContext) || (keyevent->contextCheck(ImGuiContext))) {
		switch (keyevent->keycode) {
		case GLFW_KEY_LEFT_SHIFT:			
			buttonevents[2] = false;
			break;
		case GLFW_KEY_W:
			buttonevents[3] = false;
			break;
		case GLFW_KEY_A:	
			buttonevents[4] = false;
			break;
		case GLFW_KEY_S:
			buttonevents[5] = false;
			break;
		case GLFW_KEY_D:
			buttonevents[6] = false;
			break;
		case GLFW_KEY_F:
			buttonevents[7] = false;
			break;
		case GLFW_KEY_TAB:
			buttonevents[8] = false;
			break;
		default:
			break;
		}
	}
}



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

geProject::Command* geProject::Receiver::action(float deltaTime){		
	bool movement = false;
	moveTo->dt = deltaTime;
	if (buttonPressed(8)) {
		buttonevents[8] = false;
		return buttonTab;
	}

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
		case 0:
			buttonevents[0] = mouseevent->mouseButtonDown;
			break;
		case 1:
			buttonevents[1] = mouseevent->mouseButtonDown;
			break;
		default:
			break;
		}
	}
}

void geProject::Receiver::updateKeyButtonPress(KeyPressedEvent* keyevent) {
	if (keyevent->contextCheck(GameplayContext) || (keyevent->contextCheck(ImGuiContext))) {
		switch (keyevent->keycode) {
			case 340: //LEFT SHIFT
				buttonevents[2] = true;
				break;
			case 87:	//W			
				buttonevents[3] = true;
				break;
			case 65: //A				
				buttonevents[4] = true;
				break;
			case 83:	//S
				buttonevents[5] = true;
				break;
			case 68:	//D
				buttonevents[6] = true;
				break;
			case 70:  //F
				buttonevents[7] = true;
				break;
			case 258:  //TAB
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
		case 340:			 //SHIFT
			buttonevents[2] = false;
			break;
		case 87:
			buttonevents[3] = false;
			break;
		case 65:	
			buttonevents[4] = false;
			break;
		case 83:
			buttonevents[5] = false;
			break;
		case 68:
			buttonevents[6] = false;
			break;
		case 70:
			buttonevents[7] = false;
			break;
		case 258:
			buttonevents[8] = false;
			break;
		default:
			break;
		}
	}
}



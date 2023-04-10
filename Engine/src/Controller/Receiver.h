#pragma once

#include "Command.h"
#include "ConcreteCommands.h"


namespace geProject {

	class Receiver {
	public:
		Receiver();
		Command* action(float deltaTime);
	private:
		Command* leftClick;
		Command* rightClick;
		Command* buttonShift;
		MoveCommand* moveTo;
		Command* buttonF;
		Command* buttonTab;		
		bool buttonevents[9]{ false };
		void updateMouseButtonPress(MouseButtonEvent* mouseevent);
		void updateKeyButtonPress(KeyPressedEvent* keyevent);	
		void updateKeyButtonRelease(KeyReleasedEvent* keyevent);
		bool buttonPressed(int button);
		
	};
}
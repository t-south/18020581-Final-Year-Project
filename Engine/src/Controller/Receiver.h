#pragma once
#include "Command.h"
#include "ConcreteCommands.h"
#include <ge_engine/Core.h>
#include <GLFW/glfw3.h>

namespace geProject {

	class Receiver {
	public:
		Receiver();
		Command* action();
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
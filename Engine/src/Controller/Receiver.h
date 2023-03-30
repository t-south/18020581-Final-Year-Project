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
		Command* buttonW;
		Command* buttonA;
		Command* buttonS;
		Command* buttonD;
		Command* buttonF;
		Command* buttonTab;
		std::unordered_map<int, bool> buttonevents;	
		bool buttonPressed(int button);
		void updateMouseButtonPress(MouseButtonEvent* mouseevent);
		void updateKeyButtonPress(KeyPressedEvent* keyevent);		
	};
}
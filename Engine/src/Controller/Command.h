#pragma once
#include <ge_engine/Entity.h>
#include "Controller.h"

namespace geProject {
	class Command {
	public:
		virtual ~Command() {};
		virtual void execute(Controller& entity) = 0;
	}; 

}
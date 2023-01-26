#pragma once
#include "../Core.h"
namespace geProject {

	class GE_API Input {
	public:
		static bool IsKeyPressed(int keycode) { return instance->IsKeyPressed; }
	private:
		static Input* instance;
	};
	//contexts of input
	//types of input for that context

	//convert raw input to context dependent id's

	//use callbacks instead of polling, a map of functions will be called depending on context and type

	//input every frame
	//current active context is evaluated

	//go through ordered list of contexts, if input cant be mapped move onto next context
}
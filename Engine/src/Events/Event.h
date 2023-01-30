#pragma once
#include <ge_engine/Core.h>

namespace geProject {
	enum Type
	{
		NoType = 0, 
		closeWindow, resizeWindow, focusWindow, stopFocusWindow, moveWindow,
		keyPressed, keyReleased,
		mousePressed, mouseReleased, mouseMove, mouseScroll
	};
	enum Context {
		NoCategory = 0,				/* 0b0000000000000001 */
		AppCat = (1 << 0),			/* 0b0000000000000010 */
		InputCat = (1 << 1),		/* 0b0000000000000100 */
		KeyboardCat = (1 << 2),		/* 0b0000000000001000 */
		MouseCat = (1 << 3),		/* 0b0000000000010000 */
		MouseButtonCat = (1 << 4)   /* 0b0000000000100000 */
	};

	class Event {
	public:
		
	private:
		static Event* instance;
		Context eventContext;
		virtual int getType() const = 0;
		virtual int getContexts() const = 0;

	};








	//contexts of input
	//types of input for that context

	//convert raw input to context dependent id's

	//use callbacks instead of polling, a map of functions will be called depending on context and type

	//input every frame
	//current active context is evaluated

	//go through ordered list of contexts, if input cant be mapped move onto next context
}
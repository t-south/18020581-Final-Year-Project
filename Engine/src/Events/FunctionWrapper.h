#pragma once
#include <iostream>
#include "Event.h"
#include <functional>

//credit to medium.com/@savas/nomad-game-engine-part-7-the-event-system-45a809ccb68f
namespace geProject {
	class FunctionWrapper {
	public:
		void callEvent() { callFunction(); }
		void setFunctionParam(Event* event) { setEvent(event); };
	private:
		virtual void callFunction() = 0;
		virtual void setEvent(Event* event) = 0;

	};

	template <class T, class U>
	class EventFunctionWrapper : public FunctionWrapper {
	public:
		typedef void (T::*CallBackFunction)(U*);
		EventFunctionWrapper(T* instance, CallBackFunction callback) :instance{ instance }, callback{ callback } {};
	private:
		T* instance;
		Event* eventClass;
		CallBackFunction callback;
		bool functionAssigned{ false };
		virtual void callFunction() override { 

			if (functionAssigned) { 
				(instance->*callback)(static_cast<U*>(eventClass)); functionAssigned = false; 
			}
		};

		virtual void setEvent(Event* event) override { 
			eventClass = event;		
			
			functionAssigned = true;
		};
	};
}
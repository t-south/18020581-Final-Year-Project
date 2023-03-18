#pragma once
#include "Event.h"
#include <memory>

//credit to medium.com/@savas/nomad-game-engine-part-7-the-event-system-45a809ccb68f
namespace geProject {
	class FunctionWrapper {
	public:
		void callEvent(Event* event) { callFunction(event); }
	private:
		virtual void callFunction(Event* event) = 0;
	};

	template <class T, class U>
	class EventFunctionWrapper : public FunctionWrapper {
	public:
		typedef void (T::*CallBackFunction)(U*);
		EventFunctionWrapper(T* instance, CallBackFunction callback) :instance{ instance }, callback{ callback } {};
		virtual void callFunction(Event* event) override { (instance->*callback)(static_cast<U*>(event)); };
	private:
		CallBackFunction callback;
		T* instance;
	};
}
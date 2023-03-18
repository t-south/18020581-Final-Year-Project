#pragma once
#include <map>
#include "Event.h"
#include <functional>
#include "FunctionWrapper.h"
#include <unordered_map>

namespace geProject {
	class EventHandler {
	public:
		template<class T>
		void publish(T* event) {
			auto id = event->getType();
			std::list<FunctionWrapper*>* observers = events[event->getType()];
			
			for (auto &observer : *observers) {
				observer->callEvent(event);
			}
			
			
		};

		template<class T, class U>
		void subscribe(T* observer, void (T::*callback)(U *)) {
			auto eventId = U::getType();
			std::list<FunctionWrapper*>* observers = events[eventId];
			if(observers == NULL) {
				observers = new std::list<FunctionWrapper*>();				
				events[eventId] = observers;
			}			
			observers->push_back(new EventFunctionWrapper<T, U>(observer, callback));
			


		}
		//void unsubscribe(Observer& observer, Type eventType);

	private:
		std::unordered_map<int, std::list<FunctionWrapper*>*> events;	
	};
}
#pragma once

#include <map>
#include "Event.h"
#include <functional>
#include "FunctionWrapper.h"
#include <unordered_map>
#include <iostream>

namespace geProject {
	class EventHandler {
	public:
		template<class T>
		void publish(T* event) {
			std::list<FunctionWrapper*>* observers = events[event->getType()];
			for (auto& observer : *observers) {
				observer->setFunctionParam(event);
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

		template<class T>
		void publishImmediately(T* event) {
			std::list<FunctionWrapper*>* observers = events[event->getType()];
			for (auto& observer : *observers) {
				observer->setFunctionParam(event);
				observer->callEvent();
			}
		}
		
		void handleEvents(Type eventType){	
			std::list<FunctionWrapper*>* observers = events[eventType];
			int count = 0;
			for (auto& observer : *observers) {
				count++;
				observer->callEvent();
			}			
		}

	private:
		std::unordered_map<int, std::list<FunctionWrapper*>*> events;	
	};
}
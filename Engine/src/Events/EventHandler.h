#include <map>
#include "Event.h"
#include <functional>


namespace geProject {
	class EventHandler {
	public:
		
	private:
		//typedef std::function<> callback;
		//std::map<Event::getType, callback> events;
		static EventHandler* instance;
		void registerEvent();
		void unRegisterEvent();
		void notify();
	};
}
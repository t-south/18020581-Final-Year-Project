#pragma once
#include <Windows.h>
#include <stdint.h>
#include <vector>

namespace geProject {
	class Clock {
	public:
		static Clock* getInstance();
		
		void endFrame();
		void updateTime();
		float getTime();
		float getLastFrameTime();
		float getTimeFrameAvg();
		void setFrameNumber(float frameNum);
	private:
		Clock();
		
		Clock(const Clock& obj) = delete;
		~Clock();
		static Clock* instance;
		
		int64_t startTime = 0;
		uint8_t counter = 0;	
		uint8_t frameNumber = 1;
		std::vector<float> timeElapsed;
		void changeCounter();
		void addFrameTime(int64_t endTime);
		static inline int64_t GetTicks();
	};


}
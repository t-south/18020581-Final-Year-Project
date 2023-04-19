#include "Clock.h"

geProject::Clock* geProject::Clock::instance = nullptr;

geProject::Clock::Clock() {
    timeElapsed.push_back(0);
}

geProject::Clock* geProject::Clock::getInstance() {
    //PRE: IF SINGLETON INSTANCE IS ACTIVE
    if (instance == NULL)
    {
        instance = new Clock();
        return instance;
    }
    else
    {
        return instance;
    }
}


void geProject::Clock::endFrame() {
    changeCounter();
}


float geProject::Clock::getTime() {
    return timeElapsed[counter];
}

void geProject::Clock::updateTime(){
    // PRE: IF START TIME IS 0
    if (startTime == 0)
        startTime = GetTicks();
    else {
        int64_t endTime = GetTicks();
        addFrameTime(endTime);
        startTime = 0;
    }
}

void geProject::Clock::setFrameNumber(float frameNum) {
    for (std::size_t i = 0; i < frameNum -1; ++i)
    {
        timeElapsed.push_back(0);
    }
    frameNumber = frameNum;
}

float geProject::Clock::getLastFrameTime(){
    return timeElapsed[(counter - 1) % frameNumber];
}

float geProject::Clock::getTimeFrameAvg() {
    float avg = 0;
    //iterate through vector and find average
    return avg;
}

//wrap around counter for indexing vector. should not exceed number of frames to be stored in vector
void geProject::Clock::changeCounter() {
    counter++;
    counter = counter % frameNumber;
}

void geProject::Clock::addFrameTime(int64_t endTime) {
    LARGE_INTEGER frequency;
    float delta;
    QueryPerformanceFrequency(&frequency);
    delta = (endTime - startTime) / (float)frequency.QuadPart;
    geProject::Clock::timeElapsed[counter] = delta;
    
}

//credit to https ://learn.microsoft.com/en-us/windows/win32/api/profileapi/nf-profileapi-queryperformancecounter
int64_t geProject::Clock::GetTicks()
{
    LARGE_INTEGER ticks;
    if (!QueryPerformanceCounter(&ticks))
    {
        //winrt::throw_last_error();
    }
    return ticks.QuadPart;
}

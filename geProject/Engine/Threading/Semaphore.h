#pragma once
class Semaphore {
private:

public:
	Semaphore(int count);
	void Take();
	void Give();
	void Wait();
	void Post();
	void Signal();
	void Down();
	void Up();

};
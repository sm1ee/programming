#pragma once
#include "MyHeader.h"

class CFps{
private:
	clock_t start;
	clock_t delay;
public:
	CFps(clock_t delay) :start(0), delay(delay){}
	~CFps(){}

	void SetStart(){
		this->start = clock();
	}
	bool getFps(){
		if (start + delay < clock()){
			start = clock();
			return true;
		}
		return false;
	}
};
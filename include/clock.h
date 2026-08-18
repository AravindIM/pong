#pragma once
#include <SDL3/SDL.h>

class Clock {
	Uint64 mStartTime;
public:
	Clock();
	void Reset();
	float GetDTSec();
};
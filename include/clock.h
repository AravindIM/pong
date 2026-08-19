#pragma once
#include <SDL3/SDL.h>

class Clock {
	Uint64 mStartTime{};
public:
	Clock() = default;
	void Reset();
	float GetDTSec();
};
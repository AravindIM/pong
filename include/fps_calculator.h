#pragma once

#include <SDL3/SDL.h>

class FPSCalculator {
	int mFrames;
	Uint64 mStartTime;
	Uint64 mFrameStartTime;
	Uint64 mElapsed;
	int mFPS;

public:
	FPSCalculator();
	void Reset();
	void FrameStart();
	void FrameEnd();
	int GetFPS();
	Uint64 GetFrameDT();
	Uint64 GetDT();
};
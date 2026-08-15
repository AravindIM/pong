#include "fps_calculator.h"

FPSCalculator::FPSCalculator()
	: mFrames(0)
	, mStartTime(0)
	, mFrameStartTime(0)
	, mElapsed(0)
	, mFPS(0)
{
}

void FPSCalculator::Reset() {
	mStartTime = SDL_GetTicksNS();
}
void FPSCalculator::FrameStart() {
	mFrameStartTime = SDL_GetTicksNS();
}

void FPSCalculator::FrameEnd() {
	mElapsed += GetFrameDT();
	mFrames++;
}

int FPSCalculator::GetFPS() {
	mFPS = (int)(mFrames * SDL_NS_PER_SECOND / mElapsed);
	mElapsed = 0;
	mFrames = 0;
	mStartTime = SDL_GetTicksNS();
	return mFPS;
}

Uint64 FPSCalculator::GetFrameDT() {
	return SDL_GetTicksNS() - mFrameStartTime;
}

Uint64 FPSCalculator::GetDT() {
	return SDL_GetTicksNS() - mStartTime;
}
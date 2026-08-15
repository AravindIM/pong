#include "clock.h"

Clock::Clock() : mStartTime(0) {}

void Clock::Reset() {
	mStartTime = SDL_GetTicksNS();
}

double Clock::GetDTSec() {
	return (double)(SDL_GetTicksNS() - mStartTime) / SDL_NS_PER_SECOND;
}

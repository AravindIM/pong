#include "clock.h"

void Clock::Reset() {
	mStartTime = SDL_GetTicksNS();
}

float Clock::GetDTSec() {
	return (float)(SDL_GetTicksNS() - mStartTime) / SDL_NS_PER_SECOND;
}

#pragma once
#include "config.h"
#include<SDL3/SDL.h>

struct Scoreboard {
	SDL_FRect mRect{};
	float mThickness{};
	Scoreboard() = default;
};
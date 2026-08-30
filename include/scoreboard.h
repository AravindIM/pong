#pragma once
#include "config.h"
#include<SDL3/SDL.h>

struct Scoreboard {
	SDL_FRect mRect{};
	float mThickness{};
	Scoreboard() = default;
	void RenderSegment(SDL_Renderer* renderer, Uint8 segment);
};
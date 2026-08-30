#pragma once
#include "config.h"
#include<SDL3/SDL.h>

constexpr float SCORE_THICKNESS = 5.0;

struct Scoreboard {
	SDL_FRect mRect{};
	float mThickness{ SCORE_THICKNESS };
	Scoreboard() = default;
	void RenderSegment(SDL_Renderer* renderer, Uint8 segment, SDL_FRect rect);
};
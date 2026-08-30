#pragma once
#include "config.h"
#include<SDL3/SDL.h>

constexpr float SCORE_THICKNESS = 5.0;

class Scoreboard {
	SDL_FRect mRect{};
	float mThickness{ SCORE_THICKNESS };
public:
	Scoreboard(SDL_FRect rect);
	void RenderSegment(SDL_Renderer* renderer, Uint8 segment, SDL_FRect rect);
};
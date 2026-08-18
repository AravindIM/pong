#pragma once

#include <SDL3/SDL.h>
#include "config.h"

// Dimensions
constexpr float BALL_SIZE = 10;

// Coordinates
constexpr float BALL_START_X = ((WINDOW_WIDTH - BALL_SIZE) / 2);
constexpr float BALL_START_Y((WINDOW_HEIGHT - BALL_SIZE) / 2);
constexpr float BALL_START_Y_MIN_PERCENT = 0.25;
constexpr float BALL_START_Y_MAX_PERCENT = 0.75;


// Bounds
constexpr float BALL_MIN_X = 0;
constexpr float BALL_MAX_X = (WINDOW_WIDTH - BALL_SIZE);
constexpr float BALL_MIN_Y = 0;
constexpr float BALL_MAX_Y = (WINDOW_HEIGHT - BALL_SIZE);

// Physics
constexpr float BALL_START_VX = 250;
constexpr float BALL_START_VY = 250;

struct Ball {
	SDL_FRect mRect;
	float mVx;
	float mVy;
	bool mShow;
	Ball();
	void Reset();
	void Move(float deltaTime);
	void Render(SDL_Renderer* renderer);
};

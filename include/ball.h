#pragma once

#include <SDL3/SDL.h>
#include "config.h"

// Dimensions
#define BALL_SIZE 10

// Coordinates
#define BALL_START_X ((WINDOW_WIDTH - BALL_SIZE) / 2)
#define BALL_START_Y ((WINDOW_HEIGHT - BALL_SIZE) / 2)
#define BALL_START_Y_MIN_PERCENT 0.25
#define BALL_START_Y_MAX_PERCENT 0.75


// Bounds
#define BALL_MIN_X 0
#define BALL_MAX_X (WINDOW_WIDTH - BALL_SIZE)
#define BALL_MIN_Y 0
#define BALL_MAX_Y (WINDOW_HEIGHT - BALL_SIZE)

// Physics
#define BALL_START_VX 250
#define BALL_START_VY 250

struct Ball {
	SDL_FRect mRect;
	float mVx;
	float mVy;
	bool mShow;
	Ball();
	void Reset();
	void Move(double deltaTime);
	void Render(SDL_Renderer* renderer);
};

#pragma once

#include <SDL3/SDL.h>
#include "config.h"

// Count
#define MAX_PLAYERS 2

// Dimensions
#define PLAYER_PADDLE_WIDTH 10
#define PLAYER_PADDLE_HEIGHT 80

// Coordinates
#define PLAYER_PADDLE_GAP PLAYER_PADDLE_WIDTH
#define PLAYER_PADDLE_START_Y ((WINDOW_HEIGHT - PLAYER_PADDLE_HEIGHT) / 2)
#define PLAYER1_PADDLE_START_X PLAYER_PADDLE_GAP
#define PLAYER2_PADDLE_START_X (WINDOW_WIDTH - PLAYER_PADDLE_WIDTH - PLAYER_PADDLE_GAP)

// Bounds
#define PLAYER_PADDLE_MIN_Y 0
#define PLAYER_PADDLE_MAX_Y (WINDOW_HEIGHT - PLAYER_PADDLE_HEIGHT)

// Physics
#define PLAYER_PADDLE_SPEED (WINDOW_HEIGHT / 2)


enum Direction {
	UP = -1,
	DOWN = 1
};
enum PlayerVariant {
	LEFT,
	RIGHT,
};

struct Player {
	SDL_FRect mRect;
	PlayerVariant mVariant;
	SDL_Gamepad* mPad;
	Player(PlayerVariant variant);
	void Reset();
	void Move(Direction dir, double deltaTime);
	void Render(SDL_Renderer* renderer);
};

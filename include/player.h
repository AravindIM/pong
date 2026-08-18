#pragma once

#include <SDL3/SDL.h>
#include "config.h"

// Count
constexpr int MAX_PLAYERS = 2;

// Dimensions
constexpr float PLAYER_PADDLE_WIDTH = 10;
constexpr float PLAYER_PADDLE_HEIGHT = 80;

// Coordinates
constexpr float PLAYER_PADDLE_GAP = PLAYER_PADDLE_WIDTH;
constexpr float PLAYER_PADDLE_START_Y = ((WINDOW_HEIGHT - PLAYER_PADDLE_HEIGHT) / 2);
constexpr float PLAYER1_PADDLE_START_X = PLAYER_PADDLE_GAP;
constexpr float PLAYER2_PADDLE_START_X = (WINDOW_WIDTH - PLAYER_PADDLE_WIDTH - PLAYER_PADDLE_GAP);

// Bounds
constexpr float PLAYER_PADDLE_MIN_Y = 0;
constexpr float PLAYER_PADDLE_MAX_Y = (WINDOW_HEIGHT - PLAYER_PADDLE_HEIGHT);

// Physics
constexpr float PLAYER_PADDLE_SPEED = (WINDOW_HEIGHT / 2);


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
	void Move(Direction dir, float deltaTime);
	void Render(SDL_Renderer* renderer);
};

#include "player.h"

Player::Player(PlayerVariant variant)
	: mVariant(variant) 
{
	Reset();
}

void Player::Reset() {
	float x;
	switch (mVariant) {
	case LEFT:
		x = PLAYER1_PADDLE_START_X;
		break;
	case RIGHT:
		x = PLAYER2_PADDLE_START_X;
		break;
	default:
		return;
	}
	mRect = SDL_FRect(x, PLAYER_PADDLE_START_Y, PLAYER_PADDLE_WIDTH, PLAYER_PADDLE_HEIGHT);
}

void Player::Move(Direction dir, float deltaTime) {
	float dy = PLAYER_PADDLE_SPEED * deltaTime;
	mRect.y = SDL_clamp(mRect.y + (float)dir * dy, PLAYER_PADDLE_MIN_Y, PLAYER_PADDLE_MAX_Y);
}

void Player::Render(SDL_Renderer* renderer) {
	Uint8 color = mPad? JOINED_COLOR : LEFT_COLOR;

	SDL_SetRenderDrawColor(renderer, color, color, color, 0xFF);
	SDL_RenderFillRect(renderer, &mRect);
}
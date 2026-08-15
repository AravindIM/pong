#include "player.h"

Player::Player(PlayerVariant variant)
	: mVariant(variant)
	, mPad(nullptr) {
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
	}
	mRect = SDL_FRect(x, PLAYER_PADDLE_START_Y, PLAYER_PADDLE_WIDTH, PLAYER_PADDLE_HEIGHT);
}

void Player::Move(Direction dir, double deltaTime) {
	float dy = (float)(PLAYER_PADDLE_SPEED * deltaTime);
	mRect.y = SDL_clamp(mRect.y + (float)dir * dy, PLAYER_PADDLE_MIN_Y, PLAYER_PADDLE_MAX_Y);
}
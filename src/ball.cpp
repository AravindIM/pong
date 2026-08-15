#include "ball.h"

Ball::Ball() {
	Reset();
}

void Ball::Reset() {
	mRect = SDL_FRect(BALL_START_X, BALL_START_Y, BALL_SIZE, BALL_SIZE);
	mVx = BALL_START_VX;
	mVy = BALL_START_VY;
}

void Ball::Move(double deltaTime) {
	mRect.x = SDL_clamp(mRect.x + (float)(mVx * deltaTime), BALL_MIN_X, BALL_MAX_X);
	mRect.y = SDL_clamp(mRect.y + (float)(mVy * deltaTime), BALL_MIN_Y, BALL_MAX_Y);
}
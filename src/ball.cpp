#include "ball.h"
#include "random.h"

Ball::Ball() :
	mShow(false)
{
	mVx = getRandomSign() * BALL_START_VX;
	mVy = getRandomSign() * BALL_START_VY;
	Reset();
}

void Ball::Reset() {
	mShow = false;
	constexpr float minRandomY = BALL_MIN_Y + (BALL_MAX_Y - BALL_MIN_Y) * BALL_START_Y_MIN_PERCENT;
	constexpr float maxRandomY = BALL_MIN_Y + (BALL_MAX_Y - BALL_MIN_Y) * BALL_START_Y_MAX_PERCENT;
	mRect = SDL_FRect(BALL_START_X, random<std::uniform_real_distribution<float>>(minRandomY, maxRandomY), BALL_SIZE, BALL_SIZE);
}

void Ball::Move(float deltaTime) {
	mShow = true;
	mRect.x = SDL_clamp(mRect.x + mVx * deltaTime, BALL_MIN_X, BALL_MAX_X);
	mRect.y = SDL_clamp(mRect.y + mVy * deltaTime, BALL_MIN_Y, BALL_MAX_Y);
}

void Ball::Render(SDL_Renderer* renderer) {
	if (mShow) {
		SDL_SetRenderDrawColor(renderer, FG_COLOR, FG_COLOR, FG_COLOR, 0xFF);
		SDL_RenderFillRect(renderer, &mRect);
	}
}
#include "ball.h"
#include <random>

Ball::Ball() :
	mShow(false)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::bernoulli_distribution dist(0.5);
	mVx = static_cast<float>(BALL_START_VX);
	mVy = static_cast<float>(BALL_START_VY);
	mVx = dist(rng) ? mVx : -mVx;
	mVy = dist(rng) ? mVy : -mVy;
	Reset();
}

void Ball::Reset() {
	mShow = false;
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_real_distribution<float> dist(BALL_MIN_Y + (BALL_MAX_Y - BALL_MIN_Y) * BALL_START_Y_MIN_PERCENT, BALL_MIN_Y + (BALL_MAX_Y - BALL_MIN_Y) * BALL_START_Y_MAX_PERCENT);
	mRect = SDL_FRect(BALL_START_X, dist(rng), BALL_SIZE, BALL_SIZE);
}

void Ball::Move(double deltaTime) {
	mShow = true;
	mRect.x = SDL_clamp(mRect.x + (float)(mVx * deltaTime), BALL_MIN_X, BALL_MAX_X);
	mRect.y = SDL_clamp(mRect.y + (float)(mVy * deltaTime), BALL_MIN_Y, BALL_MAX_Y);
}

void Ball::Render(SDL_Renderer* renderer) {
	if (mShow) {
		SDL_SetRenderDrawColor(renderer, FG_COLOR, FG_COLOR, FG_COLOR, 0xFF);
		SDL_RenderFillRect(renderer, &mRect);
	}
}
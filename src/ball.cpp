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
	float randomY = random<std::uniform_real_distribution<float>>(BALL_MIN_RANDOM_Y, BALL_MAX_RANDOM_Y);
	mRect = SDL_FRect(BALL_START_X, randomY, BALL_SIZE, BALL_SIZE);
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
#include "scoreboard.h"

void Scoreboard::RenderSegment(SDL_Renderer* renderer, Uint8 segment) {
	SDL_FRect sRect;
	switch (segment) {
	case 0:
		sRect = {
			.x = mRect.x,
			.y = mRect.y,
			.w = mRect.w,
			.h = mThickness
		};
		break;
	case 1:
		sRect = {
			.x = mRect.x + (mRect.w - mThickness),
			.y = mRect.y,
			.w = mThickness,
			.h = mRect.h / 2
		};
		break;
	case 2:
		sRect = {
			.x = mRect.x + (mRect.w - mThickness),
			.y = mRect.y + mRect.h / 2,
			.w = mThickness,
			.h = mRect.h / 2
		};
		break;
	case 3:
		sRect = {
			.x = mRect.x,
			.y = mRect.y + (mRect.h - mThickness),
			.w = mRect.w,
			.h = mThickness
		};
		break;
	case 4:
		sRect = {
			.x = mRect.x,
			.y = mRect.y + (mRect.h - mThickness) / 2,
			.w = mThickness,
			.h = mRect.h / 2
		};
		break;
	case 5:
		sRect = {
			.x = mRect.x,
			.y = mRect.y,
			.w = mThickness,
			.h = mRect.h / 2
		};
		break;
	case 6:
		sRect = {
			.x = mRect.x,
			.y = mRect.y + mRect.h / 2,
			.w = mRect.w,
			.h = mThickness
		};
		break;
	default:
		sRect = {
			.x = mRect.x,
			.y = mRect.y,
			.w = 0,
			.h = 0
		};
		break;
	}
	SDL_SetRenderDrawColor(renderer, FG_COLOR, FG_COLOR, FG_COLOR, 0xFF);
	SDL_RenderFillRect(renderer, &mRect);
}
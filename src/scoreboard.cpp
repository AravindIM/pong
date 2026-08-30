#include "scoreboard.h"

Scoreboard::Scoreboard(SDL_FRect rect): mRect(rect) {}

void Scoreboard::RenderSegment(SDL_Renderer* renderer, Uint8 segment, SDL_FRect rect) {
	switch (segment) {
	case 0:
		rect.h = mThickness;
		break;
	case 1:
		rect.x += rect.w - mThickness;
		rect.w = mThickness;
		rect.h /= 2;
		break;
	case 2:
		rect.x += rect.w - mThickness;
		rect.y += rect.h / 2;
		rect.w = mThickness;
		rect.h /= 2;
		break;
	case 3:
		rect.y += rect.h - mThickness,
		rect.h = mThickness;
		break;
	case 4:
		rect.y += rect.h / 2;
		rect.w = mThickness;
		rect.h /=  2;
		break;
	case 5:
		rect.w = mThickness;
		rect.h /= 2;
		break;
	case 6:
		rect.y += (rect.h - mThickness) / 2;
		rect.h = mThickness;
		break;
	default:
		break;
	}
	SDL_SetRenderDrawColor(renderer, FG_COLOR, FG_COLOR, FG_COLOR, 0xFF);
	SDL_RenderFillRect(renderer, &rect);
}
#include "scoreboard.h"
#include <cassert>

Scoreboard::Scoreboard(SDL_FRect rect): mRect(rect) {}

void Scoreboard::RenderSegmentType(SDL_Renderer* renderer, ScoreSegmentType st, SDL_FRect rect) {
	switch (st) {
	case SCORE_SEGMENT_HORIZONTAL:
		rect.h = mThickness;
	case SCORE_SEGMENT_VERTICAL:
		rect.w = mThickness;
		rect.h /= 2;
	}
	SDL_SetRenderDrawColor(renderer, FG_COLOR, FG_COLOR, FG_COLOR, 0xFF);
	SDL_RenderFillRect(renderer, &rect);
}

void Scoreboard::RenderSegment(SDL_Renderer* renderer, Uint8 segment, SDL_FRect rect) {
	ScoreSegmentType st;
	switch (segment) {
	case 0:
		st = SCORE_SEGMENT_HORIZONTAL;
		break;
	case 1:
		rect.x += rect.w - mThickness;
		st = SCORE_SEGMENT_VERTICAL;
		break;
	case 2:
		rect.x += rect.w - mThickness;
		rect.y += rect.h / 2;
		st = SCORE_SEGMENT_VERTICAL;
		break;
	case 3:
		rect.y += rect.h - mThickness,
		st = SCORE_SEGMENT_HORIZONTAL;
		break;
	case 4:
		rect.y += rect.h / 2;
		st = SCORE_SEGMENT_VERTICAL;
		break;
	case 5:
		st = SCORE_SEGMENT_VERTICAL;
		break;
	case 6:
		rect.y += (rect.h - mThickness) / 2;
		st = SCORE_SEGMENT_HORIZONTAL;
		break;
	default:
		break;
	}
	RenderSegmentType(renderer, st, rect);
}

void Scoreboard::RenderDigit(SDL_Renderer* renderer, Uint8 digit, SDL_FRect rect) {
	if (digit > SCORE_DIGIT_MAP.size()) {
		exit(-1);
	}

	Uint8 repr = SCORE_DIGIT_MAP[(size_t)digit];

	for (Uint8 segment = 0; repr != 0; ++segment) {
		if (repr % 2) {
			RenderSegment(renderer, segment, rect);
		}
		repr /= 2;
	}
}
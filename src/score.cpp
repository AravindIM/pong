#include "score.h"

void RenderScoreSegmentType(SDL_Renderer* renderer, ScoreSegmentType st, SDL_FRect rect) {
	switch (st) {
	case SCORE_SEGMENT_HORIZONTAL:
		rect.h = SCORE_SEGMENT_THICKNESS;
		break;
	case SCORE_SEGMENT_VERTICAL:
		rect.w = SCORE_SEGMENT_THICKNESS;
		rect.h /= 2;
		break;
	}
	SDL_SetRenderDrawColor(renderer, FG_COLOR, FG_COLOR, FG_COLOR, 0xFF);
	SDL_RenderFillRect(renderer, &rect);
}

void RenderScoreSegment(SDL_Renderer* renderer, Uint8 segment, SDL_FRect rect) {
	ScoreSegmentType st;
	switch (segment) {
	case 0:
		st = SCORE_SEGMENT_HORIZONTAL;
		break;
	case 1:
		rect.x += rect.w - SCORE_SEGMENT_THICKNESS;
		st = SCORE_SEGMENT_VERTICAL;
		break;
	case 2:
		rect.x += rect.w - SCORE_SEGMENT_THICKNESS;
		rect.y += rect.h / 2;
		st = SCORE_SEGMENT_VERTICAL;
		break;
	case 3:
		rect.y += rect.h - SCORE_SEGMENT_THICKNESS,
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
		rect.y += (rect.h - SCORE_SEGMENT_THICKNESS) / 2;
		st = SCORE_SEGMENT_HORIZONTAL;
		break;
	default:
		return;
	}
	RenderScoreSegmentType(renderer, st, rect);
}

void RenderScoreDigit(SDL_Renderer* renderer, Uint8 digit, SDL_FRect rect) {
	if (digit >= SCORE_DIGIT_MAP.size()) {
		return;
	}

	Uint8 repr = SCORE_DIGIT_MAP[(size_t)digit];

	for (Uint8 segment = 0; repr != 0; ++segment) {
		if (repr % 2) {
			RenderScoreSegment(renderer, segment, rect);
		}
		repr /= 2;
	}
}

void RenderScoreDigitInPlace(SDL_Renderer* renderer, Uint8 digit, Uint8 place, SDL_FRect rect) {
	Uint8 index = SCORE_MAX_DIGITS - place - 1;
	rect.x += index * (SCORE_DIGIT_WIDTH + SCORE_DIGIT_SEPERATOR_GAP);
	rect.w = SCORE_DIGIT_WIDTH;
	rect.h = SCORE_DIGIT_HEIGHT;
	RenderScoreDigit(renderer, digit, rect);
}

void RenderScore(SDL_Renderer* renderer, Uint8 score, SDL_FRect rect) {
	int place = 0;
	do {
		int digit = score % 10;
		RenderScoreDigitInPlace(renderer, digit, place, rect);
		score /= 10;
		++place;
	} while (score > 0 && place < SCORE_MAX_DIGITS);
}
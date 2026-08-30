#pragma once
#include "config.h"
#include<SDL3/SDL.h>
#include<array>

enum ScoreSegmentType {
	SCORE_SEGMENT_HORIZONTAL,
	SCORE_SEGMENT_VERTICAL
};

constexpr float SCORE_SEGMENT_THICKNESS = 5.0;

constexpr int SCORE_MAX_DIGITS = 2;

constexpr float SCORE_DIGIT_WIDTH = SCORE_SEGMENT_THICKNESS * 4;
constexpr float SCORE_DIGIT_HEIGHT = SCORE_SEGMENT_THICKNESS * 7;
constexpr float SCORE_DIGIT_SEPERATOR_GAP = SCORE_SEGMENT_THICKNESS;

constexpr float SCORE_WIDTH = SCORE_DIGIT_WIDTH * SCORE_MAX_DIGITS + SCORE_DIGIT_SEPERATOR_GAP * (SCORE_MAX_DIGITS - 1);
constexpr float SCORE_HEIGHT = SCORE_DIGIT_HEIGHT;
constexpr float SCORE_TOP_GAP = SCORE_SEGMENT_THICKNESS * 2;

constexpr std::array<Uint8, 10> SCORE_DIGIT_MAP = {
	0b00111111,
	0b00000110,
	0b01011011,
	0b01001111,
	0b01100110,
	0b01101101,
	0b01111101,
	0b00000111,
	0b01111111,
	0b01101111,
};

constexpr SDL_FRect SCORE_PLAYER1 = {
	.x = (WINDOW_WIDTH - SCORE_WIDTH) / 2,
	.y = SCORE_TOP_GAP,
	.w = SCORE_WIDTH,
	.h = SCORE_HEIGHT,
};

constexpr SDL_FRect SCORE_PLAYER2 = {
	.x = (WINDOW_WIDTH * 3 - SCORE_WIDTH * 2) / 4,
	.y = SCORE_TOP_GAP,
	.w = SCORE_WIDTH,
	.h = SCORE_HEIGHT,
};

void RenderScoreSegmentType(SDL_Renderer* renderer, ScoreSegmentType st, SDL_FRect rect);
void RenderScoreSegment(SDL_Renderer* renderer, Uint8 segment, SDL_FRect rect);
void RenderScoreDigit(SDL_Renderer* renderer, Uint8 digit, SDL_FRect rect);
void RenderScoreDigitInPlace(SDL_Renderer* renderer, Uint8 digit, Uint8 place, SDL_FRect rect);
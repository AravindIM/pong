#pragma once
#include "config.h"
#include<SDL3/SDL.h>

enum ScoreSegmentType {
	SCORE_SEGMENT_HORIZONTAL,
	SCORE_SEGMENT_VERTICAL
};

constexpr float SCORE_SEGMENT_THICKNESS = 5.0;

constexpr int SCORE_DIGIT_COUNT = 2;

constexpr float SCORE_DIGIT_WIDTH = SCORE_SEGMENT_THICKNESS * 4;
constexpr float SCORE_DIGIT_HEIGHT = SCORE_SEGMENT_THICKNESS * 7;
constexpr float SCORE_DIGIT_SEPERATOR_GAP = SCORE_SEGMENT_THICKNESS;

constexpr float SCORE_WIDTH = SCORE_DIGIT_WIDTH * SCORE_DIGIT_COUNT + SCORE_DIGIT_SEPERATOR_GAP * (SCORE_DIGIT_COUNT - 1);
constexpr float SCORE_HEIGHT = SCORE_DIGIT_HEIGHT;
constexpr float SCORE_TOP_GAP = SCORE_SEGMENT_THICKNESS * 2;

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

class Scoreboard {
	SDL_FRect mRect{};
	float mThickness{ SCORE_SEGMENT_THICKNESS };
public:
	Scoreboard(SDL_FRect rect);
	void RenderSegmentType(SDL_Renderer* renderer, ScoreSegmentType st, SDL_FRect rect);
	void RenderSegment(SDL_Renderer* renderer, Uint8 segment, SDL_FRect rect);
};
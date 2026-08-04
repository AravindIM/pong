// pong.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <string>
#include <SDL3/SDL.h>
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define FRAME_RATE 30
#define SECOND_IN_MS 1000
#define FRAME_TIME ( SECOND_IN_MS / FRAME_RATE )
#define PLAYER_PADDLE_WIDTH 50
#define PLAYER_PADDLE_HEIGHT 200
#define PLAYER_PADDLE_START_Y ((WINDOW_HEIGHT - PLAYER_PADDLE_HEIGHT) / 2)
#define PLAYER1_PADDLE_START_X 0
#define PLAYER2_PADDLE_START_X (WINDOW_WIDTH - PLAYER_PADDLE_WIDTH)
#define BALL_SIZE 50
#define BALL_START_X ((WINDOW_WIDTH - BALL_SIZE) / 2)
#define BALL_START_Y ((WINDOW_HEIGHT - BALL_SIZE) / 2)

// TODO: Reference additional headers your program requires here.

// pong.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <string>
#include <SDL3/SDL.h>
#include <SDL3/SDL_gamepad.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define MAX_PLAYERS 2
#define PLAYER_PADDLE_WIDTH 50
#define PLAYER_PADDLE_HEIGHT 200
#define PLAYER_PADDLE_START_Y ((WINDOW_HEIGHT - PLAYER_PADDLE_HEIGHT) / 2)
#define PLAYER1_PADDLE_START_X 0
#define PLAYER2_PADDLE_START_X (WINDOW_WIDTH - PLAYER_PADDLE_WIDTH)
#define PLAYER_PADDLE_MIN_Y 0
#define PLAYER_PADDLE_MAX_Y (WINDOW_HEIGHT - PLAYER_PADDLE_HEIGHT)
#define PLAYER_PADDLE_SPEED (WINDOW_HEIGHT / 2)
#define BALL_SIZE 50
#define BALL_START_X ((WINDOW_WIDTH - BALL_SIZE) / 2)
#define BALL_START_Y ((WINDOW_HEIGHT - BALL_SIZE) / 2)
#define BG_COLOR 0x00
#define FG_COLOR 0xFF
#define JOINED_COLOR 0xFF
#define LEFT_COLOR 0x55
#define JOYSTICK_DEADZONE 1000
#define VSYNC_ENABLED 1

// TODO: Reference additional headers your program requires here.

// pong.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <string>
#include <SDL3/SDL.h>
#include <SDL3/SDL_gamepad.h>


// Graphics
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define VSYNC_ENABLED 1
#define BG_COLOR 0x00
#define FG_COLOR 0xFF
#define JOINED_COLOR 0xFF
#define LEFT_COLOR 0x55


// Input
#define JOYSTICK_DEADZONE 1000


// Ball

// Dimensions
#define BALL_SIZE 10

// Coordinates
#define BALL_START_X ((WINDOW_WIDTH - BALL_SIZE) / 2)
#define BALL_START_Y ((WINDOW_HEIGHT - BALL_SIZE) / 2)

// Bounds
#define BALL_MIN_X 0
#define BALL_MAX_X (WINDOW_WIDTH - BALL_SIZE)
#define BALL_MIN_Y 0
#define BALL_MAX_Y (WINDOW_HEIGHT - BALL_SIZE)

// Physics
#define BALL_START_VX -400
#define BALL_START_VY -200


// Players

// Count
#define MAX_PLAYERS 2

// Dimensions
#define PLAYER_PADDLE_WIDTH BALL_SIZE
#define PLAYER_PADDLE_HEIGHT 80

// Coordinates
#define PLAYER_PADDLE_GAP BALL_SIZE
#define PLAYER_PADDLE_START_Y ((WINDOW_HEIGHT - PLAYER_PADDLE_HEIGHT) / 2)
#define PLAYER1_PADDLE_START_X PLAYER_PADDLE_GAP
#define PLAYER2_PADDLE_START_X (WINDOW_WIDTH - PLAYER_PADDLE_WIDTH - PLAYER_PADDLE_GAP)

// Bounds
#define PLAYER_PADDLE_MIN_Y 0
#define PLAYER_PADDLE_MAX_Y (WINDOW_HEIGHT - PLAYER_PADDLE_HEIGHT)

// Physics
#define PLAYER_PADDLE_SPEED (WINDOW_HEIGHT / 2)

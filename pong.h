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

void cleanup(SDL_Window* window);

// TODO: Reference additional headers your program requires here.

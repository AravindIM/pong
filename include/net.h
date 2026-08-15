#pragma once

#include<SDL3/SDL.h>

#define NET_WIDTH 2
#define NET_STRIP_SIZE 20
#define NET_GAP 10

void RenderNet(SDL_Renderer* renderer, float xCenter, float length);
#pragma once

#include<SDL3/SDL.h>

constexpr float NET_WIDTH = 2;
constexpr float NET_STRIP_SIZE = 20;
constexpr float NET_GAP = 10;

void RenderNet(SDL_Renderer* renderer, float xCenter, float length);
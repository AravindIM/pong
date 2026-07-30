// pong.cpp : Defines the entry point for the application.
//

#include "pong.h"

int main(int argc, char* argv[])
{
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), nullptr);
		return -1;
	}
	SDL_Window* window = SDL_CreateWindow("Pong", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
	if (!window)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), nullptr);
		cleanup(window);
		return -1;
	}

	SDL_Event event;

	bool running = true;

	while (running) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_EVENT_QUIT:
				running = false;
				break;
			default:
				break;
			}
		}
	}

	cleanup(window);
	return 0;
}

void cleanup(SDL_Window* window) {
	SDL_DestroyWindow(window);
	SDL_Quit();
}

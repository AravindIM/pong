// pong.cpp : Defines the entry point for the application.
//

#include "pong.h"

class Pong {
	SDL_Window* mWindow;
	bool mRunning;

	void MainLoop();
	void Tick();
	void Input();
	void Cleanup();

public:
	Pong();
	~Pong();
	bool Init();
	void Run();
};

void Pong::MainLoop() {
	Uint64 fps = 0;
	Uint64 lastTime = SDL_GetTicks();
	Uint64 frameStart, elapsed;
	while (mRunning) {
		frameStart = SDL_GetTicks();
		Tick();
		fps++;
		if (frameStart > lastTime + SECOND_IN_MS) {
			std::string title = "Pong: ";
			title += std::to_string(fps);
			title += " FPS";
			SDL_SetWindowTitle(mWindow, title.c_str());
			lastTime = frameStart;
			fps = 0;
		}
		elapsed = SDL_GetTicks() - frameStart;
		if (elapsed < FRAME_TIME) {
			SDL_Delay(static_cast<Uint32>(FRAME_TIME - elapsed));
		}
	}
}

void Pong::Tick() {
	Input();
}

void Pong::Input() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_EVENT_QUIT:
			mRunning = false;
			break;
		default:
			break;
		}
	}
}


void Pong::Cleanup() {
	if (mWindow) {
		SDL_DestroyWindow(mWindow);
		mWindow = nullptr;
	}
	SDL_Quit();
}

bool Pong::Init() {
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), nullptr);
		Cleanup();
		return false;
	}
	mWindow = SDL_CreateWindow("Pong", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
	if (!mWindow)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", SDL_GetError(), nullptr);
		Cleanup();
		return false;
	}
	return true;
}

Pong::Pong() : mWindow(nullptr), mRunning(false) {}

Pong::~Pong() {
	Cleanup();
}

void Pong::Run() {
	mRunning = true;
	MainLoop();
}

int main(int argc, char* argv[])
{
	Pong pong;
	if (!pong.Init()) {
		return 1;
	}
	pong.Run();
	return 0;
}

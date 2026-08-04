// pong.cpp : Defines the entry point for the application.
//

#include "pong.h"

class Pong {
	bool mRunning;
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	SDL_FRect mPlayer1;
	SDL_FRect mPlayer2;
	SDL_FRect mBall;

	void MainLoop();
	void Tick();
	void EventLoop();
	void Render();
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
	EventLoop();
	Render();
}

void Pong::EventLoop() {
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

void Pong::Render() {
	SDL_SetRenderDrawColor(mRenderer, 0x00, 0x00, 0x00, 0xFF);
	SDL_RenderClear(mRenderer);

	SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderFillRect(mRenderer, &mPlayer1);
	SDL_RenderFillRect(mRenderer, &mPlayer2);
	SDL_RenderFillRect(mRenderer, &mBall);

	SDL_RenderPresent(mRenderer);
}


void Pong::Cleanup() {
	if (mRenderer) {
		SDL_DestroyRenderer(mRenderer);
		mRenderer = nullptr;
	}
	if (mWindow) {
		SDL_DestroyWindow(mWindow);
		mWindow = nullptr;
	}
	SDL_Quit();
}

bool Pong::Init() {
	if (!SDL_Init(SDL_INIT_VIDEO))
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "SDL Could not initialise the video!", nullptr);
		Cleanup();
		return false;
	}
	mWindow = SDL_CreateWindow("Pong", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE);
	if (!mWindow)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "SDL Could not create the window!", nullptr);
		Cleanup();
		return false;
	}
	mRenderer = SDL_CreateRenderer(mWindow, nullptr);
	if (!mRenderer) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "SDL Could not create renderer!", nullptr);
		Cleanup();
		return false;
	}
	if (!SDL_SetRenderLogicalPresentation(mRenderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_INTEGER_SCALE)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "SDL Could not scale to the display!", nullptr);
		Cleanup();
		return false;
	}
	return true;
}

Pong::Pong()
	: mRunning(false)
	, mWindow(nullptr)
	, mRenderer(nullptr)
	, mPlayer1{ .x = PLAYER1_PADDLE_START_X, .y = PLAYER_PADDLE_START_Y,
				.w = PLAYER_PADDLE_WIDTH, .h = PLAYER_PADDLE_HEIGHT }
	, mPlayer2{ .x = PLAYER2_PADDLE_START_X, .y = PLAYER_PADDLE_START_Y,
				.w = PLAYER_PADDLE_WIDTH, .h = PLAYER_PADDLE_HEIGHT }
	, mBall{ .x = BALL_START_X, .y = BALL_START_Y,
				.w = BALL_SIZE, .h = BALL_SIZE }
{}

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

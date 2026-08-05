// pong.cpp : Defines the entry point for the application.
//

#include "pong.h"

struct Player {
	float mX;
	float mY;
	SDL_Gamepad* mPad;
	Player(float x, float y);
	SDL_FRect GetFRect();
};

Player::Player(float x, float y): mX(x), mY(y), mPad(nullptr){}

SDL_FRect Player::GetFRect() {
	return {
		.x = mX,
		.y = mY,
		.w = PLAYER_PADDLE_WIDTH,
		.h = PLAYER_PADDLE_HEIGHT,
	};
}

class Pong {
	bool mRunning;
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Player mPlayers[MAX_PLAYERS];
	SDL_FRect mBall;

	void MainLoop();
	void Tick();
	void EventLoop();
	void Update();
	void Render();
	void Cleanup();
	void AddPad(SDL_JoystickID id);
	void RemovePad(SDL_JoystickID id);
	void ToggleJoin(SDL_JoystickID id);

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
	Update();
	Render();
}

void Pong::EventLoop() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_EVENT_QUIT:
			mRunning = false;
			break;
		case SDL_EVENT_GAMEPAD_ADDED:
			AddPad(event.gdevice.which);
			break;
		case SDL_EVENT_GAMEPAD_REMOVED:
			RemovePad(event.gdevice.which);
			break;
		case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
			if (event.gbutton.button == SDL_GAMEPAD_BUTTON_START) {
				ToggleJoin(event.gdevice.which);
			}
			break;
		default:
			break;
		}
	}
}

void Pong::Update() {
	for (Player& p : mPlayers) {
		if (p.mPad) {
			Sint16 yAxis = SDL_GetGamepadAxis(p.mPad, SDL_GAMEPAD_AXIS_LEFTY);
			if (yAxis < JOYSTICK_TOP_DEAD_ZONE && p.mY > PLAYER_PADDLE_MIN_Y) {
				p.mY-= PLAYER_PADDLE_SPEED;
			}
			else if (yAxis > JOYSTICK_BOTTOM_DEADZONE && p.mY < PLAYER_PADDLE_MAX_Y) {
				p.mY += PLAYER_PADDLE_SPEED;
			}
		}
	}
}

void Pong::AddPad(SDL_JoystickID id) {
	SDL_OpenGamepad(id);
}

void Pong::RemovePad(SDL_JoystickID id) {
	for (Player& p : mPlayers) {
		if (p.mPad && SDL_GetGamepadID(p.mPad) == id) {
			p.mPad = nullptr;
		}
	}
	SDL_Gamepad* pad = SDL_GetGamepadFromID(id);
	if (pad) {
		SDL_CloseGamepad(pad);
	}
}

void Pong::ToggleJoin(SDL_JoystickID id) {
	SDL_Gamepad* pad = SDL_GetGamepadFromID(id);
	if (!pad) return;

	for (Player& p : mPlayers) {
		if (p.mPad == pad) {
			p.mPad = nullptr;
			return;
		}
	}

	for (Player& p : mPlayers) {
		if (!p.mPad) {
			p.mPad = pad;
			return;
		}
	}
}

void Pong::Render() {
	Uint8 color = BG_COLOR;
	SDL_SetRenderDrawColor(mRenderer, color, color, color, 0xFF);
	SDL_RenderClear(mRenderer);

	for (Player p : mPlayers) {
		SDL_FRect pRect = p.GetFRect();
		if (p.mPad) {
			color = JOINED_COLOR;
		}
		else {
			color = LEFT_COLOR;
		}
		SDL_SetRenderDrawColor(mRenderer, color, color, color, 0xFF);
		SDL_RenderFillRect(mRenderer, &pRect);
	}
	color = FG_COLOR;
	SDL_SetRenderDrawColor(mRenderer, color, color, color, 0xFF);
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
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMEPAD))
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
	, mPlayers{
		Player(PLAYER1_PADDLE_START_X, PLAYER_PADDLE_START_Y),
		Player(PLAYER2_PADDLE_START_X, PLAYER_PADDLE_START_Y)
	}
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

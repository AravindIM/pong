// pong.cpp : Defines the entry point for the application.
//

#include "pong.h"

enum Direction {
	UP = -1,
	DOWN = 1
};
enum PlayerVariant {
	LEFT,
	RIGHT,
};

struct Player {
	SDL_FRect mRect;
	PlayerVariant mVariant;
	SDL_Gamepad* mPad;
	Player(PlayerVariant variant);
	void Reset();
	void Move(Direction dir, double deltaTime);
};

Player::Player(PlayerVariant variant)
	: mVariant(variant)
	, mPad(nullptr){
	Reset();
}

void Player::Reset() {
	float x;
	switch (mVariant) {
	case LEFT:
		x = PLAYER1_PADDLE_START_X;
		break;
	case RIGHT:
		x = PLAYER2_PADDLE_START_X;
		break;
	}
	mRect = SDL_FRect(x, PLAYER_PADDLE_START_Y, PLAYER_PADDLE_WIDTH, PLAYER_PADDLE_HEIGHT);
}

void Player::Move(Direction dir, double deltaTime) {
	float dy = (float)(PLAYER_PADDLE_SPEED * deltaTime);
	mRect.y = SDL_clamp(mRect.y + (float)dir * dy, PLAYER_PADDLE_MIN_Y, PLAYER_PADDLE_MAX_Y);
}

struct Ball {
	SDL_FRect mRect;
	float mVx;
	float mVy;
	Ball();
	void Reset();
	void Move(double deltaTime);
};

Ball::Ball(){
	Reset();
}

void Ball::Reset() {
	mRect = SDL_FRect(BALL_START_X, BALL_START_Y, BALL_SIZE, BALL_SIZE);
	mVx = BALL_START_VX;
	mVy = BALL_START_VY;
}

void Ball::Move(double deltaTime) {
	mRect.x = SDL_clamp(mRect.x + (float)(mVx * deltaTime), BALL_MIN_X, BALL_MAX_X);
	mRect.y = SDL_clamp(mRect.y + (float)(mVy * deltaTime), BALL_MIN_Y, BALL_MAX_Y);
}

class Clock {
	Uint64 mStartTime;
public:
	Clock();
	void Reset();
	double GetDTSec();
};

Clock::Clock(): mStartTime(0) {}

void Clock::Reset() {
	mStartTime = SDL_GetTicksNS();
}

double Clock::GetDTSec() {
	return (double)(SDL_GetTicksNS() - mStartTime) / SDL_NS_PER_SECOND;
}

class FPSCalculator {
	int mFrames;
	Uint64 mStartTime;
	Uint64 mFrameStartTime;
	Uint64 mElapsed;
	int mFPS;

public:
	FPSCalculator();
	void Reset();
	void FrameStart();
	void FrameEnd();
	int GetFPS();
	Uint64 GetFrameDT();
	Uint64 GetDT();
};

FPSCalculator::FPSCalculator()
	: mFrames(0)
	, mStartTime(0)
	, mFrameStartTime(0)
	, mElapsed(0)
	, mFPS(0)
{}

void FPSCalculator::Reset() {
	mStartTime = SDL_GetTicksNS();
}
void FPSCalculator::FrameStart() {
	mFrameStartTime = SDL_GetTicksNS();
}

void FPSCalculator::FrameEnd() {
	mElapsed += GetFrameDT();
	mFrames++;
}

int FPSCalculator::GetFPS() {
	mFPS = (int)(mFrames * SDL_NS_PER_SECOND / mElapsed);
	mElapsed = 0;
	mFrames = 0;
	mStartTime = SDL_GetTicksNS();
	return mFPS;
}

Uint64 FPSCalculator::GetFrameDT() {
	return SDL_GetTicksNS() - mFrameStartTime;
}

Uint64 FPSCalculator::GetDT() {
	return SDL_GetTicksNS() - mStartTime;
}


class Pong {
	bool mExitGame;
	bool mFullscreen;
	bool mLobby;
	bool mPlaying;
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Player mPlayers[MAX_PLAYERS];
	Ball mBall;
	Clock mClock;

	void MainLoop();
	void Tick();
	void EventLoop();
	void DisableFullscreen();
	void ToggleFullscreen();
	void ToggleGameState();
	void StopGame();
	void Update();
	void Render();
	void Cleanup();
	void HandleCollision();
	void AddPad(SDL_JoystickID id);
	void RemovePad(SDL_JoystickID id);
	void HandleBackButton(SDL_JoystickID id);
	void HandleStartButton(SDL_JoystickID id);

public:
	Pong();
	~Pong();
	bool Init();
	void Run();
};

void Pong::MainLoop() {
	FPSCalculator fpsCalc;
	mClock.Reset();
	fpsCalc.Reset();
	while (!mExitGame) {
		fpsCalc.FrameStart();
		Tick();
		fpsCalc.FrameEnd();
		if (fpsCalc.GetDT() > SDL_NS_PER_SECOND) {
			std::string title = "Pong [";
			title += std::to_string(fpsCalc.GetFPS());
			title += " FPS]";
			SDL_SetWindowTitle(mWindow, title.c_str());
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
			mExitGame = true;
			break;
		case SDL_EVENT_GAMEPAD_ADDED:
			AddPad(event.gdevice.which);
			break;
		case SDL_EVENT_GAMEPAD_REMOVED:
			RemovePad(event.gdevice.which);
			break;
		case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
			switch (event.gbutton.button) {
			case SDL_GAMEPAD_BUTTON_START:
				HandleStartButton(event.gdevice.which);
				break;
			case SDL_GAMEPAD_BUTTON_BACK:
				HandleBackButton(event.gdevice.which);
				break;
			default:
				break;
			}
			break;
		case SDL_EVENT_KEY_DOWN:
			switch (event.key.key) {
			case SDLK_ESCAPE:
				DisableFullscreen();
				break;
			case SDLK_F11:
				ToggleFullscreen();
				break;
			default:
				break;
			}
			break;
		default:
			break;
		}
	}
}

void Pong::DisableFullscreen() {
	mFullscreen = false;
	SDL_SetWindowFullscreen(mWindow, mFullscreen);
}

void Pong::ToggleFullscreen() {
	mFullscreen = !mFullscreen;
	SDL_SetWindowFullscreen(mWindow, mFullscreen);
}

void Pong::ToggleGameState() {
	if (mLobby) {
		mLobby = false;
		mPlaying = true;
	}
	else if (!mPlaying) {
		mLobby = true;
		mBall.Reset();
		for (Player& p : mPlayers) {
			p.Reset();
		}
		mLobby = true;
	}
}

void Pong::StopGame() {
	mPlaying = false;
}

void Pong::Update() {
	double deltaTime = mClock.GetDTSec();
	mClock.Reset();
	if (mPlaying) {
		for (Player& p : mPlayers) {
			if (p.mPad) {
				Sint16 yAxis = SDL_GetGamepadAxis(p.mPad, SDL_GAMEPAD_AXIS_LEFTY);
				if (SDL_abs(yAxis) >= JOYSTICK_DEADZONE) {
					if (yAxis < 0) p.Move(UP, deltaTime);
					if (yAxis > 0) p.Move(DOWN, deltaTime);
				}
			}
		}
		mBall.Move(deltaTime);
		HandleCollision();
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

void Pong::HandleBackButton(SDL_JoystickID id) {
	SDL_Gamepad* pad = SDL_GetGamepadFromID(id);
	if (!pad) return;

	for (Player& p : mPlayers) {
		if (p.mPad == pad) {
			ToggleFullscreen();
			return;
		}
	}
}

void Pong::HandleStartButton(SDL_JoystickID id) {
	SDL_Gamepad* pad = SDL_GetGamepadFromID(id);
	if (!pad) return;

	for (Player& p : mPlayers) {
		if (p.mPad == pad) {
			ToggleGameState();
			return;
		}
		if (!p.mPad) {
			p.mPad = pad;
			return;
		}
	}
}

void Pong::HandleCollision() {
	if (mBall.mRect.y <= BALL_MIN_Y) {
		mBall.mRect.y = BALL_MIN_Y;
		mBall.mVy *= -1;
	} else if (mBall.mRect.y >= BALL_MAX_Y) {
		mBall.mRect.y = BALL_MAX_Y;
		mBall.mVy *= -1;
	}
	if (mBall.mRect.x <= BALL_MIN_X) {
		mBall.mRect.x = BALL_MIN_X;
		StopGame();
	}
	else if (mBall.mRect.x >= BALL_MAX_X) {
		mBall.mRect.x = BALL_MAX_X;
		StopGame();
	}
	for (const Player& p : mPlayers) {
		if (!SDL_HasRectIntersectionFloat(&p.mRect, &mBall.mRect)) continue;
		if (p.mVariant == RIGHT && mBall.mVx > 0) {
			mBall.mRect.x = p.mRect.x - mBall.mRect.w;
			mBall.mVx *= -1;
		} else if (p.mVariant == LEFT && mBall.mVx < 0) {
			mBall.mRect.x = p.mRect.x + p.mRect.w;
			mBall.mVx *= -1;
		}

	}
}

void Pong::Render() {
	Uint8 color = BG_COLOR;
	SDL_SetRenderDrawColor(mRenderer, color, color, color, 0xFF);
	SDL_RenderClear(mRenderer);

	for (const Player& p : mPlayers) {
		if (p.mPad) {
			color = JOINED_COLOR;
		}
		else {
			color = LEFT_COLOR;
		}
		SDL_SetRenderDrawColor(mRenderer, color, color, color, 0xFF);
		SDL_RenderFillRect(mRenderer, &p.mRect);
	}

	color = FG_COLOR;;
	SDL_SetRenderDrawColor(mRenderer, color, color, color, 0xFF);
	SDL_RenderFillRect(mRenderer, &mBall.mRect);

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
	mWindow = SDL_CreateWindow("Pong", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
	if (!mWindow)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "SDL Could not create the window!", nullptr);
		Cleanup();
		return false;
	}
	SDL_SetWindowFullscreen(mWindow, mFullscreen);
	mRenderer = SDL_CreateRenderer(mWindow, nullptr);
	if (!mRenderer) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "SDL Could not create renderer!", nullptr);
		Cleanup();
		return false;
	}
	if (!SDL_SetRenderVSync(mRenderer, SDL_RENDERER_VSYNC_ADAPTIVE)) {
		SDL_SetRenderVSync(mRenderer, VSYNC_ENABLED);
	}
	if (!SDL_SetRenderLogicalPresentation(mRenderer, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_LOGICAL_PRESENTATION_LETTERBOX)) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "SDL Could not scale to the display!", nullptr);
		Cleanup();
		return false;
	}
	return true;
}

Pong::Pong()
	: mExitGame(false)
	, mFullscreen(true)
	, mLobby(true)
	, mPlaying(false)
	, mWindow(nullptr)
	, mRenderer(nullptr)
	, mPlayers{
		Player(LEFT),
		Player(RIGHT)
	}
	, mBall()
{}

Pong::~Pong() {
	Cleanup();
}

void Pong::Run() {
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

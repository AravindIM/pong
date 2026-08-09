// pong.cpp : Defines the entry point for the application.
//

#include "pong.h"

enum Direction {
	UP = -1,
	DOWN = 1
};
enum CollisionEdge {
	COLLIDE_LEFT,
	COLLIDE_RIGHT
};

struct Player {
	SDL_FRect mRect;
	CollisionEdge mEdge;
	SDL_Gamepad* mPad;
	Player(float x, float y, CollisionEdge collisionEdge);
	void Move(Direction dir, double deltaTime);
};

Player::Player(float x, float y, CollisionEdge collisionEdge)
	: mRect(x, y, PLAYER_PADDLE_WIDTH, PLAYER_PADDLE_HEIGHT)
	, mEdge(collisionEdge)
	, mPad(nullptr){}

void Player::Move(Direction dir, double deltaTime) {
	float dy = (float)(PLAYER_PADDLE_SPEED * deltaTime);
	mRect.y = SDL_clamp(mRect.y + (float)dir * dy, PLAYER_PADDLE_MIN_Y, PLAYER_PADDLE_MAX_Y);
}

struct Ball {
	SDL_FRect mRect;
	float mVx;
	float mVy;
	Ball();
	void Move(double deltaTime);
	void PaddleBounce();
	void WallBounce();
};

Ball::Ball()
	: mRect(BALL_START_X, BALL_START_Y,BALL_SIZE, BALL_SIZE)
	, mVx(BALL_START_VX)
	, mVy(BALL_START_VY) {}

void Ball::Move(double deltaTime) {
	float dx, dy;
	if (mRect.y <= BALL_MIN_Y) {
		mRect.y = BALL_MIN_Y;
		WallBounce();
	}
	if (mRect.y >= BALL_MAX_Y) {
		mRect.y = BALL_MAX_Y;
		WallBounce();
	}
	dx = (float)(mVx * deltaTime);
	dy = (float)(mVy * deltaTime);
	mRect.x = SDL_clamp(mRect.x + dx, BALL_MIN_X, BALL_MAX_X);
	mRect.y = SDL_clamp(mRect.y + dy, BALL_MIN_Y, BALL_MAX_Y);
}

void Ball::PaddleBounce() {
	mVx *= -1;
}

void Ball::WallBounce() {
	mVy *= -1;
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
	bool mRunning;
	SDL_Window* mWindow;
	SDL_Renderer* mRenderer;
	Player mPlayers[MAX_PLAYERS];
	Ball mBall;
	Clock mClock;

	void MainLoop();
	void Tick();
	void EventLoop();
	void Update();
	void Render();
	void Cleanup();
	bool DetectCollision(Player p, Ball b);
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
	FPSCalculator fpsCalc;
	mClock.Reset();
	fpsCalc.Reset();
	while (mRunning) {
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
	double deltaTime = mClock.GetDTSec();
	mClock.Reset();
	for (Player& p : mPlayers) {
		if (p.mPad) {
			Sint16 yAxis = SDL_GetGamepadAxis(p.mPad, SDL_GAMEPAD_AXIS_LEFTY);
			if (SDL_abs(yAxis) >= JOYSTICK_DEADZONE) {
				if (yAxis < 0) p.Move(UP, deltaTime);
				if (yAxis > 0) p.Move(DOWN, deltaTime);
			}
			if (DetectCollision(p, mBall)) {
				mBall.PaddleBounce();
			}
		}
	}
	mBall.Move(deltaTime);
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

bool Pong::DetectCollision(Player p, Ball b) {
	switch (p.mEdge) {
	case COLLIDE_LEFT:
		if ((b.mRect.x + b.mRect.w) >= p.mRect.x && (b.mRect.y + b.mRect.h - COLLISION_ERROR) >= p.mRect.y && b.mRect.y <= (p.mRect.y + p.mRect.h - COLLISION_ERROR))
			return true;
		break;
	case COLLIDE_RIGHT:
		if (b.mRect.x <= (p.mRect.x + p.mRect.w) && (b.mRect.y + b.mRect.h - COLLISION_ERROR) >= p.mRect.y && b.mRect.y <= (p.mRect.y + p.mRect.h - COLLISION_ERROR))
			return true;
		break;
	default:
		break;
	}
	return false;
}

void Pong::Render() {
	Uint8 color = BG_COLOR;
	SDL_SetRenderDrawColor(mRenderer, color, color, color, 0xFF);
	SDL_RenderClear(mRenderer);

	for (const Player& p : mPlayers) {
		SDL_FRect pRect = p.mRect;
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
	SDL_FRect bRect = mBall.mRect;
	SDL_SetRenderDrawColor(mRenderer, color, color, color, 0xFF);
	SDL_RenderFillRect(mRenderer, &bRect);

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
	: mRunning(false)
	, mWindow(nullptr)
	, mRenderer(nullptr)
	, mPlayers{
		Player(PLAYER1_PADDLE_START_X, PLAYER_PADDLE_START_Y, COLLIDE_RIGHT),
		Player(PLAYER2_PADDLE_START_X, PLAYER_PADDLE_START_Y, COLLIDE_LEFT)
	}
	, mBall()
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

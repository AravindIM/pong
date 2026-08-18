#include "game.h"
#include "net.h"

Game::Game()
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

Game::~Game() {
	Cleanup();
}

bool Game::Init() {
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

void Game::Run() {
	MainLoop();
}

void Game::MainLoop() {
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

void Game::Tick() {
	EventLoop();
	Update();
	Render();
}

void Game::EventLoop() {
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
				HandleGamepadStartButton(event.gdevice.which);
				break;
			case SDL_GAMEPAD_BUTTON_BACK:
				HandleGamepadBackButton(event.gdevice.which);
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

void Game::Update() {
	float deltaTime = mClock.GetDTSec();
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

void Game::Render() {
	RenderClear();
	RenderNet(mRenderer, WINDOW_WIDTH/2, WINDOW_HEIGHT);
	for (Player& p : mPlayers) {
		p.Render(mRenderer);
	}
	mBall.Render(mRenderer);
	SDL_RenderPresent(mRenderer);
}

void Game::RenderClear() {
	SDL_SetRenderDrawColor(mRenderer, BG_COLOR, BG_COLOR, BG_COLOR, 0xFF);
	SDL_RenderClear(mRenderer);
}

void Game::HandleCollision() {
	if (mBall.mRect.y <= BALL_MIN_Y) {
		mBall.mRect.y = BALL_MIN_Y;
		mBall.mVy *= -1;
	}
	else if (mBall.mRect.y >= BALL_MAX_Y) {
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
		}
		else if (p.mVariant == LEFT && mBall.mVx < 0) {
			mBall.mRect.x = p.mRect.x + p.mRect.w;
			mBall.mVx *= -1;
		}

	}
}

void Game::AddPad(SDL_JoystickID id) {
	SDL_OpenGamepad(id);
}

void Game::RemovePad(SDL_JoystickID id) {
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

void Game::HandleGamepadBackButton(SDL_JoystickID id) {
	SDL_Gamepad* pad = SDL_GetGamepadFromID(id);
	if (!pad) return;

	for (Player& p : mPlayers) {
		if (p.mPad == pad) {
			ToggleFullscreen();
			return;
		}
	}
}

void Game::HandleGamepadStartButton(SDL_JoystickID id) {
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

void Game::StopGame() {
	mPlaying = false;
}

void Game::ToggleGameState() {
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

void Game::DisableFullscreen() {
	mFullscreen = false;
	SDL_SetWindowFullscreen(mWindow, mFullscreen);
}

void Game::ToggleFullscreen() {
	mFullscreen = !mFullscreen;
	SDL_SetWindowFullscreen(mWindow, mFullscreen);
}


void Game::Cleanup() {
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
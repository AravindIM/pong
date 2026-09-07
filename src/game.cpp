#include "game.h"
#include "net.h"
#include "score.h"

Game::~Game() {
	Cleanup();
}

bool Game::Init() {
	if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMEPAD))
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
	if (!mSound.Init()) {
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "SDL could not initialize the audio stream!", nullptr);
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
	if (mLobby || mPlaying) {
		for (Player& p : mPlayers) {
			if (p.mPad) {
				Sint16 yAxis = SDL_GetGamepadAxis(p.mPad, SDL_GAMEPAD_AXIS_LEFTY);
				if (SDL_abs(yAxis) >= JOYSTICK_DEADZONE) {
					if (yAxis < 0) p.Move(UP, deltaTime);
					if (yAxis > 0) p.Move(DOWN, deltaTime);
				}
			}
		}
	}
	if (mPlaying) {
		mBall.Move(deltaTime);
		for (Player& p : mPlayers) {
			if (p.mAI) {
				if ((p.mVariant == RIGHT && mBall.mVx > 0
					&& mBall.mRect.x > WINDOW_WIDTH / 2)
					|| (p.mVariant == LEFT && mBall.mVx < 0
						&& mBall.mRect.x < WINDOW_WIDTH / 2)) {
					if (mBall.mRect.y < p.mRect.y) {
						p.Move(UP, deltaTime);
					}
					else if (mBall.mRect.y + mBall.mRect.h > p.mRect.y + p.mRect.h) {
						p.Move(DOWN, deltaTime);
					}
				}
			}
		}
		HandleCollision();
	}
	else if (!mLobby && mDelay.GetDTSec() >= 3.0) {
		mPlaying = true;
		for (Player& p : mPlayers) {
			p.Reset();
		}
		mBall.Reset();
		mSound.Play(SOUND_START);
	}
}

void Game::Render() {
	RenderClear();
	RenderNet(mRenderer, WINDOW_WIDTH/2, WINDOW_HEIGHT);
	int i = 0;
	for (Player& p : mPlayers) {
		p.Render(mRenderer);
		RenderScore(mRenderer, p.mScore, SCORE_BOARD_PLAYERS[i]);
		i++;
	}
	mBall.Render(mRenderer);
	SDL_RenderPresent(mRenderer);
}

void Game::RenderClear() {
	SDL_SetRenderDrawColor(mRenderer, BG_COLOR, BG_COLOR, BG_COLOR, 0xFF);
	SDL_RenderClear(mRenderer);
}

void Game::HandleCollision() {
	if (!mPlaying) return;
	if (mBall.mRect.y <= BALL_MIN_Y) {
		mBall.mRect.y = BALL_MIN_Y;
		mBall.mVy *= -1;
		mSound.Play(SOUND_BOUNCE);
	}
	else if (mBall.mRect.y >= BALL_MAX_Y) {
		mBall.mRect.y = BALL_MAX_Y;
		mBall.mVy *= -1;
		mSound.Play(SOUND_BOUNCE);
	}
	if (mBall.mRect.x <= BALL_MIN_X) {
		mBall.mRect.x = BALL_MIN_X;
		mPlayers[1].IncrementScore();
		mDelay.Reset();
		mPlaying = false;
		mSound.Play(SOUND_SCORE);
		if (mPlayers[0].IsMaxScore() || mPlayers[1].IsMaxScore()) {
			StopGame();
		}
	}
	else if (mBall.mRect.x >= BALL_MAX_X) {
		mBall.mRect.x = BALL_MAX_X;
		mPlayers[0].IncrementScore();
		mDelay.Reset();
		mPlaying = false;
		mSound.Play(SOUND_SCORE);
		if (mPlayers[0].IsMaxScore() || mPlayers[1].IsMaxScore()) {
			StopGame();
		}
	}
	for (const Player& p : mPlayers) {
		if (!p.IsActive()) continue;
		if (!SDL_HasRectIntersectionFloat(&p.mRect, &mBall.mRect)) continue;
		if (p.mVariant == RIGHT && mBall.mVx > 0) {
			mBall.mRect.x = p.mRect.x - mBall.mRect.w;
			mBall.mVx *= -1;
			mSound.Play(SOUND_HIT);
		}
		else if (p.mVariant == LEFT && mBall.mVx < 0) {
			mBall.mRect.x = p.mRect.x + p.mRect.w;
			mBall.mVx *= -1;
			mSound.Play(SOUND_HIT);
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
			StartGame();
			return;
		}
	}
	for (Player& p : mPlayers) {
		if (!p.IsActive()) {
			p.mPad = pad;
			return;
		}
	}
}

void Game::StopGame() {
	mLobby = true;
	mPlaying = false;
	mBall.Reset();
}

void Game::StartGame() {
	if (mLobby) {
		mLobby = false;
		mPlaying = true;
		if (mPlayers[0].IsMaxScore() || mPlayers[1].IsMaxScore()) {
			mPlayers[0].ResetScore();
			mPlayers[1].ResetScore();
		}
		mBall.Reset();
		for (Player& p : mPlayers) {
			if (!p.IsActive()) {
				p.mAI = true;
			}
			p.Reset();
		}
		mSound.Play(SOUND_START);
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
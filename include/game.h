#pragma once

#include <string>
#include <SDL3/SDL.h>
#include <SDL3/SDL_gamepad.h>
#include "config.h"

#include "player.h"
#include "ball.h"
#include "clock.h"
#include "fps_calculator.h"
#include "sound.h"

constexpr float SCORE_FREEZE_SEC = 1.0f;
constexpr float SERVE_DELAY_SEC = 3.0f;

enum class State {
	Reset,
	Start,
	Serve,
	Playing,
	Score,
};

class Game {
	bool mExitGame{ false };
	bool mFullscreen{ true };
	State mState{State::Reset};
	SDL_Window* mWindow{};
	SDL_Renderer* mRenderer{};
	Player mPlayers[MAX_PLAYERS]{ Player(LEFT), Player(RIGHT) };
	Ball mBall{};
	Clock mClock{};
	Clock mDelay{};
	Sound mSound{};

	void MainLoop();
	void Tick();
	void EventLoop();
	void SetState(State state);
	void Update();
	void UpdatePlayers(float deltaTime);
	void UpdateAI(float deltaTime);
	void UpdateBall(float deltaTime);
	void Render();
	void RenderClear();
	void HandleCollision();
	void AddPad(SDL_JoystickID id);
	void RemovePad(SDL_JoystickID id);
	void HandleGamepadBackButton(SDL_JoystickID id);
	void HandleGamepadStartButton(SDL_JoystickID id);
	void DisableFullscreen();
	void ToggleFullscreen();
	void Cleanup();

public:
	Game() = default;
	~Game();
	bool Init();
	void Run();
};

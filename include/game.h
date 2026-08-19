#pragma once

#include <string>
#include <SDL3/SDL.h>
#include <SDL3/SDL_gamepad.h>
#include "config.h"

#include "player.h"
#include "ball.h"
#include "clock.h"
#include "fps_calculator.h"

class Game {
	bool mExitGame{ false };
	bool mFullscreen{ true };
	bool mLobby{ true };
	bool mPlaying{ false };
	SDL_Window* mWindow{};
	SDL_Renderer* mRenderer{};
	Player mPlayers[MAX_PLAYERS]{ Player(LEFT), Player(RIGHT) };
	Ball mBall{};
	Clock mClock{};

	void MainLoop();
	void Tick();
	void EventLoop();
	void Update();
	void Render();
	void RenderClear();
	void HandleCollision();
	void AddPad(SDL_JoystickID id);
	void RemovePad(SDL_JoystickID id);
	void HandleGamepadBackButton(SDL_JoystickID id);
	void HandleGamepadStartButton(SDL_JoystickID id);
	void StopGame();
	void ToggleGameState();
	void DisableFullscreen();
	void ToggleFullscreen();
	void Cleanup();

public:
	Game() = default;
	~Game();
	bool Init();
	void Run();
};

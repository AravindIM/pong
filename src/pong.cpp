// pong.cpp : Defines the entry point for the application.
//

#include "game.h"

int main(int argc, char* argv[])
{
	Game game;
	if (!game.Init()) {
		return 1;
	}
	game.Run();
	return 0;
}

#include "Game.h"
#include <iostream>

int main(int argc, char** argv)
{
	bool isGamelnit = Game::instance().initialize();
	if (isGamelnit)
	{
		Game::instance().load();
		Game::instance().loop();
		Game::instance().unload();
	}
	Game::instance().close();
	return 0;
}


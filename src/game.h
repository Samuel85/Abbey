#ifndef GAME_H
#define GAME_H

#include "AbadiaDriver.h"
#include "GameDriver.h"
#include "IPalette.h"
#include "FontManager.h"
#include "cpc6128.h"
#include "VigasocoSDL.h"

class Game{
public:	
	Game();
	~Game();	
	void mainLoop();
	void handleEvents();
	void logic();
	void render();
	
	VigasocoSDL *vigasocosdl;
};

#endif
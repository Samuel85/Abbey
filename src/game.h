#ifndef GAME_H
#define GAME_H

#include "Vigasoco.h"

class Game{
public:	
	Game();
	~Game();	
	void mainLoop();
	void handleEvents();
	void logic();
	void render();
	
	Vigasoco *vigasocosdl;
};

#endif
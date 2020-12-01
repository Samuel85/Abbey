#include "system.h"
#include "game.h"

//System sys;

int main(int argc, char *argv[]){
	Game *game = new Game();
	sys->init();
	game->mainLoop();
	sys->quit();
	return 0;
}
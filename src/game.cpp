#include "game.h"
#include "system.h"

Game::Game()
{
	vigasocosdl = new Vigasoco();
	
	if (!vigasocosdl->init()){
		vigasocosdl->end();				
		return;
	}	
}

Game::~Game()
{		
}

void Game::mainLoop()
{
	while(!sys->exit)
	{
		unsigned int frameTime = SDL_GetTicks();
		
		handleEvents();
		logic();
		render();
		
		//cap the frame rate
		if (SDL_GetTicks() - frameTime < sys->minimumFrameTime){
			SDL_Delay(sys->minimumFrameTime - (SDL_GetTicks() - frameTime));
		}
	}
}

void Game::handleEvents()
{
	sys->handleEvents();
		
	if (sys->pad.start){	
		vigasocosdl->changeState(MENU);
		sys->pad.start = false;
	}
}

void Game::logic()
{
	if (sys->informationMode){
		vigasocosdl->toggleInformationMode();
		sys->informationMode = false;
	}
	vigasocosdl->mainLoop();
}

void Game::render()
{
	sys->updateScreen();
}
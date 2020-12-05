#include "game.h"
#include "system.h"

//#define MINIMUM_FRAME_TIME 10
Game::Game()
{
	std::string g_game("abadia");
	vigasocosdl = new VigasocoSDL();
	if (!vigasocosdl->init(g_game))
	{
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
	//if (sys->pad.start)	vigasocosdl->showMenu();
	if (sys->pad.start)
	{	
		vigasocosdl->changeState(Abadia::STATES::MENU);
		sys->pad.start = false;
	}
}

void Game::logic()
{
	if (sys->informationMode)
	{
		vigasocosdl->toggleInformationMode();
		sys->informationMode = false;
	}
	vigasocosdl->mainLoop();
}

void Game::render()
{
	sys->updateScreen();
}
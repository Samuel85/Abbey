#include "game.h"
#include "system.h"

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#endif

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
#ifndef __EMSCRIPTEN__
        while(!sys->exit)
        {
		sys->initFrame();

                handleEvents();
                logic();
                render();

		sys->endFrame();
        }
#else
        if (sys->exit) {
                sys->quit();
                emscripten_cancel_main_loop();
        }
	sys->initFrame();

	if (sys->logicInterrupt) {
		handleEvents();
		logic();
	}
	render();

	sys->endFrame();
#endif
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

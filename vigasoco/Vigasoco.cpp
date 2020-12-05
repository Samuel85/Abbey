// Vigasoco.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include <cmath>
#include "IDrawPlugin.h"
#include "FileLoader.h"
#include "FontManager.h"
#include "InputHandler.h"
#include "IPalette.h"
#include "ITimer.h"
#include "Vigasoco.h"
#include "AbadiaDriver.h"

//para sprintf
#include <stdio.h>

/////////////////////////////////////////////////////////////////////////////
// initialization and cleanup
/////////////////////////////////////////////////////////////////////////////

Vigasoco::Vigasoco()
{
	_speedThrottle = true;

	_game = "abadia";
	_errorMsg = "";

	_driver = 0;
	_palette = 0;
	_drawPlugin = 0;	
	_inputHandler = 0;
	_timingHandler = 0;
	_timer = 0;
	_asyncThread = 0;
	_fontManager = 0;

	_numFrames = 0;
}

Vigasoco::~Vigasoco()
{
}

/////////////////////////////////////////////////////////////////////////////
// init
/////////////////////////////////////////////////////////////////////////////
bool Vigasoco::init(std::string name)
{
	// creates the game driver
	_driver = createGameDriver(name);

	if (!_driver) return false;

	// inits the game driver (load files, decode gfx, preprocessing, etc)
	if (!_driver->init(_palette)) return false;	

	// calls template method to perform specific actions after initialization has been completed
	initCompleted();
	_driver->preRun();

	return true;    
}

void Vigasoco::end()
{
	destroyPalette();
}

void Vigasoco::mainLoop()
{
	_driver->runAsync();
	_driver->runSync();
	_driver->render(NULL);
}

GameDriver * Vigasoco::createGameDriver(std::string game)
{
	// TODO: move this to a factory	
	return new AbadiaDriver();	
}

void Vigasoco::toggleInformationMode()
{
	_driver->showGameLogic(NULL);
}

void Vigasoco::showMenu()
{
	_driver->showMenu();
}

void Vigasoco::changeState(int newState)
{
	_driver->changeState(newState);
}
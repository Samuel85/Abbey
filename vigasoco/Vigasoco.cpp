// Vigasoco.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include <cmath>

#include "Vigasoco.h"
#include "AbadiaDriver.h"

/////////////////////////////////////////////////////////////////////////////
// initialization and cleanup
/////////////////////////////////////////////////////////////////////////////

Vigasoco::Vigasoco()
{
	_driver = 0;
	_palette = 0;
}

Vigasoco::~Vigasoco()
{
}

/////////////////////////////////////////////////////////////////////////////
// init
/////////////////////////////////////////////////////////////////////////////
bool Vigasoco::init()
{
	_palette = new SDLPalette();
	
	// creates the game driver
	_driver = createGameDriver("abadia");

	if (!_driver) return false;

	// inits the game driver (load files, decode gfx, preprocessing, etc)
	if (!_driver->init(_palette)) return false;	

	// calls template method to perform specific actions after initialization has been completed
	_driver->preRun();

	return true;    
}

void Vigasoco::end()
{
	delete _palette;
	_palette = 0;
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
// VIGASOCO Project (c) 2003-2005 by VIGASOCO Project Team
//
//	See readme.txt for license and usage information.
//
/////////////////////////////////////////////////////////////////////////////

// Vigasoco.h
//
//	Abstract class that defines the structure of a VIGASOCO port.
//
//	All ports should inherit from this class. This class defines some template 
//	methods in order to provide a common main loop for a VIGASOCO port.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _VIGASOCO_H_
#define _VIGASOCO_H_

#include <string>

#include "Singleton.h"
#include "SDLPalette.h"
#include "GameDriver.h"

#define VigasocoMain Vigasoco::getSingletonPtr()

class Vigasoco : public Singleton<Vigasoco>
{
// fields
protected:	
	GameDriver *_driver;
	SDLPalette *_palette;

// methods
public:
	// initialization and cleanup
	Vigasoco();
	virtual ~Vigasoco();
	virtual bool init();
	virtual void end();
	virtual void mainLoop();

	GameDriver *getDriver() const { return _driver; }
	SDLPalette *getPalette() const { return _palette; }

	void toggleInformationMode();
	void showMenu();
	void changeState(int newState);

	GameDriver *createGameDriver(std::string game);		
};

#endif	// _VIGASOCO_H_


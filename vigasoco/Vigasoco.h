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
#include "Types.h"

class IDrawPlugin;		// defined in IDrawPlugin.h
class GameDriver;		// defined in GameDriver.h
class FileLoader;		// defined in FileLoader.h
class FontManager;		// defined in FontManager.h
class InputHandler;		// defined in InputHandler.h
class ICriticalSection;	// defined in ICriticalSection.h
class IPalette;			// defined in IPalette.h
class IThread;			// defined in IThread.h
class ITimer;			// defined in Timer.h
class TimingHandler;	// defined in TimingHandler.h

#define VigasocoMain Vigasoco::getSingletonPtr()

class Vigasoco : public Singleton<Vigasoco>
{
// fields
protected:
	int _numFrames;
	bool _speedThrottle;

	std::string _game;
	std::string _errorMsg;
	GameDriver *_driver;
	IPalette *_palette;
	IDrawPlugin *_drawPlugin;	
	InputHandler *_inputHandler;
	ITimer *_timer;
	IThread *_asyncThread;
	TimingHandler *_timingHandler;

	FontManager *_fontManager;

// methods
public:
	// initialization and cleanup
	Vigasoco();
	virtual ~Vigasoco();
	virtual bool init(std::string name);
	virtual void end();

	virtual void mainLoop();

	// getters
	GameDriver *getDriver() const { return _driver; }
	IPalette *getPalette() const { return _palette; }
	IDrawPlugin *getDrawPlugin() const { return _drawPlugin; }	
	InputHandler *getInputHandler() const { return _inputHandler; }
	ITimer *getTimer() const { return _timer; }
	TimingHandler *getTimingHandler() const { return _timingHandler; }
	FontManager *getFontManager() const { return _fontManager; }
	const std::string getError() const { return _errorMsg; }

	// platform services
	//virtual ICriticalSection *createCriticalSection() = 0;
	void toggleInformationMode();
	void showMenu();

protected:
	// template methods

	// construction
	virtual bool platformSpecificInit() = 0;
	virtual void createPalette() = 0;
	virtual void addCustomLoaders(FileLoader *fl) = 0;
	virtual void createDrawPlugin() = 0;
	virtual void createAudioPlugin() = 0;
	virtual void addCustomInputPlugins() = 0;
	virtual void createTimer() = 0;
	virtual void createAsyncThread() = 0;
	virtual void initCompleted(){}

	// destruction
	virtual void destroyAsyncThread() = 0;
	virtual void destroyTimer() = 0;
	virtual void removeCustomInputPlugins() = 0;
	virtual void destroyDrawPlugin() = 0;
	virtual void destroyAudioPlugin() = 0;
	virtual void removeCustomLoaders(FileLoader *fl) = 0;
	virtual void destroyPalette() = 0;
	virtual void platformSpecificEnd() = 0;

	virtual bool processEvents() = 0;
	virtual void initFrame();
	virtual void endFrame(){}

	// helper methods
	GameDriver *createGameDriver(std::string game);
	void processCoreInputs();
	void showFPS(bool skipThisFrame);
};

#endif	// _VIGASOCO_H_

// 4568M-5694

// GameDriver.h
//
//	Abstract class that models a game
//
//	All game drivers should inherit from this class. This class defines some
//	template methods in order to simplify common tasks.
//
//	The lifecycle of a game driver is:
//		* fill all game fields in the constructor (video info, game files, gfx
//		format, input ports, DIPS, etc).
//		* the core calls to GameDriver::init that does the following:
//			- load all the files in the game data entities.
//			- call template method filesLoaded, where the driver does specific
//			initialization (set color palette, unscramble gfx, etc).
//			- decode all game data entity of type GRAPHICS, using the items in
//			_gfxEncoding (the first graphic entity uses the first item, and so on).
//			- call template method graphicsDecoded, where the driver can do any
//			specific processing.
//			- deallocate the memory used by the game data entities.
//			- call template method finishInit, where the driver can do last
//			specific processing before the driver starts running.
//		* the core calls to the template method videoInitialized after creating the
//		graphic plugin.
//		* while the application is running, the core calls to:
//			- GameDriver::runAsync, only once at the beginning, that starts a new
//				thread to execute game logic that isn't synchronized with a frame.
//			- GameDriver::runSync, that executes the logic for a frame.
//			- GameDriver::render, that draws the game bitmap.
//			- GameDriver::showGameLogic, that it's used to show some internal 
//			data in order to better understand how the game works.
//		* when the application is closing, the core calls to the template method
//		videoFinalizing to notify that the graphic plugin is going to be disposed.
//		After that, it calls to the template method end where the driver can 
//		perform any specific cleanup.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _GAME_DRIVER_H_
#define _GAME_DRIVER_H_

#include "FileLoader.h"
#include "GameDataEntity.h"
#include "SDLPalette.h"
#include <string>
#include "Types.h"
#include <vector>

class IDrawPlugin;		// defined in video/IDrawPlugin.h
struct GfxElement;		// defined in GfxData.h
struct GfxEncoding;		// defined in GfxData.h

class GameDriver
{
// types
public:
	typedef std::vector<GameDataEntity *> GameDataEntities;
	typedef std::vector<GfxEncoding *> GfxEncodings;
	typedef std::vector<GfxElement *> GfxElements;
// 	typedef std::vector<InputPort *> InputPorts;

// fields
protected:
	std::string _driverName;
	std::string _fullName;
	VideoInfo _videoInfo;

	int _numInterruptsPerSecond;
	int _numInterruptsPerVideoUpdate;
	int _numInterruptsPerLogicUpdate;

	GameDataEntities _gameFiles;
	GfxEncodings _gfxEncoding;
	GfxElements _gfx;

	SDLPalette *_palette;

	std::string _errorMsg;

// methods
public:
	FileLoader *theFileLoader;
	// getters
	const VideoInfo *getVideoInfo() const { return &_videoInfo; }
	const std::string getDriverName() const { return _driverName; }
	const std::string getFullName() const { return _fullName; }
	const GameDataEntities* getGameFiles() const { return &_gameFiles; }
	const GfxEncodings* getGameGfxEncoding() const { return &_gfxEncoding; }
	const GfxElements* getGameGfx() const { return &_gfx; }
	int getNumInterruptsPerSecond() const { return _numInterruptsPerSecond; }
	int getNumInterruptsPerVideoUpdate() const { return _numInterruptsPerVideoUpdate; }
	int getnumInterruptsPerLogicUpdate() const { return _numInterruptsPerLogicUpdate; }
	const std::string getError() const { return _errorMsg; }

	// game driver initialization and cleanup
	bool init(SDLPalette *pal);
	virtual void end() = 0;

	virtual void preRun() = 0;
	virtual void runSync() = 0;
	virtual void runAsync() = 0;
	virtual void render(IDrawPlugin *dp) = 0;
	virtual void showGameLogic(IDrawPlugin *dp){}
	virtual void showMenu() = 0;
	virtual void changeState(int newState) = 0;


	// initialization and cleanup
	GameDriver(std::string driverName, std::string fullName, int intsPerSecond);
	virtual ~GameDriver();

	virtual void videoInitialized(IDrawPlugin *dp){}
	virtual void videoFinalizing(IDrawPlugin *dp){}



protected:
	// template methods
	virtual void filesLoaded(){}
	virtual void graphicsDecoded(){}
	virtual void finishInit(){}

	// helper methods
	bool loadFiles();
	bool decodeGraphics();
	void deallocateFilesMemory();
};

#endif	// _GAME_DRIVER_H_

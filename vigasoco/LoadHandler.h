// LoadHandler.h
//
//	Abstract class that defines the interface of a handler to load files
//
//	A loader handler is used like this:
//		* preProcess is called before any file has been loaded in order to do
//		any specific pre processing.
//		* load is called once for each file. The handler has to update the internal
//		game data entity memory copying the file data.
//		* postProcess is called after all files have been loaded in order to do
//		any specific post processing.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _LOAD_HANDLER_H_
#define _LOAD_HANDLER_H_

#include "Types.h"

class GameDataEntity;		// defined in GameDataEntity.h

class LoadHandler
{
// fields
protected:
	GameDataEntity *_gameData;

// methods
public:
	LoadHandler(GameDataEntity *gameData){ _gameData = gameData; } 
	virtual ~LoadHandler(){}

	virtual void loadData(int i, UINT8 *data) = 0;
	virtual void preProcess() = 0;
	virtual void postProcess() = 0;
};

#endif	// _LOAD_HANDLER_H_

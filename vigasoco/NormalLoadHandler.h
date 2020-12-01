// NormalLoadHandler.h
//
//	Class that handles normal file loading
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _NORMAL_LOAD_HANDLER_H_
#define _NORMAL_LOAD_HANDLER_H_


#include "LoadHandler.h"

class GameDataEntity;	// defined in GameDataEntity.h

class NormalLoadHandler : public LoadHandler
{
// methods
public:
	NormalLoadHandler(GameDataEntity *gameData);
	~NormalLoadHandler();

	virtual void loadData(int i, UINT8 *data);
	virtual void preProcess();
	virtual void postProcess();
};

#endif	// _NORMAL_LOAD_HANDLER_H_

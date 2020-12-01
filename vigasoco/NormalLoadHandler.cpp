// NormalLoadHandler.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "GameDataEntity.h"
#include "NormalLoadHandler.h"

//para memset
#include <string.h>

/////////////////////////////////////////////////////////////////////////////
// initialization and cleanup
/////////////////////////////////////////////////////////////////////////////

NormalLoadHandler::NormalLoadHandler(GameDataEntity *gameData) : LoadHandler(gameData)
{
}

NormalLoadHandler::~NormalLoadHandler()
{
}

/////////////////////////////////////////////////////////////////////////////
// data manipulation
/////////////////////////////////////////////////////////////////////////////

void NormalLoadHandler::loadData(int i, UINT8 *data)
{
	const GameFile *gf = _gameData->getFile(i);

	// copy loaded data to the GameDataEntity buffer
	UINT8 *dest = _gameData->getData();
	memcpy(&dest[gf->baseAddress], data, gf->fileSize);
}

void NormalLoadHandler::preProcess()
{
}

void NormalLoadHandler::postProcess()
{
}

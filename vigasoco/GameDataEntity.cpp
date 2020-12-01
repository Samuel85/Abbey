// GameDataEntity.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include "GameDataEntity.h"
#include "NormalLoadHandler.h"

/////////////////////////////////////////////////////////////////////////////
// initialization and cleanup
/////////////////////////////////////////////////////////////////////////////

GameDataEntity::GameDataEntity(DataType type, std::string name, LoadingMode mode)
{
	_type = type;
	_name = name;
	_mode = mode;
	_data = 0;
	_loadHandler = setLoadHandler(mode);
}

GameDataEntity::~GameDataEntity()
{
	assert(!_data);

	free();

	// delete game's files
	for (GameFiles::size_type i = 0; i < _files.size(); i++){
		delete _files[i];
	}
	
	delete _loadHandler;
}

/////////////////////////////////////////////////////////////////////////////
// non inline getters & setters
/////////////////////////////////////////////////////////////////////////////

void GameDataEntity::addFile(GameFile *gf)
{
	_files.push_back(gf);
}

// returns total files size (there can be gaps)
UINT32 GameDataEntity::getTotalSize() const
{
	GameFiles::size_type i;

	UINT32 totalSize = 0;
	for (i = 0; i < _files.size(); i++){
		if ((_files[i]->baseAddress + _files[i]->fileSize) > totalSize){
			totalSize = _files[i]->baseAddress + _files[i]->fileSize;
		}
	}

	return totalSize;
}

/////////////////////////////////////////////////////////////////////////////
// internal data manipulation
/////////////////////////////////////////////////////////////////////////////

void GameDataEntity::preProcess()
{
	free();

	// allocates memory for all the files
	_data = new UINT8[getTotalSize()];

	// does some preprocessing if necessary
	_loadHandler->preProcess();
}

void GameDataEntity::loadData(int i, UINT8 *data)
{
	assert(data);
	assert(i < (int)_files.size());

	// handles a loaded game file
	_loadHandler->loadData(i, data);
}

void GameDataEntity::postProcess()
{
	// does some postprocessing if necessary
	_loadHandler->postProcess();
}

void GameDataEntity::free()
{
	if (_data){
		delete[] _data;
		_data = 0;
	}
}

/////////////////////////////////////////////////////////////////////////////
// helper methods
/////////////////////////////////////////////////////////////////////////////

// sets file processing mode
LoadHandler *GameDataEntity::setLoadHandler(LoadingMode mode)
{
	switch (mode){
		case NORMAL:
			return new NormalLoadHandler(this);

		default:
			assert(false);
			return new NormalLoadHandler(this);
	}
}

// GameDriver.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "FileLoader.h"
#include "GameDataEntity.h"
#include "GameDriver.h"
#include "GfxData.h"
#include "InputPort.h"
#include <iomanip>
#include <sstream>

/////////////////////////////////////////////////////////////////////////////
// initialization and cleanup
/////////////////////////////////////////////////////////////////////////////

GameDriver::GameDriver(std::string driverName, std::string fullName, int intsPerSecond)
{
	_driverName = driverName;
	_fullName = fullName;

	_numInterruptsPerSecond = intsPerSecond;
	_numInterruptsPerVideoUpdate = 1;
	_numInterruptsPerLogicUpdate = 1;

	_palette = 0;

	_errorMsg = "";
	theFileLoader = new FileLoader();
}

GameDriver::~GameDriver()
{
	// delete graphics elements
	GfxElements::size_type i;

	for (i = 0; i < _gfx.size(); i++){
		delete _gfx[i];
	}

	// delete graphics encodings
	GfxEncodings::size_type j;

	for (j = 0; j < _gfxEncoding.size(); j++){
		delete _gfxEncoding[j];
	}

	// delete game's data files
	GameDataEntities::size_type k;

	for (k = 0; k < _gameFiles.size(); k++){
		_gameFiles[k]->free();
		delete _gameFiles[k];
	}

	// delete input ports
	InputPorts::size_type l;

	for (l = 0; l < _inputs.size(); l++){
		delete _inputs[l];
	}
}

/////////////////////////////////////////////////////////////////////////////
// game driver initialization and cleanup
/////////////////////////////////////////////////////////////////////////////

bool GameDriver::init(SDLPalette *pal)
{
	_palette = pal;

	// recalculate the refresh rate
	_videoInfo.refreshRate = _numInterruptsPerSecond/_numInterruptsPerVideoUpdate;

	// try to load all the files
	if (!loadFiles()){
		return false;
	}

	// call template method to do any necessary processing
	filesLoaded();

	// try to decode the graphics
	if (!decodeGraphics()){
		_errorMsg = "unable to decode GFX";
		deallocateFilesMemory();

		return false;
	}

	// call template method to do any necessary processing
	graphicsDecoded();

	// deallocate memory used by the files
	deallocateFilesMemory();

	// init input port values
	for (InputPorts::size_type i = 0; i < _inputs.size(); i++){
		_inputs[i]->reset();
	}

	// call template method to inform that the initialization process has finished
	finishInit();

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// helper methods
/////////////////////////////////////////////////////////////////////////////

bool GameDriver::loadFiles()
{
	GameDataEntities::size_type i, j;

	// load the files
	for (i = 0; i < _gameFiles.size(); i++){
		// if a file is missing, deallocates used memory and init fails
		if (!theFileLoader->loadGameData(_driverName, _gameFiles[i])){
			// compose error message
			std::ostringstream buffer;
			buffer << "Unable to load " << _gameFiles[i]->getName() << "\nNeeded files:" << std::hex << std::setfill('0');

			for (int f = 0; f < _gameFiles[i]->getNumFiles(); f++){
				const GameFile *file = _gameFiles[i]->getFile(f);
				buffer << std::endl << file->fileName << " (size = 0x" << file->fileSize 
						<< ") (CRC32 = 0x" << std::setw(8) << file->CRC32 << ")"; 
			}

			_errorMsg = buffer.str();

			// free loaded files
			for (j = 0; j <= i; j++){
				_gameFiles[j]->free();
			}

			return false;
		}
	}

	// if all files were loaded, return ok status
	return true;
}

bool GameDriver::decodeGraphics()
{
	GfxEncodings::size_type i = 0;
	GameDataEntities::size_type j = 0;

	while (i < _gfxEncoding.size()){
		// if it's a graphic entity, decode it
		if (_gameFiles[j]->getType() == GRAPHICS){

			// decode this data with the current encoding
			_gfx.push_back(GfxDecoder::decode(_gfxEncoding[i], _gameFiles[j]->getData()));

			i++, j++;
		} else {
			// otherwise skip it
			j++;

			// if we've processed all files and not all encodings, error!
			if (j == _gameFiles.size()){
				return false;
			}
		}
	}

	return true;
}

void GameDriver::deallocateFilesMemory()
{
	for (GameDataEntities::size_type i = 0; i < _gameFiles.size(); i++){
		_gameFiles[i]->free();
	}
}

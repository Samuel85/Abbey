// GameDataEntity.h
//
//	Class with all data needed to load a game data entity. 
//
//	A game data entity is a collection of files (GameFiles) with related data.
//	Each game data entity has an associated load handler, that reorganizes the
//	internal memory as needed.
//
//	To populate the game data entity:
//		* call preProcess in order to allocate internal memory for all the
//		files and to do any specific load handler pre processing.
//		* for all the files in the game data entity, call loadData with the
//		file data. The load handler will copy and process the data as needed.
//		* call postProcess in order to do any specific load handler post 
//		processing.
//
//	To deallocate the internal memory used by the game files, call free.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _GAME_DATA_ENTITY_H_
#define _GAME_DATA_ENTITY_H_


#include <string>
#include "Types.h"
#include <vector>

class LoadHandler;		// defined in LoadHandler.h

// struct that models a game file
struct GameFile {
	std::string fileName;
	UINT32 baseAddress;
	UINT32 fileSize;
	UINT32 CRC32;
	UINT32 extraData;

	GameFile(std::string file, UINT32 address, UINT32 size, UINT32 crc, UINT32 data)
	{
		fileName = file;
		baseAddress = address;
		fileSize = size;
		CRC32 = crc;
		extraData = data;
	}
};

// enumerations
enum LoadingMode {
	NORMAL,
	FILES_EVEN_ODD	// not implemented yet
};

enum DataType {
	GRAPHICS,
	SOUND,
	PALETTE,
	MIXED
};

class GameDataEntity
{
// types
protected:
	typedef std::vector<GameFile *> GameFiles;

// fields
protected:
	DataType _type;					// type of the game data entity
	std::string _name;				// name of the game data entity
	GameFiles _files;				// collection of files
	LoadingMode _mode;				// file loading mode
	UINT8 *_data;					// internal memory
	LoadHandler *_loadHandler;		// associated load handler

// methods
public:
	// initialization and cleanup
	GameDataEntity(DataType type, std::string name, LoadingMode mode = NORMAL);
	~GameDataEntity();

	// getters & setters
	DataType getType() const { return _type; }
	const std::string getName() const { return _name; }
	UINT8 *getData() const { return _data; }
	int getNumFiles() const { return (int)_files.size(); }
	const GameFile *getFile(int i) const { return _files[i]; };

	void addFile(GameFile *gf);
	UINT32 getTotalSize() const;

	// data manipulation
	void preProcess();
	void loadData(int i, UINT8 *data);
	void postProcess();
	void free();

// helper methods
protected:
	LoadHandler *setLoadHandler(LoadingMode mode);
};


#endif	// _GAME_DATA_ENTITY_H_

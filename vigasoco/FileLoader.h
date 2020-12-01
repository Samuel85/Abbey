// FileLoader.h
//
//	Singleton class that loads game data entities. The class has a collection of
//	loaders that can try to load the collection of files in a specific format
//	(uncompressed, .zip, .rar, etc). The FileLoader also has multiple path support.
//
//	By default, the only loader is the UncompressedLoader and the path is "roms".
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _FILE_LOADER_H_
#define _FILE_LOADER_H_

#include <list>
#include <string>
#include "Singleton.h"
#include "Types.h"

class GameDataEntity;	// defined in GameDataEntity.h
class ILoader;			// defined in ILoader.h

//#define theFileLoader FileLoader::getSingletonPtr()

//class FileLoader : public Singleton<FileLoader>
class FileLoader
{
// types
protected:
	typedef std::list<ILoader *> Loaders;
	typedef std::list<std::string> Paths;

// fields
protected:
	Loaders _loaders;				// collection of loaders
	Paths _paths;					// collection of paths to search the files

// methods
public:
	FileLoader();
	~FileLoader();

	void addLoader(ILoader *l);
	void removeLoader(ILoader *l);
	void addPath(std::string path);

	bool loadGameData(std::string game, GameDataEntity *gde);
};

#endif	// _FILE_LOADER_H_

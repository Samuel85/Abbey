// FileLoader.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include "GameDataEntity.h"
#include "FileLoader.h"
#include "ILoader.h"
#include "UncompressedLoader.h"

/////////////////////////////////////////////////////////////////////////////
// initialization and cleanup
/////////////////////////////////////////////////////////////////////////////

FileLoader::FileLoader()
{
	// adds the default loader and path
	addLoader(new UncompressedLoader());
	addPath("roms");
}

FileLoader::~FileLoader()
{
	// deallocate loaders
	for (Loaders::iterator i = _loaders.begin(); i != _loaders.end(); i++){
		delete *i;
	}
}

/////////////////////////////////////////////////////////////////////////////
// loaders management
/////////////////////////////////////////////////////////////////////////////

void FileLoader::addLoader(ILoader *l)
{
	_loaders.push_back(l);
}

void FileLoader::removeLoader(ILoader *l)
{
	_loaders.remove(l);
}

void FileLoader::addPath(std::string path)
{
	_paths.push_back(path);
}


bool FileLoader::loadGameData(std::string game, GameDataEntity *gde)
{
	// allocate memory to keep track of allocated files
	bool *fileLoaded = new bool[gde->getNumFiles()];

	for (int num = 0; num < gde->getNumFiles(); num++){
		fileLoaded[num] = false;
	}

	Loaders::iterator i;

	gde->preProcess();

	// try to load all available files from each loader before trying the next one
	for (i = _loaders.begin(); i != _loaders.end(); i++){
		ILoader *loader = *i;
		Paths::iterator pathIter;

		for (pathIter = _paths.begin(); pathIter != _paths.end(); pathIter++){

			// if all the files have been loaded, exit
			bool loadComplete = true;
			for (int num = 0; num < gde->getNumFiles(); num++){
				if (!fileLoaded[num]){
					loadComplete = false;
					break;
				}
			}

			if (loadComplete){
				break;
			}

			// otherwise, try to load the remaining files with this loader
			loader->open(*pathIter, game);

			for (int num = 0; num < gde->getNumFiles(); num++){
				const GameFile *gf = gde->getFile(num);

				if (!fileLoaded[num]){
					UINT8 *data;

					data = loader->load(gf->fileName, gf->fileSize, gf->CRC32);

					if (data){
						fileLoaded[num] = true;

						// send the data back to the GameDataEntity to be processed
						gde->loadData(num, data);

						// makes the loader to dispose the memory it reserved
						loader->dispose(data);
					}
				}
			}

			loader->close();
		}
	}

	gde->postProcess();

	// check if all the files have been loaded
	bool loadComplete = true;
	for (int num = 0; num < gde->getNumFiles(); num++){
		if (!fileLoaded[num]){
			loadComplete = false;
			break;
		}
	}

	return loadComplete;
}

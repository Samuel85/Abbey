// UncompressedLoader.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include <fstream>
#include "UncompressedLoader.h"
#include "system.h"

#ifdef ANDROID
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>
#endif
/////////////////////////////////////////////////////////////////////////////
// initialization and cleanup
/////////////////////////////////////////////////////////////////////////////

UncompressedLoader::UncompressedLoader()
{
}

UncompressedLoader::~UncompressedLoader()
{
}

/////////////////////////////////////////////////////////////////////////////
// data loading
/////////////////////////////////////////////////////////////////////////////

void UncompressedLoader::open(std::string path, std::string name)
{
	_path = path + "/" + name + "/";    
}

UINT8 *UncompressedLoader::load(std::string fileName, UINT32 fileSize, UINT32 CRC32)
{
    UINT8 *ptr = new UINT8[fileSize];
    std::string fname = _path + fileName;
    SDL_RWops *rw = SDL_RWFromFile(fname.c_str(),"r");
    if (rw == NULL) {
        sys->print("Unable to load file\n");
    }
    else
    {               
        SDL_RWread(rw, ptr, fileSize, 1);
        SDL_RWclose(rw);
    }

	return ptr;
}

void UncompressedLoader::dispose(UINT8 *ptr)
{
	delete[] ptr;
}

void UncompressedLoader::close()
{
	_path = "";
}

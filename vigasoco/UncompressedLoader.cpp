// UncompressedLoader.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include <fstream>
#include "UncompressedLoader.h"

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
	std::ifstream in((_path + fileName).c_str(), std::ios::binary);

	// try to open the file and exits if there was an error
	if (in.fail()){
		return 0;
	}

	// allocates memory and tries to read the file
	UINT8 *ptr = new UINT8[fileSize];
	in.read((char *)ptr, fileSize);
	if (in.fail()){
		delete[] ptr;
		return 0;
	}

	// file succesfully read. Close it and return the data
	in.close();

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

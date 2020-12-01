// UncompressedLoader.h
//
//	Class to load uncompressed files
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _UNCOMPRESSED_LOADER_H_
#define _UNCOMPRESSED_LOADER_H_


#include "ILoader.h"

class UncompressedLoader : public ILoader
{
// fields
protected:
	std::string _path;

// methods
public:
	UncompressedLoader();
	virtual ~UncompressedLoader();

	// loader interface
	virtual void open(std::string path, std::string name);
	virtual UINT8 *load(std::string fileName, UINT32 fileSize, UINT32 CRC32);
	virtual void dispose(UINT8 *ptr);
	virtual void close();
};

#endif	// _UNCOMPRESSED_LOADER_H_

// ILoader.h
//
//	Abstract class that defines the interface for all loaders
//
//	A loader is used like this:
//		* open is called in order to do any necessary intialization when opening
//		a file or folder (for example, reading a zip header).
//		* load is called once for each file to load. Load should return 0 if
//		it can't load the file, or a pointer to the allocated memory (the 
//		memory should be deallocated by the caller).
//		* if the file was loaded, dispose will be called.
//		* close is called in order to do any necessary cleanup.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _ILOADER_H_
#define _ILOADER_H_


#include <string>
#include "Types.h"

class ILoader
{
// methods
public:
	ILoader(){}
	virtual ~ILoader(){}

	virtual void open(std::string path, std::string name) = 0;
	virtual UINT8 *load(std::string fileName, UINT32 fileSize, UINT32 CRC32) = 0;
	virtual void dispose(UINT8 *ptr) = 0;
	virtual void close() = 0;
};

#endif	// _ILOADER_H_

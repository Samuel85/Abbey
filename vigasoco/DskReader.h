// DskReader.h
//
//	Class to extract data from extended disk images
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _DSK_READER_H_
#define _DSK_READER_H_


#include "Types.h"

class DskReader
{
// fields
protected:
	UINT8 *_data;		// pointer to the disk data
	int _numTracks;		// number of tracks of the disk
	int _numSides;		// number of sides of the disk

	bool _isOK;			// true if the disk image is valid

// methods
public:
	void getTrackData(int numTrack, UINT8 *buffer, int bufferSize, int &bytesWritten);
	
	// initialization and cleanup
	DskReader(UINT8 *dsk);
	~DskReader();

// helper methods
protected:
	void checkHeader();
	int getTrackOffset(int numTrack);
};

#endif	// _DSK_READER_H_

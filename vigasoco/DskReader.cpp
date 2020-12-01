// DskReader.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <cstring>

#include "DskReader.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////
// initialization and cleanup
/////////////////////////////////////////////////////////////////////////////

DskReader::DskReader(UINT8 *dsk)
{
	_data = dsk;

	// checks if the data is valid
	checkHeader();
}

DskReader::~DskReader()
{
}

/////////////////////////////////////////////////////////////////////////////
// get track data
/////////////////////////////////////////////////////////////////////////////

void DskReader::getTrackData(int numTrack, UINT8 *buffer, int bufferSize, int &bytesWritten)
{
	assert(_isOK);
	assert((numTrack >= 0) && (numTrack < _numTracks));

	// gets a pointer to the track's starting address
	UINT8 *start = &_data[getTrackOffset(numTrack)];

	// get track information
	int sectorSize = start[0x14]*256;
	int numSectors = start[0x15];

	// check the length of the data to be copied
	bytesWritten = numSectors*sectorSize;

	if (numSectors*sectorSize > bufferSize){
		bytesWritten = bufferSize;
	}

	// copy all sectors for this track
	memcpy(buffer, &start[0x100], bytesWritten);
}

/////////////////////////////////////////////////////////////////////////////
// helper methods
/////////////////////////////////////////////////////////////////////////////

// check if the header is for an extended cpc dsk file and extract image information
void DskReader::checkHeader()
{
	assert(_data != 0);

	UINT8 aux = _data[0x15];
	_data[0x15] = 0;

	// check if it's a valid extended dsk file
	if (strcmp((const char *)&_data[0], "EXTENDED CPC DSK File") != 0){
		_isOK = false;

		return;
	}

	_data[0x15] = aux;

	// get disk information
	_numTracks = _data[0x30];
	_numSides = _data[0x31];

	_isOK = true;
}

// gets the starting offset for a track
int DskReader::getTrackOffset(int numTrack)
{
	assert(_isOK);
	assert((numTrack >= 0) && (numTrack < _numTracks));

	int offset = 0x00000100;

	for (int i = 0; i < numTrack; i++){
		offset += _data[0x34 + i]*256;
	}

	return offset;
}

// GfxData.h
//
//	Structures and classes to manipulate data and decode graphics
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _GFX_DATA_H_
#define _GFX_DATA_H_

#include <string>
#include "Types.h"

// struct used to decode graphics
struct GfxEncoding {
	int sizeX, sizeY;
	int numEntries;
	int bpp;
	UINT32 planeOffsets[8];
	int xOffs[32];
	int yOffs[32];
	int entryBitSize;

	static GfxEncoding *clone(GfxEncoding *g);
};

// sprite attributes
enum SpriteAttr {
	NO_FLIP = 0,
	FLIP_X,
	FLIP_Y,
	FLIP_XY,	// = FLIP_X | FLIP_Y
	TRANSPARENCY_PEN = 0x80,
	TRANSPARENCY_COLOR = 0x100
};

// struct used to draw decoded graphics
struct GfxElement {
	int sizeX, sizeY;
	int numEntries;
	UINT32 entrySize;
	int bpp;
	UINT8 **data;

	GfxElement(){ data = 0; }
	~GfxElement(){ 
		if (data){
			for (int i = 0; i < 4; i++){ 
				delete[] data[i]; 
			} 
			delete[] data; 
		}
	}
};

// class to decode graphics
class GfxDecoder
{
public:
	// decodes graphics
	static GfxElement *decode(const struct GfxEncoding *enc, const UINT8 *pRawGfx);

protected:
	// check if a bit is set. Bit order is 01234567 instead of the usual 76543210
	static inline bool isBitSet(const UINT8 *ptr, int bit)
	{
		return (ptr[bit >> 3] & (0x80 >> (bit & 0x07))) != 0;
	}

private:
	GfxDecoder();
	~GfxDecoder();
};

#endif	// _GFX_DATA_H_

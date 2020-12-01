// GfxData.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include "GfxData.h"

// para memset y memcpy
#include <string.h>

GfxElement * GfxDecoder::decode(const struct GfxEncoding *enc, const UINT8 *pRawGfx)
{
	assert(enc->bpp <= 8);

	int entrySize = enc->sizeX*enc->sizeY;

	// allocate memory for the decoded graphics (1 pixel -> 1 byte)
	UINT8 **decodedGfx = new UINT8*[4];
	for (int i = 0; i < 4; i++){
		decodedGfx[i] = new UINT8[enc->numEntries*entrySize];
		memset(decodedGfx[i], 0, enc->numEntries*entrySize);
	}

	// for each tile
	for (int num = 0; num < enc->numEntries; num++){
		// for each bitplane
		for (int plane = 0; plane < enc->bpp; plane++){

			// get bit to test (order 01234567 instead of the usual 76543210)
			int currBit = 1 << (enc->bpp - 1 - plane);

			// get offset to the starting of the current bitplane (in bits)
			int offs = enc->planeOffsets[plane] + num*enc->entryBitSize;

			for (int y = 0; y < enc->sizeY; y++){
				// get start of this line (in bits)
				int lineOffset = offs + enc->yOffs[y];

				for (int x = 0; x < enc->sizeX; x++){
					// check bit in the final position
					if (isBitSet(pRawGfx, lineOffset + enc->xOffs[x])){
						decodedGfx[NO_FLIP][entrySize*num + (y*enc->sizeX + x)] |= currBit;
						decodedGfx[FLIP_X] [entrySize*num + (y*enc->sizeX + (enc->sizeX - x - 1))] |= currBit;
						decodedGfx[FLIP_Y] [entrySize*num + ((enc->sizeY - y - 1)*enc->sizeX + x)] |= currBit;
						decodedGfx[FLIP_XY][entrySize*num + ((enc->sizeY - y - 1)*enc->sizeX + (enc->sizeX - x - 1))] |= currBit;
					}
				}
			}
		}
	}

	// return GfxElement struc
	GfxElement *g = new GfxElement();
	g->sizeX = enc->sizeX;
	g->sizeY = enc->sizeY;
	g->numEntries = enc->numEntries;
	g->entrySize = entrySize;
	g->bpp = enc->bpp;
	g->data = decodedGfx;

	return g;
}

/////////////////////////////////////////////////////////////////////////////
// cloning
/////////////////////////////////////////////////////////////////////////////

GfxEncoding *GfxEncoding::clone(GfxEncoding *g)
{
	GfxEncoding *gfx = new GfxEncoding();
	gfx->sizeX = g->sizeX;
	gfx->sizeY = g->sizeY;
	gfx->numEntries = g->numEntries;
	gfx->bpp = g->bpp;
	memcpy(gfx->planeOffsets, g->planeOffsets, sizeof(UINT32)*8);
	memcpy(gfx->xOffs, g->xOffs, sizeof(int)*32);
	memcpy(gfx->yOffs, g->yOffs, sizeof(int)*32);
	gfx->entryBitSize = g->entryBitSize;

	return gfx;
}


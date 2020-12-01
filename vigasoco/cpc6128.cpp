// cpc6128.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include <cassert>

#include "cpc6128.h"

// para memset
#include <string.h>

/////////////////////////////////////////////////////////////////////////////
// initialization and cleanup
/////////////////////////////////////////////////////////////////////////////

//CPC6128::CPC6128(ICriticalSection *criticalSection)
CPC6128::CPC6128()
{
	//cs = criticalSection;

	// cleans the screen buffer
	// CPC memset(screenBuffer, 0x80, 640*200);
	// TODO: VGA
	memset(screenBuffer,0,sizeof(screenBuffer));
	memset(DirtyPixels,0,sizeof(DirtyPixels));
}

CPC6128::~CPC6128()
{
}

/////////////////////////////////////////////////////////////////////////////
// CPC 6128 palette
/////////////////////////////////////////////////////////////////////////////

// hardware colors
UINT8 CPC6128::hardwarePalette[32][3] = {
	{ 0x080, 0x080, 0x080 },	// 00 white
	{ 0x080, 0x080, 0x080 },	// 01 white
	{ 0x000, 0x0ff, 0x080 },	// 02 sea green
	{ 0x0ff, 0x0ff, 0x080 },	// 03 pastel yellow
	{ 0x000, 0x000, 0x080 },	// 04 blue
	{ 0x0ff, 0x000, 0x080 },	// 05 purple
	{ 0x000, 0x080, 0x080 },	// 06 cyan
	{ 0x0ff, 0x080, 0x080 },	// 07 pink
	{ 0x0ff, 0x000, 0x080 },	// 08 purple
	{ 0x0ff, 0x0ff, 0x080 },	// 09 pastel yellow
	{ 0x0ff, 0x0ff, 0x000 },	// 10 bright yellow
	{ 0x0ff, 0x0ff, 0x0ff },	// 11 bright white
	{ 0x0ff, 0x000, 0x000 },	// 12 bright red
	{ 0x0ff, 0x000, 0x0ff },	// 13 bright magenta
	{ 0x0ff, 0x080, 0x000 },	// 14 orange
	{ 0x0ff, 0x080, 0x0ff },	// 15 pastel magenta
	{ 0x000, 0x000, 0x080 },	// 16 blue
	{ 0x000, 0x0ff, 0x080 },	// 17 sea green
	{ 0x000, 0x0ff, 0x000 },	// 18 bright green
	{ 0x000, 0x0ff, 0x0ff },	// 19 bright cyan
	{ 0x000, 0x000, 0x000 },	// 20 black
	{ 0x000, 0x000, 0x0ff },	// 21 bright blue
	{ 0x000, 0x080, 0x000 },	// 22 green
	{ 0x000, 0x080, 0x0ff },	// 23 sky blue
	{ 0x080, 0x000, 0x080 },	// 24 magenta
	{ 0x080, 0x0ff, 0x080 },	// 25 pastel green
	{ 0x080, 0x0ff, 0x080 },	// 26 lime
	{ 0x080, 0x0ff, 0x0ff },	// 27 pastel cyan
	{ 0x080, 0x000, 0x000 },	// 28 red
	{ 0x080, 0x000, 0x0ff },	// 29 mauve
	{ 0x080, 0x080, 0x000 },	// 30 yellow
	{ 0x080, 0x080, 0x0ff }		// 31 pastel blue
};

// mapping from inks to hardware colors
int CPC6128::inkColors[27] = {
	20,		// 00 black
	4,		// 01 blue
	21,		// 02 brigth blue
	28,		// 03 red
	24,		// 04 magenta
	29,		// 05 mauve
	12,		// 06 bright red
	5,		// 07 purple
	13,		// 08 bright magenta
	22,		// 09 green
	6,		// 10 cyan
	23,		// 11 sky blue
	30,		// 12 yellow
	0,		// 13 white
	31,		// 14 pastel blue
	14,		// 15 orange
	7,		// 16 pink
	15,		// 17 pastel magenta
	18,		// 18 bright green
	2,		// 19 sea green
	19,		// 20 bright cyan
	26,		// 21 lime
	25,		// 22 pastel green
	27,		// 23 pastel cyan
	10,		// 24 bright yellow
	3,		// 25 pastel yellow
	11		// 26 bright white
};

void CPC6128::setHardwareColor(IPalette *pal, int numInk, int color)
{
	assert((numInk >= 0) && (numInk < 16));
	assert((color >= 0) && (color < 32));
	pal->setColor(numInk, CPC6128::hardwarePalette[color][0], CPC6128::hardwarePalette[color][1], CPC6128::hardwarePalette[color][2]);
}

void CPC6128::setInkColor(IPalette *pal, int numInk, int color)
{
	assert((numInk >= 0) && (numInk < 16));
	assert((color >= 0) && (color < 27));

	pal->setColor(numInk, CPC6128::hardwarePalette[inkColors[color]][0], CPC6128::hardwarePalette[inkColors[color]][1], CPC6128::hardwarePalette[inkColors[color]][2]);
}

/////////////////////////////////////////////////////////////////////////////
// CPC 6128 screen modes helpers
/////////////////////////////////////////////////////////////////////////////

// sets a pixel in mode 0 (160x200, x pixels = 4 width, y pixels = 2 height, 16 colors)
void CPC6128::setMode0Pixel(int x, int y, int color)
{
	assert((x >= 0) && (x < 160));
	assert((y >= 0) && (y < 200));
	assert((color >= 0) && (color < 16));

	setPixel(4*x, y, color);
	setPixel(4*x + 1, y, color);
	setPixel(4*x + 2, y, color);
	setPixel(4*x + 3, y, color);
}

// sets a pixel in mode 1 (320x200, x pixels = 2 width, y pixels = 2 height, 4 colors)
void CPC6128::setMode1Pixel(int x, int y, int color)
{
	assert((x >= 0) && (x < 320));
	assert((y >= 0) && (y < 200));
//CPC	assert((color >= 0) && (color < 4));
	assert((color >= 0) && (color < 256)); //TODO VGA

	setPixel(2*x, y, color);
	setPixel(2*x + 1, y, color);
}

// TODO: SEPARAR ESTO EN OTRO FUENTE QUE NO SEA CPC
// sets a pixel in mode 1 (320x200, x pixels = 2 width, y pixels = 2 height, 256 colors)
void CPC6128::setVGAPixel(int x, int y, int color)
{
	assert((x >= 0) && (x < 320));
	assert((y >= 0) && (y < 200));
	assert((color >= 0) && (color < 256));

	setPixel(2*x, y, color);
	setPixel(2*x + 1, y, color);
}

// sets a pixel in mode 2 (640x200, x pixels = 1 width, y pixels = 2 height, 2 colors)
void CPC6128::setMode2Pixel(int x, int y, int color)
{
	assert((x >= 0) && (x < 640));
	assert((y >= 0) && (y < 200));
	assert((color >= 0) && (color < 2));

	setPixel(x, y, color);
}

// gets a pixel in mode 0 (160x200, x pixels = 4 width, y pixels = 2 height, 16 colors)
int CPC6128::getMode0Pixel(int x, int y)
{
	assert((x >= 0) && (x < 160));
	assert((y >= 0) && (y < 200));

	return getPixel(4*x, y);
}

// gets a pixel in mode 1 (320x200, x pixels = 2 width, y pixels = 2 height, 4 colors)
int CPC6128::getMode1Pixel(int x, int y)
{
	assert((x >= 0) && (x < 320));
	assert((y >= 0) && (y < 200));

	return getPixel(2*x, y);
}

// gets a pixel in mode 2 (640x200, x pixels = 1 width, y pixels = 2 height, 2 colors)
int CPC6128::getMode2Pixel(int x, int y)
{
	assert((x >= 0) && (x < 640));
	assert((y >= 0) && (y < 200));

	return getPixel(x, y);
}

// fills a rectange in mode 0 (160x200, x pixels = 4 width, y pixels = 2 height, 16 colors)
void CPC6128::fillMode0Rect(int x, int y, int width, int height, int color)
{
	assert((x >= 0) && (x < 160));
	assert((y >= 0) && (y < 200));
	assert((color >= 0) && (color < 16));
	assert(((x + width) <= 160) && ((y + height) <= 200));

	fillRect(x*4, y, width*4, height, color);
}

// sets a pixel in mode 1 (320x200, x pixels = 2 width, y pixels = 2 height, 4 colors)
void CPC6128::fillMode1Rect(int x, int y, int width, int height, int color)
{
	assert((x >= 0) && (x < 320));
	assert((y >= 0) && (y < 200));
	// CPC assert((color >= 0) && (color < 4));
	assert((color >= 0) && (color < 256)); // VGA ya que se llama en Marcador::dibujaBarra y ya se usan colores VGA
	assert(((x + width) <= 320) && ((y + height) <= 200));

	fillRect(x*2, y, width*2, height, color);
}

// sets a pixel in mode 2 (640x200, x pixels = 1 width, y pixels = 2 height, 2 colors)
void CPC6128::fillMode2Rect(int x, int y, int width, int height, int color)
{
	assert((x >= 0) && (x < 640));
	assert((y >= 0) && (y < 200));
	assert((color >= 0) && (color < 2));
	assert(((x + width) <= 640) && ((y + height) <= 200));

	fillRect(x, y, width, height, color);
}


// shows a screen stored in standard mode 0 format
void CPC6128::showMode0Screen(const UINT8 *data)
{
	// scan the rows
	for (int j = 0; j < 200; j++){
		// convert from CPC VRAM to standard coordinates
		const UINT8 *lineData = &data[(j & 0x07)*0x800 + (j >> 3)*80];

		// scan the cols
		for (int i = 0; i < 160/2; i++){
			int data = *lineData;

			// draw pixels
			setMode0Pixel(2*i, j, unpackPixelMode0(data, 0));
			setMode0Pixel(2*i + 1, j, unpackPixelMode0(data, 1));

			lineData++;
		}
	}
}

// shows a screen stored in 320x200 1 pixel por byte
void CPC6128::showVGAScreen(const UINT8 *data)
{
	// scan the rows
	for (int j = 0; j < 200; j++){
		// scan the cols
		for (int i = 0; i < 320; i++){
			// draw pixels
			setVGAPixel(i,j,*data++);
		}
	}
// TODO: como en la pantalla de presentacion se usan mas de 127
// colores, no se puede usar el ultimo bit para marcar
// un pixel como cambiado
}



// marks all pixels as dirty
void CPC6128::markAllPixelsDirty()
{
/* CPC
	UINT8 *buf = screenBuffer;

	for (int j = 0; j < 200; j++){
		for (int i = 0; i < 640; i++){
			*buf = *buf | 0x80;
			buf++;
		}
	}
	*/

	//TODO VGA
	UINT8 *buf = DirtyPixels;
	memset(DirtyPixels,0xFF,sizeof(DirtyPixels));

}

/////////////////////////////////////////////////////////////////////////////
// helper methods
/////////////////////////////////////////////////////////////////////////////

// fill a rectangle
void CPC6128::fillRect(int x, int y, int width, int height, int color)
{
	int xLimit = width + x - 1;

	for (; height > 0; height--, y++){
		if (xLimit < x) {
			std::swap<int>(x, xLimit);
		}

		for (int xx = x; xx <= xLimit; xx++){
			setPixel(xx, y, color);
		}
	}
}

// cpc6128.h
//
//	This class has helper methods for working with CPC6128 games
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _CPC_6128_H_
#define _CPC_6128_H_

#include <cassert>
//#include "ICriticalSection.h"
#include "IPalette.h"
#include "Types.h"
#include "system.h"
class CPC6128
{
// tables
protected:
	// hardware palette
	static UINT8 hardwarePalette[32][3];

	// ink to hardware color conversion
	static int inkColors[27];

// enumerations
public:
	// friendly color names
	enum Inks {
		BLACK,
		BLUE,
		BRIGHT_BLUE,
		RED,
		MAGENTA,
		MAUVE,
		BRIGHT_RED,
		PURPLE,
		BRIGHT_MAGENTA,
		GREEN,
		CYAN,
		SKY_BLUE,
		YELLOW,
		WHITE,
		PASTEL_BLUE,
		ORANGE,
		PINK,
		PASTEL_MAGENTA,
		BRIGHT_GREEN,
		SEA_GREEN,
		BRIGHT_CYAN,
		LIME,
		PASTEL_GREEN,
		PASTEL_CYAN,
		BRIGHT_YELLOW,
		PASTEL_YELLOW,
		BRIGHT_WHITE
	};

// fields
public:
	UINT8 screenBuffer[640*200];	// CPC6128 video RAM
	//TODO VGA hay que usar esto ya que no se puede usar un bit como marcador
	//al igual que en CPC
	//UINT8 DirtyPixels[sizeof(screenBuffer)/8]; // pixeles modificados en la CPC6128 video RAM
	UINT8 DirtyPixels[(640*200)/8]; // pixeles modificados en la CPC6128 video RAM

//protected:
//	ICriticalSection *cs;			// critical section to sync drawing between threads

// methods
public:
	// initialization and cleanup
	//CPC6128(ICriticalSection *criticalSection);
	CPC6128();
	~CPC6128();

	// palette related
	void setHardwareColor(IPalette *pal, int color, int value);
	void setInkColor(IPalette *pal, int color, int value);

	// pixel drawing/retrieving
	void setMode0Pixel(int x, int y, int color);
	void setMode1Pixel(int x, int y, int color);
	void setVGAPixel(int x, int y, int color); // TODO: VGA
	void setMode2Pixel(int x, int y, int color);
	int getMode0Pixel(int x, int y);
	int getMode1Pixel(int x, int y);
	int getMode2Pixel(int x, int y);

	void markAllPixelsDirty();

	void showMode0Screen(const UINT8 *data);
	void showVGAScreen(const UINT8 *data);

	// rectangle filling
	void fillMode0Rect(int x, int y, int width, int height, int color);
	void fillMode1Rect(int x, int y, int width, int height, int color);
	void fillMode2Rect(int x, int y, int width, int height, int color);

	// pixel unpacking
	inline int unpackPixelMode0(int data, int pixel)
	{
		return (((data >> (1 - pixel)) & 0x01) << 3) | (((data >> (5 - pixel)) & 0x01) << 2) | (((data >> (3 - pixel)) & 0x01) << 1) | (((data >> (7 - pixel)) & 0x01) << 0);
	}

	inline int unpackPixelMode1(int data, int pixel)
	{
		return (((data >> (3 - pixel)) & 0x01) << 1) | ((data >> (7 - pixel)) & 0x01);
	}

	inline int unpackPixelMode2(int data, int pixel)
	{
		return (data >> (7 - pixel)) & 0x01;
	}

	// pixel packing
	inline int packPixelMode0(int oldByte, int pixel, int color)
	{
		assert ((pixel >= 0) && (pixel < 2));
		assert ((color >= 0) && (color < 16));

		// find out the 4 bits of the new pixel
		int mask = 0xaa;
		mask = mask >> pixel;

		// save the other pixels
		oldByte = (oldByte & (~mask)) & 0xff;

		// array with the sixteen colors
		static int byteColors[16] = {
			0x00, 0xc0, 0x44, 0xcc, 0x30, 0xf0, 0x74, 0xfc,
			0x03, 0xc3, 0x47, 0xcf, 0x33, 0xf3, 0x77, 0xff
		};

		// combines the other pixels with the new pixel
		return oldByte | (byteColors[color] & mask);
	}

	// pixel packing
	inline int packPixelMode1(int oldByte, int pixel, int color)
	{
		assert ((pixel >= 0) && (pixel < 4));
		assert ((color >= 0) && (color < 4));

		// find out the 2 bits of the new pixel
		int mask = 0x88;
		mask = mask >> pixel;

		// save the other pixels
		oldByte = (oldByte & (~mask)) & 0xff;

		// array with the four colors
		static int byteColors[4] = { 0x00, 0xf0, 0x0f, 0xff };

		// combines the other pixels with the new pixel
		return oldByte | (byteColors[color] & mask);
	}

	// pixel packing
	inline int packPixelMode2(int oldByte, int pixel, int color)
	{
		assert ((pixel >= 0) && (pixel < 8));
		assert ((color >= 0) && (color < 2));

		// find out the bit of the new pixel
		int mask = 0x80;
		mask = mask >> pixel;

		// save the other pixels
		oldByte = (oldByte & (~mask)) & 0xff;

		// array with the two colors
		static int byteColors[2] = { 0x00, 0xff };

		// combines the other pixels with the new pixel
		return oldByte | (byteColors[color] & mask);
	}

	// pixel get/set
	inline void setPixel(int x, int y, int color)
	{
		/* CPC
		// sets pixel and marks the pixel as dirty
		cs->enter();
		screenBuffer[y*640 + x] = color | 0x80;
		cs->leave();
		*/

		// VGA
		//cs->enter();
		// set pixel
		screenBuffer[y*640 + x] = color;
		// mark the pixel as dirty
		UINT8 *p=&(DirtyPixels[(y*640 + x)/8]);
		UINT8 bit = 1<<((y*640 + x)%8);
		*p = *p|bit;
		//cs->leave();					
		
	}

	inline int getPixel(int x, int y)
	{
		// CPC return screenBuffer[y*640 + x] & 0x0f;
		// VGA
		return screenBuffer[y*640 + x];
	}

// helper methods
protected:
	void fillRect(int x, int y, int width, int height, int color);
};

#endif	// _CPC_6128_H_

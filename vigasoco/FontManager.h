// FontManager.h
//
//	Class with functionality for writing text to bitmaps
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _FONT_MANAGER_H_
#define _FONT_MANAGER_H_


#include <string>
#include "Singleton.h"
#include "Types.h"

// font types (each font type encodes the font number in the font array and the size)
enum FontSize {
	FONT_6x10 = ((0 << 16) | (10 << 8) | 6),
	FONT_9x18 = ((1 << 16) | (18 << 8) | 9),
};

class IDrawPlugin;		// defined in IDrawPlugin.h
class IPalette;			// defined in IPalette.h
struct GfxElement;		// defined in GfxData.h


#define theFontManager FontManager::getSingletonPtr()

class FontManager : public Singleton<FontManager>
{
// types
protected:
	struct FontManagerState {
		int currentFont;
		int fontWidth, fontHeight;
		int xPadding, yPadding;
		UINT8 inkColorR, inkColorG, inkColorB;
		UINT8 bgColorR, bgColorG, bgColorB;
	};

// fields
protected:
	int _fontColorEntry;
	IPalette *_palette;

	int _fontWidth;
	int _fontHeight;
	int _currentFont;

	int _xPadding;
	int _yPadding;

	FontManagerState _stack[8];		// stack for storing/retreiving state
	int _stackPos;					// actual stack position

	// font data
	static UINT8 font_6x10[];
	static UINT8 font_9x18[];

	// available fonts
	static UINT8 *fonts[2];
	static int fontDesc[2];
	GfxElement *_fontGfx[2];

// methods
public:
	// setters
	void setFont(int size);
	void setXPadding(int xPad) { _xPadding = xPad; }
	void setYPadding(int yPad) { _yPadding = yPad; }
	void setInkColor(UINT8 r, UINT8 g, UINT8 b);
	void setBackGroundColor(UINT8 r, UINT8 g, UINT8 b);

	// getters
	int getXPadding() { return _xPadding; }
	int getYPadding() { return _yPadding; }

	// load/save state
	void pushSettings();
	void popSettings();

	// string printing
	void print(IDrawPlugin *dp, std::string str, int x, int y);
	void printTrans(IDrawPlugin *dp, std::string str, int x, int y);

	// initialization and cleanup
	FontManager(IPalette *palette, int entry);
	virtual ~FontManager();

	void init();

	// helper methods
	void measureString(std::string str, int &width, int &height);
};

#endif	// _FONT_MANAGER_H_

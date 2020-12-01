// SDLPalette.cpp
//
//	Es una copia de Win32Palette de VigasocoWin32
//	, quizas deberia usar las definiciones de paletas y colores de SDL
/////////////////////////////////////////////////////////////////////////////

#include "SDLPalette.h"


/////////////////////////////////////////////////////////////////////////////
// initialization and cleanup
/////////////////////////////////////////////////////////////////////////////

SDLPalette::SDLPalette()
{
	_palette = 0;
	_colors = 0;
}

SDLPalette::~SDLPalette()
{
}

void SDLPalette::init(int colors)
{
	assert(_palette == 0);
	_palette = new PaletteEntry[colors];
	_colors = colors;
}

void SDLPalette::end()
{
	delete[] _palette;
}

/////////////////////////////////////////////////////////////////////////////
// getters & setters
/////////////////////////////////////////////////////////////////////////////

void SDLPalette::setColor(int index, UINT8 r, UINT8 g, UINT8 b)
{
	assert((index >= 0) && (index < _colors));

	_palette[index].R = r;
	_palette[index].G = g;
	_palette[index].B = b;
	_palette[index].alpha = 0xff;

	notify(index);
}

void SDLPalette::getColor(int index, UINT8 &r, UINT8 &g, UINT8 &b)
{
	assert((index >= 0) && (index < _colors));

	r = _palette[index].R;
	g = _palette[index].G;
	b = _palette[index].B;
}

void SDLPalette::setColor(int index, PaletteEntry pe)
{
	assert((index >= 0) && (index < _colors));

	_palette[index].R = pe.R;
	_palette[index].G = pe.G;
	_palette[index].B = pe.B;
	_palette[index].alpha = pe.alpha;

	notify(index);
}

PaletteEntry SDLPalette::getColor(int index) const
{
	assert((index >= 0) && (index < _colors));

	return _palette[index];
}

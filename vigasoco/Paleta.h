// Paleta.h
//
//	Clase con todos los m�todos relacionados con la paleta
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _PALETA_H_
#define _PALETA_H_

#include "SDLPalette.h"

class Paleta {
public:
	Paleta(UINT8 *gfxVGA);
	~Paleta();

	SDLPalette *paleta2;		// paleta real del juego
	void setIntroPalette();
	void setGamePalette(int pal);

	static int introPalette[16];
	static int palettes[4][4];
	UINT8 *PaletasVGA; // Paleta + graficos VGA del remake de PC con 256 colores 
	static int despPaletasVGA[6]; // a cuantos bytes desde el comienzo de PaletasVGA esta la paleta
};

#endif	// _PALETA_H_

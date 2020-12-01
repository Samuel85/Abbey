// SpriteLuz.h
//
//	Clase que representa un sprite de luz
//
//	El sprite de la luz tiene un tratamiento especial, pues su posición depende
//	del personaje al que se le asocia la luz, y el dibujado consiste en pintar
//	de negro parte de la zona que ocupa el sprite
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _SPRITE_LUZ_H_
#define _SPRITE_LUZ_H_


#include "Sprite.h"

namespace Abadia {

class Personaje;					// definido en Personaje.h

class SpriteLuz : public Sprite
{
// campos
public:
	bool flipX;						// indica si el personaje asociado a la luz está girado en x
	int rellenoAbajo;				// número de pixels de relleno para completar el sprite por abajo
	int rellenoArriba;				// número de pixels de relleno para completar el sprite por arriba
	int rellenoDerecha;				// número de pixels de relleno para completar el sprite por la derecha
	int rellenoIzquierda;			// número de pixels de relleno para completar el sprite por la izquierda

protected:
	static int rellenoLuz[16];		// tabla con el patrón de relleno de la luz

// métodos
public:
	void ajustaAPersonaje(Personaje *pers);

	virtual void dibuja(Sprite *spr, UINT8 *bufferMezclas, int lgtudClipX, int lgtudClipY, int dist1X, int dist2X, int dist1Y, int dist2Y);

	// inicialización y limpieza
	SpriteLuz();
	virtual ~SpriteLuz();
};


}

#endif	// _SPRITE_LUZ_H_

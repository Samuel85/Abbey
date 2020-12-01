// Sprite.h
//
//	Clase que representa un sprite gen�rico
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _SPRITE_H_
#define _SPRITE_H_


#include "Types.h"

namespace Abadia {


class Sprite
{
// campos
public:
	bool esVisible;			// indica si el sprite es visible
	bool haCambiado;		// indica si el sprite ha cambiado desde que se dibuj� por �ltima vez
	bool seHaProcesado;		// indica si el sprite ha sido procesado por el mezclador
	int profundidad;		// profundidad del sprite (coordenada y de pantalla)

	int posXPant;			// coordenada x del sprite en la pantalla (en m�ltiplos de 4 pixels)
	int posYPant;			// coordenada y del sprite en la pantalla (en pixels)
	int oldPosXPant;		// anterior coordenada x del sprite en la pantalla (en m�ltiplos de 4 pixels)
	int oldPosYPant;		// anterior coordenada y del sprite en la pantalla (en pixels)

	bool desaparece;		// indica si el sprite va a desaparecer de la pantalla
	int ancho;				// ancho del sprite (en m�ltiplos de 4 pixels)
	int alto;				// alto del sprite (en pixels)
	int despGfx;			// desplazamiento a los gr�ficos que forman el sprite
	int oldAncho;			// anterior ancho del sprite (en bytes)
	int oldAlto;			// anterior alto del sprite (en pixels)

	int posXTile;			// coordenada x del tile en el que se empieza a dibujar el sprite (en m�ltiplos de 4 pixels)
	int posYTile;			// coordenada y del tile en el que se empieza a dibujar el sprite (en pixels)
	int anchoFinal;			// ancho final del �rea a dibujar del sprite (en bytes)
	int altoFinal;			// alto final del �rea a dibujar del sprite (en pixels)
	int despBuffer;			// desplazamiento en el buffer para mezclar los sprites
	int posXLocal;			// coordenada x en la coordenadas locales de la c�mara
	int posYLocal;			// coordenada y en la coordenadas locales de la c�mara

// m�todos
public:
	// ajuste de las dimensiones para el dibujado
	void ajustaATiles();
	void ampliaDimViejo();

	void preparaParaCambio();

	// dibujo del sprite
	virtual void dibuja(Sprite *spr, UINT8 *bufferMezclas, int lgtudClipX, int lgtudClipY, int dist1X, int dist2X, int dist1Y, int dist2Y);

	// inicializaci�n y limpieza
	Sprite();
	virtual ~Sprite();
private:
	void dibujaVGA(Sprite *spr, UINT8 *bufferMezclas, int lgtudClipX, int lgtudClipY, int dist1X, int dist2X, int dist1Y, int dist2Y);


};


}

#endif	// _SPRITE_H_

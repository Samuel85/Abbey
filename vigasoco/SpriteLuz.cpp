// SpriteLuz.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "Juego.h"
#include "Personaje.h"
#include "SpriteLuz.h"
#include "cpc6128.h"

using namespace Abadia;

/////////////////////////////////////////////////////////////////////////////
// tabla con el patr�n de relleno de la luz
/////////////////////////////////////////////////////////////////////////////

int SpriteLuz::rellenoLuz[16] = {
	0x00e0,
	0x03f8,
	0x07fc,
	0x07fc,
	0x0ffe,
	0x0ffe,
	0x1fff,
	0x1fff,
	0x1fff,
	0x1fff,
	0x0ffe,
	0x0ffe,
	0x07fc,
	0x07fc,
	0x03f8,
	0x00e0
};

/////////////////////////////////////////////////////////////////////////////
// inicializaci�n y limpieza
/////////////////////////////////////////////////////////////////////////////

SpriteLuz::SpriteLuz()
{
	ancho = oldAncho = 80/4;
	alto = oldAlto = 80;

	posXLocal = posYLocal = 0xfe;

	flipX = false;
	rellenoAbajo = 0;
	rellenoArriba = 0;
	rellenoDerecha = 0;
	rellenoIzquierda = 0;
}

SpriteLuz::~SpriteLuz()
{
}

/////////////////////////////////////////////////////////////////////////////
// colocaci�n de la luz
/////////////////////////////////////////////////////////////////////////////

// ajusta el sprite de la luz a la posici�n del personaje que se le pasa
void SpriteLuz::ajustaAPersonaje(Personaje *pers)
{
	// asigna una profundidad en pantalla muy alta al sprite de la luz
	posXLocal = 0xfe;
	posYLocal = 0xfe;

	// calcula los rellenos del sprite de la luz seg�n la posici�n del personaje
	rellenoIzquierda = (pers->sprite->posXPant & 0x03)*4;
	rellenoDerecha = (4 - (pers->sprite->posXPant & 0x03))*4;
	rellenoArriba = ((pers->sprite->posYPant & 0x07) >= 4) ? 0xf0*4 : 0xa0*4;
	rellenoAbajo = ((pers->sprite->posYPant & 0x07) >= 4) ? 0xa0*4 : 0xf0*4;

	// coloca la posici�n de la luz basada en la posici�n del personaje (ajustando la posici�n al inicio de un tile)
	posXPant = (pers->sprite->posXPant & 0xfc) - 8;
	if (posXPant < 0) posXPant = 0;
	posYPant = (pers->sprite->posYPant & 0xf8) - 24;
	if (posYPant < 0) posYPant = 0;

	oldPosXPant = posXPant;
	oldPosYPant = posYPant;

	// obtiene si el personaje est� girado
	flipX = pers->flipX;
}

/////////////////////////////////////////////////////////////////////////////
// dibujado de sprites
/////////////////////////////////////////////////////////////////////////////

// dibuja la parte visible del sprite actual en el �rea ocupada por el sprite que se le pasa como par�metro
void SpriteLuz::dibuja(Sprite *spr, UINT8 *bufferMezclas, int lgtudClipX, int lgtudClipY, int dist1X, int dist2X, int dist1Y, int dist2Y)
{
	// rellena de negro la parte superior del sprite
	for (int i = 0; i < rellenoArriba; i++){
		// CPC *bufferMezclas = 3;
		*bufferMezclas = 0; // VGA
		bufferMezclas++;
	}

	// para 15 bloques
	for (int j = 0; j < 15; j++){
		// guarda la posici�n inicial de este bloque
		UINT8 *posBuffer = bufferMezclas;

		// obtiene el patr�n para rellenar este bloque
		int patron = rellenoLuz[j];

		// rellena 4 l�neas de alto en la parte de la izquierda
		for (int i = 0; i < rellenoIzquierda; i++){
			/* CPC
			bufferMezclas[0] = 3;
			bufferMezclas[20*4] = 3;
			bufferMezclas[40*4] = 3;
			bufferMezclas[60*4] = 3;
			*/
			// VGA
			bufferMezclas[0] = bufferMezclas[20*4] = bufferMezclas[40*4] = bufferMezclas[60*4] = 0;

			bufferMezclas++;
		}

		// modifica levemente el patr�n dependiendo de a donde mira el personaje
		if (flipX){
			patron = patron << 1;
		}

		// completa el sprite de la luz seg�n el patr�n de relleno
		for (int i = 0; i < 16; i++){
			// si el bit actual es 0, rellena de negro un bloque de 4x4
			if ((patron & 0x8000) == 0){
				for (int k = 0; k < 4; k++){
					/* CPC
					bufferMezclas[0] = 3;
					bufferMezclas[20*4] = 3;
					bufferMezclas[40*4] = 3;
					bufferMezclas[60*4] = 3;
					*/
					// VGA
					bufferMezclas[0] = bufferMezclas[20*4] = bufferMezclas[40*4] = bufferMezclas[60*4] = 0;

					bufferMezclas++;
				}
			} else {
				bufferMezclas += 4;
			}

			patron = patron << 1;
		}

		// rellena 4 l�neas de alto en la parte de la derecha
		for (int i = 0; i < rellenoDerecha; i++){
			/* CPC
			bufferMezclas[0] = 3;
			bufferMezclas[20*4] = 3;
			bufferMezclas[40*4] = 3;
			bufferMezclas[60*4] = 3; */
			// VGA
			bufferMezclas[0] = bufferMezclas[20*4] = bufferMezclas[40*4] = bufferMezclas[60*4] = 0;

			bufferMezclas++;
		}

		// avanza la posici�n hasta la del siguiente bloque
		bufferMezclas = posBuffer + 80*4;
	}

	// rellena de negro la parte inferior del sprite
	for (int i = 0; i < rellenoAbajo; i++){
		// CPC *bufferMezclas = 3;
		*bufferMezclas = 0; // VGA
		bufferMezclas++;
	}
}

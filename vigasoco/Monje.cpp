// Monje.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "Monje.h"
#include "MotorGrafico.h"
#include "SpriteMonje.h"

using namespace Abadia;

/////////////////////////////////////////////////////////////////////////////
// tabla de la animación de los monjes
/////////////////////////////////////////////////////////////////////////////

// CPC
/*
Personaje::DatosFotograma Monje::tablaAnimacion[8] = {
	{ 0x0000, 0x05, 0x22 },
	{ 0x0000, 0x05, 0x24 },
	{ 0x0000, 0x05, 0x22 },
	{ 0x0000, 0x05, 0x22 },
	{ 0x0000, 0x05, 0x21 },
	{ 0x0000, 0x05, 0x23 },
	{ 0x0000, 0x05, 0x21 },
	{ 0x0000, 0x05, 0x21 }
};
*/
Personaje::DatosFotograma Monje::tablaAnimacion[8] = {
	{ 0x0000, 0x05, 0x22 },
	{ 0x0000, 0x05, 0x24 },
	{ 0x0000, 0x05, 0x22 },
	{ 0x0000, 0x05, 0x22 },
	{ 0x0000, 0x05, 0x21 },
	{ 0x0000, 0x05, 0x23 },
	{ 0x0000, 0x05, 0x21 },
	{ 0x0000, 0x05, 0x21 }
};

/////////////////////////////////////////////////////////////////////////////
// inicialización y limpieza
/////////////////////////////////////////////////////////////////////////////

Monje::Monje(SpriteMonje *spr) : PersonajeConIA(spr)
{
	// guarda una referencia al sprite del monje
	sprMonje = spr;

	// asigna la tabla de animación del personaje
	animacion = tablaAnimacion;
	numFotogramas = 8;

	// inicialmente no tiene datos de cara
	datosCara[0] = datosCara[1] = 0x0000;
}

Monje::~Monje()
{
}

/////////////////////////////////////////////////////////////////////////////
// animación del monje
/////////////////////////////////////////////////////////////////////////////

// calcula el fotograma que hay que poner al monje
Personaje::DatosFotograma * Monje::calculaFotograma()
{
	// obtiene la orientación del personaje según la posición de la cámara
	int oriCamara = elMotorGrafico->ajustaOrientacionSegunCamara(orientacion);

	// actualiza la animación del traje
	sprMonje->animacionTraje = (oriCamara << 2) | contadorAnimacion;

	// selecciona un fotograma dependiendo de la orientación y de si el personaje va hacia la derecha o a la izquierda
	int numAnim = (((oriCamara + 1) & 0x02) << 1) | contadorAnimacion;

	assert(numAnim < numFotogramas);

	// modifica los datos del fotograma con la dirección de la cara del personaje
	animacion[numAnim].dirGfx = datosCara[(numAnim & 0x04) ? 1 : 0];

	// devuelve los datos del fotograma de la animación del personaje
	return &animacion[numAnim];
}

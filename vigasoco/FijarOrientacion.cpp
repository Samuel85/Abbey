// FijarOrientacion.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "FijarOrientacion.h"
#include "RejillaPantalla.h"

using namespace Abadia;

/////////////////////////////////////////////////////////////////////////////
// métodos para indicar las posiciones a las que hay que ir según la orientación a coger
/////////////////////////////////////////////////////////////////////////////

// marca el rango de posiciones especificado como destino de la búsqueda
void FijarOrientacion::marcaPosiciones(RejillaPantalla *rejilla, int posX, int posY, int incX, int incY)
{
	for (int i = 0; i < 16; i++){
		rejilla->bufAlturas[posY][posX] |= 0x40;

		posX += incX;
		posY += incY;
	}
}

// marca como punto de destino cualquiera que vaya a la pantalla de la derecha
void FijaOrientacion0::fijaPos(RejillaPantalla *rejilla)
{
	marcaPosiciones(rejilla, 20, 4, 0, 1);
}

// marca como punto de destino cualquiera que vaya a la pantalla de abajo
void FijaOrientacion1::fijaPos(RejillaPantalla *rejilla)
{
	marcaPosiciones(rejilla, 4, 3, 1, 0);
}

// marca como punto de destino cualquiera que vaya a la pantalla de la izquierda
void FijaOrientacion2::fijaPos(RejillaPantalla *rejilla)
{
	marcaPosiciones(rejilla, 3, 4, 0, 1);
}

// marca como punto de destino cualquiera que vaya a la pantalla de arriba
void FijaOrientacion3::fijaPos(RejillaPantalla *rejilla)
{
	marcaPosiciones(rejilla, 4, 20, 1, 0);
}

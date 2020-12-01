// EntidadJuego.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "EntidadJuego.h"
#include "Sprite.h"

using namespace Abadia;

/////////////////////////////////////////////////////////////////////////////
// inicialización y limpieza
/////////////////////////////////////////////////////////////////////////////

PosicionJuego::PosicionJuego()
{
	orientacion = DERECHA;
	posX = 	posY = altura = 0;
}

PosicionJuego::PosicionJuego(int ori, int pX, int pY, int alt)
{
	orientacion = ori;
	posX = pX;
	posY = pY;
	altura = alt;
}

PosicionJuego::~PosicionJuego()
{
}

EntidadJuego::EntidadJuego(Sprite *spr) : PosicionJuego()
{
	sprite = spr;
}

EntidadJuego::~EntidadJuego()
{
}

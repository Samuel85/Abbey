// EntidadJuego.h
//
//	Clase abstracta que representa una entidad del juego
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _ENTIDAD_JUEGO_H_
#define _ENTIDAD_JUEGO_H_

namespace Abadia {


enum Orientacion {
	DERECHA = 0,			// hacia +x
	ABAJO = 1,			// hacia -y
	IZQUIERDA = 2,			// hacia -x
	ARRIBA = 3			// hacia +y
};

class PosicionJuego
{
public:
	int orientacion;		// orientaci�n de la posici�n en el mundo
	int posX;			// posici�n x en coordenadas de mundo
	int posY;			// posici�n y en coordenadas de mundo
	int altura;			// altura en coordenadas de mundo

	PosicionJuego();
	PosicionJuego(int ori, int pX, int pY, int alt);
	virtual ~PosicionJuego();
};

class Sprite;				// definido en Sprite.h

class EntidadJuego : public PosicionJuego
{
// campos
public:
	Sprite *sprite;			// sprite asociado a la entidad

// m�todos:
public:
	virtual void cambioPantalla(){}
	virtual void notificaVisibleEnPantalla(int posXPant, int posYPant, int profundidad) = 0;

	// inicializaci�n y limpieza
	EntidadJuego(Sprite *spr);
	virtual ~EntidadJuego();
};


}

#endif	// _ENTIDAD_JUEGO_H_

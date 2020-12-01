// Severino.h
//
//	Clase que representa a Severino
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _SEVERINO_H_
#define _SEVERINO_H_


#include "Monje.h"

namespace Abadia {


class Severino : public Monje
{
// campos
public:
	bool estaVivo;								// indica si el personaje está vivo

protected:
	static PosicionJuego posicionesPredef[4];	// posiciones a las que puede ir el personaje según el estado

// métodos
public:
	virtual void piensa();

	// inicialización y limpieza
	Severino(SpriteMonje *spr);
	virtual ~Severino();
};


}

#endif	// _SEVERINO_H_

// Bernardo.h
//
//	Clase que representa a Bernardo Gui
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _BERNARDO_H_
#define _BERNARDO_H_


#include "Monje.h"

namespace Abadia {


class Bernardo : public Monje
{
// campos
public:
	bool estaEnLaAbadia;						// indica si está en la abadía o no

protected:
	static PosicionJuego posicionesPredef[5];	// posiciones a las que puede ir el personaje según el estado

// métodos
public:
	virtual void piensa();

	// inicialización y limpieza
	Bernardo(SpriteMonje *spr);
	virtual ~Bernardo();
};


}

#endif	// _BERNARDO_H_

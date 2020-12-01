// Jorge.h
//
//	Clase que representa a Jorge
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _JORGE_H_
#define _JORGE_H_


#include "Monje.h"

namespace Abadia {


class Jorge : public Monje
{
// campos
public:
	bool estaActivo;							// indica si el personaje está activo o no
	int contadorHuida;							// contador usado para la huida

protected:
	static PosicionJuego posicionesPredef[2];	// posiciones a las que puede ir el personaje según el estado

// métodos
public:
	virtual void piensa();

	// inicialización y limpieza
	Jorge(SpriteMonje *spr);
	virtual ~Jorge();
};


}

#endif	// _JORGE_H_

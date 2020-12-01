// Guillermo.h
//
//	Clase que representa a Guillermo
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _GUILLERMO_H_
#define _GUILLERMO_H_


#include "Personaje.h"
#include "Types.h"


namespace Abadia {


class Guillermo : public Personaje
{
// campos
public:
	int incrPosY;								// incremento de la posici�n y si el estado no es 0

protected:
	static DatosFotograma tablaAnimacion[8];	// tabla con los distintos fotogramas de la animaci�n del personaje

// m�todos
public:
	virtual void run();
	virtual void ejecutaMovimiento();

	// inicializaci�n y limpieza
	Guillermo(Sprite *spr);
	virtual ~Guillermo();
};


}

#endif	// _GUILLERMO_H_

// Malaquias.h
//
//	Clase que representa a Malaquías
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _MALAQUIAS_H_
#define _MALAQUIAS_H_

#include <iosfwd>
#include "Monje.h"

namespace Abadia {


class Malaquias : public Monje
{
// campos
public:
	int estaMuerto;								// indica si el personaje está muerto o muriéndose
	int estado2;								// guarda información extra sobre el estado del personaje

protected:
	int contadorEnScriptorium;					// indica el tiempo que guillermo está sin salir del scriptorium
	static PosicionJuego posicionesPredef[9];	// posiciones a las que puede ir el personaje según el estado

// métodos
public:
	virtual void piensa();

	// inicialización y limpieza
	Malaquias(SpriteMonje *spr);
	virtual ~Malaquias();

	// cargar/salvar
	friend std::ofstream& operator<<(std::ofstream&,const Malaquias* const);
	friend std::ifstream& operator>>(std::ifstream&,Malaquias* const);

protected:
	virtual void avanzaAnimacionOMueve();
};


}

#endif	// _MALAQUIAS_H_

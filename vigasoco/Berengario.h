// Berengario.h
//
//	Clase que representa a Berengario
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _BERENGARIO_H_
#define _BERENGARIO_H_


#include "Monje.h"
#include <iosfwd>

namespace Abadia {


class Berengario : public Monje
{
// campos
public:
	bool estaVivo;								// indica si el personaje está vivo
	int estado2;								// guarda información extra sobre el estado del personaje
	int contadorPergamino;						// contador usado para informar al abad si guillermo no suelta el pergamino

protected:
	static PosicionJuego posicionesPredef[6];	// posiciones a las que puede ir el personaje según el estado

// métodos
public:
	virtual void piensa();
	void fijaCapucha(bool puesta);

	// inicialización y limpieza
	Berengario(SpriteMonje *spr);
	virtual ~Berengario();

	friend std::ofstream& operator<<(std::ofstream& out,
					const Berengario* const berengario);

protected:
	bool guillermoHaCogidoElPergamino();
};


}

#endif	// _BERENGARIO_H_

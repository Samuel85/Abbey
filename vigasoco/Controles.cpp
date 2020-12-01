// Controles.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "Controles.h"

// para memset
#include <string.h>

using namespace Abadia;

/////////////////////////////////////////////////////////////////////////////
// inicialización y limpieza
/////////////////////////////////////////////////////////////////////////////

Controles::Controles()
{
	ih = 0;
}

Controles::~Controles()
{
}

/////////////////////////////////////////////////////////////////////////////
// métodos
/////////////////////////////////////////////////////////////////////////////

// inicia los controles para el juego
void Controles::init(InputHandler *input)
{
	ih = input;
	memset(_oldControles, 0, END_OF_INPUTS);
}

// actualiza el estado de los controles
void Controles::actualizaEstado()
{
	// obtiene el estado de los controles
	ih->copyInputsState(_controles);

	// combina el estado actual de los controles con el anterior para poder detectar pulsaciones
	for (int i = 0; i < END_OF_INPUTS; i++){
		_oldControles[i] = (_oldControles[i] << 1) & 0x03;
		if (_controles[i] > 0){
			_oldControles[i] |= 1;
		}
	}
}

// comprueba si se acaba de pulsar una tecla
bool Controles::seHaPulsado(Inputs input)
{
	// detecta transiciones de 0 a 1
	return _oldControles[input] == 0x01;
}

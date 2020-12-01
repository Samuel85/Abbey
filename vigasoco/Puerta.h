// Puerta.h
//
//	Clase que representa una puerta del juego
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _PUERTA_H_
#define _PUERTA_H_


#include "EntidadJuego.h"


namespace Abadia {

class Personaje;						// definido en Personaje.h
class RejillaPantalla;					// definido en RejillaPantalla.h

class Puerta : public EntidadJuego
{
// campos
public:
	int identificador;					// identificador de la puerta
	bool estaAbierta;					// indica si la puerta está abierta
	bool haciaDentro;					// indica si la puerta se abre hacia dentro o hacia fuera
	bool estaFija;						// indica si la puerta se queda fija
	bool hayQueRedibujar;				// indica si hay que redibujar la puerta

protected:
	static int despOrientacion[4][12];

// métodos
public:
	virtual void notificaVisibleEnPantalla(int posXPant, int posYPant, int profundidad);
	void compruebaAbrirCerrar(Personaje **personajes, int numPersonajes);
	void marcaPosiciones(RejillaPantalla *rejilla, int valor);

	// inicialización y limpieza
	Puerta(Sprite *spr);
	virtual ~Puerta();

// métodos de ayuda
protected:
	bool puedeAbrir(Personaje *pers);
	bool accionesAbrirCerrar(bool abrir);
};


}

#endif	// _PUERTA_H_

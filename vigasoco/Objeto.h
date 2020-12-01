// Objeto.h
//
//	Clase que representa un objeto del juego
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _OBJETO_H_
#define _OBJETO_H_


#include "EntidadJuego.h"

namespace Abadia {


class Personaje;				// definido en Personaje.h

class Objeto : public EntidadJuego
{
	// campos
	public:
		bool seEstaCogiendo;		// indica si el objeto se está cogiendo o dejando
		bool seHaCogido;			// indica si el objeto está disponible o ha sido cogido
		Personaje *personaje;		// personaje que tiene el objeto (en el caso de que haya sido cogido)

		// métodos
	public:
		virtual void notificaVisibleEnPantalla(int posXPant, int posYPant, int profundidad);

		bool seHaCogidoPor(Personaje *pers, int mascara);
		void dejar(Personaje *pers, int mascara, int posXObj, int posYObj, int alturaObj);

		// inicialización y limpieza
		Objeto(Sprite *spr);
		virtual ~Objeto();
};

}

#endif	// _OBJETO_H_

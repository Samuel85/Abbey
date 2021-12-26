// Personaje.h
//
//	Clase que representa un personaje del juego
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _PERSONAJE_H_
#define _PERSONAJE_H_


#include "Types.h"

#include "EntidadJuego.h"


namespace Abadia {

class RejillaPantalla;			// definido en RejillaPantalla.h

class Personaje : public EntidadJuego
{
// tipos
protected:
	struct DatosFotograma {
		int dirGfx;				// direcci�n de los gr�ficos de este fotograma de la animaci�n
		int ancho;				// ancho de este fotograma de la animaci�n (en m�ltiplos de 4 pixels)
		int alto;				// alto de este fotograma de la animaci�n (en pixels)
	};

// campos
public:
	int estado;					// estado del personaje
	int contadorAnimacion;		// contador para animar el personaje
	bool bajando;				// indica si el personaje est� bajando en altura
	bool enDesnivel;			// indica que el personaje est� en un desnivel
	bool giradoEnDesnivel;		// indica que el personaje no est� avanzando en el sentido del desnivel
	bool flipX;					// indica si los gr�ficos est�n girados en x
	int despFlipX;				// desplazamiento de los gr�ficos girados en x
	int despX;					// desplazamiento en x para dibujar el personaje (en m�ltiplos de 4 pixels)
	int despY;					// desplazamiento en y para dibujar el personaje (en pixels)
	int valorPosicion;			// valor a grabar en las posiciones de la rejilla en las que est� el personaje

	bool puedeQuitarObjetos;	// indica si el personaje puede quitar objetos o a otro personaje
	int objetos;				// objetos que tiene el personaje
	int mascaraObjetos;			// m�scara de los objetos que puede coger el personaje
	int contadorObjetos;		// contador para no coger/dejar los objetos varias veces

	int permisosPuertas;		// puertas que puede abrir el personaje

	DatosFotograma *animacion;	// tabla con los datos para las animaciones
	int numFotogramas;			// n�mero de fotogramas de la tabla de animaciones
	


protected:
	// tabla para el c�lculo del desplazamiento, seg�n la animaci�n y la c�mara de un personaje
	static int difPosAnimCam[4][4][14];

// m�todos
public:
	virtual void run() = 0;

	void notificaVisibleEnPantalla(int posXPant, int posYPant, int profundidad) override;

	int puedeDejarObjeto(int &posXObj, int &posYObj, int &alturaObj);

	void marcaPosicion(RejillaPantalla *rejilla, int valor);
	void actualizaSprite();

	// inicializaci�n y limpieza
	Personaje(Sprite *spr);
	virtual ~Personaje();

protected:
	virtual void ejecutaMovimiento() = 0;
	virtual void avanzaAnimacionOMueve();

	void mueve();

	void trataDeAvanzar(int difAltura1, int difAltura2, int avanceX, int avanceY);
	void incrementaPos(int avanceX, int avanceY);
	void gira(int difOrientacion);
	bool cambioCentro();

	void actualizaPosPantSprite(int posXPant, int posYPant, int profundidad);
	void avanzaAnimacion();
	virtual DatosFotograma *calculaFotograma();
	void actualizaAnimacion(DatosFotograma *da, int profundidad);
};


}

#endif	// _PERSONAJE_H_

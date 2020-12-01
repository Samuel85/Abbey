// Guillermo.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "Controles.h"
#include "Guillermo.h"
#include "Juego.h"
#include "Logica.h"
#include "MotorGrafico.h"
#include "RejillaPantalla.h"
#include "Sprite.h"

#include "system.h"

using namespace Abadia;

/////////////////////////////////////////////////////////////////////////////
// tabla de la animaci�n del personaje
/////////////////////////////////////////////////////////////////////////////
/* CPC
Personaje::DatosFotograma Guillermo::tablaAnimacion[8] = {
	{ 0xa3b4, 0x05, 0x22 },
	{ 0xa300, 0x05, 0x24 },
	{ 0xa3b4, 0x05, 0x22 },
	{ 0xa45e, 0x05, 0x22 },
	{ 0xa666, 0x04, 0x21 },
	{ 0xa508, 0x05, 0x23 },
	{ 0xa666, 0x04, 0x21 },
	{ 0xa5b7, 0x05, 0x21 }
};
*/
// VGA
Personaje::DatosFotograma Guillermo::tablaAnimacion[8] = {
	{ 54480, 0x05, 0x22 },
	{ 53760, 0x05, 0x24 },
	{ 54480, 0x05, 0x22 },
	{ 55180, 0x05, 0x22 },
	{ 57240, 0x04, 0x21 },
	{ 55880, 0x05, 0x23 },
	{ 57240, 0x04, 0x21 },
	{ 56580, 0x05, 0x21 }
};

/////////////////////////////////////////////////////////////////////////////
// inicializaci�n y limpieza
/////////////////////////////////////////////////////////////////////////////

Guillermo::Guillermo(Sprite *spr) : Personaje(spr)
{
	// asigna la tabla de animaci�n del personaje
	animacion = tablaAnimacion;
	numFotogramas = 8;

	incrPosY = 2;
}

Guillermo::~Guillermo()
{
}

/////////////////////////////////////////////////////////////////////////////
// movimiento
/////////////////////////////////////////////////////////////////////////////

// m�todo llamado desde el bucle principal para que el personaje interactue con el mundo virtual
void Guillermo::run()
{
	mueve();
}

// mueve el personaje seg�n el estado en el que se encuentra
void Guillermo::ejecutaMovimiento()
{
	// si est� vivo, responde a la pulsaci�n de los cursores
	if (estado == 0){
		// si la c�mara no sigue a guillermo, sale
		if (laLogica->numPersonajeCamara != 0) return;

		// dependiendo de la tecla que se pulse, act�a en consecuencia
		//if (losControles->estaSiendoPulsado(P1_LEFT)){
		if (sys->pad.left){
			gira(1);
			sys->pad.left = false;
		//} else if (losControles->estaSiendoPulsado(P1_RIGHT)){
		} else if (sys->pad.right){
			gira(-1);
			sys->pad.right = false;
		//} else if (losControles->estaSiendoPulsado(P1_UP)){
		} else if (sys->pad.up){
			int difAltura1, difAltura2, avanceX, avanceY;

			// obtiene la altura de las posiciones hacia las que se va a mover
			elMotorGrafico->rejilla->obtenerAlturaPosicionesAvance(this, difAltura1, difAltura2, avanceX, avanceY);
			trataDeAvanzar(difAltura1, difAltura2, avanceX, avanceY);
		}
	} else {
		// si ha llegado al �ltimo estado cuando est� muerto, sale
		if (estado == 1) return;

		estado = estado - 1;

		// si ha caido en la trampa del espejo, lo mete en el agujero
		if (estado == 0x13){
			if (incrPosY == 2){
				posX = posX - 1;
				actualizaSprite();
				return;
			}
		}

        if (estado != 1){
			// modifica la posici�n y del sprite en pantalla
			sprite->posYPant += incrPosY;
			sprite->haCambiado = true;

			laLogica->hayMovimiento = true;
		} else {
			// en el estado 1 desaparece el sprite de guillermo
			sprite->esVisible = false;
		}
	}
}

// Objeto.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "MotorGrafico.h"
#include "Personaje.h"
#include "Objeto.h"
#include "Sprite.h"

using namespace Abadia;

/////////////////////////////////////////////////////////////////////////////
// inicialización y limpieza
/////////////////////////////////////////////////////////////////////////////

Objeto::Objeto(Sprite *spr) : EntidadJuego(spr)
{
	seEstaCogiendo = false;
	seHaCogido = false;
	personaje = 0;
}

Objeto::~Objeto()
{
}

/////////////////////////////////////////////////////////////////////////////
// actualización del entorno cuando un objeto es visible en la pantalla actual
/////////////////////////////////////////////////////////////////////////////

// actualiza la posición del sprite dependiendo de su posición con respecto a la cámara
void Objeto::notificaVisibleEnPantalla(int posXPant, int posYPant, int profundidad)
{
	// si el objeto no se ha cogido
	if (!seHaCogido){
		// marca el sprite para dibujar
		sprite->haCambiado = true;
		sprite->esVisible = true;
		sprite->profundidad = profundidad;

		// ajusta la posición del sprite (-8, -8)
		sprite->posXPant = posXPant - 2;
		sprite->posYPant = posYPant - 8;
	}
}

/////////////////////////////////////////////////////////////////////////////
// métodos relacionados con coger/dejar los objetos
/////////////////////////////////////////////////////////////////////////////

// comprueba si el personaje puede coger el objeto
bool Objeto::seHaCogidoPor(Personaje *pers, int mascara)
{
	// si el objeto se está cogiendo o dejando, no puede ser cogido
	if (seEstaCogiendo) return false;

	// guarda la posición del objeto
	int posXObj = posX;
	int posYObj = posY;
	int alturaObj = altura;

	// si el objeto está cogido, su posición viene dada por la del personaje que lo tiene
	if (seHaCogido){
		// si el personaje no puede quitar objetos, sale
		if (!pers->puedeQuitarObjetos) return false;

		// obtiene la posición del personaje
		posXObj = personaje->posX;
		posYObj = personaje->posY;
		alturaObj = personaje->altura;
	}

	// comprueba si el personaje está en una posición que permita coger el objeto
	int difAltura = alturaObj - pers->altura;
	if ((difAltura < 0) || (difAltura >= 5)) return false;

	int posXPers = pers->posX + 2*elMotorGrafico->tablaDespOri[pers->orientacion][0];
	if (posXObj != posXPers) return false;

	int posYPers = pers->posY + 2*elMotorGrafico->tablaDespOri[pers->orientacion][1];
	if (posYObj != posYPers) return false;

	// si el objeto está cogido por un personaje, se lo quita
	if (seHaCogido){
		personaje->objetos = personaje->objetos ^ mascara;
	}

	// si el sprite del objeto es visible, indica que va a desaparecer
	if (sprite->esVisible){
		sprite->haCambiado = true;
		sprite->desaparece = true;
	}

	// guarda el personaje que tiene el objeto, indica que el objeto se ha cogido e inicia el contador
	personaje = pers;
	seHaCogido = true;
	seEstaCogiendo = true;
	pers->contadorObjetos = 0x10;
	pers->objetos = pers->objetos | mascara;

	return true;
}

// deja el objeto que tenía el personaje en la posición indicada
void Objeto::dejar(Personaje *pers, int mascara, int posXObj, int posYObj, int alturaObj)
{
	// guarda la posición y orientación del objeto e indica que ya no está cogido
	posX = posXObj;
	posY = posYObj;
	altura = alturaObj;
	orientacion = pers->orientacion ^ 0x02;
	seHaCogido = false;
	personaje = 0;
	
	// inicia el contador para coger/dejar objetos y le quita el objeto al personaje
	pers->contadorObjetos = 0x10;
	pers->objetos = pers->objetos & (~mascara);

	// salta a la rutina de redibujado de objetos para redibujar solo el objeto que se deja
	// actualiza la posición del sprite según la cámara
	int posXPant, posYPant, sprPosY;

	if (elMotorGrafico->actualizaCoordCamara(this, posXPant, posYPant, sprPosY) != -1){
		notificaVisibleEnPantalla(posXPant, posYPant, sprPosY);
	} else {
		sprite->esVisible = false;
	}

	sprite->oldPosXPant = sprite->posXPant;
	sprite->oldPosYPant = sprite->posYPant;
}

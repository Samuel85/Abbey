// Personaje.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include "Juego.h"
#include "Logica.h"
#include "MotorGrafico.h"
#include "Personaje.h"
#include "RejillaPantalla.h"
#include "Sprite.h"

using namespace Abadia;

/////////////////////////////////////////////////////////////////////////////
// tabla para el cálculo del desplazamiento, según la animación y la cámara, de un personaje
/////////////////////////////////////////////////////////////////////////////

int Personaje::difPosAnimCam[4][4][14] = {
	{ 
		{ 0, 0,  -1, -2,  -1, +2,  -2,  0,  +1, +2,   0,  0,   0, -2 },
		{ 0, 0,  -1, +2,  +1, +2,   0, +4,  -1, +2,  -2, +6,  -2,  0 },
		{ 0, 0,  +1, +2,  -1, +2,   0, +4,  +1, +2,  +2, +6,  +2,  0 },
		{ 0, 0,  +1, -2,  +1, +2,  +2,  0,  -1, +2,   0,  0,   0, -2 }
	},
	{ 
		{ 0, 0,  -1, -2,  -1, +2,  -2,  0,  -1, -2,  -2, -4,  -2, -6 },
		{ 0, 0,  -1, +2,  -1, -2,  -2,  0,  -1, +2,  -2, +6,  -2,  0 },
		{ 0, 0,  +1, +2,  -1, +2,   0, +4,  -1, -2,   0, +2,   0, -4 },
		{ 0, 0,  +1, -2,  -1, -2,   0, -4,  -1, +2,   0,  0,   0, -2 }
	},
	{ 
		{ 0, 0,  -1, -2,  +1, -2,   0, -4,  -1, -2,  -2, -4,  -2, -6 },
		{ 0, 0,  -1, +2,  -1, -2,  -2,  0,  +1, -2,   0, +2,   0, -3 },
		{ 0, 0,  +1, +2,  +1, -2,  +2,  0,  -1, -2,   0, +2,   0, -4 },
		{ 0, 0,  +1, -2,  -1, -2,   0, -4,  +1, -2,  +2, -4,  +2, -6 }
	},
	{ 
		{ 0, 0,  -1, -2,  +1, -2,   0, -4,  +1, +2,   0,  0,   0, -2 },
		{ 0, 0,  -1, +2,  +1, +2,   0, +4,  +1, -2,   0, +2,   0, -4 },
		{ 0, 0,  +1, +2,  +1, -2,  +2,  0,  +1, +2,  +2, +6,  +2,  0 },
		{ 0, 0,  +1, -2,  +1, +2,  +2,  0,  +1, -2,  +2, -4,  +2, -6 }
	}
};

/////////////////////////////////////////////////////////////////////////////
// inicialización y limpieza
/////////////////////////////////////////////////////////////////////////////

Personaje::Personaje(Sprite *spr) : EntidadJuego(spr)
{
	estado = 0;
	puedeQuitarObjetos = false;
	objetos = 0;
	mascaraObjetos = 0;
	contadorObjetos = 0;

	contadorAnimacion = 0;
	bajando = false;
	giradoEnDesnivel = false;
	enDesnivel = false;
	flipX = false;
	despFlipX = 0xc000;
	despX = despY = 0;
	valorPosicion = 0x10;

	permisosPuertas = 0;

	animacion = 0;
	numFotogramas = 0;
}

Personaje::~Personaje()
{
}

/////////////////////////////////////////////////////////////////////////////
// actualización del entorno cuando un personaje es visible en la pantalla actual
/////////////////////////////////////////////////////////////////////////////

void Personaje::actualizaPosPantSprite(int posXPant, int posYPant, int profundidad)
{
	int entrada = 0;

	// si el personaje ocupa sólo una posición porque está en un desnivel, hay que tener en cuenta varios casos
	if (enDesnivel){
		entrada += 2;

		if (!giradoEnDesnivel){
			entrada += 2;

			if ((contadorAnimacion & 0x01) == 0x01){
				entrada += 1;
				if (bajando){
					entrada += 1;
				}
			}
		} else {
			entrada += contadorAnimacion & 0x01;
		}
	} else {
		// si el personaje ocupa cuatro posiciones, la posición solo depende del contador de la animación
		entrada += contadorAnimacion & 0x01;
	}

	// actualiza la posición en pantalla del sprite asociado al personaje dependiendo de la cámara
	int oriAjustada = elMotorGrafico->ajustaOrientacionSegunCamara(orientacion);
	sprite->posXPant = posXPant + despX + difPosAnimCam[elMotorGrafico->oriCamara][oriAjustada][2*entrada];
	sprite->posYPant = posYPant + despY + difPosAnimCam[elMotorGrafico->oriCamara][oriAjustada][2*entrada + 1];

	// si el sprite no es visible, fija también la posición anterior
	if (!sprite->esVisible){
		sprite->oldPosXPant = sprite->posXPant;
		sprite->oldPosYPant = sprite->posYPant;
	}
}

// actualiza la posición y la animación del sprite dependiendo de su posición con respecto a la cámara
void Personaje::notificaVisibleEnPantalla(int posXPant, int posYPant, int profundidad)
{
	// actualiza la posición en pantalla del sprite
	actualizaPosPantSprite(posXPant, posYPant, profundidad);

	// actualiza la animación del personaje
	DatosFotograma *df = calculaFotograma();
	actualizaAnimacion(df, profundidad);
}

/////////////////////////////////////////////////////////////////////////////
// animación del personaje
/////////////////////////////////////////////////////////////////////////////

// calcula el fotograma que hay que poner al personaje
Personaje::DatosFotograma * Personaje::calculaFotograma()
{
	// obtiene la orientación del personaje según la posición de la cámara
	int oriCamara = elMotorGrafico->ajustaOrientacionSegunCamara(orientacion);

	// selecciona un fotograma dependiendo de la orientación y de si el personaje va hacia la derecha o a la izquierda
	int numAnim = (((oriCamara + 1) & 0x02) << 1) | contadorAnimacion;

	assert(numAnim < numFotogramas);

	// devuelve los datos del fotograma de la animación del personaje
	return &animacion[numAnim];
}

// actualiza la animación del sprite con el fotograma que se le pasa
void Personaje::actualizaAnimacion(DatosFotograma *df, int profundidad)
{
	sprite->esVisible = true;
	sprite->haCambiado = true;
	sprite->profundidad = profundidad;

	// obtiene la orientación del personaje según la posición de la cámara
	int oriCamara = elMotorGrafico->ajustaOrientacionSegunCamara(orientacion);

	// comprueba si hay que girar los gráficos del personaje por el cambio de la orientación del personaje
	if (((oriCamara >> 1) & 0x01) ^ ((flipX) ? 1 : 0)){
		flipX = !flipX;
	}

	sprite->despGfx = df->dirGfx;
	sprite->ancho = df->ancho;
	sprite->alto = df->alto;

	if (flipX){
		//CPC sprite->despGfx += despFlipX;
		sprite->despGfx += 120305;
	}

	laLogica->hayMovimiento = true;
}

// avanza la animación del sprite y ajusta según la cámara
void Personaje::avanzaAnimacion()
{
	// avanza el contador de la animación
	contadorAnimacion = (contadorAnimacion + 1) & 0x03;

	// actualiza la posición y dimensiones del sprite del personaje según la animación y la cámara
	actualizaSprite();
}

// actualiza la posición y dimensiones del sprite del personaje según la animación y la cámara
void Personaje::actualizaSprite()
{
	int posXPant, posYPant, profundidad;

	// comprueba si el sprite es visible en la pantalla actual
	if (elMotorGrafico->actualizaCoordCamara(this, posXPant, posYPant, profundidad) != -1){
		// si el sprite es visible, actualiza su posición en pantalla
		actualizaPosPantSprite(posXPant, posYPant, profundidad);

		// obtiene el fotograma actual del personaje
		DatosFotograma *df = calculaFotograma();
		actualizaAnimacion(df, profundidad);
	} else {
		// si el sprite era visible, lo hace desaparecer
		if (sprite->esVisible){
			sprite->desaparece = true;
			sprite->haCambiado = true;
			sprite->profundidad = 0;
		}
	}

	laLogica->hayMovimiento = true;
}

/////////////////////////////////////////////////////////////////////////////
// métodos relacionados con el movimiento del personaje
/////////////////////////////////////////////////////////////////////////////

void Personaje::mueve()
{
	// pone la posición y dimensiones actuales como posición y dimensiones antiguas
	sprite->preparaParaCambio();

	// si el personaje está en las posiciones centrales de la pantalla actual, limpia las posiciones que ocupa
	marcaPosicion(elMotorGrafico->rejilla, 0);

	// avanza la animación del personaje o se mueve
	avanzaAnimacionOMueve();

	// si el personaje está en las posiciones centrales de la pantalla actual, marca las posiciones que ocupa
	marcaPosicion(elMotorGrafico->rejilla, valorPosicion);
}

// dependiendo de como esté la animación del personaje, avanza la animación o realiza un movimiento
void Personaje::avanzaAnimacionOMueve()
{
	if ((contadorAnimacion & 0x01) == 0x01){
		avanzaAnimacion();
	} else {
		ejecutaMovimiento();
	}
}

// si el personaje puede avanzar en la orientación actual, avanza
void Personaje::trataDeAvanzar(int difAltura1, int difAltura2, int avanceX, int avanceY)
{
	bajando = false;

	// si el personaje ocupa 4 posiciones
	if (!enDesnivel){
		// si se quiere subir o bajar, cambia la altura e indica que el personaje está en desnivel
		if (difAltura1 == 1){
			altura++;
			enDesnivel = true;
		} else if (difAltura1 == -1){
			altura--;
			enDesnivel = true;
			bajando = true;
		} else if (difAltura1 != 0){
			// si se quiere pasar a un desnivel de más de una posición, sale
			return;
		}

		// si se anda por una zona sin desnivel, actualiza la posición y la animación del personaje según hacia donde se avanza
		if (difAltura1 == 0){
			incrementaPos(avanceX, avanceY);
			avanzaAnimacion();
		} else {
			// si se va a subir o a bajar
			incrementaPos(avanceX, avanceY);

			if (cambioCentro()){
				incrementaPos(avanceX, avanceY);
			}
			avanzaAnimacion();
		}
	} else {
		// si el personaje ocupa 1 posición

		// si se quiere avanzar a una posición donde hay un personaje, sale
		if ((difAltura2 == 0x10) || (difAltura2 == 0x20)) return;

		if (!giradoEnDesnivel){
			// si se quiere subir o bajar, cambia la altura
			if (difAltura1 == 1){
				altura++;
			} else if (difAltura1 == -1){
				altura--;
				bajando = true;
			} else {
				// si el desnivel es muy grande, sale
				return;
			}

			// si las 2 posiciones que hay avanzando tienen la misma altura, indica que ya no está en desnivel
			if (difAltura1 == difAltura2){
				enDesnivel = false;

				incrementaPos(avanceX, avanceY);

				if (!cambioCentro()){
					incrementaPos(avanceX, avanceY);
				}
				avanzaAnimacion();
			} else {
				incrementaPos(avanceX, avanceY);
				avanzaAnimacion();
			}
		} else {
			// si el personaje está girado en un desnivel
			int difAltura = cambioCentro() ? difAltura2 : difAltura1;
			
			if (difAltura != 0) return;

			incrementaPos(avanceX, avanceY);
			avanzaAnimacion();
		}
	}
}

// actualiza la posición del personaje según el avance que se le pasa
void Personaje::incrementaPos(int avanceX, int avanceY)
{
	posX += avanceX;
	posY += avanceY;
}

// gira el personaje a la derecha o a la izquierda
void Personaje::gira(int difOrientacion)
{
	contadorAnimacion = 0;

	// si está en desnivel, obtiene si avanza en el sentido del desnivel o no
	if (enDesnivel){
		giradoEnDesnivel = !giradoEnDesnivel;
	}

	// actualiza la orientación del personaje
	orientacion = (orientacion + difOrientacion) & 0x03;

	// actualiza la posición y dimensiones del sprite del personaje según la animación y la cámara
	actualizaSprite();
}

// si la orientación del personaje es DERECHA o ARRIBA devuelve false, en otro caso devuelve true
bool Personaje::cambioCentro()
{
	return (orientacion == ABAJO) || (orientacion == IZQUIERDA);
}

/////////////////////////////////////////////////////////////////////////////
// métodos relacionados con los objetos
/////////////////////////////////////////////////////////////////////////////

// indica si se puede dejar un objeto. Si no se puede, devuelve -1. En otro caso devuelve el número de objeto que puede dejar
int Personaje::puedeDejarObjeto(int &posXObj, int &posYObj, int &alturaObj)
{
	// inicia la máscara para el primer objeto
	int mascara = 1 << Juego::numObjetos;

	// si el personaje está cogiendo o dejando algún objeto, sale
	contadorObjetos--;
	if (contadorObjetos != -1) return -1;
	contadorObjetos++;

	// recorre los objetos del juego
	for (int i = 0; i < Juego::numObjetos; i++){
		mascara = mascara >> 1;

		// si no se tiene el objeto que actual, pasa al siguiente
		if ((objetos & mascara) == 0) continue;

		// obtiene la posición en la que se dejará el objeto 
		posXObj = posX + 2*elMotorGrafico->tablaDespOri[orientacion][0];
		posYObj = posY + 2*elMotorGrafico->tablaDespOri[orientacion][1];
		alturaObj = altura;
		int alturaBasePlantaObj = elMotorGrafico->obtenerAlturaBasePlanta(altura);
		int alturaRelativa = altura - alturaBasePlantaObj;

		bool estaEnPantallaActual = false;

		// si el objeto está en la misma planta que la que se muestra en pantalla
		if (alturaBasePlantaObj == elMotorGrafico->rejilla->minAltura){
			RejillaPantalla *rejilla = elMotorGrafico->rejilla;

			int posXObjRejilla, posYObjRejilla;

			// comprueba si la posición en la que se deja el objeto está en la rejilla de pantalla que se muestra
			if (rejilla->ajustaAPosRejilla(posXObj, posYObj, posXObjRejilla, posYObjRejilla)){
				int altPos = rejilla->bufAlturas[posYObjRejilla][posXObjRejilla];

				// si hay algún personaje en la posición en la que se quiere dejar el objeto, sale
				if ((altPos & 0xf0) != 0) return -1;

				// se queda con la altura de esa posición
				altPos = altPos & 0x0f;

				// si se va a dejar a una altura muy alta de la planta, sale
				if (altPos >= 0x0d) return -1;

				// si hay mucha diferencia de altura del personaje a donde se va dejar, sale
				if ((altPos - alturaRelativa) >= 5) return -1;

				// si la altura de la posición en donde se va a dejar el objeto es distinta de la de sus vecinos, sale
				if (altPos != rejilla->bufAlturas[posYObjRejilla][posXObjRejilla - 1]) return -1;
				if (altPos != rejilla->bufAlturas[posYObjRejilla - 1][posXObjRejilla]) return -1;
				if (altPos != rejilla->bufAlturas[posYObjRejilla - 1][posXObjRejilla - 1]) return -1;

				// calcula la altura final del objeto
				alturaObj = rejilla->minAltura + altPos;

				estaEnPantallaActual = true;
			}
		}

		// si el objeto no se va a dejar en la pantalla que se muestra actualmente, se deja en la posición del personaje
		if (!estaEnPantallaActual){
			posXObj = posX;
			posYObj = posY;
			alturaObj = altura;
		}

		// devuelve el objeto que se puede dejar
		return i;
	}

	// si llega aquí es que no se pudo dejar ningún objeto
	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// posición en el buffer de alturas
/////////////////////////////////////////////////////////////////////////////

// marca la posición ocupada por el personaje en el buffer de alturas
void Personaje::marcaPosicion(RejillaPantalla *rejilla, int valor)
{
	int posXRejilla, posYRejilla;

	// si el personaje está en las 20x20 posiciones centrales de la rejilla, marca las posiciones que ocupa
	if (rejilla->estaEnRejillaCentral(this, posXRejilla, posYRejilla)){
		// marca la posición (x, y) en el buffer de alturas
		rejilla->bufAlturas[posYRejilla][posXRejilla] = (rejilla->bufAlturas[posYRejilla][posXRejilla] & 0x0f) | valor;

		// si el personaje no está en un desnivel, ocupa 4 posiciones ((x, y)(x-1, y)(x, y-1)(x-1, y-1))
		if (!enDesnivel){
			rejilla->bufAlturas[posYRejilla][posXRejilla - 1] = (rejilla->bufAlturas[posYRejilla][posXRejilla - 1] & 0x0f) | valor;
			rejilla->bufAlturas[posYRejilla - 1][posXRejilla] = (rejilla->bufAlturas[posYRejilla - 1][posXRejilla] & 0x0f) | valor;
			rejilla->bufAlturas[posYRejilla - 1][posXRejilla - 1] = (rejilla->bufAlturas[posYRejilla - 1][posXRejilla - 1] & 0x0f) | valor;
		}
	}
}

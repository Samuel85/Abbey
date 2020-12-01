// Puerta.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "Logica.h"
#include "MotorGrafico.h"
#include "Personaje.h"
#include "Puerta.h"
#include "Sprite.h"
#include "RejillaPantalla.h"

#include "sonidos.h"
#include "system.h"
using namespace Abadia;

/////////////////////////////////////////////////////////////////////////////
// tabla para el c�lculo de los desplazamientos de las puertas seg�n la orientaci�n
/////////////////////////////////////////////////////////////////////////////

int Puerta::despOrientacion[4][12] = {
	{ +2,  0,    0, -1,   -1, -34,   +1,    +1,     0,  0,    +1,  0 },
	{  0, -4,   -1, -1,   -1, -42,    0,     0,     0,  0,     0, -1 },
	{ -2,  0,   -1,  0,   -5, -42,    0,    +1,     0,  0,    -1,  0 },
	{  0, +4,    0,  0,   -5, -34,   +1,     0,    +1, +1,     0, +1 }
};

/////////////////////////////////////////////////////////////////////////////
// inicializaci�n y limpieza
/////////////////////////////////////////////////////////////////////////////

Puerta::Puerta(Sprite *spr) : EntidadJuego(spr)
{
	identificador = 0;
	estaAbierta = false;
	estaFija = false;
	haciaDentro = false;
	hayQueRedibujar = false;
}

Puerta::~Puerta()
{
}

/////////////////////////////////////////////////////////////////////////////
// actualizaci�n del entorno cuando una puerta es visible en la pantalla actual
/////////////////////////////////////////////////////////////////////////////

// actualiza la posici�n del sprite dependiendo de su posici�n con respecto a la c�mara
void Puerta::notificaVisibleEnPantalla(int posXPant, int posYPant, int profundidad)
{
	// pone la posici�n y dimensiones actuales como posici�n y dimensiones antiguas
	sprite->preparaParaCambio();

	int oriEntrada = elMotorGrafico->ajustaOrientacionSegunCamara(ARRIBA);

	sprite->posXLocal += despOrientacion[oriEntrada][2];
	sprite->posYLocal += despOrientacion[oriEntrada][3];

	int oriPuerta = elMotorGrafico->ajustaOrientacionSegunCamara(orientacion);
	sprite->posXPant = posXPant + despOrientacion[oriEntrada][0] + despOrientacion[oriPuerta][4];
	sprite->posYPant = posYPant + despOrientacion[oriEntrada][1] + despOrientacion[oriPuerta][5];
	sprite->profundidad = profundidad + despOrientacion[oriPuerta][6];
	sprite->haCambiado = elMotorGrafico->hayQueRedibujar | hayQueRedibujar;
	sprite->esVisible = true;
	// CPC sprite->despGfx = 0x0aa49 + 0xc000*despOrientacion[oriPuerta][7];
	sprite->despGfx = 69708 + 120305*despOrientacion[oriPuerta][7]; // VGA
	// 69708 es la posicion de la puerta dentro de GraficosVGA
	// 120305 es el desplazamiento entre el grafico de la puerta y su
	// grafico "flipeado"
	sprite->posXLocal += despOrientacion[oriPuerta][8];
	sprite->posYLocal += despOrientacion[oriPuerta][9];

	// marca las posiciones ocupadas por la puerta en el buffer de alturas
	marcaPosiciones(elMotorGrafico->rejilla, 0x0f);
}

/////////////////////////////////////////////////////////////////////////////
// m�todos para abrir y cerrar las puertas
/////////////////////////////////////////////////////////////////////////////

// comprueba si hay que abrir o cerrar una puerta
void Puerta::compruebaAbrirCerrar(Personaje **personajes, int numPersonajes)
{
	// si la puerta est� fija, sale
	if (estaFija) return;

	int mascara = (laLogica->mascaraPuertas | 0x10) & identificador;

	// TODO: si guillermo o adso est�n cerca de la puerta y no tienen permisos para abrirla, se cierra

	for (int i = 0; i < numPersonajes; i++){
		// si tiene permisos para entrar en esa puerta
		if ((personajes[i]->permisosPuertas & mascara) != 0){
			if (puedeAbrir(personajes[i])){
				return;
			}
		}
	}

	// aqu� llega si la puerta no se ha abierto

	// ejecuta las acciones correspondientes para cerrar la puerta
	accionesAbrirCerrar(false);
}

// comprueba si el personaje est� cerca de la puerta y si es as�, si se puede abrir
bool Puerta::puedeAbrir(Personaje *pers)
{
	int difX = pers->posX - (posX - 1);
	// si no est� cerca de la puerta en x, sale
	if ((difX < 0) || (difX >= 4)) return false;

	int difY = pers->posY - (posY - 1);
	// si no est� cerca de la puerta en y, sale
	if ((difY < 0) || (difY >= 4)) return false;

	// ejecuta las acciones correspondientes para abrir la puerta
	return accionesAbrirCerrar(true);
}

// ejecuta las acciones correspondientes para abrir o cerrar la puerta
bool Puerta::accionesAbrirCerrar(bool abrir)
{
	if (abrir){
		// si la puerta est� abierta, sale
		if (estaAbierta) return true;
		// si lo ponemos aqui, suena aunque la puerta no este en la pantalla actual
		//VigasocoMain->getAudioPlugin()->Play(1,false);
	} else {
		// si la puerta est� cerrada, sale
		if (!estaAbierta) return true;
		//VigasocoMain->getAudioPlugin()->Play(2,false);
	}

	// marca la puerta como abierta o cerrada e indica que hay que redibujarla
	estaAbierta = abrir;
	hayQueRedibujar = true;

	// pone a 0 la altura de las posiciones ocupadas por la puerta cuando estaba abierta o cerrada
	marcaPosiciones(elMotorGrafico->rejilla, altura);

	int difOri = (abrir) ? 1 : -1;

	// cambia la orientaci�n de la puerta dependiendo de hacia donde se abra o se cierre
	orientacion = (haciaDentro) ? (orientacion - difOri) & 0x03 : (orientacion + difOri) & 0x03;

	int posXRejilla, posYRejilla;
	RejillaPantalla *rejilla = elMotorGrafico->rejilla;

	// si la puerta est� en las 20x20 posiciones centrales de la rejilla
	if (rejilla->estaEnRejillaCentral(this, posXRejilla, posYRejilla)){
		posXRejilla += 2*despOrientacion[orientacion][10];
		posYRejilla += 2*despOrientacion[orientacion][11];

		// si no hay un personaje en la posici�n en la que se abre o cierra la puerta, sale
		//if ((rejilla->bufAlturas[posYRejilla][posXRejilla] & 0xf0) == 0) return true;
		if ((rejilla->bufAlturas[posYRejilla][posXRejilla] & 0xf0) == 0)
		{
			if (abrir)
			{
			  //VigasocoMain->getAudioPlugin()->Play(SONIDOS::Abrir);
				sys->playSound(OPEN);
			}
			else
			{
			  //VigasocoMain->getAudioPlugin()->Play(SONIDOS::Cerrar);
				sys->playSound(CLOSE);
			}
			return true;
		}

		// en otro caso, deshace el cambio
		estaAbierta = !abrir;
		hayQueRedibujar = false;
		orientacion = (haciaDentro) ? (orientacion + difOri) & 0x03 : (orientacion - difOri) & 0x03;

		marcaPosiciones(elMotorGrafico->rejilla, altura);
	}	

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// m�todos de ayuda
/////////////////////////////////////////////////////////////////////////////

// marca la altura de las posiciones ocupadas por la puerta
void Puerta::marcaPosiciones(RejillaPantalla *rejilla, int valor)
{
	int posXRejilla, posYRejilla;

	// si la puerta est� en las 20x20 posiciones centrales de la rejilla, marca las posiciones que ocupa
	if (rejilla->estaEnRejillaCentral(this, posXRejilla, posYRejilla)){
		for (int i = 0; i < 3; i++){
			rejilla->bufAlturas[posYRejilla][posXRejilla] = valor;
			posXRejilla += despOrientacion[orientacion][10];
			posYRejilla += despOrientacion[orientacion][11];
		}
	}
}

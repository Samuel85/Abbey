// AccionesDia.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "cpc6128.h"

#include "Abad.h"
#include "AccionesDia.h"
#include "Berengario.h"
#include "Bernardo.h"
#include "Guillermo.h"
#include "Jorge.h"
#include "Juego.h"
#include "Logica.h"
#include "Marcador.h"
#include "MotorGrafico.h"
#include "Objeto.h"
#include "Paleta.h"
#include "Puerta.h"

#include "sonidos.h"
#include "system.h"

using namespace Abadia;

// tabla con las acciones programadas
AccionProgramada *AccionesDia::acciones[7];

/////////////////////////////////////////////////////////////////////////////
// inicializaci?n y limpieza
/////////////////////////////////////////////////////////////////////////////

AccionesDia::AccionesDia()
{
	// crea las acciones programadas seg?n el momento del d?a
	acciones[0] = new AccionesNoche();
	acciones[1] = new AccionesPrima();
	acciones[2] = new AccionesTercia();
	acciones[3] = new AccionesSexta();
	acciones[4] = new AccionesNona();
	acciones[5] = new AccionesVisperas();
	acciones[6] = new AccionesCompletas();
}

AccionesDia::~AccionesDia()
{
	for (int i = 0; i < 7; i++){
		delete acciones[i];
	}
}

/////////////////////////////////////////////////////////////////////////////
// ejecuci?n de las acciones programadas
/////////////////////////////////////////////////////////////////////////////

void AccionesDia::ejecutaAccionesProgramadas()
{
	// si no ha cambiado el momento del d?a, sale
	if (laLogica->momentoDia == laLogica->oldMomentoDia) return;

	laLogica->oldMomentoDia = laLogica->momentoDia;

	laLogica->cntMovimiento = 0;

	// ejecuta unas acciones dependiendo del momento del d?a
	acciones[laLogica->momentoDia]->ejecuta(this);
}

/////////////////////////////////////////////////////////////////////////////
// acciones programadas seg?n el momento del d?a
/////////////////////////////////////////////////////////////////////////////

void AccionesNoche::ejecuta(AccionesDia *ad)
{
	if (laLogica->dia == 5){
		// pone las gafas en la habitaci?n iluminada del laberinto
		ad->colocaObjeto(elJuego->objetos[2], 0x1b, 0x23, 0x18);

		// pone la llave 1 en el altar
		ad->colocaObjeto(elJuego->objetos[4], 0x89, 0x3e, 0x08);
	} else if (laLogica->dia == 6){
		// pone la llave de la habitaci?n de severino en la mesa de malaqu?as
		ad->colocaObjeto(elJuego->objetos[5], 0x35, 0x35, 0x13);

		// coloca a jorge en la habitaci?n de detr?s del espejo
		ad->colocaPersonaje(laLogica->jorge, 0x12, 0x65, 0x18, ARRIBA);
		laLogica->jorge->estaActivo = true;
	}
}

void AccionesPrima::ejecuta(AccionesDia *ad)
{
	// dibuja el efecto de la espiral
	ad->dibujaEfectoEspiral();

	// modifica las puertas que pueden abrirse
	laLogica->mascaraPuertas = 0xef;

	// fija la paleta de d?a
	elJuego->paleta->setGamePalette(2);

	// abre las puertas del ala izquierda de la abad?a
	elJuego->puertas[5]->orientacion = IZQUIERDA;
	elJuego->puertas[5]->haciaDentro = true;
	elJuego->puertas[5]->estaFija = true;
	elJuego->puertas[5]->estaAbierta = true;
	elJuego->puertas[6]->orientacion = IZQUIERDA;
	elJuego->puertas[6]->haciaDentro = false;
	elJuego->puertas[6]->estaFija = true;
	elJuego->puertas[6]->estaAbierta = true;

	//VigasocoMain->getAudioPlugin()->Play(SONIDOS::Campanas);
	sys->playSound(BELLS);
	
	if (laLogica->dia >= 3){
		// si se ha usado la l?mpara, desaparece
		laLogica->reiniciaContadoresLampara();

		// si la l?mpara hab?a desaparecido, la pone en la cocina
		if (laLogica->lamparaDesaparecida){
			laLogica->lamparaDesaparecida = false;

			ad->colocaObjeto(elJuego->objetos[7], 0x5a, 0x2a, 0x04);
		}
	}

	if (laLogica->dia == 2){
		// desaparecen las gafas
		laLogica->guillermo->objetos &= 0xdf;
		laLogica->berengario->objetos &= 0xdf;

		ad->colocaObjeto(elJuego->objetos[2], 0, 0, 0);

		// dibuja los objetos que tiene guillermo en el marcador
		elMarcador->dibujaObjetos(laLogica->guillermo->objetos, 0xff);
	}

	if (laLogica->dia == 3){
		// jorge coge el libro y lo esconde
		laLogica->jorge->objetos = LIBRO;
		ad->colocaObjeto(elJuego->objetos[0], 0x0f, 0x2e, 0x00);

		// escribe un comando para pensar un nuevo movimiento
		laLogica->jorge->numBitAcciones = 0;
		laLogica->jorge->posAcciones = 0;
		laLogica->jorge->bufAcciones[0] = 0x10;

		// coloca a jorge al final del pasillo de las celdas de los monjes
		ad->colocaPersonaje(laLogica->jorge, 0xc8, 0x24, 0x00, DERECHA);
		laLogica->jorge->estaActivo = true;

		// indica que el abad no tiene ning?n objeto
		laLogica->abad->objetos = 0;

		// si guillermo no tiene el pergamino, se coloca en la habitaci?n de detr?s del espejo
		if ((laLogica->guillermo->objetos & PERGAMINO) == 0){
			ad->colocaObjeto(elJuego->objetos[3], 0x18, 0x64, 0x18);
			laLogica->pergaminoGuardado = true;
		}
	}

	// si es el quinto d?a y no tenemos la llave 1, ?sta desaparece
	if ((laLogica->dia == 5) && ((laLogica->guillermo->objetos & LLAVE1) == 0)){
		ad->colocaObjeto(elJuego->objetos[4], 0, 0, 0);
	}
}

void AccionesTercia::ejecuta(AccionesDia *ad)
{
	// dibuja el efecto de la espiral
	ad->dibujaEfectoEspiral();

	//VigasocoMain->getAudioPlugin()->Play(SONIDOS::Tintineo);
	sys->playSound(JINGLE);
}

void AccionesSexta::ejecuta(AccionesDia *ad)
{
	//VigasocoMain->getAudioPlugin()->Play(SONIDOS::Campanas);
	sys->playSound(BELLS);

	if (laLogica->dia == 4){
		// bernardo gui aparece en las escaleras de la abad?a
		laLogica->bernardo->estaEnLaAbadia = true;
		ad->colocaPersonaje(laLogica->bernardo, 0x88, 0x88, 0x02, DERECHA);

		// indica que bernardo puede coger el pergamino
		laLogica->bernardo->mascaraObjetos = PERGAMINO;
	}
}

void AccionesNona::ejecuta(AccionesDia *ad)
{
	// dibuja el efecto de la espiral
	ad->dibujaEfectoEspiral();

	// si es el tercer d?a, jorge pasa a estar inactivo y desaparece
	if (laLogica->dia == 3){
		laLogica->jorge->estaActivo = false;
		laLogica->jorge->posX = laLogica->jorge->posY = laLogica->jorge->altura = 0; 
	}
	sys->playSound(JINGLE);
}

void AccionesVisperas::ejecuta([[maybe_unused]] AccionesDia *ad)
{
	sys->playSound(BELLS);
}

void AccionesCompletas::ejecuta(AccionesDia *ad)
{
	// dibuja el efecto de la espiral
	ad->dibujaEfectoEspiral();

	// fija la paleta de noche
	elJuego->paleta->setGamePalette(3);

	// modifica las puertas que pueden abrirse
	laLogica->mascaraPuertas = 0xdf;

	sys->playSound(JINGLE);
}

/////////////////////////////////////////////////////////////////////////////
// dibujo de la espiral
/////////////////////////////////////////////////////////////////////////////

// genera el efecto de la espiral
void AccionesDia::dibujaEfectoEspiral()
{
	dibujaEspiral(3);	// dibuja la espiral
	dibujaEspiral(0);	// borra la espiral

	// indica un cambio de pantalla
	elMotorGrafico->posXPantalla = elMotorGrafico->posYPantalla = -1;
}

// dibuja una espiral cuadrada del color que se le pasa
void AccionesDia::dibujaEspiral(int color)
{
	// fija la posici?n inicial
	int posX = 0;
	int posY = 0;

	// fija la longitud de las tiras
	int derecha = 0x3f;
	int abajo = 0x4f;
	int izquierda = 0x3f;
	int arriba = 0x4e;

	int colorAUsar = 0;

	// milisegundos que esperar para ver el efecto
	int retardo = 4;

	// repite 32 veces
	for (int i = 0; i < 32; i++){
		int num = derecha;
		derecha -= (i == 0) ? 1 : 2;

		// dibuja una tira (de izquierda a derecha) del ancho indicado por derecha
		for (int j = 0; j < num; j++){
			dibujaBloque(posX, posY, colorAUsar);
			posX++;
		}

		// espera un poco para que se vea el resultado
		//timer->sleep(retardo);

		num = abajo;
		abajo -=2;

		// dibuja una tira (de arriba a abajo) del alto indicado por abajo
		for (int j = 0; j < num; j++){
			dibujaBloque(posX, posY, colorAUsar);
			posY += 2;
		}

		// espera un poco para que se vea el resultado
		//timer->sleep(retardo);

		num = izquierda;
		izquierda -= 2;

		// dibuja una tira (de derecha a izquierda) del ancho indicado por izquierda
		for (int j = 0; j < num; j++){
			dibujaBloque(posX, posY, colorAUsar);
			posX--;
		}

		// espera un poco para que se vea el resultado
		//timer->sleep(retardo);

		num = arriba;
		arriba -= 2;

		// dibuja una tira (de abajo a arriba) del alto indicado por arriba
		for (int j = 0; j < num; j++){
			dibujaBloque(posX, posY, colorAUsar);
			posY -= 2;
		}

		// espera un poco para que se vea el resultado
		//timer->sleep(retardo);

		// invierte el color a usar
		colorAUsar = colorAUsar ^ color;

		if ((i != 0) && ((i % 8) == 0)){
			retardo--;
		}
	}

	dibujaBloque(posX, posY, colorAUsar);
}

// dibuja un bloque de 4x8 del color que se le pasa
void AccionesDia::dibujaBloque(int posX, int posY, int color)
{
	for (int i = 0; i < 4; i++){
		elJuego->cpc6128->setMode1Pixel(32 + posX*4 + i, posY, color);
		elJuego->cpc6128->setMode1Pixel(32 + posX*4 + i, posY + 1, color);
	}
}

/////////////////////////////////////////////////////////////////////////////
// m?todo de ayuda para colocar los objetos y los personajes
/////////////////////////////////////////////////////////////////////////////

void AccionesDia::colocaObjeto(Objeto *obj, int posX, int posY, int altura)
{
	obj->seHaCogido = false;
	obj->seEstaCogiendo = false;
	obj->personaje = 0;
	obj->posX = posX;
	obj->posY = posY;
	obj->altura = altura;
	obj->orientacion = DERECHA;
}

void AccionesDia::colocaPersonaje(Personaje *pers, int posX, int posY, int altura, int orientacion)
{
	pers->posX = posX;
	pers->posY = posY;
	pers->altura = altura;
	pers->orientacion = orientacion;
	pers->enDesnivel = false;
	pers->giradoEnDesnivel = false;
	pers->bajando = false;
}

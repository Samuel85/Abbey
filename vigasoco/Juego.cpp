///Juego.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include <string>

#include "cpc6128.h"
#include "InputHandler.h"
#include "IPalette.h"

#include "Vigasoco.h"

#include "Abad.h"
#include "Adso.h"
#include "Berengario.h"
#include "Bernardo.h"
#include "BuscadorRutas.h"
#include "Controles.h"
#include "GestorFrases.h"
#include "Guillermo.h"
#include "InfoJuego.h"
#include "Jorge.h"
#include "Juego.h"
#include "Logica.h"
#include "Malaquias.h"
#include "Marcador.h"
#include "Monje.h"
#include "MotorGrafico.h"
#include "Objeto.h"
#include "Paleta.h"
#include "Pergamino.h"
#include "Personaje.h"
#include "PersonajeConIA.h"
#include "Puerta.h"
#include "RejillaPantalla.h"
#include "Severino.h"
#include "Sprite.h"
#include "SpriteLuz.h"
#include "SpriteMonje.h"

#include "Serializar.h"

#include "sonidos.h"
#include <iostream>
// memcpy
#include <string.h>
#include "system.h"

#include "texts.h"
#include <SDL_ttf.h>
using namespace Abadia;



const char *Juego::savefile[7] = {
	 "abadia0.save",
	 "abadia1.save",
	 "abadia2.save",
	 "abadia3.save",
	 "abadia4.save",
	 "abadia5.save",
	 "abadia6.save"
};

/////////////////////////////////////////////////////////////////////////////
// inicializaci??n y limpieza
/////////////////////////////////////////////////////////////////////////////

Juego::Juego(UINT8 *romData, CPC6128 *cpc)
{		
	idioma=0; // 0 espa??ol
	mute=false; 
	slot=0;
	GraficosCPC=false;
	// apunta a los datos del juego, pero salt??ndose la de la presentaci??n
	roms = romData + 0x4000;

	cpc6128 = cpc;

	// inicia los sprites del juego
	for (int i = 0; i < numSprites; i++){
		sprites[i] = 0;
	}

	// inicia los personajes del juego
	for (int i = 0; i < numPersonajes; i++){
		personajes[i] = 0;
	}

	// inicia las puertas del juego
	for (int i = 0; i < numPuertas; i++){
		puertas[i] = 0;
	}

	// inicia los objetos del juego
	for (int i = 0; i < numObjetos; i++){
		objetos[i] = 0;
	}

	//timer = 0;

	// crea los objetos principales que usar?? el juego
	//paleta = new Paleta();
	paleta = new Paleta(romData+0x24000-1); // le pasamos los datos de la paleta VGA
	pergamino = new Pergamino();
	motor = new MotorGrafico(buffer, 8192);
	marcador = new Marcador();
	logica = new Logica(roms, buffer, 8192); 
	infoJuego = new InfoJuego();
	controles = new Controles();

	pausa = false;
	modoInformacion = false;
	seleccionado = 0;
	
	currentState = INTRO;
	showingMenu = false;
	firstTime = true;
}

Juego::~Juego()
{
	// borra los sprites del juego
	for (int i = 0; i < numSprites; i++){
		delete sprites[i];
	}

	// borra los personajes del juego
	for (int i = 0; i < numPersonajes; i++){
		delete personajes[i];
	}

	// borra las puertas del juego
	for (int i = 0; i < numPuertas; i++){
		delete puertas[i];
	}

	// borra los objetos del juego
	for (int i = 0; i < numObjetos; i++){
		delete objetos[i];
	}

	delete infoJuego;
	delete logica;
	delete marcador;
	delete motor;
	delete pergamino;
	delete paleta;
	delete controles;
}

void Juego::ReiniciaPantalla(void)
{
	// limpia el ??rea de juego y dibuja el marcador
	// CPC limpiaAreaJuego(0);
	limpiaAreaJuego(12); // el 0 es el cyan en CPC, no se cual poner en VGA
	// pongo el 12 que es un amarillo cantoso, para comparar con Abadia32

	marcador->dibujaMarcador();

	// inicia el contador de la interrupci??n
	contadorInterrupcion = 0;

	// pone una posici??n de pantalla inv??lida para que se redibuje la pantalla
	motor->posXPantalla = motor->posYPantalla = -1;

	// dibuja los objetos que tiene guillermo en el marcador
	marcador->dibujaObjetos(personajes[0]->objetos, 0xff);

	// inicia el marcador (d??a y momento del d??a, obsequium y el espacio de las frases)
	marcador->muestraDiaYMomentoDia();
	marcador->decrementaObsequium(0);
	marcador->limpiaAreaFrases();
}

void Juego::pintaMenuCargar(int seleccionado,bool efecto)
{
	pintaMenuGrabar(seleccionado,efecto);
}

bool Juego::menuCargar2()
{
	pintaMenuCargar(seleccionado,true);
	int i = 0;	
	if (sys->pad.up)
	{
		i--;
		sys->pad.up = false;
	}		
	else if (sys->pad.down)
	{
		i++;
		sys->pad.down = false;
	}				
	seleccionado += i;
	if (seleccionado > 7)
	{
		seleccionado = 0;
	}
	else if (seleccionado < 0)
	{
		seleccionado = 7;
	}	
	pintaMenuCargar(seleccionado,true);
	
	if (sys->pad.button1)
	{
		sys->pad.button1 = false;
		if (seleccionado != 7)
		{
			laLogica->inicia();			
			cargar(seleccionado);			
			changeState(PLAY);
			ReiniciaPantalla();
			firstTime = false;
		}
		else
		{
			seleccionado = 8;
			changeState(MENU);
		}		
	}
	return false;
}

void Juego::pintaMenuGrabar(int seleccionado,bool efecto)
{
	// limpia el ??rea que ocupa el marcador
	limpiaAreaJuego(0); 
	marcador->limpiaAreaMarcador();	

	//repintar con un efecto para que vaya apareciendo el
	// menu de izquierda a derecha y asi dar tiempo a soltar las teclas
	// al usuario

	// repinta todo el menu
	for(int x=efecto?8:88;x<88;x+=10)
	{
		cpc6128->fillMode1Rect(8, 0, x-1, 160, 0);
		for (int i=0;i<8;i++)
		{
			marcador->imprimeFrase(textSave[idioma][i], x, 32+(i*16),4, 0);
		}		
	}
	cpc6128->fillMode1Rect(8, 0, 88, 160, 0);
	for (int i=0;i<8;i++)
	{
		marcador->imprimeFrase(textSave[idioma][i], 88, 32+(i*16),4, 0);
	}

	// pinta la opci??n seleccionado con el color de fondo y el color
	// de letra cambiado 
	marcador->imprimeFrase(textSave[idioma][seleccionado], 88, 
		32+(seleccionado*16), 0, 4);
	
}
bool Juego::menuGrabar2()
{
	pintaMenuGrabar(seleccionado,true);
	int i = 0;	
	if (sys->pad.up)
	{
		i--;
		sys->pad.up = false;
	}		
	else if (sys->pad.down)
	{
		i++;
		sys->pad.down = false;
	}				
	seleccionado += i;
	if (seleccionado > 7)
	{
		seleccionado = 0;
	}
	else if (seleccionado < 0)
	{
		seleccionado = 7;
	}	
	pintaMenuCargar(seleccionado,true);	
	if (sys->pad.button1)
	{
		sys->pad.button1 = false;
		if (seleccionado != 7)
		{			
			save(seleccionado);						
			changeState(PLAY);
			ReiniciaPantalla();
			return true;
		}
		else
		{
			seleccionado = 8;
			changeState(MENU);
		}
	}
	return false;	
}

bool Juego::menuIntroduccion()
{
	limpiaAreaJuego(0);
	pergamino->muestraTexto(Pergamino::pergaminoIntroduccion[idioma]);

	// espera a que se suelte el bot??n
	bool espera = true;

	while (espera)
	{
		controles->actualizaEstado();		
		espera = controles->estaSiendoPulsado(P1_BUTTON1);
	}

	// Es necesario ya que el pergamino 
	// cambia la paleta
	marcador->limpiaAreaMarcador();
	ReiniciaPantalla();

	while (	losControles->estaSiendoPulsado(P1_BUTTON1) ||
			losControles->estaSiendoPulsado(KEYBOARD_INTRO) ) {
		losControles->actualizaEstado();
		//timer->sleep(50);
	}

	return false;
}

void Juego::pintaMenuMejoras(int seleccionado)
{

	// limpia el ??rea que ocupa el marcador
	limpiaAreaJuego(0); 

	for (int i=0;i<9;i++)
	{
		marcador->imprimeFrase(textEnhancements[idioma][i], 8, 16+(i*16),4, 0);
	}

	// pinta la opci??n seleccionado con el color de fondo y el color
	// de letra cambiado 
	marcador->imprimeFrase(textEnhancements[idioma][seleccionado], 8, 
		16+(seleccionado*16), 0, 4);
	
}

bool Juego::menuMejoras()
{
	limpiaAreaJuego(0);

	pintaMenuMejoras(0);
	{
		bool salir=false;
		while(salir==false) {	
			//timer->sleep(100);
			losControles->actualizaEstado();

			if (losControles->estaSiendoPulsado(P1_BUTTON1) ||
			    losControles->estaSiendoPulsado(KEYBOARD_INTRO) ) {
				salir=true;
			}
		}
	}

	return false;
}

void Juego::pintaMenuCamaras(int seleccionado)
{

	// limpia el ??rea que ocupa el marcador
	limpiaAreaJuego(0); 

	for (int i=0;i<9;i++)
	{
		marcador->imprimeFrase(textCameras[idioma][i], 0, 16+(i*16),4, 0);
	}

	// pinta la opci??n seleccionado con el color de fondo y el color
	// de letra cambiado 
	marcador->imprimeFrase(textCameras[idioma][seleccionado], 1, 
		16+(seleccionado*16), 0, 4);
	
}

bool Juego::menuCamaras()
{
	limpiaAreaJuego(0);

	pintaMenuCamaras(0);
	{
		bool salir=false;
		while(salir==false) {	
			//timer->sleep(100);
			losControles->actualizaEstado();

			if (losControles->estaSiendoPulsado(P1_BUTTON1) ||
			    losControles->estaSiendoPulsado(KEYBOARD_INTRO) ) {
				salir=true;
			}
		}
	}

	while (	losControles->estaSiendoPulsado(P1_BUTTON1) ||
		losControles->estaSiendoPulsado(KEYBOARD_INTRO) ) {
		losControles->actualizaEstado();
		//timer->sleep(50);
	}

	return false;
}

void Juego::pintaMenuIdioma(int seleccionado,bool efecto)
{

	// limpia el ??rea que ocupa el marcador
	limpiaAreaJuego(0); 

	// repintar con un efecto para que vaya apareciendo el
	// menu de izquierda a derecha y asi dar tiempo a soltar las teclas
	// al usuario

	// repinta todo el menu
	for(int x=efecto?8:88;x<88;x+=10)
	{
		cpc6128->fillMode1Rect(8, 0, x-1, 160, 0);
		for (int i=0;i<8;i++)
		{
			marcador->imprimeFrase(textLanguage[idioma][i], x, 
				32+(i*16),4, 0);
		}
		//timer->sleep(50);
	}
	cpc6128->fillMode1Rect(8, 0, 88, 160, 0);
	for (int i=0;i<8;i++)
	{
		marcador->imprimeFrase(textLanguage[idioma][i], 88, 32+(i*16),4, 0);
	}

	// pinta la opci??n seleccionado con el color de fondo y el color
	// de letra cambiado 
	marcador->imprimeFrase(textLanguage[idioma][seleccionado], 88, 
		32+(seleccionado*16), 0, 4);
	
}

bool Juego::menuIdioma()
{
	int seleccionado=idioma;
	int pulsado=-1;

	limpiaAreaJuego(0);

	// pruebas menu elegir idioma
	pintaMenuIdioma(seleccionado,true);
	{
		bool salir=false;
		while(salir==false) {	
			pulsado=-1;
			//timer->sleep(100);
			losControles->actualizaEstado();

			if (losControles->estaSiendoPulsado(P1_DOWN)) {
				seleccionado++;
				if (seleccionado==8) seleccionado=0;
				pintaMenuIdioma(seleccionado);
			}
			if (losControles->estaSiendoPulsado(P1_UP)) {
				seleccionado--;
				if (seleccionado==-1) seleccionado=7;
				pintaMenuIdioma(seleccionado);
			}
			if (losControles->estaSiendoPulsado(P1_BUTTON1) ||
			    losControles->estaSiendoPulsado(KEYBOARD_INTRO) ) {
				pulsado=seleccionado;
			}
			
			if (losControles->estaSiendoPulsado(KEYBOARD_0) ||
				pulsado==0 )
			{
				idioma=0;
				salir=true;
			} 
			if (losControles->estaSiendoPulsado(KEYBOARD_1) ||
				pulsado==1 )
			{
				idioma=1;
				salir=true;
			}
			if (losControles->estaSiendoPulsado(KEYBOARD_2) ||
				pulsado==2 )
			{
				idioma=2;
				salir=true;
			}
			if (losControles->estaSiendoPulsado(KEYBOARD_3) ||
				pulsado==3 )
			{
				idioma=3;
				salir=true;
			}
			if (losControles->estaSiendoPulsado(KEYBOARD_4) ||
				pulsado==4 )
			{
				idioma=4;
				salir=true;
			}
			if (losControles->estaSiendoPulsado(KEYBOARD_5) ||
				pulsado==5 )
			{
				idioma=5;
				salir=true;
			}
			if (losControles->estaSiendoPulsado(KEYBOARD_6) ||
				pulsado==6 )
			{
				idioma=6;
				salir=true;
			}
			if (losControles->estaSiendoPulsado(KEYBOARD_7) ||
				pulsado==7 )
			{
				idioma=7;
				salir=true;
			} 
		}
	
	}

	return false;
}


void Juego::pintaMenuPrincipal(int seleccionado,bool efecto)
{

	// limpia el ??rea que ocupa el marcador
	limpiaAreaJuego(0); 

	// repinta todo el menu
	for(int x=efecto?8:88;x<88;x+=10)
	{
		cpc6128->fillMode1Rect(8, 0, x-1, 160, 0);
		for (int i=0;i<9;i++)
		{
			marcador->imprimeFrase(menuText[idioma][i], x, 
				16+(i*16),4, 0);
		}
	}

	cpc6128->fillMode1Rect(8, 0, 88, 160, 0);
	for (int i=0;i<9;i++)
	{
		marcador->imprimeFrase(menuText[idioma][i], 88, 16+(i*16),4, 0);
	}

	// pinta la opci??n seleccionado con el color de fondo y el color
	// de letra cambiado 
	marcador->imprimeFrase(menuText[idioma][seleccionado], 88, 
		16+(seleccionado*16), 0, 4);
}

bool Juego::menu2()
{	
	pintaMenuPrincipal(seleccionado,true);		
	int i = 0;	
	
	if (sys->pad.up)
	{
		i--;
		sys->pad.up = false;
	}		
	else if (sys->pad.down)
	{
		i++;
		sys->pad.down = false;
	}				
	seleccionado += i;
	if (seleccionado > 8)
		seleccionado = 0;	
	else if (seleccionado < 0)
		seleccionado = 8;

	pintaMenuPrincipal(seleccionado,true);	
	if (sys->pad.button1)
	{
		sys->pad.button1 = false;
		switch(seleccionado)
		{
			case 0: //IDIOMA
				break;
			case 1: //CARGAR
				//currentState = LOAD;
				changeState(LOAD);
				ReiniciaPantalla();
				paleta->setGamePalette(2);		
				marcador->limpiaAreaMarcador();	
				ReiniciaPantalla();
				sys->pad.button1 = false;
				contadorInterrupcion = 0;
				break;
			case 2: //GRABAR
				if (!firstTime)
				{
					//currentState = SAVE;
					changeState(SAVE);
					ReiniciaPantalla();				
					paleta->setGamePalette(2);		
					marcador->limpiaAreaMarcador();	
					ReiniciaPantalla();
					sys->pad.button1 = false;
					contadorInterrupcion = 0;
				}
				break;
			case 3: //Graficos VGA-CPC
				break;
			case 4: //AYUDA
				break;
			case 5: //TUTORIAL
				break;
			case 6: //reiniciar
				break;
			case 7: // SONIDO
				break;			
			case 8: //JUGAR
				if (firstTime)
				{				
					//currentState = SCROLL;
					changeState(SCROLL);
					ReiniciaPantalla();
					paleta->setGamePalette(2);		
					marcador->limpiaAreaMarcador();	
					ReiniciaPantalla();
					sys->pad.button1 = false;
					contadorInterrupcion = 0;
					sys->minimumFrameTime = SCROLL_FRAME_TIME;
					sys->playMusic(START);	
				}
				else
				{					
					changeState(PLAY);
					ReiniciaPantalla();
				}		
				break;			
		}
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////
// m??todo principal del juego
/////////////////////////////////////////////////////////////////////////////
void Juego::preRun()
{	
	marcador->limpiaAreaMarcador();

	// crea las entidades del juego (sprites, personajes, puertas y objetos)
	creaEntidadesJuego();
	generaGraficosFlipeados();
	motor->personaje = personajes[0];
	infoJuego->inicia();
	logica->despHabitacionEspejo();
	logica->inicia();

	marcador->limpiaAreaMarcador();
	logica->inicia();
	ReiniciaPantalla();
}

void Juego::changeState(int newState)
{
	ReiniciaPantalla();
	marcador->limpiaAreaMarcador();
	currentState = newState;
}

void Juego::stateMachine()
{
	switch (currentState)
	{
		case INTRO:			
			muestraPresentacion();
			break;
		case MENU:
			menu2();
			break;
		case LOAD:
			menuCargar2();
			break;
		case SAVE:
			menuGrabar2();
			break;		
		case SCROLL:
			muestraIntroduccion();
			break;
		case PLAY:
			run();
			break;
		case ENDING:
			muestraFinal();
			break;
	}
}

void Juego::run()
{	
		elBuscadorDeRutas->contadorAnimGuillermo = laLogica->guillermo->contadorAnimacion;
		
		logica->compruebaAbreEspejo();
		
		compruebaPausa();
		
		compruebaSave();
	
		logica->actualizaVariablesDeTiempo();

		// si guillermo ha muerto, empieza una partida
		if (muestraPantallaFinInvestigacion()) return;		
		
		logica->compruebaLecturaLibro();	
		
		marcador->realizaScrollMomentoDia();		
		
		logica->ejecutaAccionesMomentoDia();
		
		logica->compruebaBonusYCambiosDeCamara();
		
		motor->compruebaCambioPantalla();
		
		logica->compruebaCogerDejarObjetos();

		logica->compruebaAbrirCerrarPuertas();
				
		for (int i = 0; i < numPersonajes; i++){
			personajes[i]->run();
		}
		
		logica->buscRutas->generadoCamino = false;	
		
		actualizaLuz();

		// si guillermo o adso est??n frente al espejo, muestra su reflejo
		laLogica->realizaReflejoEspejo();
		
		if (cambioModoInformacion && modoInformacion)
		{
			limpiaAreaJuego(12);			
			cambioModoInformacion=false;
		}

		if (cambioModoInformacion && !modoInformacion )
		{
			limpiaAreaJuego(12);
			motor->compruebaCambioPantalla(true);	
			cambioModoInformacion=false;			
		}

		if (modoInformacion)
		{
			infoJuego->muestraInfo();
		} 
		else
		{		
			motor->dibujaPantalla();
			motor->dibujaSprites();				
		}
	
		if (laLogica->guillermo->contadorAnimacion==1)
		{
			//audio_plugin->Play(SONIDOS::Pasos);
			sys->playSound(STEPS);
		}
	
}

void Juego::updateScreen(){
	//motor->dibujaPantalla();
	//motor->dibujaSprites();	
}

// limpia el ??rea de juego de color que se le pasa y los bordes de negro
void Juego::limpiaAreaJuego(int color)
{
	// VGA
	cpc6128->fillMode1Rect(0, 0, 32, 160, 0);
	cpc6128->fillMode1Rect(32, 0, 256, 160, color);
	cpc6128->fillMode1Rect(32 + 256, 0, 32, 160, 0);	
}


// flipea respecto a x todos los gr??ficos del juego que lo necesiten
void Juego::generaGraficosFlipeados()
{
	generaGraficosFlipeadosVGA(); //  VGA

	UINT8 tablaFlipX[256];

	// inicia la tabla para flipear los gr??ficos
	for (int i = 0; i < 256; i++)
	{
		// extrae los pixels
		int pixel0 = cpc6128->unpackPixelMode1(i, 0);
		int pixel1 = cpc6128->unpackPixelMode1(i, 1);
		int pixel2 = cpc6128->unpackPixelMode1(i, 2);
		int pixel3 = cpc6128->unpackPixelMode1(i, 3);

		int data = 0;

		// combina los pixels en orden inverso
		data = cpc6128->packPixelMode1(data, 0, pixel3);
		data = cpc6128->packPixelMode1(data, 1, pixel2);
		data = cpc6128->packPixelMode1(data, 2, pixel1);
		data = cpc6128->packPixelMode1(data, 3, pixel0);

		// guarda el resultado
		tablaFlipX[i] = data;
	}

	// genera los gr??ficos de las animaciones de guillermo flipeados respecto a x
	flipeaGraficos(tablaFlipX, &roms[0x0a300], &roms[0x16300], 5, 0x366);
	flipeaGraficos(tablaFlipX, &roms[0x0a666], &roms[0x16666], 4, 0x084);

	// genera los gr??ficos de las animaciones de adso flipeados respecto a x
	flipeaGraficos(tablaFlipX, &roms[0x0a6ea], &roms[0x166ea], 5, 0x1db);
	flipeaGraficos(tablaFlipX, &roms[0x0a8c5], &roms[0x168c5], 4, 0x168);

	// genera los gr??ficos de los trajes de los monjes flipeados respecto a x
	flipeaGraficos(tablaFlipX, &roms[0x0ab59], &roms[0x16b59], 5, 0x2d5);

	// genera los gr??ficos de las caras de los monjes flipeados respecto a x
	flipeaGraficos(tablaFlipX, &roms[0x0b103], &roms[0x17103], 5, 0x2bc);

	// genera los gr??ficos de las puertas flipeados respecto a x
	flipeaGraficos(tablaFlipX, &roms[0x0aa49], &roms[0x16a49], 6, 0x0f0);
}

// flipea respecto a x todos los gr??ficos del juego que lo necesiten
void Juego::generaGraficosFlipeadosVGA()
{
	UINT8 *romsVGA = &roms[0x24000-1-0x4000];
	UINT8 *romsVGAFlip = &roms[0x24000 + 174065 -1 - 0x4000];
	int dest = 0;
	int size = 57240-53760;

	// genera los gr??ficos de las animaciones de guillermo flipeados respecto a x
	flipeaGraficosVGA(&romsVGA[53760], &romsVGAFlip[dest], 5*4, size);
	dest += size;
	size = 57768-57240; // ok, coincide con los 0x084 que se pasaba para CPC * 4 ya que en VGA cada pixel es un byte , y no 4 pixel en un byte
	flipeaGraficosVGA(&romsVGA[57240], &romsVGAFlip[dest], 4*4, size );
	dest += size;

	// genera los gr??ficos de las animaciones de adso flipeados respecto a x
	size = 59668 - 57768; // ok, coincide con 0x1db *4 
	flipeaGraficosVGA(&romsVGA[57768], &romsVGAFlip[dest], 5*4, size);
	dest += size;
	size = 61108 - 59668; // ok , es 0x168 * 4
	flipeaGraficosVGA(&romsVGA[59668], &romsVGAFlip[dest], 4*4, size);

	// genera los gr??ficos de los trajes de los monjes flipeados respecto a x
	dest += size;
	size = 64008 - 61108; // ok , es 0x2d5*4
	flipeaGraficosVGA(&romsVGA[61108], &romsVGAFlip[dest], 5*4, size);

	// genera los gr??ficos de las caras de los monjes flipeados respecto a x
	dest += size;
	dest+=2900; // TODO: cambio temporal, para que la distancia entre los graficos y su homologo flipeados sea siempre 120305
	// a ver si hay suerte, y en estos 2900 bytes lo que estan son los graficos de las puertas que son los que nos faltan !!! -> pues no, estan justo detras de las caras
	size = 69708 - 66908; // OK , es 0x2bc*4
	flipeaGraficosVGA(&romsVGA[66908], &romsVGAFlip[dest], 5*4, size);

	// genera los gr??ficos de las puertas flipeados respecto a x
	dest += size;
	size = 24*40; // OK , es 0x0f0 * 4
	flipeaGraficosVGA(&romsVGA[69708], &romsVGAFlip[dest], 6*4, size);
}

// copia los gr??ficos de origen en el destino y los flipea
void Juego::flipeaGraficos(UINT8 *tablaFlip, UINT8 *src, UINT8 *dest, int ancho, int bytes)
{
	// copia los gr??ficos del origen al destino
	memcpy(dest, src, bytes);

	// calcula las variables que controlan el bucle
	int numLineas = bytes/ancho;
	int numIntercambios = (ancho + 1)/2;

	// recorre todas las l??neas que forman el gr??fico
	for (int j = 0; j < numLineas; j++)
	{
		UINT8 *ptr1 = dest;
		UINT8 *ptr2 = ptr1 + ancho - 1;

		// realiza los intercambios necesarios para flipear esta l??nea
		for (int i = 0; i < numIntercambios; i++)
		{
			UINT8 aux = *ptr1;
			*ptr1 = tablaFlip[*ptr2];
			*ptr2 = tablaFlip[aux];

			ptr1++;
			ptr2--;
		}

		// pasa a la siguiente l??nea
		dest = dest + ancho;
	}
}

// copia los gr??ficos de origen en el destino y los flipea
void Juego::flipeaGraficosVGA(UINT8 *src, UINT8 *dest, int ancho, int bytes)
{
	// copia los gr??ficos del origen al destino
	memcpy(dest, src, bytes);

	// calcula las variables que controlan el bucle
	int numLineas = bytes/ancho;
	int numIntercambios = (ancho + 1)/2;

	// recorre todas las l??neas que forman el gr??fico
	for (int j = 0; j < numLineas; j++)
	{
		UINT8 *ptr1 = dest;
		UINT8 *ptr2 = ptr1 + ancho - 1;

		// realiza los intercambios necesarios para flipear esta l??nea
		for (int i = 0; i < numIntercambios; i++)
		{
			UINT8 aux = *ptr1;
			*ptr1 = *ptr2;
			*ptr2 = aux;

			ptr1++;
			ptr2--;
		}

		// pasa a la siguiente l??nea
		dest = dest + ancho;
	}
}

// actualiza el sprite de la luz para que se mueva siguiendo a adso
void Juego::actualizaLuz()
{
	// desactiva el sprite de la luz
	sprites[spriteLuz]->esVisible = false;

	// si la pantalla est?? iluminada, sale
	if (motor->pantallaIluminada) return;

	// si adso no es visible en la pantalla actual
	if (!(personajes[1]->sprite->esVisible))
	{
		for (int i = 0; i < numSprites; i++)
		{
			if (sprites[i]->esVisible)
			{
				sprites[i]->haCambiado = false;
			}
		}
		return;
	}

	// actualiza las caracter??sticas del sprite de la luz seg??n la posici??n del personaje
	SpriteLuz *sprLuz = (SpriteLuz *) sprites[spriteLuz];
	sprLuz->ajustaAPersonaje(personajes[1]);
}

// comprueba si se debe pausar el juego
void Juego::compruebaPausa()
{
	// si se ha pulsado suprimir, se para hasta que se vuelva a pulsar
	//if (controles->seHaPulsado(KEYBOARD_SUPR)){
	//	pausa = true;

	//	while (true){
	//		controles->actualizaEstado();
	//		timer->sleep(10);
	//		if (controles->seHaPulsado(KEYBOARD_SUPR)){
	//			pausa = false;
	//			break;
	//		}
	//	}
	//}
}

// comprueba si se desea cambiar los graficos VGA por CPC
void Juego::cambioCPC_VGA()
{
	// En Memoria esta la rom de 0x24000 bytes 
	//  seguidos de 174065 bytes con los graficos de 8 bits a usar
	//  seguidos de 174065+21600 bytes con los graficos VGA de 8 bits
	//  seguidos de 174065+21600 bytes con los graficos CPC de 8 bits
	// asi que copiamos despues de la rom los graficos que vamos a usar
	// (los 21600 son los graficos flipeados)
	if (GraficosCPC)
	{
		memcpy(	&roms[0x24000-1-0x4000],
				&roms[0x24000-1-0x4000+(174065+21600)],
				174065);

		GraficosCPC=false;
	}
	else
	{
		memcpy(	&roms[0x24000-1-0x4000],
				&roms[0x24000-1-0x4000+(174065+21600)*2],
				174065);

		GraficosCPC=true;
	}

	// genera los gr??ficos flipeados en x de las entidades que lo necesiten
	generaGraficosFlipeados();

	ReiniciaPantalla();		
}


// comprueba si se desea cambiar los graficos VGA por CPC
void Juego::compruebaCambioCPC_VGA()
{
	// ESTE FUNCIONAMIENTO NO SE CORRESPONDE
	// CON EL DE LA VERSION ORIGINAL
	if (controles->seHaPulsado(FUNCTION_2))
	{
		// adelanto las inicializaciones antes del pergamino inicial
		// para que se pueda llamar a ReiniciaPantalla en cambioCPC_VGA
		cambioCPC_VGA();
	}
}

bool Juego::cargar(int slot)
{
	std::ifstream in(savefile[slot]);
	in >> logica;
	if (in.fail())
	{
		/* CPC
		   elMarcador->imprimeFrase("            ", 110, 164, 2, 3);
		   elMarcador->imprimeFrase("??????ERROR!!!", 110, 164, 2, 3); */
		// VGA
		elMarcador->imprimeFrase("                  ", 100, 164, 4, 0);
		elMarcador->imprimeFrase("ERROR: PRESS SPACE", 100, 164, 4, 0);
		// el juego ha podido quedar destrozado
		// lo suyo seria cargar en un objeto logica temporal
		// y luego intercambiar los punteros
		// pero como es un singleton...
		// intentemos , al menos, reiniciar
		do
		{
			losControles->actualizaEstado();
		}while (losControles->estaSiendoPulsado(P1_BUTTON1) == false);

		// CPC elMarcador->imprimeFrase("           ", 110, 164, 2, 3);
		elMarcador->imprimeFrase("                  ", 100, 164, 4, 0);
		logica->inicia();
		// devolvemos true, para que se reinicie todo
		return true;
	}
	else {return true;}
}

void Juego::save(int slot)
{
	std::ofstream out(savefile[slot],
			std::ofstream::out|std::ofstream::trunc);

	out << logica; 

	if (out.fail())
	{
		/* CPC
		   elMarcador->imprimeFrase("            ", 110, 164, 2, 3);
		   elMarcador->imprimeFrase("??????ERROR!!!", 110, 164, 2, 3); */
		// VGA
		elMarcador->imprimeFrase("                  ", 100, 164, 4, 0);
		elMarcador->imprimeFrase("ERROR: PRESS SPACE", 100, 164, 4, 0);
		do{
			losControles->actualizaEstado();
		}while (losControles->estaSiendoPulsado(P1_BUTTON1) == false);
	}
}

// comprueba si se desea grabar la partida
void Juego::compruebaSave()
{
	// ESTE FUNCIONAMIENTO NO SE CORRESPONDE
	// CON EL DE LA VERSION ORIGINAL

	if (controles->seHaPulsado(KEYBOARD_G))
	{
		// Frase vacia para parar la frase actual
		elGestorFrases->muestraFraseYa(0x38);
		
		// Esperamos a que se limpie el marcador
		while (elGestorFrases->mostrandoFrase)
		{
			elGestorFrases->actualizaEstado();
		} 

		// Preguntamos
		// CPC elMarcador->imprimeFrase("??GRABAR? S:N", 110, 164, 2, 3);
		//elMarcador->imprimeFrase(",.W??", 110, 164, 4, 0); // VGA
		elMarcador->imprimeFrase("??GRABAR? S:N", 110, 164, 4, 0); // VGA
		//se estaba guardando el ?? como multibyte c2bf 
		// en vez de un simple char bf
		// con imprimeFrase no se cambia la ?? por la W
		// la ?? por el caracter adecuado ...
		//elMarcador->imprimeFrase("\xbfGRABAR? S:N", 110, 164, 4, 0); // VGA
		// TODO esto deberia tener su numero de frase  y estar traducido
		// e imprimirse con  
		//elGestorFrases->muestraFrase??ya?

		do
		{
			losControles->actualizaEstado();
			if (losControles->estaSiendoPulsado(KEYBOARD_S))
			{
				save(0);
				break;
			}
		}while (losControles->estaSiendoPulsado(KEYBOARD_N) == false);
		elGestorFrases->muestraFraseYa(0x38);
	}
}


// comprueba si se desea cargar la partida
bool Juego::compruebaLoad()
{
	// ESTE FUNCIONAMIENTO NO SE CORRESPONDE
	// CON EL DE LA VERSION ORIGINAL

	if (controles->seHaPulsado(KEYBOARD_C))
	{
		// Frase vacia para parar la frase actual
		elGestorFrases->muestraFraseYa(0x38);

		// Esperamos a que se limpie el marcador
		while (elGestorFrases->mostrandoFrase)
		{
			elGestorFrases->actualizaEstado();
		} 

		// Preguntamos
		// CPC elMarcador->imprimeFrase("??CARGAR? S:N", 110, 164, 2, 3);
		elMarcador->imprimeFrase("??CARGAR? S:N", 110, 164, 4, 0);  // VGA

		do
		{
			losControles->actualizaEstado();
			if (losControles->estaSiendoPulsado(KEYBOARD_S))
			{
				return cargar(0);
			}

		}while (losControles->estaSiendoPulsado(KEYBOARD_N) == false);
		elGestorFrases->muestraFraseYa(0x38);
	} 
	return false;
}

// comprueba si se desea entrar en el menu
bool Juego::compruebaMenu()
{
	// ESTE FUNCIONAMIENTO NO SE CORRESPONDE
	// CON EL DE LA VERSION ORIGINAL

	if (controles->seHaPulsado(KEYBOARD_Z))
	{
		bool res=menu2();
		motor->compruebaCambioPantalla(true);	
		return res;
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////
// m??todos para mostrar distintas las pantallas de distintas situaciones del juego
/////////////////////////////////////////////////////////////////////////////

// muestra la imagen de presentaci??n del juego
void Juego::muestraPresentacion()
{
	if (contadorInterrupcion < 1)
	{
		paleta->setIntroPalette();
		//VGA
		UINT8 *romsVGA = &roms[0x24000-1-0x4000];
		cpc6128->showVGAScreen(romsVGA + 0x1ADF0);
	}
	else
	{				
		if (sys->pad.button1)
		{
			currentState = MENU;
			ReiniciaPantalla();
			marcador->limpiaAreaMarcador();	
		}
	}
}

// muestra el pergamino de presentaci??n
void Juego::muestraIntroduccion()
{
	// muestra la introducci??n
	pergamino->muestraTexto(Pergamino::pergaminoInicio[idioma]);
	
	if (pergamino->finished)
	{
		// coloca la paleta negra
		paleta->setGamePalette(0);
		currentState = PLAY;
	
		sys->stopMusic();
		ReiniciaPantalla();
		paleta->setGamePalette(2);		
		marcador->limpiaAreaMarcador();

		ReiniciaPantalla();
		sys->pad.button1 = false;
		contadorInterrupcion = 0;
		
		sys->setNormalSpeed();
		firstTime = false;
	}

}

// muestra indefinidamente el pergamino del final
void Juego::muestraFinal()
{
	//audio_plugin->Play(SONIDOS::Final,true);
	
	// muestra el texto del final
	pergamino->muestraTexto(Pergamino::pergaminoFinal[idioma]);
	
	//if (pergamino->finished){
	//	sys->minimumFrameTime = GAME_FRAME_TIME;
	//}

}

// muestra la parte de misi??n completada. Si se ha completado el juego, muestra el final
bool Juego::muestraPantallaFinInvestigacion()
{
	std::string porcentaje[8] = {
	"  XX POR CIENTO DE", // 0 castellano
	"  XX  PER  CENT", // 1 ingles
	"  XX POR CENTO DA", // 2 portugues brasil
	"  XX PER CENT DE", // 3 CATAL??N
	"  XX POR CENTO DA", // 4 GALLEGO
	"  XX PER CENTO", // 5 ITALIANO
	"  XX  PER  CENT", // 6 fines
	"  XX POR CENTO DA" // 7 PORTUGUES
	};

	// si guillermo est?? vivo, sale
	if (!logica->haFracasado) {return false;}

	// indica que la c??mara siga a guillermo y lo haga ya
	laLogica->numPersonajeCamara = 0x80;

	// si est?? mostrando una frase por el marcador, espera a que se termine de mostrar
	if (elGestorFrases->mostrandoFrase) {return false;}

	// oculta el ??rea de juego
	// CPC limpiaAreaJuego(3);
	limpiaAreaJuego(0); // VGA

	// calcula el porcentaje de misi??n completada. Si se ha completado el juego, muestra el final
	int porc = logica->calculaPorcentajeMision();

	porcentaje[idioma][2] = ((porc/10) % 10) + 0x30;
	porcentaje[idioma][3] = (porc % 10) + 0x30;
	
	// VGA con idiomas
	marcador->imprimeFrase(frase1[idioma], 96, 32, 4, 0);
	marcador->imprimeFrase(porcentaje[idioma], 88, 48, 4, 0);
	marcador->imprimeFrase(frase3[idioma], 90, 64, 4, 0);
	marcador->imprimeFrase(frase4[idioma], 56, 128, 4, 0);

	// espera a que se pulse y se suelte el bot??n
	bool espera = true;

	while (espera)
	{
		controles->actualizaEstado();		
		espera = !(controles->estaSiendoPulsado(P1_BUTTON1) || controles->estaSiendoPulsado(KEYBOARD_SPACE));
	}

	espera = true;

	// TODO Revisar por que esta esto duplicado
	while (espera)
	{
		controles->actualizaEstado();		
		espera = controles->estaSiendoPulsado(P1_BUTTON1) || controles->estaSiendoPulsado(KEYBOARD_SPACE);
	}

	return true;
}

/////////////////////////////////////////////////////////////////////////////
// creaci??n de las entidades del juego
/////////////////////////////////////////////////////////////////////////////

// crea los sprites, personajes, puertas y objetos del juego
void Juego::creaEntidadesJuego()
{
	// sprites de los personajes

	// sprite de guillermo
	sprites[0] = new Sprite();

	// sprite de adso
	sprites[1] = new Sprite();

	// sprite de los monjes
	for (int i = 2; i < 8; i++)
	{
		sprites[i] = new SpriteMonje();
	}

	// sprite de las puertas
	for (int i = primerSpritePuertas; i < primerSpritePuertas + numPuertas; i++)
	{
		sprites[i] = new Sprite();
		sprites[i]->ancho = sprites[i]->oldAncho = 0x06;
		sprites[i]->alto = sprites[i]->oldAlto = 0x28;
	}

	// CPC int despObjetos[8] = { 0x88f0, 0x9fb0, 0x9f80, 0xa010, 0x9fe0, 0x9fe0, 0x9fe0, 0x88c0 };
	// VGA
	/*
	int despObjetos[8] = { 
		0x24000 - 1 - 0x4000 + 11200,  // LIBRO
		0x24000 - 1 - 0x4000 + 34496,  // GUANTES
		0x24000 - 1 - 0x4000 + 34304,  // GAFAS
		0x24000 - 1 - 0x4000 + 34880,  // ?PERGAMINO?
		0x24000 - 1 - 0x4000 + 34688,  // LLAVE
		0x24000 - 1 - 0x4000 + 34688,
		0x24000 - 1 - 0x4000 + 34688,
		0x24000 - 1 - 0x4000 + 11008  // ?LAMPARA?
	}; */

	// En Sprite::dibujaVGA ya le anyade el 0x24000 - 1 - 0x4000 para saltarse la rom CPC e ir a los graficos VGA
	// ?????? ojo !!! , entonces en Marcador::dibujaObjetos hay que incluir este salto ...
	int despObjetos[8] = { 
		11200,  // LIBRO
		34496,  // GUANTES
		34304,  // GAFAS
		34880,  // ?PERGAMINO?
		34688,  // LLAVE
		34688,
		34688,
		11008  // ?LAMPARA?
	};// TODO: los desplazamientos estan bien, lo que no se si se corresponde
	// del todo es el indice, o sea, que el objeto 7 en el juego es la lampara
	// y el 3 el pergamino... 

	// sprite de los objetos
	for (int i = primerSpriteObjetos; i < primerSpriteObjetos + numObjetos; i++)
	{
		sprites[i] = new Sprite();
		sprites[i]->ancho = sprites[i]->oldAncho = 0x04;
		sprites[i]->alto = sprites[i]->oldAlto = 0x0c;

		// CPC sprites[i]->despGfx = despObjetos[i - primerSpriteObjetos];
		// VGA
		// 0x24000 para pasar las roms CPC
		// -1 de empezar a contar en el cero
		// - 0x4000 ya que en el puntero con el que se trabaja se han pasado los primeros 0x4000 bytes con la pantalla de presentacion CPC
		// +34304 que es donde esta el tile 228 , que es por donde anda la imagen del primer objeto
		// como los objetos son de 16*12 (??????ojo!!! no son como el resto de tiles!!!)
		// sumo 16*12* el numero de objeto que busco
		//sprites[i]->despGfx = 0x24000 - 1 - 0x4000 + 34304 + (i - primerSpriteObjetos)*16*12;
		//muy bonito , si los graficos estuviesen en el mismo orden logico que los objetos en el juego
	
		// igual que en CPC , pero con el array despObjetos "actualizado" a las posiciones VGA
		sprites[i]->despGfx = despObjetos[i - primerSpriteObjetos];

	}

	// sprite de los reflejos en el espejo
	sprites[spritesReflejos] = new Sprite();
	sprites[spritesReflejos + 1] = new Sprite();

	// sprite de la luz
	sprites[spriteLuz] = new SpriteLuz();

	// crea los personajes del juego
	personajes[0] = new Guillermo(sprites[0]);
	personajes[1] = new Adso(sprites[1]);
	personajes[2] = new Malaquias((SpriteMonje *)sprites[2]);
	personajes[3] = new Abad((SpriteMonje *)sprites[3]);
	personajes[4] = new Berengario((SpriteMonje *)sprites[4]);
	personajes[5] = new Severino((SpriteMonje *)sprites[5]);
	personajes[6] = new Jorge((SpriteMonje *)sprites[6]);
	personajes[7] = new Bernardo((SpriteMonje *)sprites[7]);

	// inicia los valores comunes
	for (int i = 0; i < 8; i++)
	{
		personajes[i]->despX = -2;
		personajes[i]->despY = -34;
	}
	personajes[1]->despY = -32;
	
	// crea las puertas del juego
	for (int i = 0; i < numPuertas; i++)
	{
		puertas[i] = new Puerta(sprites[primerSpritePuertas + i]);
	}

	// crea los objetos del juego
	for (int i = 0; i < numObjetos; i++)
		objetos[i] = new Objeto(sprites[primerSpriteObjetos + i]);
	
}

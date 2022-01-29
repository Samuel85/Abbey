// Juego.h
//
//		Clase principal del juego. Almacena el estado y las entidades del juego.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __ABADIA_JUEGO_H__
#define __ABADIA_JUEGO_H__

#include <stdio.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <csignal>
#include <sys/stat.h> 

#include "Singleton.h"
#include "Types.h"
#include "Paleta.h"
#include "configreader.h"

class CPC6128;					// definido en CPC6128.h

namespace Abadia {

class InfoJuego;				// definido en InfoJuego.h
class Logica;					// definido en Logica.h
class Marcador;					// definido en Marcador.h
class MotorGrafico;				// definido en MotorGrafico.h
class Objeto;					// definido en Objeto.h
class Pergamino;				// definido en Pergamino.h
class Personaje;				// definido en Personaje.h
class Puerta;					// definido en Puerta.h
class Sprite;					// definido en Sprite.h

#define elJuego Juego::getSingletonPtr()

class Juego : public Singleton<Juego>
{
// constantes
public:
	static const int numPersonajes = 8;
	static const int numPuertas = 7;
	static const int numObjetos = 8;

	static const int primerSpritePersonajes = 0;
	static const int primerSpritePuertas = primerSpritePersonajes + numPersonajes;
	static const int primerSpriteObjetos = primerSpritePuertas + numPuertas;
	static const int spritesReflejos = primerSpriteObjetos + numObjetos;
	static const int spriteLuz = spritesReflejos + 2;
	static const int numSprites = spriteLuz + 1;
	static const char *savefile[7];
	
// campos
private:
	bool mute;
	unsigned short slot;
public:		
	int idioma;  // idioma de los textos TODO ponerlo como un enumerado
	// GraficosCPC estaba como privado
	// se pone como publico para que pueda acceder
	// el MotorGrafico.cpp y decidir si tiene que parchear las pantallas
	// o no
	// TODO: Poner metodos get para no dar acceso directo y que lo 
	// pueda cambiar por error desde otro lado
	bool GraficosCPC; // Indica si se usan los datos del archivo GraficosCPC
			  // o del GraficosVGA
			  // En ambos casos, son de 8 bits
	CPC6128 *cpc6128;						// objeto de ayuda para realizar operaciones gr??ficas del cpc6128
	
	Paleta *paleta;							// paleta del juego
	UINT8 buffer[8192];						// buffer para mezclar los sprites y para buscar las rutas
	UINT8 *roms;							// puntero a las roms originales
	Logica *logica;							// objeto que se encarga de gestionar la l??gica del juego
	
	Pergamino *pergamino;					// pergamino para la presentaci??n y el final
	Marcador *marcador;						// marcador del juego
	MotorGrafico *motor;					// motor gr??fico

	Sprite *sprites[numSprites];			// sprites del juego
	Puerta *puertas[numPuertas];			// puertas del juego
	Objeto *objetos[numObjetos];			// objetos del juego
	Personaje *personajes[numPersonajes];	// personajes del juego



	bool pausa;								// indica si el juego est?? pausado
	bool modoInformacion;					// modo de informaci??n del juego
	bool cambioModoInformacion; // se ha cambiado el estado
	InfoJuego *infoJuego;					// objeto para mostrar informaci??n interna del juego
	int currentState;
	int firstTime;	
	int seleccionado;
	ConfigReader *configReader;
	int selectedSlot;


private:
	bool cargar(int slot);
	void save(int slot);
	
    /////////////////////////////////////////////////////////
	void pintaMenuCargar(int seleccionado,bool efecto=false);

	bool menuCargar2();
	void pintaMenuGrabar(int seleccionado,bool efecto=false);
	bool menuGrabar2();
	bool menuIntroduccion(void);
	void pintaMenuTeclado(int seleccionado);
	bool menuTeclado(void);
	void pintaMenuCamaras(int seleccionado);
	bool menuCamaras(void);
	void pintaMenuMejoras(int seleccionado);
	bool menuMejoras(void);
	void pintaMenuTutorial(int seleccionado,bool efecto=false);
	bool menuTutorial(void);
	void pintaMenuAyuda(int seleccionado,bool efecto=false);
	bool menuAyuda(void);
	void pintaMenuIdioma(int seleccionado,bool efecto=false);
	bool menuIdioma(void);
	void pintaMenuPrincipal(int seleccionado,bool efecto=false);
	bool menu(void);
    ////////////////////////////////////////////////////////////
    
	void cambioCPC_VGA(void);
	void compruebaCambioCPC_VGA(void);
	bool compruebaMenu(void);
	void ReiniciaPantalla(void);
public:
	void muestraFinal();
	void limpiaAreaJuego(int color);

	// bucle principal del juego
	void preRun();
	void run();
	void run2();
	void stateMachine();
	void changeState(int newState);

	// inicializaci??n y limpieza
	Juego(UINT8 *romData, CPC6128 *cpc);
	~Juego();
	bool showingMenu;
	bool activeGame;

protected:
	void muestraPresentacion();
	void muestraIntroduccion();
	bool muestraPantallaFinInvestigacion();

	void creaEntidadesJuego();
	
	void actualizaLuz();
	void generaGraficosFlipeados();
	void flipeaGraficos(UINT8 *tablaFlip, UINT8 *src, UINT8 *dest, int ancho, int bytes);
	void generaGraficosFlipeadosVGA();
	void flipeaGraficosVGA(UINT8 *src, UINT8 *dest, int ancho, int bytes);
	
	void checkForSaveFiles();
	bool saveFileExist[7];
	string saveFile[7];
	void checkConfigFile();
	bool readConfigFile();
	bool saveConfigFile();
	string getDateAndTime();
	
	void menuAsk();	
	void askForNewGame();
	void askForNewGameLogic();

	void askToContinue();
	void askToContinueLogic();

	void askExit();
	void askExitLogic();

};


}

#endif	// _ABADIA_JUEGO_H_

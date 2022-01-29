///Juego.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include <string>

#include "cpc6128.h"



#include "Vigasoco.h"

#include "Abad.h"
#include "Adso.h"
#include "Berengario.h"
#include "Bernardo.h"
#include "BuscadorRutas.h"

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


#define VITA_SAVE_DIR "ux0:data/Abbey/"


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
	idioma=1; // Default english language
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

	// crea los objetos principales que usar?? el juego
	paleta = new Paleta(romData+0x24000-1); // le pasamos los datos de la paleta VGA
	pergamino = new Pergamino();
	motor = new MotorGrafico(buffer, 8192);
	marcador = new Marcador();
	logica = new Logica(roms, buffer, 8192); 	
	infoJuego = new InfoJuego();
	
	pausa = false;
	modoInformacion = false;
	seleccionado = 0;
	
	currentState = INTRO;	
	showingMenu = false;
	firstTime = true;
	activeGame = false;
	
	#ifdef RG350	
	configReader = new ConfigReader("/usr/local/home/Abbey/config.txt");
	#else
	configReader = new ConfigReader("config.txt");
	#endif

	selectedSlot = -1;

	checkConfigFile();
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
    delete configReader;
}

void Juego::ReiniciaPantalla(void)
{
	// limpia el ??rea de juego y dibuja el marcador
	// CPC limpiaAreaJuego(0);
	limpiaAreaJuego(12); // el 0 es el cyan en CPC, no se cual poner en VGA
	// pongo el 12 que es un amarillo cantoso, para comparar con Abadia32

	marcador->dibujaMarcador();

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
	if (seleccionado > 7){
		seleccionado = 0;
	}
	else if (seleccionado < 0){
		seleccionado = 7;
	}	
	pintaMenuCargar(seleccionado,true);
	
	if (BUTTON_YES)
	{
		BUTTON_YES = false;
		if (seleccionado != 7)
		{
			selectedSlot = seleccionado;

			// Ask to continue and loose all progress
			if (activeGame)
			{
				changeState(ASK_CONTINUE);
				ReiniciaPantalla();
				marcador->limpiaAreaMarcador();	
				return false;
			}

			laLogica->inicia();			
			cargar(seleccionado);			
			changeState(PLAY);
			ReiniciaPantalla();
			firstTime = false;
		}
		else
		{
			seleccionado = 4;
			changeState(MENU);
		}		
	}
	return false;
}

bool Juego::cargar(int slot)
{	
	std::string path = "";

	#ifdef RG350
	path = "/usr/local/home/Abbey/";
	#endif

	#ifdef ANDROID
	if (SDL_AndroidGetExternalStorageState() != 0){
		path = SDL_AndroidGetExternalStoragePath();
		path += "/";
	}
	#endif
	
	#ifdef VITA
	path = VITA_SAVE_DIR;
	#endif

	std::ifstream in((path + savefile[slot]).c_str());
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
		//do
		//{
		//	losControles->actualizaEstado();
		//}while (losControles->estaSiendoPulsado(P1_BUTTON1) == false);

		// CPC elMarcador->imprimeFrase("           ", 110, 164, 2, 3);
		elMarcador->imprimeFrase("                  ", 100, 164, 4, 0);
		logica->inicia();
		// devolvemos true, para que se reinicie todo
		return true;
	}
	else {return true;}
}

string Juego::getDateAndTime()
{
	time_t t = time(NULL);
  	struct tm tm = *localtime(&t);
  	char buff[100];
  
  	snprintf(buff, sizeof(buff), "%d.%02d.%02d-%02d:%02d\n", 
	  tm.tm_mon + 1, tm.tm_mday, tm.tm_year + 1900, tm.tm_hour, tm.tm_min);
	std::string buffAsStdStr = buff;
  	return buffAsStdStr;
}

void Juego::askExitLogic()
{
	int i = 0;
	
	if (sys->pad.left)
	{
		i++;
		sys->pad.left = false;
		
	}		
	else if (sys->pad.right)
	{
		i--;
		sys->pad.right = false;
	}				
	seleccionado += i;
	if (seleccionado > 1){
		seleccionado = 0;
	}
	else if (seleccionado < 0){
		seleccionado = 1;
	}	

	if (BUTTON_YES)
	{
		BUTTON_YES = false;

		// exit
		if (seleccionado == 0){				
			sys->exitGame();
		}
		else if (seleccionado==1)
		{
			// Back to the game
			changeState(PLAY);
			ReiniciaPantalla();
			paleta->setGamePalette(2);
			marcador->limpiaAreaMarcador();	
			ReiniciaPantalla();
			BUTTON_YES = false;

			sys->setNormalSpeed();
			//sys->playMusic(START);
			activeGame = true;
		}
	}	
}

void Juego::askExit()
{
	int x = 0;	

	askExitLogic();

	// clear screen
	limpiaAreaJuego(0); 
	
	x = 32;

	// find the delimiter
	int delimiterPosition = continueQuestionText[idioma].find("\n");
	
	// get token and value
	string line1 = continueQuestionText[idioma].substr(0, delimiterPosition);
	string line2 = continueQuestionText[idioma].substr(delimiterPosition+1,
		continueQuestionText[idioma].length());

	x = (320 - line1.length()*8)>>1;
	marcador->imprimeFrase(line1, x, 32, 4, 0);
	x = (320 - line2.length()*8)>>1;
	marcador->imprimeFrase(line2, x, 43, 4, 0);

	const int x1 = 140;
	marcador->imprimeFrase(yesText[idioma], x1, 64, 4, 0);
	const int x2 = 170;
	marcador->imprimeFrase(noText[idioma], x2, 64, 4, 0);

	if (seleccionado == 0){
		marcador->imprimeFrase(yesText[idioma], x1, 64, 0, 4);
	}
	else{		
		marcador->imprimeFrase(noText[idioma], x2, 64, 0, 4);
	}
}

void Juego::askForNewGameLogic()
{	
	int i = 0;
	
	if (sys->pad.left)
	{
		i++;
		sys->pad.left = false;
		
	}		
	else if (sys->pad.right)
	{
		i--;
		sys->pad.right = false;
	}				
	seleccionado += i;
	if (seleccionado > 1){
		seleccionado = 0;
	}
	else if (seleccionado < 0){
		seleccionado = 1;
	}	

	if (BUTTON_YES)
	{
		BUTTON_YES = false;

		//restart the game
		if (seleccionado == 0){				
			logica->inicia();		
		}

		// Back to the game
		changeState(PLAY);
		ReiniciaPantalla();
		paleta->setGamePalette(2);
		marcador->limpiaAreaMarcador();	
		ReiniciaPantalla();
		BUTTON_YES = false;

		sys->setNormalSpeed();
		//sys->playMusic(START);
		activeGame = true;
	}
}

void Juego::askForNewGame()
{
	int x = 0;	

	askForNewGameLogic();

	// clear screen
	limpiaAreaJuego(0); 
	
	x = 32;

	// find the delimiter
	int delimiterPosition = newGameQuestionText[idioma].find("\n");
	
	// get token and value
	string line1 = newGameQuestionText[idioma].substr(0, delimiterPosition);
	string line2 = newGameQuestionText[idioma].substr(delimiterPosition+1,
		newGameQuestionText[idioma].length());

	x = (320 - line1.length()*8)>>1;
	marcador->imprimeFrase(line1, x, 32, 4, 0);
	x = (320 - line2.length()*8)>>1;
	marcador->imprimeFrase(line2, x, 43, 4, 0);

	const int x1 = 140;
	marcador->imprimeFrase(yesText[idioma], x1, 64, 4, 0);
	const int x2 = 170;
	marcador->imprimeFrase(noText[idioma], x2, 64, 4, 0);

	if (seleccionado == 0){
		marcador->imprimeFrase(yesText[idioma], x1, 64, 0, 4);
	}
	else{		
		marcador->imprimeFrase(noText[idioma], x2, 64, 0, 4);
	}
}

void Juego::askToContinueLogic()
{	
	int i = 0;
	
	if (sys->pad.left)
	{
		i++;
		sys->pad.left = false;
		
	}		
	else if (sys->pad.right)
	{
		i--;
		sys->pad.right = false;
	}				
	seleccionado += i;
	if (seleccionado > 1){
		seleccionado = 0;
	}
	else if (seleccionado < 0){
		seleccionado = 1;
	}	

	if (BUTTON_YES)
	{
		BUTTON_YES = false;
		if (seleccionado == 1) //NO
		{	
			changeState(PLAY);
			ReiniciaPantalla();
			paleta->setGamePalette(2);
			marcador->limpiaAreaMarcador();	
			ReiniciaPantalla();
			BUTTON_YES = false;

			sys->setNormalSpeed();
			//sys->playMusic(START);
			activeGame = true;		
			
		}
		else //YES
		{	
			laLogica->inicia();
			cargar(selectedSlot);						
			changeState(PLAY);
			ReiniciaPantalla();
			firstTime = false;
		}
	}
}
void Juego::askToContinue()
{
	int x = 0;	

	askToContinueLogic();

	// clear screen
	limpiaAreaJuego(0); 
	
	x = 32;

	// find the delimiter
	int delimiterPosition = continueQuestionText[idioma].find("\n");
	
	// get token and value
	string line1 = continueQuestionText[idioma].substr(0, delimiterPosition);
	string line2 = continueQuestionText[idioma].substr(delimiterPosition+1,
		continueQuestionText[idioma].length());

	x = (320 - line1.length()*8)>>1;
	marcador->imprimeFrase(line1, x, 32, 4, 0);
	x = (320 - line2.length()*8)>>1;
	marcador->imprimeFrase(line2, x, 43, 4, 0);

	const int x1 = 140;
	marcador->imprimeFrase(yesText[idioma], x1, 64, 4, 0);
	const int x2 = 170;
	marcador->imprimeFrase(noText[idioma], x2, 64, 4, 0);

	if (seleccionado == 0){
		marcador->imprimeFrase(yesText[idioma], x1, 64, 0, 4);
	}
	else{		
		marcador->imprimeFrase(noText[idioma], x2, 64, 0, 4);
	}
}

void Juego::save(int slot)
{
	
	//Update config.txt
	string d = getDateAndTime();
	string token = "SAVEX";
	token[4] = '0' + slot;		
	configReader->setValue(token, d);
	saveConfigFile();

	// save "abadiaX.save"

	std::string path = "";

	#ifdef RG350
	path = "/usr/local/home/Abbey/";
	#endif

	#ifdef ANDROID
	if (SDL_AndroidGetExternalStorageState() != 0){
		path = SDL_AndroidGetExternalStoragePath();
		path += "/";
	}
	#endif
	
	#ifdef VITA
	path = VITA_SAVE_DIR;
	#endif

	std::ofstream out((path + savefile[slot]).c_str(),
			std::ofstream::out|std::ofstream::trunc);
	
	out << logica; 

	if (out.fail())
	{
		// VGA
		elMarcador->imprimeFrase("                  ", 100, 164, 4, 0);
		elMarcador->imprimeFrase("ERROR: PRESS SPACE", 100, 164, 4, 0);		
	}
}

void Juego::pintaMenuGrabar(int seleccionado,bool efecto)
{	
	// limpia el ??rea que ocupa el marcador
	limpiaAreaJuego(0); 
	marcador->limpiaAreaMarcador();	

	//repintar con un efecto para que vaya apareciendo el
	// menu de izquierda a derecha y asi dar tiempo a soltar las teclas
	// al usuario

	int x = 0;
	const int y = 32;
	cpc6128->fillMode1Rect(8, 0, 88, 160, 0);
	for (int i=0;i<7;i++)
	{
		if (i == seleccionado){
			x = (320 - saveFile[i].length()*8)>>1;
			marcador->imprimeFrase(saveFile[i], x, y+(i*16),0, 4);
		}
		else{
			x = (320 - saveFile[i].length()*8)>>1;
			marcador->imprimeFrase(saveFile[i], x, y+(i*16),4, 0);
		}		
	}
	// Text for the option "Back"
	if (seleccionado == 7){
		x = (320 - textSave[idioma].length()*8)>>1;
		marcador->imprimeFrase(textSave[idioma], x, y+(7*16),0, 4);
	}
	else{
		x = (320 - textSave[idioma].length()*8)>>1;
		marcador->imprimeFrase(textSave[idioma], x, y+(7*16),4, 0);
	}
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
	if (seleccionado > 7){
		seleccionado = 0;
	}
	else if (seleccionado < 0){
		seleccionado = 7;
	}	
	pintaMenuCargar(seleccionado,true);	
	if (BUTTON_YES)
	{
		BUTTON_YES = false;
		if (seleccionado != 7)
		{			
			save(seleccionado);						
			changeState(PLAY);
			ReiniciaPantalla();
			return true;
		}
		else
		{
			seleccionado = 4;
			changeState(MENU);
		}
	}
	return false;	
}

void Juego::pintaMenuIdioma(int seleccionado,bool efecto)
{

	// limpia el ??rea que ocupa el marcador
	limpiaAreaJuego(0); 

	int x = 0;
	
	for (int i=0;i<8;i++){
		x = (320 - textLanguage[i].length()*8)>>1;
		marcador->imprimeFrase(textLanguage[i], x, 32+(i*16),4, 0);
	}

	x = (320 - textLanguage[seleccionado].length()*8)>>1;
	marcador->imprimeFrase(textLanguage[seleccionado], x, 
		32+(seleccionado*16), 0, 4);
	
}

bool Juego::menuIdioma()
{
	limpiaAreaJuego(0);

	pintaMenuIdioma(seleccionado,true);
				
	if (sys->pad.down) {
		seleccionado++;		
		sys->pad.down = false;
	}
	else if (sys->pad.up) {
		seleccionado--;						
		sys->pad.up = false;
	}
	if (seleccionado==8) seleccionado=0;
	else if (seleccionado==-1) seleccionado=7;

	pintaMenuIdioma(seleccionado);

	if (BUTTON_YES)
	{
		BUTTON_YES = false;
		idioma=seleccionado;

		//Update config.txt
		string d = getDateAndTime();
		string token = "LANGUAGE";
		token[4] = '0' + idioma;		
		configReader->setValue(token, d);
		saveConfigFile();

		seleccionado = 4;
		changeState(MENU);
		return true;
	}	
	
	return false;
}


void Juego::pintaMenuPrincipal(int seleccionado,bool efecto)
{
	// limpia el ??rea que ocupa el marcador
	limpiaAreaJuego(0); 

	int x = 0;
	const int y = 32;

	for (int i=0;i<6;i++)
	{						
		if ((i==4 || i==2) && !activeGame){
			x = (320 - principalMenuText[idioma][i].length()*8)>>1;
			marcador->imprimeFrase(principalMenuText[idioma][i], x, y+(i*16),5, 0);
		}
		else{
			x = (320 - principalMenuText[idioma][i].length()*8)>>1;
			marcador->imprimeFrase(principalMenuText[idioma][i], x, y+(i*16),4, 0);
		}
	}
	
	x = (320 - principalMenuText[idioma][seleccionado].length()*8)>>1;
	marcador->imprimeFrase(principalMenuText[idioma][seleccionado], x, 
		y+(seleccionado*16), 0, 4);	
}

bool Juego::menu()
{	
	if (BUTTON_YES)
	{
		BUTTON_YES = false;
		switch(seleccionado)
		{
			case 0: //New game					
				if (!activeGame)
				{									
					changeState(SCROLL);
					ReiniciaPantalla();
					paleta->setGamePalette(2);
					marcador->limpiaAreaMarcador();	
					ReiniciaPantalla();
					BUTTON_YES = false;

					sys->minimumFrameTime = SCROLL_FRAME_TIME;
					sys->playMusic(START);
					activeGame = true;						
				}
				else
				{
					//std::raise(SIGTRAP);	
					// Ask player to continue and loose game
					
					seleccionado = 1;
					changeState(ASK_NEW_GAME);
					ReiniciaPantalla();
					paleta->setGamePalette(2);		
					marcador->limpiaAreaMarcador();					
					BUTTON_YES = false;

					activeGame = true;	
					return true;
				}
				return true;		
				break;		
			case 1: //Load				
				checkForSaveFiles();				
				changeState(LOAD);
				ReiniciaPantalla();
				paleta->setGamePalette(2);		
				marcador->limpiaAreaMarcador();					
				BUTTON_YES = false;

				activeGame = true;
				
				return true;
				break;
			case 2: //save
				if (activeGame)
				{
					checkForSaveFiles();
					changeState(SAVE);
					ReiniciaPantalla();				
					paleta->setGamePalette(2);		
					marcador->limpiaAreaMarcador();						
					BUTTON_YES = false;

					activeGame = true;	
				}
				return true;
				break;
			case 3: //Language
				seleccionado = idioma;
				changeState(LANGUAGE);
				ReiniciaPantalla();
				paleta->setGamePalette(2);		
				marcador->limpiaAreaMarcador();					
				BUTTON_YES = false;

				return true;
				break;					
			case 4: //Continue
				if (activeGame)
				{
					changeState(PLAY);
					ReiniciaPantalla();
					activeGame = true;	
					return true;
				}
				break;
			case 5: //EXIT
				seleccionado = 1;
				changeState(ASK_EXIT);
				ReiniciaPantalla();				
				paleta->setGamePalette(2);		
				marcador->limpiaAreaMarcador();						
				BUTTON_YES = false;

				break;
		}
	}

	pintaMenuPrincipal(seleccionado,true);		
	
	if (sys->pad.up)
	{
		seleccionado--;
		sys->pad.up = false;
	}		
	else if (sys->pad.down)
	{
		seleccionado++;
		sys->pad.down = false;
	}					

	if (seleccionado > 5){
		seleccionado = 0;
	}	
	else if (seleccionado < 0){
		seleccionado = 5; // EXIT
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
		case LANGUAGE:			
			menuIdioma();
			break;
		case MENU:
			menu();
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
		case ASK_NEW_GAME:
			askForNewGame();
			break;
		case ASK_CONTINUE:
			askToContinue();
			break;
		case ASK_EXIT:
			askExit();
			break;
		case ENDING:
			muestraFinal();
			break;
	}
}

// check for save files correct the menu options
void Juego::checkForSaveFiles()
{
	checkConfigFile();
	for (int i=0;i<7;i++)
	{
		string token = "SAVEX";
		token[4] = '0' + i;		
		saveFile[i] = configReader->getValue(token); 
	}
}

void Juego::checkConfigFile()
{
	if (!readConfigFile())
	{
		if (saveConfigFile()){
			// Success!, config file created.
		}
	}
}

bool Juego::readConfigFile()
{
	bool r = false;

	if (!configReader->isEmpty()){
		delete configReader;
	}

	std::string path = "";

	#ifdef RG350
	path = "/usr/local/home/Abbey/";
	#endif

	#ifdef ANDROID
	if (SDL_AndroidGetExternalStorageState() != 0){
		path = SDL_AndroidGetExternalStoragePath();
		path += "/";
	}
	#endif
	
	#ifdef VITA
	path = VITA_SAVE_DIR;
	#endif

	configReader = new ConfigReader((path + "config.txt").c_str());

	if  (configReader->parse())
	{
		//idioma = stoi(configReader->getValue("LANGUAGE"));
		string s = configReader->getValue("LANGUAGE");
		idioma = atoi(s.c_str());
		r = true;
	}

	return r;
}

bool Juego::saveConfigFile()
{
	bool r = false;
	ofstream f;

	std::string path = "";

	#ifdef RG350
	path = "/usr/local/home/Abbey/";
	#endif

	#ifdef ANDROID
	if (SDL_AndroidGetExternalStorageState() != 0){
		path = SDL_AndroidGetExternalStoragePath();
		path += "/";	
	}
	#endif
	
	#ifdef VITA
	path = VITA_SAVE_DIR;
	#endif
	
	f.open((path + "config.txt").c_str());

	// write current language
	f << "LANGUAGE="<< idioma <<"\n";

	for (int i=0;i<7;i++)
	{
		// Change token
		string token = "SAVEX";
		token[4] = '0' + i;

		// If exist retrieve value from the linked list
		if (!configReader->isEmpty()){			
			f << "SAVE" << i << "=" << configReader->getValue(token) <<"\n";
		}
		else{ // default			
			f << "SAVE" << i << "=" <<"--" <<"\n";
		}
	}

	f.close();
	return r;
}


void Juego::run()
{		
	elBuscadorDeRutas->contadorAnimGuillermo = laLogica->guillermo->contadorAnimacion;
	
	logica->compruebaAbreEspejo();
	
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

	if (modoInformacion){
		infoJuego->muestraInfo();
	} 
	else
	{		
		motor->dibujaPantalla();
		motor->dibujaSprites();				
	}

	if (laLogica->guillermo->contadorAnimacion==1){			
		sys->playSound(STEPS);
	}
	
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
			if (sprites[i]->esVisible){
				sprites[i]->haCambiado = false;
			}
		}
		return;
	}

	// actualiza las caracter??sticas del sprite de la luz seg??n la posici??n del personaje
	SpriteLuz *sprLuz = (SpriteLuz *) sprites[spriteLuz];
	sprLuz->ajustaAPersonaje(personajes[1]);
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


/////////////////////////////////////////////////////////////////////////////
// m??todos para mostrar distintas las pantallas de distintas situaciones del juego
/////////////////////////////////////////////////////////////////////////////

// muestra la imagen de presentaci??n del juego
void Juego::muestraPresentacion()
{
        paleta->setIntroPalette();
        UINT8 *romsVGA = &roms[0x24000-1-0x4000];
        cpc6128->showVGAScreen(romsVGA + 0x1ADF0);

        if (BUTTON_YES)
        {
                currentState = MENU;
                ReiniciaPantalla();
                marcador->limpiaAreaMarcador();
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
		BUTTON_YES = false;
		
		sys->setNormalSpeed();
		firstTime = false;
	}

}

// muestra indefinidamente el pergamino del final
void Juego::muestraFinal()
{
	//audio_plugin->Play(SONIDOS::Final,true);
        sys->playMusic(END);
	// muestra el texto del final
	pergamino->muestraTexto(Pergamino::pergaminoFinal[idioma]);
}

// muestra la parte de misi??n completada. Si se ha completado el juego, muestra el final
bool Juego::muestraPantallaFinInvestigacion()
{
	std::string porcentaje[8] = {
	"XX POR CIENTO DE", // 0 castellano
	"XX  PER  CENT", // 1 ingles
	"XX POR CENTO DA", // 2 portugues brasil
	"XX PER CENT DE", // 3 CATAL??N
	"XX POR CENTO DA", // 4 GALLEGO
	"XX PER CENTO", // 5 ITALIANO
	"XX  PER  CENT", // 6 fines
	"XX POR CENTO DA" // 7 PORTUGUES
	};

	//activeGame = false;

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

	porcentaje[idioma][0] = ((porc/10) % 10) + 0x30;
	porcentaje[idioma][1] = (porc % 10) + 0x30;
	
	// Show message
	int x = 0;
	x = (320 - frase1[idioma].length()*8)>>1;
	marcador->imprimeFrase(frase1[idioma], x, 32, 4, 0);
	x = (320 - porcentaje[idioma].length()*8)>>1;
	marcador->imprimeFrase(porcentaje[idioma], x, 48, 4, 0);
	x = (320 - frase3[idioma].length()*8)>>1;
	marcador->imprimeFrase(frase3[idioma], x, 64, 4, 0);
	x = (320 - frase4[idioma].length()*8)>>1;
	marcador->imprimeFrase(frase4[idioma], x, 128, 4, 0);

	if (sys->pad.button1 ||sys->pad.button2 ||sys->pad.button3 ||sys->pad.button4)
	{
		paleta->setIntroPalette();
		//VGA
		UINT8 *romsVGA = &roms[0x24000-1-0x4000];
		cpc6128->showVGAScreen(romsVGA + 0x1ADF0);

		currentState = INTRO;
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
	for (int i = 2; i < 8; i++){
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
	for (int i = 0; i < 8; i++){
		personajes[i]->despX = -2;
		personajes[i]->despY = -34;
	}
	personajes[1]->despY = -32;
	
	// crea las puertas del juego
	for (int i = 0; i < numPuertas; i++){
		puertas[i] = new Puerta(sprites[primerSpritePuertas + i]);
	}

	// crea los objetos del juego
	for (int i = 0; i < numObjetos; i++){
		objetos[i] = new Objeto(sprites[primerSpriteObjetos + i]);
	}
	
}

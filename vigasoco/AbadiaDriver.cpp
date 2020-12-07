// AbadiaDriver.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "AbadiaDriver.h"
#include "cpc6128.h"
#include "DskReader.h"
#include "ICriticalSection.h"
#include "IDrawPlugin.h"
#include "GameDataEntity.h"
#include "GameDriver.h"
#include "GfxData.h"

#include "system.h"

#include "GestorFrases.h"
#include "Juego.h"

//para print
#include <stdio.h>
//para memcpy
#include <string.h>
#include <iostream>
#include <stdio.h>

using namespace Abadia;

/////////////////////////////////////////////////////////////////////////////
// initialization and cleanup
/////////////////////////////////////////////////////////////////////////////

AbadiaDriver::AbadiaDriver() : GameDriver("abadia", "La abadia del crimen", 300)
{
	_videoInfo.width = 640;
	_videoInfo.height = 400;
	_videoInfo.visibleArea = Rect(_videoInfo.width, _videoInfo.height);
	_videoInfo.colors = 32;			// 16 del juego + 16 para mostrar informaci?n interna del juego
	_videoInfo.colors = 256;		// TODO: PRUEBAS VGA
	_videoInfo.refreshRate = 50;

	_numInterruptsPerVideoUpdate = 6;
	_numInterruptsPerLogicUpdate = 1;

	_abadiaGame = 0;
	cpc6128 = 0;
	cs = 0;
	romsPtr = 0;

	createGameDataEntities();
	
}

AbadiaDriver::~AbadiaDriver()
{
}

/////////////////////////////////////////////////////////////////////////////
// creates the necessary file info, graphics specifications and inputs
/////////////////////////////////////////////////////////////////////////////

void AbadiaDriver::createGameDataEntities()
{
	// el c?digo y los gr?ficos est?n mezclados en la imagen
	GameDataEntity *roms = new GameDataEntity(MIXED, "Code + Graphics + Sound");
	roms->addFile(new GameFile("abadia.dsk", 0x00000, 0x27400, 0xd37cf8e7, 0));
	_gameFiles.push_back(roms);

	// Graficos VGA
	GameDataEntity *GfxVGA = new GameDataEntity(GRAPHICS,"Graphics VGA + Palette");
	GfxVGA->addFile(new GameFile("GraficosVGA",0,174065,0,0));
	_gameFiles.push_back(GfxVGA);

	// Graficos CPC adaptados a modo 256 colores
	GameDataEntity *GfxCPC = new GameDataEntity(GRAPHICS,"Graphics CPC + Palette");
	GfxCPC->addFile(new GameFile("GraficosCPC",0,174065,0,0));
	_gameFiles.push_back(GfxCPC);

	// Los sonidos se cargan directamente en el AudioPlugin
	// para reducir el uso de memoria y no tenerlos
	// cargados en 2 sitios a la vez

	// El delete de roms, GfxVGA y GfxCPC
	// lo hace GameDriver::deallocateFilesMemory()
	// asi , que cuidado al usar los _gameFiles despues de iniciar, porque
	// no estan disponibles
}

void AbadiaDriver::createGameGfxDescriptions()
{
}

void AbadiaDriver::createGameInputsAndDips()
{
}

/////////////////////////////////////////////////////////////////////////////
// template method overrides to customize initialization
/////////////////////////////////////////////////////////////////////////////
void AbadiaDriver::filesLoaded()
{
	int bytesWritten;
	UINT8 auxBuffer[0xff00];

	// reserva espacio para los datos del juego
	// TODO: VGA
	// para no pasar mas punteros y cambiar las interfaces
	// pegamos al final de romsPtr los datos de GraficosVGA del "remake" a 256 colores

	romsPtr = NULL;
	romsPtr = new UINT8[	0x24000 +
				((_gameFiles[1]->getTotalSize()+21600)*3) ];

	// TODO: La funcion filesLoaded no debe ser void y se debe revisar si se pudo conseguir la memoria
	// y devolver este resultado
	if (romsPtr==NULL ) printf("No se pudo reservar memoria para la rom \n");

	// 0x24000 de la rom original de CPC
	// 174065 de Graficos VGA
	// 21600 para copiar los graficos VGA flipeados necesarios ... (falta ajustar este valor)
	// Y por 3, porque  tenemos primero los graficos a utilizar, y
	// luego una copia de los graficos VGA de 8 bits
	// y otra copia de los graficos CPC de 8 bits para poder
	// cambiar en caliente entre ellos
	// ver Juego::compruebaCambioCPC_VGA
	// los sonidos en formato WAV no se copian aqui, porque
	// el AudioPlugin vuelve a reservar memoria para ellos
	// y seria desperdiciar mucha memoria

	// extrae los datos del juego de la imagen del disco
	DskReader dsk(_gameFiles[0]->getData());

	// obtiene los datos de las pistas 0x01-0x11
	for (int i = 0x01; i <= 0x11; i++){
		dsk.getTrackData(i, &auxBuffer[(i - 0x01)*0x0f00], 0x0f00, bytesWritten);
	}

	// reordena los datos y los copia al destino
	reOrderAndCopy(&auxBuffer[0x0000], &romsPtr[0x00000], 0x4000);	// abadia0.bin
	reOrderAndCopy(&auxBuffer[0x4000], &romsPtr[0x0c000], 0x4000);	// abadia3.bin
	reOrderAndCopy(&auxBuffer[0x8000], &romsPtr[0x20000], 0x4000);	// abadia8.bin
	reOrderAndCopy(&auxBuffer[0xc000], &romsPtr[0x04100], 0x3f00);	// abadia1.bin

	// obtiene los datos de las pistas 0x12-0x16
	for (int i = 0x12; i <= 0x16; i++){
		dsk.getTrackData(i, &auxBuffer[(i - 0x12)*0x0f00], 0x0f00, bytesWritten);
	}

	// reordena los datos y los copia al destino
	reOrderAndCopy(&auxBuffer[0x0000], &romsPtr[0x1c000], 0x4000);	// abadia7.bin

	// obtiene los datos de las pistas 0x17-0x1b
	for (int i = 0x17; i <= 0x1b; i++){
		dsk.getTrackData(i, &auxBuffer[(i - 0x17)*0x0f00], 0x0f00, bytesWritten);
	}

	// reordena los datos y los copia al destino
	reOrderAndCopy(&auxBuffer[0x0000], &romsPtr[0x18000], 0x4000);	// abadia6.bin

	// obtiene los datos de las pistas 0x1c-0x21
	for (int i = 0x1c; i <= 0x21; i++){
		dsk.getTrackData(i, &auxBuffer[(i - 0x1c)*0x0f00], 0x0f00, bytesWritten);
	}

	// reordena los datos y los copia al destino
	reOrderAndCopy(&auxBuffer[0x0000], &romsPtr[0x14000], 0x4000);	// abadia5.bin

	// obtiene los datos de las pistas 0x21-0x25
	for (int i = 0x21; i <= 0x25; i++){
		dsk.getTrackData(i, &auxBuffer[(i - 0x21)*0x0f00], 0x0f00, bytesWritten);
	}

	// reordena los datos y los copia al destino
	reOrderAndCopy(&auxBuffer[0x0000], &romsPtr[0x08000], 0x4000);	// abadia2.bin

	// TODO: VGA
	// Copiamos de primeras los graficos VGA despues de la rom
	// y detras copiamos otra vez los VGA y luego los CPC
	// para poder cambiar en caliente entre ellos
	// ver Abadia::Juego::compruebaCambioCPC_VGA
	memcpy(&romsPtr[0x24000-1],_gameFiles[1]->getData(),_gameFiles[1]->getTotalSize());
	memcpy(&romsPtr[0x24000-1+_gameFiles[1]->getTotalSize()+21600],_gameFiles[1]->getData(),_gameFiles[1]->getTotalSize());
	memcpy(&romsPtr[0x24000-1+(_gameFiles[1]->getTotalSize()+21600)*2],_gameFiles[2]->getData(),_gameFiles[1]->getTotalSize());
	// Los sonidos no se copian, y se cargan directamente en Audioplugin
}

// reordena los datos gr?ficos y los copia en el destino
void AbadiaDriver::reOrderAndCopy(UINT8 *src, UINT8 *dst, int size)
{
	for (int i = 0; i < size; i++){
		dst[size - i - 1] = src[i];
	}
}

void AbadiaDriver::finishInit()
{
	// crea e inicia la secci?n cr?tica para la sincronizaci?n del dibujado de gr?ficos
	//cs = VigasocoMain->createCriticalSection();
	//cs->init();

	//crea el objeto para tratar con gr?ficos del amstrad
	//cpc6128 = new CPC6128(cs);
	cpc6128 = new CPC6128();

	// crea el objeto del juego
	_abadiaGame = new Abadia::Juego(romsPtr, cpc6128);
	_abadiaGame->contadorInterrupcion = 0;
}

void AbadiaDriver::videoInitialized(IDrawPlugin *dp)
{
}

/////////////////////////////////////////////////////////////////////////////
// template method overrides to customize cleanup
/////////////////////////////////////////////////////////////////////////////

void AbadiaDriver::videoFinalizing(IDrawPlugin *dp)
{
}

void AbadiaDriver::end()
{
	// destruye la secci?n cr?tica
	if (cs != 0){
		cs->destroy();
		delete cs;
	}

	// borra el objeto de ayuda para los gr?ficos
	delete cpc6128;

	// borra el objeto del juego
	delete _abadiaGame;

	// libera la memoria utilizada por las roms del juego
	delete[] romsPtr;
}

/////////////////////////////////////////////////////////////////////////////
// run and refresh methods
/////////////////////////////////////////////////////////////////////////////

void AbadiaDriver::runSync()
{
	if (!_abadiaGame->pausa){
		// incrementa el contador de la interrupci?n
		_abadiaGame->contadorInterrupcion++;

		// si se est? mostrando alguna frase en el marcador, contin?a mostr?ndola
		elGestorFrases->procesaFraseActual();
	}
}
void AbadiaDriver::preRun()
{
	_abadiaGame->preRun();
}
void AbadiaDriver::runAsync()
{	
	//_abadiaGame->run();
	_abadiaGame->stateMachine();	
}
void AbadiaDriver::showMenu()
{
	//_abadiaGame->currentState = MENU;
	_abadiaGame->changeState(MENU);
}
void AbadiaDriver::changeState(int newState)
{
	_abadiaGame->changeState(newState);
}
void AbadiaDriver::render(IDrawPlugin *dp)
{
	//TODO: VGA
	//El codigo si usasemos los graficos originales
	//de abadia.dsk seria diferente

	UINT8 *posPant = cpc6128->screenBuffer;
	UINT8 *posPantTmp = NULL;
	// dibuja los datos que el juego escribe en el otro hilo en el buffer de pantalla
	uint8_t *pixels = (uint8_t*)sys->surface->pixels;
	int pitch = sys->surface->pitch;
	Uint32 *p = NULL;
	Uint8 *dirtyByte, bit, r,g,b;
	bool dirty = false;	
	int data;

	/*	
	for (int y = 0; y < 200; y++)
	{
		p = (Uint32 *)(pixels + pitch*y);			
		for (int x = 0; x < 640; x++)
		{
			dirtyByte = &(cpc6128->DirtyPixels[(y*640 + x)/8]);
			bit = 1<<((y*640 + x)%8);
			dirty = *dirtyByte & bit;
			if (dirty)
			{
				*dirtyByte = *dirtyByte & (~bit);
				data = *posPant;														
				r = (Uint8) _abadiaGame->paleta->paleta2->_palette[data].R;
				g = (Uint8) _abadiaGame->paleta->paleta2->_palette[data].G;
				b = (Uint8) _abadiaGame->paleta->paleta2->_palette[data].B;															
				*p = sys->RGBA(r,g,b,0xFF);
				
			}
			p++;
			posPant++;
		}
	}
	*/
	//p = (Uint32 *)(pixels + pitch*y);
	p = (Uint32 *)pixels;
	for (int y = 0; y < TEXTURE_HEIGHT>>1; y++)
	{	
		posPantTmp = posPant;			
		for (int x = 0; x < TEXTURE_WIDTH; x++)
		{			
			data = *posPant;														
			r = (Uint8) _abadiaGame->paleta->paleta2->_palette[data].R;
			g = (Uint8) _abadiaGame->paleta->paleta2->_palette[data].G;
			b = (Uint8) _abadiaGame->paleta->paleta2->_palette[data].B;															
			*p = sys->RGBA(r,g,b,0xFF);
			
			p++;
			posPant++;
		}
		posPant = posPantTmp;
		for (int x = 0; x < TEXTURE_WIDTH; x++)
		{			
			data = *posPant;														
			r = (Uint8) _abadiaGame->paleta->paleta2->_palette[data].R;
			g = (Uint8) _abadiaGame->paleta->paleta2->_palette[data].G;
			b = (Uint8) _abadiaGame->paleta->paleta2->_palette[data].B;															
			*p = sys->RGBA(r,g,b,0xFF);
			
			p++;
			posPant++;
		}
	}

	sys->updateTexture();
}

/////////////////////////////////////////////////////////////////////////////
// display internal game information
/////////////////////////////////////////////////////////////////////////////

void AbadiaDriver::showGameLogic(IDrawPlugin *dp)
{
	// actualiza el modo de informaci?n
	//if (theInputHandler->hasBeenPressed(FUNCTION_5)){
		_abadiaGame->modoInformacion = !_abadiaGame->modoInformacion;
		_abadiaGame->cambioModoInformacion = true;
	//}
}

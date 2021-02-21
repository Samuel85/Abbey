// GeneradorPantallas.h
//
//	Clase que se encarga de generar los bloques que forman las pantallas, calcular la zona que
//	tapa el bloque y grabar toda esa informaci�n en la capas que forman el buffer de tiles.
//	Adem�s, esta clase se encarga de dibujar las pantallas una vez que se ha rellenado el buffer 
//	de tiles.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _GENERADOR_PANTALLAS_H_
#define _GENERADOR_PANTALLAS_H_

#include <iostream>

class CPC6128;							// definido en CPC6128.h

namespace Abadia {

class Comando;							// definido en Comando.h


class GeneradorPantallas
{
// constantes
public:
	//static const int nivelesProfTiles = 3;
	//Con 3 en CPC se ve bastante bien
	//pero con VGA hay imperfecciones visuales bastante notables (ver la pantalla en la iglesia justo antes de llegar
	//a la pantalla del altar. Hay un pulpito al que se llega por unas escaleras, en el que queda un hueco sin dibujar que
	//queda con el color de fondo de la pantalla !!!
	// En el remake 32 bits 256 colores el pulpito se ve bien, pero en el suelo al empezar las escaleras, se mezclan 2 tipos 
	// de suelos � cual es el bueno ? por que en la pantalla anterior, en la otra vista, el suelo es gris completo antes de
	// empezar las escaleras ..., pero en esta pantalla salen losas cuadradas ...
	//
static const int nivelesProfTiles = 4; // Con esto ya se ve bien casi todo, incluso a veces mejor al remake 32 bits 256 colores
	// static const int nivelesProfTiles = 5; // No se nota diferencia con el anterior ...
	//hay que usar al menos 2, ya que los parches al mapeo de pantallas
	//copiados del remake de PC modifican los 2 primeros niveles
	//y si no existen da core al sobreescribir memoria


// tipos
public:
	struct TileInfo {
		UINT8 profX[nivelesProfTiles];	// profundidad del tile en x (en coordenadas locales)
		UINT8 profY[nivelesProfTiles];	// profundidad del tile en y (en coordenadas locales)
		UINT8 tile[nivelesProfTiles];	// n�mero de tile
	};

// campos
public:
	UINT8 *roms;						// puntero a los datos del juego
	CPC6128	*cpc6128;					// objeto de ayuda para realizar operaciones gr�ficas del cpc6128

	TileInfo bufferTiles[20][16];		// buffer de tiles (16x20 tiles)
	int mascaras[4][4];					// tablas de m�scaras and y or para cada uno de los colores

	UINT8 *datosPantalla;				// puntero a los datos que forman la pantalla
	int comandosBloque;					// desplazamiento a los datos de los comandos que forman un bloque
	int datosBloque[17];				// buffer donde guarda los datos para construir el bloque actual
	int tilePosX, tilePosY;				// posici�n actual en el buffer de tiles
	bool cambioSistemaCoord;			// indica si se ha cambiado el sistema de coordenadas
	int estadoOpsX[4];					// usado para cambiar el sentido de las x en algunas operaciones

protected:
	int pila[64];						// pila para evaluar los comandos de generaci�n de bloques
	int posPila;						// posici�n actual de la pila

	Comando* manejadores[0x1c];			// tabla con los manejadores para cada comando	

// m�todos
public:
	// m�todos para la generaci�n de bloques
	void genera(UINT8 *datosPantalla);
	void Parchea(UINT8 numPantalla);
	void iniciaInterpretacionBloque(UINT8 *tilesBloque, bool modificaTiles, int altura);

	// m�todos de dibujado de tiles
	void grabaTile(int tile);
	void actualizaTile(int tile, TileInfo *tileDesc);
	void limpiaPantalla(int color);
	void dibujaBufferTiles();
	
	bool dibujaBufferTiles2();

	// operaciones sobre registros y expresiones del generador de bloques
	int leeDatoORegistro(int *posReg);
	int obtenerRegistro(int reg, int *posReg);
	int actualizaRegistro(int reg, int delta);
	int evaluaExpresion(int rdo);

	// operaciones sobre la pila
	void push(int data);
	int pop();

	// inicializaci�n y limpieza
	GeneradorPantallas();
	~GeneradorPantallas();

	// m�todos de ayuda
	int obtenerDir(int direccion);

	int tilesAnimationCounter;
	int x, y;
	int abajo, derecha, arriba, izquierda;	
protected:
	void transformaCoordBloqueRejilla(int altura);
	void interpretaComandos();

	void generaMascaras();

	void dibujaTile(int x, int y, int num);
	void dibujaTira(int &x, int &y, int deltaX, int deltaY, int veces);
};


}

#endif	// _GENERADOR_PANTALLAS_H_

// AbadiaDriver.h
//
//	Driver para "La abad�a del crimen"
//
//	Dedicado a la memoria de Paco Menendez
//
//	Reingenier�a inversa por Manuel Abad�a <vigasoco@gmail.com>
//
//	Notas de la conversi�n:
//	=======================
//		* la conversi�n se ha hecho trabajando sobre la versi�n de Amstrad CPC 6128, que fue
//		el ordenador en el que se cre� originalmente el juego, entendiendo el c�digo del Z80
//		y creando una serie de objetos que interactuan entre si para que el resultado sea
//		equivalente al juego original.
//
//		* el juego original tiene 2 capas de tiles para generar las pantallas y crear un efecto
//		de profundidad. Para una gran parte de las pantallas, estas 2 capas son suficiente, 
//		aunque para algunas pantallas con gran cantidad de bloques que coinciden en los mismos
//		tiles, como la pantalla 0x34, esto provoca ligeras imperfecciones gr�ficas. Probablemente 
//		en el juego original no se usaron m�s capas porque cada capa adicional supone 960 bytes,
//		una pasada m�s a la hora de dibujar la pantalla y una pasada m�s en el bucle interior del
//		dibujado de sprites, y este incremento en memoria y c�lculos consigue una mejora gr�fica
//		muy peque�a. Aprovechando que ahora los ordenadores son m�s potentes, he generalizado los
//		algoritmos que tratan con las capas de forma que el n�mero de capas que se usan viene 
//		determinado por la constante nivelesProfTiles de la clase GeneradorPantallas (que por
//		defecto vale 3, para que las pantallas se vean sin errores gr�ficos).
//
//		* el juego posee un int�rprete para la construcci�n de los bloques que forman las
//		pantallas. Actualmente se interpretan los bloques ya compilados (que est�n en los 
//		datos originales), aunque estar�a bien crear ficheros con los scripts de cada bloque 
//		y un compilador de bloques, de forma que se interprete el c�digo que genere el 
//		compilador de bloques en vez del que viene en los datos originales.
//
//		* el comportamiento de los personajes se basa en el int�rprete de scripts que trae el
//		juego original. En la conversi�n, el comportamiento se ha pasado directamente a C++.
//
//	Por hacer:
//	==========
//		* a�adir sonido -> A�adido en la version SDL
//
//		* cargar/grabar partidas -> A�adido en la version SDL
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _ABADIA_DRIVER_H_
#define _ABADIA_DRIVER_H_

#include "GameDriver.h"
#include "Juego.h"

class CPC6128;						// definido en CPC6128.h

class AbadiaDriver : public GameDriver
{
// campos
protected:
	Abadia::Juego *_abadiaGame;		// objeto principal del juego
	CPC6128 *cpc6128;				// clase de ayuda para realizar operaciones gr�ficas	
	UINT8 *romsPtr;					// puntero a los datos del juego una vez extraidos del disco
							// y de GraficosCPC y GraficosVGA
							// los wav del sonido no van aqui

// m�todos
public:
	// iniciaci�n y limpieza
	AbadiaDriver();
	virtual ~AbadiaDriver();
	virtual void preRun();
	virtual void runSync();
	virtual void runAsync();
	virtual void showMenu();
	virtual void changeState(int newState);
	virtual void render(IDrawPlugin *dp);
	virtual void showGameLogic(IDrawPlugin *dp);

	// template methods overrides
	virtual void videoInitialized(IDrawPlugin *dp);
	virtual void videoFinalizing(IDrawPlugin *dp);
	int state;

protected:
	// template methods overrides
	virtual void filesLoaded();
	virtual void finishInit();
	virtual void end();

	// m�todos de ayuda
	void createGameDataEntities();
	void createGameGfxDescriptions();
	void createGameInputsAndDips();

	void drawGrid(IDrawPlugin *dp);
	void drawDestPos(IDrawPlugin *dp);

	void reOrderAndCopy(UINT8 *src, UINT8 *dst, int size);
};

#endif	// _ABADIA_DRIVER_H_

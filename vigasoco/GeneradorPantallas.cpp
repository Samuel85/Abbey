// GeneradorPantallas.cpp
//
/////////////////////////////////////////////////////////////////////////////



#include "Comandos.h"
#include "cpc6128.h"
#include "Juego.h"
#include "IDrawPlugin.h"

#include "GeneradorPantallas.h"

// para printf
//#include <stdio.h>

using namespace Abadia;

/////////////////////////////////////////////////////////////////////////////
// inicializaci�n y limpieza
/////////////////////////////////////////////////////////////////////////////

GeneradorPantallas::GeneradorPantallas()
{
	roms = elJuego->roms;
	cpc6128 = elJuego->cpc6128;

	cambioSistemaCoord = false;
	estadoOpsX[0] = 1;
	estadoOpsX[1] = 1;
	estadoOpsX[2] = 1;
	estadoOpsX[3] = 0;

	// crea los comandos de generaci�n de bloques
	manejadores[0xff^0xff] = new EndBlock();
	manejadores[0xfe^0xff] = new WhileParam1();
	manejadores[0xfd^0xff] = new WhileParam2();
	manejadores[0xfc^0xff] = new PushTilePos();
	manejadores[0xfb^0xff] = new PopTilePos();
	manejadores[0xfa^0xff] = new EndWhile();
	manejadores[0xf9^0xff] = new DrawTileDecY();
	manejadores[0xf8^0xff] = new DrawTileIncX();
	manejadores[0xf7^0xff] = new UpdateReg();
	manejadores[0xf6^0xff] = new IncTilePosY();
	manejadores[0xf5^0xff] = new IncTilePosX();
	manejadores[0xf4^0xff] = new DecTilePosY();
	manejadores[0xf3^0xff] = new DecTilePosX();
	manejadores[0xf2^0xff] = new UpdateTilePosY();
	manejadores[0xf1^0xff] = new UpdateTilePosX();
	manejadores[0xf0^0xff] = new IncParam1();
	manejadores[0xef^0xff] = new IncParam2();
	manejadores[0xee^0xff] = new DecParam1();
	manejadores[0xed^0xff] = new DecParam2();
	manejadores[0xec^0xff] = new Call();
	manejadores[0xeb^0xff] = new DrawTileDecX();
	manejadores[0xea^0xff] = new ChangePC();
	manejadores[0xe9^0xff] = new FlipX();
	manejadores[0xe8^0xff] = new FlipX();
	manejadores[0xe7^0xff] = new FlipX();
	manejadores[0xe6^0xff] = new FlipX();
	manejadores[0xe5^0xff] = new FlipX();
	manejadores[0xe4^0xff] = new CallPreserve();

	// genera las m�scaras para combinar los pixels
	generaMascaras();
	tilesAnimationCounter = 0;
	x = 7;
	y = 8;

	abajo   = 4;
	derecha = 1;
	arriba  = 5;
	izquierda=2;

}

GeneradorPantallas::~GeneradorPantallas()
{
	// elimina los comandos de generaci�n de bloques
	for (int i = 0; i < 0x1c; i++){
		delete manejadores[i];
	}
}

void GeneradorPantallas::Parchea(UINT8 numPantalla)
{
//	printf("GeneradorPantallas::Parchea %d\n",numPantalla);
//	fflush(stdout);

/* Cedidos por Antonio Giner del RemakePC 
; Parches usados para corregir los defectos de los escenarios
NO USO TODOS YA QUE ALGUNOS SE VEN PEOR O NO SE LO QUE HACEN

ParchesEscenarios LABEL DWORD

db 03h,2,4,8,9,9,13,6,98,154
db 06h,1,8,4,40,222
db 08h,1,14,7,10,14
db 0ah,3,3,7,27,43,3,8,0,70,0,19,10,127
db 0ch,2,4,7,43,27,4,8,0,70,0bh,1,0,19,0,10
db 0dh,1,10,1,7,143
db 0eh,3,0,7,9,163,2,5,9,163,4,3,9,163
db 0fh,4,10,11,222,55,10,12,0,223,4,5,222,55,4,6,4,223
db 10h,1,14,2,27,52
db 11h,1,3,1,10,40
db 12h,1,11,2,9,43
db 14h,2,13,5,222,55,13,6,223,179
db 15h,3,7,0,7,143,13,6,7,143,10,13,27,52
db 18h,3,0,8,2,163,2,6,2,163,2,16,224,55
db 1fh,2,8,8,1,222,8,9,0,223
db 23h,3,9,14,8,239,10,13,8,241,9,16,9,155
db 24h,2,15,0,7,143,15,7,7,143
db 27h,4,1,19,7,143,9,0,7,143,7,11,5,7,8,11,5,2
db 28h,1,9,19,1,40
db 2ah,4,0,12,98,3,10,6,27,43,10,7,79,70,13,11,43,222
db 2bh,4,1,6,2,163,3,4,2,163,5,2,2,163,7,0,2,163
db 2ch,4,11,2,7,143,13,4,7,143,15,6,7,143,1,4,2,163
db 2eh,1,1,5,9,199
db 2fh,3,1,17,4,155,1,18,4,151,1,19,4,154
db 30h,4,4,7,4,155,4,8,4,151,4,9,4,154,4,10,4,154
db 31h,5,8,0,10,241,14,11,4,155,14,12,4,151,14,13,4,154,14,14,4,154
db 34h,6,5,2,9,198,5,7,4,155,5,8,4,151,5,9,4,154,5,10,4,154,7,0,9,191
db 3ch,1,15,9,38,207
db 3eh,2,10,1,38,203,11,2,38,203
db 40h,2,0,3,222,55,0,4,5,223
db 41h,1,8,11,222,55
db 42h,3,4,14,27,52,15,4,222,55,15,5,0,223
db 44h,2,0,19,222,55,11,17,3,118
db 45h,6,6,6,52,27,6,7,70,130,7,7,10,52,7,8,10,128,7,9,70,50,9,15,5,54
db 47h,4,10,4,3,50,10,5,2,51,12,7,9,51,15,10,9,51
db 4ah,1,9,5,7,30
db 4bh,1,6,3,7,53
db 4ch,3,12,6,27,52,12,7,0,70,15,19,0,5
db 0ffh */
struct parche {
	int pantalla; // numero de pantalla
	int x;
	int y;
	int fg; // elemento del primer plano
	int bg;	// elemento del fondo
	} parches[]=
	{
// Algunos errores son porque no coincide el n� de tile
// del mapa CPC y los tiles del remake
// para no tener que tocar
// todas las pantallas donde sale
// se actualiza el n� de tile en ::grabaTile
// y no se toca aqui


// En la habitacion 0
// hay un defecto en una escalera
// pero en el remake no hay ningun
// parche y en el visor Java de Manuel
// Pazos tampoco se ve
// Arreglado en :grabaTile



// Arregla un pico de una escalera
// que sobresale por el muro
// que hay delante
{ 0x03,4,8,9,9 },
{ 0x03,13,6,98,154 } ,


// En la habitacion 5
// hay un defecto en una escalera
// pero en el remake no hay ningun
// parche y en el visor Java de Manuel
// Pazos tampoco se ve
// Arreglado en :grabaTile

// Con este parche un trozo
// de suelo queda por delante
// de los sprites ...
// no lo aplico
//{ 0x06,8,4,40,222 } ,

// no se que arregla esto
// asi que no lo aplico
//{ 0x08,14,7,10,14 } ,

//un trozo de muro queda
//por delante de los sprites
//no aplico este parche
//{ 0x0a,3,7,27,43},
//{ 0x0a,3,8,0,70} ,
//{ 0x0a,0,19,10,127 },
//aparte en esta pantalla
//tenemos el mismo tile cambiado
//de la escalera de la pantalla
//0,42,etc.
//solucionado en :grabaTile

// El muro de debajo de la barandilla
// queda siempre por encima de los sprites
// asi que no aplico este parche
//{ 0x0c,4,7,43,27},
{ 0x0c,4,8,0,70},


// No veo que cambia esto
// no lo aplico
//{ 0x0b,0,19,0,10 },


// Arregla el tipico ventanal
// que se ve bien en CPC
// y mal con los tiles VGA
{ 0x0d,10,1,7,143 },


// Arregla los tipicos ventanales
// que se ve bien en CPC
// y mal con los tiles VGA
{ 0x0e,0,7,9,163} ,
{ 0x0e,2,5,9,163},
{ 0x0e,4,3,9,163 },

// No aplico estos porque 
// uno de los tiles de la
// escalera queda por delante
// de los sprites y ademas
// no veo que defecto grafico
// arregla
//{ 0x0f,10,11,222,55},
//{ 0x0f,10,12,0,223},
//{ 0x0f,4,5,222,55},
//{ 0x0f,4,6,4,223 },


// No se que arregla
// no lo aplico
//{ 0x10,14,2,27,52 },

// No se que arregla
// no lo aplico
//{ 0x11,3,1,10,40 },

// No se que arregla
// no lo aplico
//{ 0x12,11,2,9,43 },


//Con estos cuando pasa el sprite
//por las escaleras uno de los
//tiles queda por delante ??
//mejor no lo aplico
//{ 0x14,13,5,222,55},
//{ 0x14,13,6,223,179 } ,


// Arregla los tipicos ventanales
// que se ve bien en CPC
// y mal con los tiles VGA
{ 0x15,7,0,7,143},
{ 0x15,13,6,7,143},
// No se que arregla
// no lo aplico
//{ 0x15,10,13,27,52 },

	/*
//pruebas en la 0x17
//a la entrada para probar tiles
{ 23 , 0,0,0,0 },
{ 23 , 1,0,0,0 },
{ 23 , 2,0,0,0 },
{ 23 , 0,1,0,0 },
//{ 23 , 1,1,107,107 },
{ 23 , 1,1,237,237 }, // este es el que nos falta para la escalera
{ 23 , 2,1,0,0 },
{ 23 , 0,2,0,0 },
{ 23 , 1,2,0,0 },
{ 23 , 2,2,0,0 },
*/

// Arregla los tipicos ventanales
// que se ve bien en CPC
// y mal con los tiles VGA
{ 0x18,0,8,2,163},
{ 0x18,2,6,2,163},
// no parece arreglar nada
// y ademas el tile queda
// por delante de los sprites
// no lo aplico
//{ 0x18,2,16,224,55 },

// Corrigen un peque�o
// y casi inapreciable detale
{ 0x1f,8,8,1,222},
{ 0x1f,8,9,0,223 },


// 0x2A tenia un fallo en las escaleras
// pero arreglado en :grabaTile

// no veo que arreglan de la columna
// no los aplico
//{ 0x23,9,14,8,239},
//{ 0x23,10,13,8,241},
//{ 0x23,9,16,9,155 },

// Arregla los tipicos ventanales
// que se ve bien en CPC
// y mal con los tiles VGA

{ 0x24,15,0,7,143 },
{ 0x24,15,7,7,143 },


// Arregla los tipicos ventanales
// que se ve bien en CPC
// y mal con los tiles VGA
{ 0x27,1,19,7,143},
{ 0x27,9,0,7,143},
// No se que arreglan
// no los aplico
{ 0x27,7,11,5,7},
{ 0x27,8,11,5,2 },


// No veo que arregla nada
// no lo aplico
//{ 0x28,9,19,1,40 },

// En la habbitacion 42 (2a)
// se ven 2 escalones mal
// (el mismo defecto de la pantalla 0)
// Arreglado en :grabaTile
// no veo que arreglan los parches
// copiados del remake 
// ademas provocan que los ladrillos
// de una escalera queden siempre
// por delante de los sprites
// no los aplico
// { 0x2a,0,12,98,3},
// { 0x2a,10,6,27,43},
// { 0x2a,10,7,79,70},
// { 0x2a,13,11,43,222 },

// Arregla los tipicos ventanales
// que se ve bien en CPC
// y mal con los tiles VGA
{ 0x2b,1,6,2,163 },
{ 0x2b,3,4,2,163 },
{ 0x2b,5,2,2,163 },
{ 0x2b,7,0,2,163 },
{ 0x2c,11,2,7,143},
{ 0x2c,13,4,7,143},
{ 0x2c,15,6,7,143},
{ 0x2c,1,4,2,163 },

// No veo que arregla
// no lo aplico
//{ 0x2e,1,5,9,199 },

// No arregla. Incluso
// se ve peor aplicando
// estos parches
//{ 0x2f,1,17,4,155},
//{ 0x2f,1,18,4,15},
//{ 0x2f,1,19,4,154 },

// No veo que arreglen nada
// no los aplico
//{ 0x30,4,7,4,155},
//{ 0x30,4,8,4,151},
//{ 0x30,4,9,4,154},
//{ 0x30,4,10,4,154 },


// No veo que arreglen nada
// no los aplico
//{ 0x31,8,0,10,241},
//{ 0x31,14,11,4,155},
//{ 0x31,14,12,4,151},
//{ 0x31,14,13,4,154},
//{ 0x31,14,14,4,154 },



// No veo que arreglen nada
// no los aplico
//{ 0x34,5,2,9,198},
//{ 0x34,5,7,4,155},
//{ 0x34,5,8,4,151},
//{ 0x34,5,9,4,154},
//{ 0x34,5,10,4,154},
//{ 0x34,7,0,9,191 },


// No veo que arreglen nada
// no los aplico
//{ 0x3c,15,9,38,207 },
//{ 0x3e,10,1,38,203},
//{ 0x3e,11,2,38,203 },


// No veo que arreglen nada
// no los aplico
//{ 0x40,0,3,222,55},
//{ 0x40,0,4,5,223 },

// Un trozo de suelo queda delante
// de los sprites
// no usar este parche
//{ 0x41,8,11,222,55 },

// Lo de abajo de una barandilla queda
// delante de los sprites
// no usar este parche
//{ 0x42,4,14,27,52},
//{ 0x42,15,4,222,55},
//{ 0x42,15,5,0,223 },


// No veo que arreglen nada
// no los aplico
//{ 0x44,0,19,222,55},
//{ 0x44,11,17,3,118 },
	
//REVISANDO VOY POR AQUI
// algo queda delante de los sprites
// no veo que arregle nada
// no usar este parche
//{ 0x45,6,6,52,27},
//{ 0x45,6,7,70,130},
//{ 0x45,7,7,10,52},
//{ 0x45,7,8,10,128},
//{ 0x45,7,9,70,50},
//{ 0x45,9,15,5,54 },


// la pantalla tiene unos defectos muy feos
// pero no veo que mejore con esto
// no los aplico
//{ 0x47,10,4,3,50 },
//{ 0x47,10,5,2,51 },
//{ 0x47,12,7,9,51 },
//{ 0x47,15,10,9,51 },

// no veo que arregla
// no lo uso
//{ 0x4a,9,5,7,30 },


// no veo que arregla
// no lo uso
//{ 0x4b,6,3,7,53 },

// una barandilla
// queda por delante de los 
// sprites
// no uso estos parches
//{ 0x4c,12,6,27,52},
//{ 0x4c,12,7,0,70},
//{ 0x4c,15,19,0,5 }  
 
};

	for (	int i=0;
		i<sizeof(parches)/sizeof(struct parche) &&
		parches[i].pantalla<=numPantalla;
		i++)
	{
		if(parches[i].pantalla==numPantalla) 
		{	
			// TODO nivprofTiles debe ser al menos 2
			// 2 es lo usado en la version CPC
			// 3 es lo que usaba Vigasoco
			// 4 es lo que usa VigasocoSDL
			// los parches del remake solo son para
			// los 2 niveles
			// TODO poner un assert o comprobar
			// para evitar errores
			int a=nivelesProfTiles;
			bufferTiles[parches[i].y][parches[i].x].tile[a]=
				parches[i].fg;
			bufferTiles[parches[i].y][parches[i].x].tile[a-1]=
				parches[i].bg;
			if(numPantalla==0x14) {
/*				printf("prof %d %d %d %d\nprofy %d %d %d %d\n",
			bufferTiles[parches[i].y][parches[i].x].profX[0],
			bufferTiles[parches[i].y][parches[i].x].profX[1],
			bufferTiles[parches[i].y][parches[i].x].profX[2],
			bufferTiles[parches[i].y][parches[i].x].profX[3],
			bufferTiles[parches[i].y][parches[i].x].profY[0],
			bufferTiles[parches[i].y][parches[i].x].profY[1],
			bufferTiles[parches[i].y][parches[i].x].profY[2],
			bufferTiles[parches[i].y][parches[i].x].profY[3] );*/
/*				printf("prof %d %d \nprofy %d %d \n",
			bufferTiles[parches[i].y][parches[i].x].profX[0],
			bufferTiles[parches[i].y][parches[i].x].profX[1],
			bufferTiles[parches[i].y][parches[i].x].profY[0],
			bufferTiles[parches[i].y][parches[i].x].profY[1]
			 );*/
//			bufferTiles[parches[i].y][parches[i].x].profX[0]=0;
////			bufferTiles[parches[i].y][parches[i].x].profX[1]=0;
////			bufferTiles[parches[i].y][parches[i].x].profY[0]=0;
//			bufferTiles[parches[i].y][parches[i].x].profY[1]=0;

			}



		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// m�todos de generaci�n de bloques
//
// Los bloques puede tener 3 o 4 bytes de longitud. El formato del bloque es:
//		byte 0:
//			bits 7-1: tipo del bloque a construir
//			bit 0: si es 1 indica que el bloque puede ocultar a los sprites
//		byte 1:
//			bits 7-5: par�metro 1 (su funci�n depende del tipo de bloque a construir)
//			bits 4-0: posici�n inicial en x (sistema de coordenadas del buffer de tiles)
//		byte 2:
//			bits 7-5: par�metro 2 (su funci�n depende del tipo de bloque a construir)
//			bits 4-0: posici�n inicial en y (sistema de coordenadas del buffer de tiles)
//		byte 3: altura inicial del bloque
//
/////////////////////////////////////////////////////////////////////////////

// genera los capas de tiles que forman una pantalla dados los datos de los bloques que la forman
void GeneradorPantallas::genera(UINT8 *datosPantalla)
{
	// inicia la pila
	posPila = 0;

	// repite el proceso de generaci�n de bloques hasta que no se encuentre el marcador de fin de datos
	while (*datosPantalla != 0xff){

		if (datosPantalla[0] == 0x2f){
			if ((datosPantalla[1] == 0x0e) && (datosPantalla[2] == 0xb0) && (datosPantalla[3] == 0x0e)){
				int a = 0;
			}
		}
		// los 7 bits m�s significativos del primer byte de los datos indican el tipo de bloque a construir
		int despTipoBloque = obtenerDir(0x156d + (datosPantalla[0] & 0xfe));

		// obtiene un puntero a los tiles que forman el bloque
		UINT8 *tilesBloque = &roms[obtenerDir(despTipoBloque)];
		
		// avanza el desplazamiento hasta los comandos que forman el bloque
		comandosBloque = despTipoBloque + 2;

		// lee la posici�n desde donde se dibujar� el bloque
		tilePosX = datosPantalla[1] & 0x1f;
		tilePosY = datosPantalla[2] & 0x1f;

		// lee los par�metros del bloque
		datosBloque[12] = (datosPantalla[1] >> 5) & 0x07;
		datosBloque[13] = (datosPantalla[2] >> 5) & 0x07;

		// inicia la profundidad del bloque en la rejilla a (0, 0)
		datosBloque[15] = datosBloque[16] = 0;

		int altura = 0xff;

		// si la entrada es de 4 bytes, el cuarto byte indica la altura del bloque, y tambi�n implica
		// que se calcular�n los datos de profundidad a lo largo del proceso de generaci�n del bloque
		if (datosPantalla[0] & 0x01){
			altura = datosPantalla[3];
			datosPantalla++;
		}

		// avanza a la siguiente entrada
		datosPantalla += 3;

		// guarda la altura para despu�s
		datosBloque[14] = altura;

		// inicia la evaluaci�n del bloque
		iniciaInterpretacionBloque(tilesBloque, true, altura);
	}
}

// realiza la iniciaci�n necesaria para interpretar los datos de un bloque
void GeneradorPantallas::iniciaInterpretacionBloque(UINT8 *tilesBloque, bool modificaTiles, int altura)
{
	if (modificaTiles){
		// copia los datos de los tiles que forman el bloque al buffer
		for (int i = 0; i < 12; i++){
			datosBloque[i] = tilesBloque[i];
		}
	}

	// transforma la posici�n del bloque en el buffer de tiles al sistema de coordenadas de la rejilla
	transformaCoordBloqueRejilla(altura);

	// comienza a interpretar los comandos
	interpretaComandos();
}

// transforma la posici�n del bloque en el buffer de tiles al sistema de coordenadas de la rejilla
// las ecuaciones de cambio de sistema de coordenadas son:
// mapa de tiles -> rejilla:		Xrejilla = Ymapa + Xmapa - 15
//									Yrejilla = Ymapa - Xmapa + 16
void GeneradorPantallas::transformaCoordBloqueRejilla(int altura)
{
	// solo realiza la transformaci�n si el bloque puede ocultar a los sprites
	if (altura != 0xff){
		datosBloque[15] = (tilePosY + altura/2) + tilePosX - 15;
		datosBloque[16] = (tilePosY + altura/2) - tilePosX + 16;
	}
}

// interpreta los comandos que se indican en comandosBloque
void GeneradorPantallas::interpretaComandos()
{
	while (true){
		UINT8 *comando = &roms[comandosBloque];
		// obtiene el n�mero de la rutina que est� complementado
		int numRutina = (*comando) ^ 0xff;

		comandosBloque++;

		// llama a la rutina correspondiente para que realice su procesamiento
		bool terminado = manejadores[numRutina]->ejecutar(this);

		// si era el �ltimo comando de generaci�n del bloque, sale
		if (terminado){
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// m�todos de ayuda para los comandos de dibujo de los tiles
/////////////////////////////////////////////////////////////////////////////

// actualiza los datos de un tile del buffer de tiles
void GeneradorPantallas::actualizaTile(int tile, TileInfo *tileDesc)
{
	assert ((tile >= 0) && (tile < 0x100));

	int newProfX = datosBloque[15];
	int newProfY = datosBloque[16];

	for (int i = 1; i < nivelesProfTiles; i++){
		// obtiene los valores de mayor profundidad de esa entrada
		int oldProfX = tileDesc->profX[i];
		int oldProfY = tileDesc->profY[i];
		int oldTile = tileDesc->tile[i];

		// si el nuevo elemento que se dibuja en este tile tiene menor profundidad que el elemento que estaba
		// antes en este tile, ajusta la profundidad del elemento anterior a la profundidad del elemento
		if (newProfX < oldProfX){
			if (newProfY < oldProfY){
				oldProfX = newProfX;
				oldProfY = newProfY;
			}
		}
	
		// pasa los datos anteriores de mayor profundidad a la capa de menor profundidad
		tileDesc->profX[i - 1] = oldProfX;
		tileDesc->profY[i - 1] = oldProfY;
		tileDesc->tile[i - 1] = oldTile;
	}

	// graba los nuevos datos de mayor profundidad
	tileDesc->profX[nivelesProfTiles - 1] = newProfX;
	tileDesc->profY[nivelesProfTiles - 1] = newProfY;
	tileDesc->tile[nivelesProfTiles - 1] = tile;
}

// si el tile es visible, actualiza el buffer de tiles
void GeneradorPantallas::grabaTile(int tile)
{
	// comprueba si el tile es de la parte central, trasladando la parte central al origen
	int posX = tilePosX - 8;
	int posY = tilePosY - 8;

	if ((posX < 0) || (posX >= 16)) return;
	if ((posY < 0) || (posY >= 20)) return;

	// Algunos tiles son distintos en abadia.dsk
	// y en el GraficosVGA del remake
	// Los cambiamos aqui para no tener que estar
	// parcheando todas las pantallas en los que se usan
	// TODO: Usar el GraficosVGA del nuevo remake
	// o el tiles.png del remake Java en los que el orden
	// parece correcto. Lo malo del GraficosVGA nuevo
	// es que habr�a que recalcular todas las posiciones
	// de la pantalla de introduccion, los sprites,etc.
	
	// Cambio los tiles de los libros del mapa CPC 
	// a su correspondiente en los graficosVGA
	if (tile==57) tile=201;
	if (tile==58) tile=225;
	// Cambiar un tile de escalera que esta cambiado
	if (tile==105) tile=237;


	// actualiza la informaci�n de esa entrada del tile en el buffer de tiles
	actualizaTile(tile, &bufferTiles[posY][posX]);
}

/////////////////////////////////////////////////////////////////////////////
// operaciones sobre la pila
/////////////////////////////////////////////////////////////////////////////

// mete un dato en la pila
void GeneradorPantallas::push(int data)
{
	assert(posPila < 64);

	pila[posPila] = data;
	posPila++;
}

// saca un dato en la pila
int GeneradorPantallas::pop()
{
	assert(posPila > 0);

	posPila--;
	return pila[posPila];
}

/////////////////////////////////////////////////////////////////////////////
// operaciones sobre registros y expresiones del generador de bloques
/////////////////////////////////////////////////////////////////////////////

// obtiene un valor inmediato o el contenido de un registro
int GeneradorPantallas::leeDatoORegistro(int *posReg)
{
	// lee un dato del buffer
	int dato = roms[comandosBloque];
	comandosBloque++;

	// si el dato es menor que 0x60, es un valor inmediato
	if (dato < 0x60){
		return dato;
	}

	// el 0x82 es un marcador que indica que hay que devolver el siguiente byte
	if (dato == 0x82){
		dato = roms[comandosBloque];
		comandosBloque++;

		return dato;
	}

	// si se cambi� el sentido de las x, intercambia los registros 0x70 y 0x71
	if (dato >= 0x70){
		dato = dato ^ estadoOpsX[3];
	}

	// en otro caso, es una lectura de un registro del buffer con los datos del bloque
	int pos = dato - 0x61;

	// si se nos solicit� la posici�n del registro, la graba
	if (posReg != 0){
		*posReg = pos;
	}

	// devuelve el dato que hab�a en el registro
	return datosBloque[pos];
}

// obtiene el valor de un registro de generaci�n el bloque actual
int GeneradorPantallas::obtenerRegistro(int reg, int *posReg)
{
	assert((reg != 0x82) && (reg > 0x60));

	// si se cambi� el sentido de las x, intercambia los registros 0x70 y 0x71
	if (reg >= 0x70){
		reg = estadoOpsX[3];
	}

	int pos = reg - 0x61;

	// si se nos solicit� la posici�n del registro, la graba
	if (posReg != 0){
		*posReg = pos;
	}

	// devuelve el dato que hab�a en el registro
	return datosBloque[pos];
}

// actualiza un registro de generaci�n del bloque actual
int GeneradorPantallas::actualizaRegistro(int reg, int delta)
{
	assert((reg != 0x82) && (reg > 0x60));

	// si se cambi� el sentido de las x, intercambia los registros 0x70 y 0x71
	if (reg >= 0x70){
		reg = reg ^ estadoOpsX[3];
	}

	int pos = reg - 0x61;

	// actualiza el registro y devuelve el nuevo valor
	datosBloque[pos] = datosBloque[pos] + delta;

	return datosBloque[pos];
}

// evalua una ristra de bytes calculando la expresi�n generada
int GeneradorPantallas::evaluaExpresion(int rdo)
{
	while (true){
		// lee un byte de datos
		int op = roms[comandosBloque];

		// si se ha terminado la expresi�n, sale
		if (op >= 0xc8){
			return rdo;
		}

		// 0x84 indica el cambio de signo de la expresi�n calculada
		if (op == 0x84){
			rdo = -rdo;
			comandosBloque++;
		} else {
			// en otro caso, suma un registro o valor inmediato
			rdo += (INT8)leeDatoORegistro(0);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// m�todos de dibujo del buffer de tiles
/////////////////////////////////////////////////////////////////////////////

// dibuja en pantalla el contenido del buffer de tiles desde el centro hacia fuera
//void GeneradorPantallas::dibujaBufferTiles()
bool GeneradorPantallas::dibujaBufferTiles()
{
	// posici�n inicial en el buffer de tiles
	int x = 7;
	int y = 8;

	// fija las variables de recorrido
	int abajo = 4;	
	int derecha = 1;
	int arriba = abajo + 1;
	int izquierda = derecha + 1;
	
	// milisegundos que esperar entre iteraciones para ver el efecto
	//const int retardo = 100;

	// repite mientras no se complete toda la pantalla visible
	while (abajo < 20){	
	//if (abajo < 20){	
		// guarda el instante actual de tiempo
		//INT64 tIni = timer->getTime();

		// dibuja 4 tiras: una hacia abajo, otra a la derecha, otra hacia arriba y la otra a la izquierda
		dibujaTira(x, y, 0, 1,  abajo);
		dibujaTira(x, y, 1, 0,  derecha);
		dibujaTira(x, y, 0, -1,  arriba);
		dibujaTira(x, y, -1, 0, izquierda); 

		// aumenta el tama�o del rect�ngulo que dibuja
		abajo += 2;
		derecha += 2;
		arriba += 2;
		izquierda += 2;

		// espera un poco para que se vea el resultado
		//timer->sleep(retardo);		
	}
	return false;
}

bool GeneradorPantallas::dibujaBufferTiles2()
{
	// posici�n inicial en el buffer de tiles	
	bool ret = false;

	//int abajo, derecha, arriba, izquierda;		
	

	// milisegundos que esperar entre iteraciones para ver el efecto
	//const int retardo = 100;

	// repite mientras no se complete toda la pantalla visible
	if (abajo < 20)
	{
		// dibuja 4 tiras: una hacia abajo, otra a la derecha, otra hacia arriba y la otra a la izquierda
		dibujaTira(x, y, 0, 1,  abajo);
		dibujaTira(x, y, 1, 0,  derecha);
		dibujaTira(x, y, 0, -1,  arriba);
		dibujaTira(x, y, -1, 0, izquierda); 

		// aumenta el tama�o del rect�ngulo que dibuja
		abajo += 2;
		derecha += 2;
		arriba += 2;
		izquierda += 2;		
		ret = false;
	}
	else
	{				
		x = 7;
		y = 8;

		abajo   = 4;
		derecha = 1;
		arriba  = 5;
		izquierda=2;
		ret = true;
	}
	return ret;
}



// dibuja una tira de tiles
void GeneradorPantallas::dibujaTira(int &x, int &y, int deltaX, int deltaY, int veces)
{
	// para cada tile de la tira
	for (int i = 0; i < veces; i++){
		// por cada capa de profundidad
		for (int k = 0; k < nivelesProfTiles; k++){
			// obtiene el n�mero de tile asociado a esta profundidad del buffer de tiles
			int tile = bufferTiles[y][x].tile[k];

			// si hay alg�n tile, lo dibuja
			/* CPC
			if (tile != 0){
				dibujaTile(32 + x*16, y*8, tile);
			}
			EN CPC el tile 0 es totalmente transparente, o todo del color del fondo
			y por eso creo que no se pinta
			*/
			// En VGA si pintamos todos los tiles, incluidos el 0
			dibujaTile(32 + x*16, y*8, tile);
		}

		// pasa a la siguiente posici�n
		x = x + deltaX;
		y = y + deltaY;
	}
}
/* codigo original 
// dibuja un tile de 16x8 en la posici�n indicada
void GeneradorPantallas::dibujaTile(int x, int y, int num)
{
	assert((num >= 0x00) && (num < 0x100));

	// halla el desplazamiento del tile (cada tile ocupa 32 bytes)
	UINT8 *tileData = &roms[0x8300 + num*32];

	int numTabla = (num & 0x80) ? 2 : 0;

	// dibuja cada linea del tile
	for (int j = 0; j < 8; j++){
		// repite para 4 bytes (16 pixels)
		for (int i = 0; i < 4; i++){
			// lee un byte del gr�fico (4 pixels)
			int data = *tileData;

			// para cada pixel del byte leido
			for (int k = 0; k < 4; k++){
				// obtiene el color del pixel
				int color = cpc6128->unpackPixelMode1(data, k);

				// obtiene el color del pixel en pantalla
				int oldColor = cpc6128->getMode1Pixel(x, y);

				// combina el color del pixel de pantalla con el nuevo
				color = (oldColor & mascaras[numTabla + 1][color]) | mascaras[numTabla][color];

				// pinta el color resultante
				cpc6128->setMode1Pixel(x, y, color);

				// avanza al siguiente pixel
				x++;
			}

			// avanza la posici�n del gr�fico
			tileData++;
		}
		// pasa a la siguiente l�nea de pantalla
		x -= 16;
		y++;
	}
}
*/
/* la paleta de dia es 
 * 0 - 06 cyan
 * 1 - 14 orange
 * 2 - 03 pastel yellow
 * 3 - 20 black 
 */

// dibuja un tile de 16x8 en la posici�n indicada
void GeneradorPantallas::dibujaTile(int x, int y, int num)
{
	assert((num >= 0x00) && (num < 0x100));

	// CPC
	// halla el desplazamiento del tile (cada tile ocupa 32 bytes)
	//UINT8 *tileData = &roms[0x8300 + num*32];  

	// VGA
	// en el remake de PC cada pixel ocupa un byte y no vienen en pack de 4, como en la version CPC
	// y cada tile ocupa 8*16 bytes
	// parece que con 0x1400 va bien (5888 del tile 6 que localizamos - 768 (6 tiles por 16*8 bytes que tiene un tile)
	// = 5120 , que es 0x1400 en hexadecimal

	UINT8 *tileData =  &roms[0x24000-1 - 0x4000 + 0x1400 + num*8*16]; // pasamos los 0x20000 de la rom CPC y ahi esta el GraficosVGA
		// 0x20000 de los 0x24000 de rom CPC menos los 0x4000 de la presentacion que el puntero de roms se salta
		// en el constructor de Juego.cpp

	int numTabla = (num & 0x80) ? 2 : 0;

	// dibuja cada linea del tile
	for (int j = 0; j < 8; j++){
		// repite para 16 bytes (16 pixels)
		for (int i = 0; i < 16; i++){
			// lee un byte del gr�fico (4 pixels)
			int data = *tileData;
			if(data!=0)
			{					
				cpc6128->setVGAPixel(x, y, data);				
			}
                        // avanza al siguiente pixel
                        x++;

			// avanza la posici�n del gr�fico
			tileData++;
		}
		// pasa a la siguiente l�nea de pantalla
		x -= 16;
		y++;
	}
}


/////////////////////////////////////////////////////////////////////////////
// m�todos de ayuda
/////////////////////////////////////////////////////////////////////////////

// obtiene la direcci�n de memoria que hay en una direcci�n de memoria
int GeneradorPantallas::obtenerDir(int direccion)
{
	UINT8 *aux = &roms[direccion];
	return aux[0] | (aux [1] << 8);
}

// genera las m�scaras necesarias para combinar los gr�ficos
void GeneradorPantallas::generaMascaras()
{
	// rellena las tablas de las m�scaras
	for (int i = 0; i < 4; i++){
		int bit0 = (i >> 0) & 0x01;
		int bit1 = (i >> 1) & 0x01;

		// tabla de m�scaras or (0->0, 1->1, 2->0, 3->3)
		mascaras[0][i] = ((bit1 & bit0) << 1) | bit0;

		// tabla de m�scaras and (0->0, 1->0, 2->3, 3->0)
		mascaras[1][i] = (((bit1 ^ bit0) & bit1) << 1) | ((bit1 ^ bit0) & bit1);

		// tabla de m�scaras or (0->0, 1->0, 2->2, 3->3)
		mascaras[2][i] = ((bit1) << 1) | (bit1 & bit0);

		// tabla de m�scaras and (0->0, 1->3, 2->0, 3->0)
		mascaras[3][i] = (((bit1 ^ bit0) & bit0) << 1) | ((bit1 ^ bit0) & bit0);
	}
}

// prepara el buffer de tiles y limpia el �rea de juego
void GeneradorPantallas::limpiaPantalla(int color)
{
	// limpia el buffer de tiles
	for (int j = 0; j < 20; j++){
		for (int i = 0; i < 16; i++){
			for (int k = 0; k < nivelesProfTiles; k++){
				bufferTiles[j][i].profX[k] = 0;
				bufferTiles[j][i].profY[k] = 0;
				bufferTiles[j][i].tile[k] = 0;
			}
		}
	}

	// limpia el �rea de juego
	cpc6128->fillMode1Rect(32, 0, 256, 160, color);
}

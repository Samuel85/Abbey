// Comandos.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "Comandos.h"
#include "Juego.h"
#include "GeneradorPantallas.h"
#include "MotorGrafico.h"

using namespace Abadia;

/////////////////////////////////////////////////////////////////////////////
// comandos para saltos
/////////////////////////////////////////////////////////////////////////////

// cambia el origen de los datos del generador de bloques
bool ChangePC::ejecutar(GeneradorPantallas *gen)
{
	gen->comandosBloque = gen->obtenerDir(gen->comandosBloque);

	return false;
}

void call(GeneradorPantallas *gen, bool modificaTiles)
{
	// guarda el estado necesario para reanudar la interpretación del bloque
	gen->push(gen->tilePosX);
	gen->push(gen->tilePosY);

	gen->push(gen->estadoOpsX[0]);
	gen->push(gen->estadoOpsX[1]);
	gen->push(gen->estadoOpsX[2]);
	gen->push(gen->estadoOpsX[3]);

	gen->push(gen->datosBloque[12]);
	gen->push(gen->datosBloque[13]);
	gen->push(gen->datosBloque[14]);
	gen->push(gen->datosBloque[15]);
	gen->push(gen->datosBloque[16]);

	// obtiene un puntero a las características del bloque
	int despTipoBloque = gen->obtenerDir(gen->comandosBloque);
	UINT8 *caractBloque = &gen->roms[despTipoBloque];
	gen->comandosBloque = gen->comandosBloque + 2;

	gen->push(gen->comandosBloque);

	// obtiene un puntero a los tiles que forman el bloque
	UINT8 *tilesBloque = &gen->roms[gen->obtenerDir(despTipoBloque)];

	// avanza el puntero hasta los comandos que forman el bloque
	gen->comandosBloque = despTipoBloque + 2;

	// interpreta otro bloque
	gen->iniciaInterpretacionBloque(tilesBloque, modificaTiles, gen->datosBloque[14]);

	// recupera los valores introducidos en la pila
	gen->comandosBloque = gen->pop();

	gen->datosBloque[16] = gen->pop();
	gen->datosBloque[15] = gen->pop();
	gen->datosBloque[14] = gen->pop();
	gen->datosBloque[13] = gen->pop();
	gen->datosBloque[12] = gen->pop();

	gen->estadoOpsX[3] = gen->pop();
	gen->estadoOpsX[2] = gen->pop();
	gen->estadoOpsX[1] = gen->pop();
	gen->estadoOpsX[0] = gen->pop();

	gen->tilePosY = gen->pop();
	gen->tilePosX = gen->pop();
}

// interpreta otro bloque, sin modificar los tiles que se usan
bool CallPreserve::ejecutar(GeneradorPantallas *gen)
{
	// indica que se ha cambiado el sentido de las x
	gen->cambioSistemaCoord = true;

	// interpreta otro bloque sin modificar los tiles que se usan
	call(gen, false);

	return false;
}

// interpreta otro bloque, sin modificar los tiles que se usan
bool Call::ejecutar(GeneradorPantallas *gen)
{
	// interpreta otro bloque modificando los tiles que se usan
	call(gen, true);

	return false;
}

/////////////////////////////////////////////////////////////////////////////
// método de ayuda para los bucles
/////////////////////////////////////////////////////////////////////////////

void avanzaHastaFinDeWhile(GeneradorPantallas *gen)
{
	int profWhile = 1;

	// mientras no se hayan pasado las instrucciones del while
	while (profWhile > 0){
		int dato = gen->roms[gen->comandosBloque];

		// si encuentra un marcador, avanza 2 bytes
		if (dato == 0x82){
			gen->comandosBloque += 2;
		} else {
			// en otro caso, sigue pasando y contando los while a los que entra y a los que sale
			if ((dato == 0xfe) || (dato == 0xfd)){
				profWhile++;
			} else if (dato == 0xfa){
				profWhile--;
			}

			gen->comandosBloque++;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// comandos para bucles
/////////////////////////////////////////////////////////////////////////////

// inicia la ejecución de una serie de instrucciones mientras el parámetro 1 sea > 0
bool WhileParam1::ejecutar(GeneradorPantallas *gen)
{
	// obtiene el valor del parámetro 1
	int aux = gen->obtenerRegistro(0x6d, 0);

	// si el bucle se va a ejecutar alguna vez, inserta en la pila la dirección de retorno y el valor actual del parámetro 1
	if (aux > 0){
		gen->push(gen->comandosBloque);
		gen->push(aux);
	} else {
		// en otro caso, salta las instrucciones hasta el fín del while
		avanzaHastaFinDeWhile(gen);
	}

	return false;
}

// inicia la ejecución de una serie de instrucciones mientras el parámetro 2 sea > 0
bool WhileParam2::ejecutar(GeneradorPantallas *gen)
{
	// obtiene el valor del parámetro 2
	int aux = gen->obtenerRegistro(0x6e, 0);

	// si el bucle se va a ejecutar alguna vez, inserta en la pila la dirección de retorno y el valor actual del parámetro 2
	if (aux > 0){
		gen->push(gen->comandosBloque);
		gen->push(aux);
	} else {
		// en otro caso, salta las instrucciones hasta el fín del while
		avanzaHastaFinDeWhile(gen);
	}

	return false;
}

// termina la ejecución de un blucle mientras
bool EndWhile::ejecutar(GeneradorPantallas *gen)
{
	// recupera el contador del bucle
	int contador = gen->pop();
	contador--;

	// si no se ha terminado todavía
	if (contador > 0){
		// recupera la dirección de inicio del while
		gen->comandosBloque = gen->pop();

		// inserta en la pila la dirección de retorno y el contador
		gen->push(gen->comandosBloque);
		gen->push(contador);
	} else {
		// en otro caso se limpia la pila
		gen->pop();
	}

	return false;
}

/////////////////////////////////////////////////////////////////////////////
// comandos sobre los parámetros
/////////////////////////////////////////////////////////////////////////////

// incrementa el primer parámetro en el buffer de los datos del bloque
bool IncParam1::ejecutar(GeneradorPantallas *gen)
{
	gen->actualizaRegistro(0x6d, 1);

	return false;
}

// decrementa el primer parámetro en el buffer de los datos del bloque
bool DecParam1::ejecutar(GeneradorPantallas *gen)
{
	gen->actualizaRegistro(0x6d, -1);

	return false;
}

// incrementa el segundo parámetro en el buffer de los datos del bloque
bool IncParam2::ejecutar(GeneradorPantallas *gen)
{
	gen->actualizaRegistro(0x6e, 1);

	return false;
}

// decrementa el segundo parámetro en el buffer de los datos del bloque
bool DecParam2::ejecutar(GeneradorPantallas *gen)
{
	gen->actualizaRegistro(0x6e, -1);

	return false;
}

/////////////////////////////////////////////////////////////////////////////
// comandos sobre la posición
/////////////////////////////////////////////////////////////////////////////

// incrementa la coordenada x del buffer de tiles
bool IncTilePosX::ejecutar(GeneradorPantallas *gen)
{
	gen->tilePosX = gen->tilePosX + gen->estadoOpsX[0];

	return false;
}

// decrementa la coordenada x del buffer de tiles
bool DecTilePosX::ejecutar(GeneradorPantallas *gen)
{
	gen->tilePosX = gen->tilePosX - gen->estadoOpsX[1];

	return false;
}

// incrementa la coordenada y del buffer de tiles
bool IncTilePosY::ejecutar(GeneradorPantallas *gen)
{
	gen->tilePosY++;

	return false;
}

// decrementa la coordenada y del buffer de tiles
bool DecTilePosY::ejecutar(GeneradorPantallas *gen)
{
	gen->tilePosY--;

	return false;
}

// cambia la coordenada x del buffer de tiles
bool UpdateTilePosX::ejecutar(GeneradorPantallas *gen)
{
	// obtiene el valor inicial de la expresión
	int rdo = gen->leeDatoORegistro(0);

	// evalua una expresión
	rdo = gen->evaluaExpresion(rdo);

	// modifica la posición en x en el buffer de tiles
	gen->tilePosX = gen->tilePosX + rdo;

	return false;
}


// cambia la coordenada y del buffer de tiles
bool UpdateTilePosY::ejecutar(GeneradorPantallas *gen)
{
	// obtiene el valor inicial de la expresión
	int rdo = gen->leeDatoORegistro(0);

	// evalua una expresión
	rdo = gen->evaluaExpresion(rdo);

	// modifica la posición en y en el buffer de tiles
	gen->tilePosY = gen->tilePosY + rdo;

	return false;
}

// guarda en la pila la posición actual en el buffer de tiles
bool PushTilePos::ejecutar(GeneradorPantallas *gen)
{
	gen->push(gen->tilePosX);
	gen->push(gen->tilePosY);

	return false;
}

// recupera de la pila una posición en el buffer de tiles
bool PopTilePos::ejecutar(GeneradorPantallas *gen)
{
	gen->tilePosY = gen->pop();
	gen->tilePosX = gen->pop();

	return false;
}

/////////////////////////////////////////////////////////////////////////////
// comandos de dibujo
/////////////////////////////////////////////////////////////////////////////

// dibuja un tile en el buffer de tiles (si es visible), cambiando la posición actual en el buffer
void dibujaTileYMueve(GeneradorPantallas *gen, int deltax, int deltay)
{
	while (true){
		// lee el siguiente operando del buffer de construcción del bloque
		int num = gen->leeDatoORegistro(0);

		// lee el próximo byte a procesar
		int dato = gen->roms[gen->comandosBloque];

		// si se encuentra una nueva orden, pinta, actualiza la posición y sale
		if (dato >= 0xc8){
			gen->grabaTile(num);
			gen->tilePosX += deltax;
			gen->tilePosY += deltay;

			break;
		}

		gen->comandosBloque++;

		// si se encuentra un 0x80, pinta, actualiza la posición y continúa
		if (dato == 0x80){
			gen->grabaTile(num);
			gen->tilePosX += deltax;
			gen->tilePosY += deltay;
		} else if (dato == 0x81){
			// si lee 0x81, pinta y continúa
			gen->grabaTile(num);
		} else {
			// lee el número de veces que ha de repteir la operación
			int numVeces = gen->leeDatoORegistro(0);

			// repite la misma operación las veces leidas
			for (int i = 0; i < numVeces; i++){
				gen->grabaTile(num);
				gen->tilePosX += deltax;
				gen->tilePosY += deltay;
			}

			// lee el próximo byte a procesar
			dato = gen->roms[gen->comandosBloque];

			// si se encuentra una nueva orden, sale
			if (dato >= 0xc8){
				break;
			} else {
				// en otro caso se salta algo y continúa
				gen->comandosBloque++;
			}
		}
	}
}

// dibuja una serie de tiles y decrementa la coordenada y del buffer de tiles
bool DrawTileDecY::ejecutar(GeneradorPantallas *gen)
{
	dibujaTileYMueve(gen, 0, -1);

	return false;
}

// dibuja una serie de tiles e incrementa la coordenada x del buffer de tiles
bool DrawTileIncX::ejecutar(GeneradorPantallas *gen)
{
	dibujaTileYMueve(gen, gen->estadoOpsX[2], 0);

	return false;
}

// dibuja una serie de tiles y decrementa la coordenada x del buffer de tiles
bool DrawTileDecX::ejecutar(GeneradorPantallas *gen)
{
	dibujaTileYMueve(gen, -1, 0);

	return false;
}

/////////////////////////////////////////////////////////////////////////////
// resto de comandos
/////////////////////////////////////////////////////////////////////////////

// actualiza un registro relacionado con los datos del bloque
bool UpdateReg::ejecutar(GeneradorPantallas *gen)
{
	// lee el registro al que se va a acceder
	int dato = gen->roms[gen->comandosBloque];

	int posReg = -1;

	// obtiene la posición del registro que se va a modificar
	gen->leeDatoORegistro(&posReg);

	// obtiene el valor inicial de la expresión
	int rdo = gen->leeDatoORegistro(0);

	// evalua una expresión
	rdo = gen->evaluaExpresion(rdo);

	// si se modifica un registro de coordenadas locales de la rejilla, ajusta el resultado entre 0 y 100
	if (dato >= 0x70){
		// si no se estaban calculando las coordenadsa locales de la rejilla para este bloque, sale
		if (gen->datosBloque[posReg] == 0){
			return false;
		}

		// si hay desbordamiento, modifica el resultado
		if (rdo > 100){
			rdo = 0;
		}
	}

	// actualiza el registro
	gen->datosBloque[posReg] = rdo;

	return false;
}

// termina la evaluación de un bloque
bool EndBlock::ejecutar(GeneradorPantallas *gen)
{
	bool seCambioSistemaCoord = gen->cambioSistemaCoord;

	gen->cambioSistemaCoord= false;

	// si se empezó a trabajar con respecto al nuevo sistema de coordenadas
	if (!seCambioSistemaCoord){
		gen->estadoOpsX[0] = 1;
		gen->estadoOpsX[1] = 1;
		gen->estadoOpsX[2] = 1;
		gen->estadoOpsX[3] = 0;
	}

	return true;
}

// cambia el sentido de las x
bool FlipX::ejecutar(GeneradorPantallas *gen)
{
	gen->estadoOpsX[0] = -gen->estadoOpsX[0];
	gen->estadoOpsX[1] = -gen->estadoOpsX[1];
	gen->estadoOpsX[2] = -gen->estadoOpsX[2];
	gen->estadoOpsX[3] ^= 0x01;

	return false;
}

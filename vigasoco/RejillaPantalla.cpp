// RejillaPantalla.cpp
//
/////////////////////////////////////////////////////////////////////////////
#include <cassert>
#include "cpc6128.h"
#include "MotorGrafico.h"
#include "Juego.h"
#include "Personaje.h"
#include "RejillaPantalla.h"

using namespace Abadia;

/////////////////////////////////////////////////////////////////////////////
// inicializaci�n y limpieza
/////////////////////////////////////////////////////////////////////////////

RejillaPantalla::RejillaPantalla(MotorGrafico *motorGrafico)
{
	roms = elJuego->roms;
	motor = motorGrafico;
}

RejillaPantalla::~RejillaPantalla()
{
}

/////////////////////////////////////////////////////////////////////////////
// tabla para el c�lculo del avance seg�n las posiciones que ocupa el personaje
/////////////////////////////////////////////////////////////////////////////

int RejillaPantalla::calculoAvancePosicion[4][8] = {
	{  0, +1,   -1,  0,   +1, -2,   +2, -1 },
	{ +1,  0,    0, +1,   -2, -2,   -1, -2 },
	{  0, -1,   +1,  0,   -2, +1,   -2, +1 },
	{ -1,  0,    0, -1,   +1, +1,   +1, +2 }
};

/////////////////////////////////////////////////////////////////////////////
// 
/////////////////////////////////////////////////////////////////////////////


// dada la posici�n de un personaje, calcula los m�nimos valores visibles del �rea de juego
void RejillaPantalla::calculaMinimosValoresVisibles(Personaje *pers)
{
	minPosX = (pers->posX & 0xf0) - 4;
	minPosY = (pers->posY & 0xf0) - 4;
	minAltura = motor->obtenerAlturaBasePlanta(pers->altura);
}

// dado un personaje, rellena la rejilla con la informaci�n de altura de la planta recortada para la pantalla
void RejillaPantalla::rellenaAlturasPantalla(Personaje *pers)
{
	// limpia la matriz de alturas
	for (int j = 0; j < 24; j++){
		for (int i = 0; i < 24; i++){
			bufAlturas[j][i] = 0;
		}
	}

	// obtiene los m�nimos valores visibles para la pantalla en la que se encuentra el personaje
	calculaMinimosValoresVisibles(pers);

	// halla el desplazamiento a los datos de la altura para la planta en la que se encuentra el personaje
	int datosAlturaPlanta[] = { 0x18a00, 0x18f00, 0x19080 };
	UINT8 *datosAltura = &roms[datosAlturaPlanta[motor->obtenerPlanta(minAltura)]];

	// mientras queden datos de altura de la planta
	while ((*datosAltura) != 0xff){
		int tipoBloque = datosAltura[0];

		// si el bloque no es de un tipo conocido, sale
		if (((tipoBloque & 0x07) == 0) || ((tipoBloque & 0x07) >= 6)){
			break;
		}

		int lgtudX = datosAltura[3];
		int lgtudY = datosAltura[4];

		// si la entrada no es de 5 bytes, la longitud se codifica en 4 bits en vez de en 8
		if ((tipoBloque & 0x08) == 0){
			lgtudY = lgtudX & 0x0f;
			lgtudX = (lgtudX >> 4) & 0x0f;
		}
	
		int altura = (tipoBloque >> 4) & 0x0f;
		int posX = datosAltura[1];
		int posY = datosAltura[2];

		// avanza a la siguiente entrada
		if ((tipoBloque & 0x08) == 0){
			datosAltura += 4;
		} else {
			datosAltura += 5;
		}

		lgtudX++;
		lgtudY++;

		// rechaza los bloques que est�n completamente fuera de la zona de pantalla

		// halla la distancia en x entre las coordenadas
		int distX = posX - minPosX;

		// si el bloque empieza antes que el rect�ngulo de recorte
		if (distX < 0){
			// si el bloque termina antes de que empiece la zona visible
			if (-distX >= lgtudX){
				continue;
			}
		} else if (distX >= 24){
			// si el bloque empieza despu�s de que termine la zona visible
			continue;
		}

		// halla la distancia en y entre las coordenadas
		int distY = posY - minPosY;

		// si el bloque empieza antes que el rect�ngulo de recorte
		if (distY < 0){
			// si el bloque termina antes de que empiece la zona visible
			if (-distY >= lgtudY){
				continue;
			}
		} else if (distY >= 24){
			// si el bloque empieza despu�s de que termine la zona visible
			continue;
		}

		// si llega hasta aqu�, alguna parte del bloque es visible, por lo que modifica el buffer de alturas

		// seg�n el tipo de bloque, fija los datos de la altura
		if ((tipoBloque & 0x07) != 5){
			static int incrementos[4][2] = {
				{  1,  0 },
				{  0, -1 },
				{ -1,  0 },
				{  0,  1 }
			};

			// modifica la tabla de alturas con los datos del bloque
			for (int j = 0; j < lgtudY; j++){
				int oldAltura = altura;

				for (int i = 0; i < lgtudX; i++){
					fijaAlturaRecortando(posX + i, posY + j, altura);
					altura += incrementos[(tipoBloque & 0x07) - 1][0];
				}

				altura = oldAltura + incrementos[(tipoBloque & 0x07) - 1][1];
			}
		} else {
			// halla la distancia en x entre las coordenadas
			distX = posX - minPosX;

			// si el bloque empieza antes que el rect�ngulo de recorte
			if (distX < 0){
				posX = 0;

				// si el bloque es m�s grande que la zona visible, se recorta en longitud
				if ((distX + lgtudX) > 24){
					lgtudX = 24;
				} else {
					// en otro caso, recorta la longitud del bloque a la zona visible
					lgtudX = lgtudX + distX;
				}
			} else {
				// si el bloque empieza despu�s del inicio de la zona visible
				posX = distX;

				// si el bloque es m�s grande que la zona visible, recorta la longitud del bloque
				if ((distX + lgtudX) > 24){
					lgtudX = lgtudX - (distX + lgtudX - 24);
				}
			}

			// halla la distancia en y entre las coordenadas
			distY = posY - minPosY;

			// si el bloque empieza antes que el rect�ngulo de recorte
			if (distY < 0){
				posY = 0;

				// si el bloque es m�s grande que la zona visible, se recorta en longitud
				if ((distY + lgtudY) > 24){
					lgtudY = 24;
				} else {
					// en otro caso, recorta la longitud del bloque a la zona visible
					lgtudY = lgtudY + distY;
				}
			} else {
				// si el bloque empieza despu�s del inicio de la zona visible
				posY = distY;

				// si el bloque es m�s grande que la zona visible, recorta la longitud del bloque
				if ((distY + lgtudY) > 24){
					lgtudY = lgtudY - (distY + lgtudY - 24);
				}
			}

			// modifica la tabla de alturas con el bloque recortado
			for (int j = 0; j < lgtudY; j++){
				for (int i = 0; i < lgtudX; i++){
					bufAlturas[posY + j][posX + i] = altura;
				}
			}
		}
	}
}

// comprueba si la posici�n que se le pasa (en coordenadas de mundo) est� dentro de las 20x20 posiciones
// centrales de la rejilla y si es as�, devuelve la posici�n en el sistema de coordenadas de la rejilla
bool RejillaPantalla::ajustaAPosRejilla(int posX, int posY, int &posXRejilla, int &posYRejilla)
{
	posXRejilla = posX - minPosX;

	// si est� fuera del rango en las x, devuelve false
	if (posXRejilla < 2) return false;
	if (posXRejilla >= 22) return false;

	posYRejilla = posY - minPosY;

	// si est� fuera del rango en las y, devuelve false
	if (posYRejilla < 2) return false;
	if (posYRejilla >= 22) return false;

	return true;
}

// comprueba si la posici�n que se le pasa est� en las 20x20 posiciones centrales de la rejilla de la
// pantalla actual, y de ser as�, se devuelve su posici�n en el sistema de coordenadas de la rejilla
bool RejillaPantalla::estaEnRejillaCentral(PosicionJuego *pos, int &posXRejilla, int &posYRejilla)
{
	// si la posici�n no est� en la misma planta que la de la rejilla actual, sale
	if (motor->obtenerAlturaBasePlanta(pos->altura) != minAltura){
		return false;
	}

	// si la posici�n no est� en las 20x20 posiciones centrales de la rejilla, sale
	if (!ajustaAPosRejilla(pos->posX, pos->posY, posXRejilla, posYRejilla)){
		return false;
	}

	return true;
}

// devuelve la diferencia de altura y posici�n del personaje si sigue avanzando hacia donde mira
bool RejillaPantalla::obtenerAlturaPosicionesAvance(Personaje *pers, int &difAltura1, int &difAltura2, int &avanceX, int &avanceY)
{
	// si el personaje no est� en la misma planta que la de la rejilla, sale
	if (elMotorGrafico->obtenerAlturaBasePlanta(pers->altura) != minAltura) return false;

	// obtiene la altura relativa con respecto a esta planta
	int alturaLocal = pers->altura - elMotorGrafico->obtenerAlturaBasePlanta(pers->altura);

	return obtenerAlturaPosicionesAvanceComun(pers, alturaLocal, difAltura1, difAltura2, avanceX, avanceY);
}

// devuelve la diferencia de altura y posici�n del personaje si sigue avanzando hacia donde mira
bool RejillaPantalla::obtenerAlturaPosicionesAvance2(Personaje *pers, int &difAltura1, int &difAltura2, int &avanceX, int &avanceY)
{
	return obtenerAlturaPosicionesAvanceComun(pers, 0, difAltura1, difAltura2, avanceX, avanceY);
}

/////////////////////////////////////////////////////////////////////////////
// m�todos de ayuda
/////////////////////////////////////////////////////////////////////////////

// devuelve la diferencia de altura y posici�n del personaje si sigue avanzando hacia donde mira
bool RejillaPantalla::obtenerAlturaPosicionesAvanceComun(Personaje *pers, int alturaLocal, int &difAltura1, int &difAltura2, int &avanceX, int &avanceY)
{
	int posXLocal, posYLocal;

	// si la posici�n no est� dentro de las 20x20 posiciones centrales de la pantalla que se muestra, sale
	if (!estaEnRejillaCentral(pers, posXLocal, posYLocal)) return false;

	// calcula la primera posici�n de la rejilla a probar
	int despIni = pers->enDesnivel ? 6 : 4;
	posXLocal += calculoAvancePosicion[pers->orientacion][despIni];
	posYLocal += calculoAvancePosicion[pers->orientacion][despIni + 1];

	// rellena el buffer para el c�lculo del avance con las posiciones relevantes seg�n la orientaci�n
	for (int j = 0; j < 4; j++){
		int oldPosXLocal = posXLocal;
		int oldPosYLocal = posYLocal;
		
		for (int i = 0; i < 4; i++){
			// obtiene la altura de la posici�n
			int alturaPos = bufAlturas[posYLocal][posXLocal];

			if (alturaPos < 0x10){
				// si no hay un personaje en esa posici�n, obtiene la diferencia de altura entre la posici�n y el personaje
				alturaPos = alturaPos - alturaLocal;
			} else {
				alturaPos = alturaPos & 0x30;
			}
			bufCalculoAvance[j][i] = alturaPos;

			// apunta a la siguiente posici�n
			posXLocal += calculoAvancePosicion[pers->orientacion][0];
			posYLocal += calculoAvancePosicion[pers->orientacion][1];
		}

		// apunta a la siguiente posici�n
		posXLocal = oldPosXLocal + calculoAvancePosicion[pers->orientacion][2];
		posYLocal = oldPosYLocal + calculoAvancePosicion[pers->orientacion][3];
	}

	// si el personaje ocupa 4 posiciones en la rejilla
	if (!pers->enDesnivel){
		difAltura1 = bufCalculoAvance[0][1];
		difAltura2 = bufCalculoAvance[0][2];

		// si en las 2 posiciones hacia las que quiere avanzar el personaje no hay la misma altura
		if (difAltura1 != difAltura2){
			// indica que hay una diferencia de altura > 1
			difAltura1 = 2;
		}
	} else {
		// si el personaje ocupa una posici�n en la rejilla, guarda la diferencia de altura de las 2 posiciones hacia las que quiere avanzar
		difAltura1 = bufCalculoAvance[1][1];
		difAltura2 = bufCalculoAvance[0][1];
	}

	// guarda el avance en cada coordenada seg�n la orientaci�n en la que se quiere avanzar
	avanceX = elMotorGrafico->tablaDespOri[pers->orientacion][0];
	avanceY = elMotorGrafico->tablaDespOri[pers->orientacion][1];

	return true;
}

// si los datos de altura est�n dentro de la zona de la rejilla, los graba
void RejillaPantalla::fijaAlturaRecortando(int posX, int posY, int altura)
{
	// recorta en y
	posY = posY - minPosY;

	// si la coordenada y est� fuera de la zona visible en y, sale
	if ((posY < 0) || (posY >= 24)){
		return;
	}

	// recorta en x
	posX = posX - minPosX;

	// si la coordenada x est� fuera de la zona visible en x, sale
	if ((posX < 0) || (posX >= 24)){
		return;
	}

	bufAlturas[posY][posX] = altura;
}

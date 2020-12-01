// Severino.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "Abad.h"
#include "BuscadorRutas.h"
#include "GestorFrases.h"
#include "Guillermo.h"
#include "Logica.h"
#include "Severino.h"

using namespace Abadia;

/////////////////////////////////////////////////////////////////////////////
// posiciones a las que puede ir el personaje según el estado
/////////////////////////////////////////////////////////////////////////////

PosicionJuego Severino::posicionesPredef[4] = {
	PosicionJuego(ABAJO, 0x8c, 0x4b, 0x02),		// posición en la iglesia
	PosicionJuego(ARRIBA, 0x36, 0x35, 0x02),	// posición en el refectorio
	PosicionJuego(DERECHA, 0x68, 0x55, 0x00),	// posición en su celda
	PosicionJuego(DERECHA, 0xc9, 0x2a, 0x00)	// pantalla de al lado de las celdas de los monjes
};

/////////////////////////////////////////////////////////////////////////////
// inicialización y limpieza
/////////////////////////////////////////////////////////////////////////////

Severino::Severino(SpriteMonje *spr) : Monje(spr)
{
	// coloca los datos de la cara de severino
	// CPC
	/*
	datosCara[0] = 0xb103;
	datosCara[1] = 0xb103 + 0x32;
	*/
	// VGA
        datosCara[0] = 66908;
        datosCara[1] = 66908+0x32*4;

	mascarasPuertasBusqueda = 0x2f;

	// asigna las posiciones predefinidas
	posiciones = posicionesPredef;
}

Severino::~Severino()
{
}

/////////////////////////////////////////////////////////////////////////////
// comportamiento
/////////////////////////////////////////////////////////////////////////////

// Los bit del estado indican eventos para severino:
//		bit 0 -> indica que guillermo está en el ala izquierda de la abadía
//		bit 1 -> indica que ha terminado de presentarse ante guillermo
//		bit 2 -> indica que se ha presentado ante guillermo
void Severino::piensa()
{
	// severino ha muerto, sale
	if (!estaVivo){
		elBuscadorDeRutas->seBuscaRuta = false;

		return;
	}

	// realiza acciones dependiendo del momento del día
	switch (laLogica->momentoDia){
		case NOCHE: case COMPLETAS:	// durante la noche y completas va a su celda
			aDondeVa = 2;
			aDondeHaLlegado = 2;
			return;

		case PRIMA:
			// si está mostrando una frase y va a por guillermo, sale
			if (elGestorFrases->mostrandoFrase && (aDondeVa == POS_GUILLERMO)) return;

			// en otro caso, va a la iglesia
			aDondeVa = 0;

			// si es el quinto día y guillermo no está en el ala izquierda de la abadía, va a por él
			if ((laLogica->dia == 5) && ((estado & 0x01) == 0)){
				if (laLogica->guillermo->posX < 0x60){
					estado |= 0x01;
				} else {
					aDondeVa = POS_GUILLERMO;

					// si alcanza a guillermo le dice la frase ESCUCHAD HERMANO, HE ENCONTRADO UN EXTRAÑO LIBRO EN MI CELDA
					if (aDondeHaLlegado == POS_GUILLERMO){
						elGestorFrases->muestraFraseYa(0x0f);
						estado |= 0x01;
					}
				}
			}

			return;

		case SEXTA:	// si es sexta, se va al refectorio
			aDondeVa = 1;
			return;

		case TERCIA: case NONA:
			// a partir del segundo día, si severino no va a su celda y no se ha presentado
			if (((estado & 0x02) == 0) && ((aDondeHaLlegado >= 2) || (aDondeHaLlegado == POS_GUILLERMO)) && (laLogica->dia >= 2) && (laLogica->abad->aDondeVa != POS_GUILLERMO)){
				// si severino no se ha presentado y no se está reproduciendo una voz
				if (((estado & 0x04) == 0) && (!elGestorFrases->mostrandoFrase)){
					// si está cerca de guillermo, se acerca a él y se presenta
					if (estaCerca(laLogica->guillermo)){
						estado = 4;
						aDondeVa = POS_GUILLERMO;

						// pone en el marcador la frase VENERABLE HERMANO, SOY SEVERINO, EL ENCARGADO DEL HOSPITAL. QUIERO ADVERTIROS QUE EN ESTA ABADIA SUCEDEN COSAS MUY EXTRAÑAS. ALGUIEN NO QUIERE QUE LOS MONJES DECIDAN POR SI SOLOS LO QUE DEBEN SABER
						elGestorFrases->muestraFrase(0x37);

						return;
					}
				}

				// si ya se ha presentado y termina de hablar, va a su celda
				if ((estado & 0x04) == 0x04){
					aDondeVa = POS_GUILLERMO;

					if (!elGestorFrases->mostrandoFrase){
						aDondeVa = 2;
						aDondeHaLlegado = 3;
						estado |= 2;
					}

					return;
				}
			}

			// si está junto a guillermo y no se está reproduciendo ninguna frase, le cuenta las manchas de berengario
			if (aDondeHaLlegado == POS_GUILLERMO){
				if (!elGestorFrases->mostrandoFrase){
					// pone en el marcador la frase ES MUY EXTRAÑO, HERMANO GUILLERMO. BERENGARIO TENIA MANCHAS NEGRAS EN LA LENGUA Y EN LOS DEDOS
					elGestorFrases->muestraFrase(0x26);

					// al terminar la frase avanza el momento del día
					laLogica->avanzarMomentoDia = true;
				}

				return;
			}

			// si ha llegado a su celda
			if (aDondeHaLlegado == 2){
				// si es el quinto día, no se mueve de su celda
				if (laLogica->dia == 5){
					elBuscadorDeRutas->seBuscaRuta = false;

					return;
				}

				// si es tercia del cuarto día, va a por guillermo
				if ((laLogica->dia == 4) && (laLogica->momentoDia == TERCIA)){
					aDondeVa = POS_GUILLERMO;

					// si está cerca de guillermo, le dice que espere
					if (estaCerca(laLogica->guillermo)){
						// pone en el marcador la frase ESPERAD, HERMANO
						elGestorFrases->muestraFrase(0x2c);
					}

					return;
				}

				// en otro caso, va a la habitación que está al lado de las celdas de los monjes
				aDondeVa = 3;

				return;
			}
			// se va a su celda
			aDondeVa = 2;

			return;
		default: // se va a la iglesia
			aDondeVa = 0;
	}
}

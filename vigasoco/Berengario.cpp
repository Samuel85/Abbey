// Berengario.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "Abad.h"
#include "Berengario.h"
#include "BuscadorRutas.h"
#include "GestorFrases.h"
#include "Guillermo.h"
#include "Logica.h"
#include "Malaquias.h"
#include "Marcador.h"
#include "MotorGrafico.h"

using namespace Abadia;

/////////////////////////////////////////////////////////////////////////////
// posiciones a las que puede ir el personaje según el estado
/////////////////////////////////////////////////////////////////////////////

PosicionJuego Berengario::posicionesPredef[6] = {
	PosicionJuego(ABAJO, 0x8c, 0x48, 0x02),		// posición en la iglesia
	PosicionJuego(ARRIBA, 0x32, 0x35, 0x02),	// posición en el refectorio
	PosicionJuego(IZQUIERDA, 0x3d, 0x5c, 0x0f),	// posición de su mesa en el scriptorium
	PosicionJuego(DERECHA, 0xbc, 0x15, 0x02),	// celda de los monjes
	PosicionJuego(ARRIBA, 0x52, 0x67, 0x04),	// posición al pie de las escaleras para subir al scriptorium
	PosicionJuego(ARRIBA, 0x68, 0x57, 0x02)		// celda de severino
};

/////////////////////////////////////////////////////////////////////////////
// inicialización y limpieza
/////////////////////////////////////////////////////////////////////////////

Berengario::Berengario(SpriteMonje *spr) : Monje(spr)
{
	// coloca los datos de la cara de berengario
	fijaCapucha(false);

	mascarasPuertasBusqueda = 0x3f;

	// asigna las posiciones predefinidas
	posiciones = posicionesPredef;
}

Berengario::~Berengario()
{
}

/////////////////////////////////////////////////////////////////////////////
// comportamiento
/////////////////////////////////////////////////////////////////////////////

// Los estados en los que puede estar berengario son:
//		0x00 -> estado incial
//		0x01 -> estado en vísperas para ir a la iglesia
//		0x04 -> estado en el que está pendiente de que guillermo deje el pergamino
//		0x05 -> estado en el que va a avisar al abad de que guillermo ha cogido el pergamino
//		0x06 -> en este estado se ha colocado la capucha y se va a por el libro y después a la celda de severino
void Berengario::piensa()
{
	// si no está vivo, sale
	if (!estaVivo){
		elBuscadorDeRutas->seBuscaRuta = false;

		return;
	}

	// si es sexta, va al comedor
	if (laLogica->momentoDia == SEXTA){
		aDondeVa = 1;

		return;
	}

	// si es prima, va a la iglesia
	if (laLogica->momentoDia == PRIMA){
		aDondeVa = 0;

		return;
	}

	// en completas se va a la celda de los monjes
	if (laLogica->momentoDia == COMPLETAS){
		aDondeVa = 3;

		return;
	}

	// si es de noche
	if (laLogica->momentoDia == NOCHE){
		// si es el tercer día
		if (laLogica->dia == 3){
			if (estado == 6){
				// indica que puede coger el libro
				mascaraObjetos = LIBRO;

				// si está en su celda, va hacia las escaleras al pie del scriptorium
				if (aDondeHaLlegado == 3){
					aDondeVa = 4;
				} else {
					// en otro caso, se dirige a por el libro
					aDondeVa = POS_LIBRO;

					// si tiene el libro
					if ((objetos & LIBRO) == LIBRO){
						// si ha llegado a la celda de severino, muere y avanza el momento del día
						if (aDondeHaLlegado == 5){
							estaVivo = false;
							posX = posY = altura = 0;

							laLogica->avanzarMomentoDia = true;
						}

						// se dirige a la celda de severino
						aDondeVa = 5;
					}
				}
				
				return;
			}
			
			// si está en su celda, cambia su cara por la del encapuchado y pasa al estado 6
			if (aDondeHaLlegado == 3){
				fijaCapucha(true);
				estado = 6;

				return;
			}
		}
		
		// se dirige a la celda de los monjes
		aDondeVa = 3;

		return;
	}

	// si es vísperas
	if (laLogica->momentoDia == VISPERAS){
		// si es el segundo día y malaquías no ha bajado del scriptorium, se queda quieto protegiendo el pergamino
		if ((laLogica->dia == 2) && (laLogica->malaquias->estado < 0x04)){
			elBuscadorDeRutas->seBuscaRuta = false;
		} else {
			// pasa al estado 1 y va a la iglesia
			estado = 1;
			aDondeVa = 0;
		}

		return;
	}

	// si es el primer o segundo día
	if (laLogica->dia < 3){
		// si berengario está pendiente de que guillermo deje el pergamino
		if (estado == 4){
			// incrementa el contador del tiempo que tiene el pergamino guillermo
			contadorPergamino++;

			// si aún no ha llegado al límite de tiempo y no ha cambiado de pantalla
			if ((contadorPergamino < 0x41) && (elMotorGrafico->numPantalla == 0x40)){
				// si guillermo no tiene el pergamino, cambia el estado
				if ((laLogica->guillermo->objetos & PERGAMINO) == 0){
					estado = 0;
				}
			} else {
				// cambia el estado de berengario porque va a avisar al abad
				estado = 5;

				// deshabilita el contador para que avance el momento del día de forma automática
				laLogica->duracionMomentoDia = 0;
			}

			return;
		}
		
		if (estado == 5){
			aDondeVa = POS_ABAD;

			// si ha llegado a la posición del abad, le informa de que guillermo ha cogido el pergamino
			if (aDondeHaLlegado == POS_ABAD){
				estado = 0;

				laLogica->abad->guillermoHaCogidoElPergamino = true;
				laLogica->abad->contador = 0xc9;

				// indica que ha avisado al abad
				estado2 |= 0x01;
			}
			
			return;

		}

		// si berengario está en la mesa del scriptorium
		if (aDondeHaLlegado == 2){
			// si guillermo ha cogido el pergamino
			if (guillermoHaCogidoElPergamino()){
				// reinicia el contador para avisar al abad y cambia de estado
				contadorPergamino = 0;
				estado = 4;

				// si está cerca de guillermo, le dice que deje el manuscrito o advertirá al abad
				if (estaCerca(laLogica->guillermo)){
					// pone en el marcador la frase DEJAD EL MANUSCRITO DE VENACIO O ADVERTIRE AL ABAD
					elGestorFrases->muestraFrase(0x04);
				}  else {
					// si ya no está cerca de él, avis al abad
					estado = 5;

					// deshabilita el contador para que avance el momento del día de forma automática
					laLogica->duracionMomentoDia = 0;
				}
				
				return;
			}
		}

		// si malaquías le ha dicho a guillermo que le enseñe el scriptorium y éste no está en la planta baja
		if (((laLogica->malaquias->estado2 & 0x40) == 0x40) && (laLogica->guillermo->altura >= 0x0d)){
			// si no le ha dicho que aquí trabajan los mejores copistas
			if ((estado2 & 0x10) == 0){
				aDondeVa = POS_GUILLERMO;

				// si está cerca de guillermo
				if (estaCerca(laLogica->guillermo)){
					if (!elGestorFrases->mostrandoFrase){
						aDondeHaLlegado = POS_GUILLERMO;

						// descarta los movimientos pensados
						descartarMovimientosPensados();

						// indica que ya le ha dicho la frase
						estado2 |= 0x10;

						// pone en el marcador la frase AQUI TRABAJAN LOS MEJORES COPISTAS DE OCCIDENTE
						elGestorFrases->muestraFrase(0x35);

						elBuscadorDeRutas->seBuscaRuta = false;
					}
				}
				
				return;
			}

			// si no le ha enseñado dónde trabajaba venacio
			if ((estado2 & 0x08) == 0){
				// va a su sitio en el scriptorium
				aDondeVa = 2;

				// si está cerca de guillermo
				if (estaCerca(laLogica->guillermo)){
					// si ha llegado al scriptorium y no estaba reproduciendo una voz
					if ((aDondeHaLlegado == 2) && (!elGestorFrases->mostrandoFrase)){
						// indica que ya le ha enseñado dónde trabaja venacio
						estado2 |= 0x08;

						// pone en el marcador la frase AQUI TRABAJABA VENACIO
						elGestorFrases->muestraFrase(0x36);
					}
					
					return;
				}
			}
		}

		estado = 0;

		// no se mueve de su puesto de trabajo
		aDondeVa = 2;

		return;
	}
}

/////////////////////////////////////////////////////////////////////////////
// métodos de ayuda
/////////////////////////////////////////////////////////////////////////////

// se pone o se quita la capucha
void Berengario::fijaCapucha(bool puesta)
{
	//CPC
	/*
	if (!puesta){
        // coloca los datos de la cara de berengario
		datosCara[0] = 0xb22f;
		datosCara[1] = 0xb22f + 0x32;
	} else {
		// coloca los datos del encapuchado
		datosCara[0] = 0xb35b;
		datosCara[1] = 0xb35b + 0x32;
	}
	*/
	// VGA
	if (!puesta){
		// coloca los datos de la cara de berengario
		datosCara[0] = 68108;
		datosCara[1] = 68108+0x32*4;
	} else {
		// coloca los datos del encapuchado
		datosCara[0] = 69308;
		datosCara[1] = 69308+0x32*4;
	}
}

// comprueba si guillermo ha cogido el pergamino
bool Berengario::guillermoHaCogidoElPergamino()
{
    // si ha avisado al abad, sale
	if ((estado2 & 0x01) == 0x01) return false;

	// si guillermo tiene el pergamino, sale
	if ((laLogica->guillermo->objetos & PERGAMINO) == PERGAMINO) return true;

	return false;
}

// Adso.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include <cassert>

#include "Adso.h"
#include "BuscadorRutas.h"

#include "GestorFrases.h"
#include "GeneradorPantallas.h"
#include "Guillermo.h"
#include "Juego.h"
#include "Logica.h"
#include "Marcador.h"
#include "MotorGrafico.h"
#include "RejillaPantalla.h"

#include "system.h"

using namespace Abadia;

/////////////////////////////////////////////////////////////////////////////
// tabla de orientaciones a probar para moverse en un determinado sentido
// Hay 2 grandes grupos de entradas en la tabla. Las primeras 4 entradas dan m?s prioridad a los 
// movimientos a la derecha y las 4 ?ltimas a los movimientos a la izquierda. Dentro de cada grupo 
// de entradas, las 2 primeras entradas dan m?s prioridad a los movimientos hacia abajo, y las otras 
// 2 entradas dan m?s prioridad a los movimientos hacia arriba.
/////////////////////////////////////////////////////////////////////////////

int Adso::oriMovimiento[8][4] = {
	{ 3, 0, 2, 1 },
	{ 0, 3, 1, 2 },
	{ 1, 0, 2, 3 },
	{ 0, 1, 3, 2 },

	{ 3, 2, 0, 1 },
	{ 2, 3, 1, 0 },
	{ 1, 2, 0, 3 },
	{ 2, 1, 3, 0 }
};

/////////////////////////////////////////////////////////////////////////////
// tabla de desplzamientos dentro del buffer de alturas seg?n la orientaci?n 
/////////////////////////////////////////////////////////////////////////////

int Adso::despBufferSegunOri[4][2] = {
	{ +1,  0 },
	{  0, -1 },
	{ -1,  0 },
	{  0, +1 }
};

/////////////////////////////////////////////////////////////////////////////
// tabla de la animaci?n del personaje
/////////////////////////////////////////////////////////////////////////////

// CPC
/*
Personaje::DatosFotograma Adso::tablaAnimacion[8] = {
	{ 0xa78a, 0x05, 0x20 },
	{ 0xa6ea, 0x05, 0x20 },
	{ 0xa78a, 0x05, 0x20 },
	{ 0xa82a, 0x05, 0x1f },
	{ 0xa8c5, 0x04, 0x1e },
	{ 0xa93d, 0x04, 0x1e },
	{ 0xa8c5, 0x04, 0x1e },
	{ 0xa9b5, 0x04, 0x1e }
};
*/
// VGA 
Personaje::DatosFotograma Adso::tablaAnimacion[8] = {
	{ 58408, 0x05, 0x20 },
	{ 57768, 0x05, 0x20 },
	{ 58408, 0x05, 0x20 },
	{ 59048, 0x05, 0x1f },
	{ 59668, 0x04, 0x1e },
	{ 60148, 0x04, 0x1e },
	{ 59668, 0x04, 0x1e },
	{ 60628, 0x04, 0x1e }
};

/////////////////////////////////////////////////////////////////////////////
// posiciones a las que puede ir el personaje seg?n el estado
/////////////////////////////////////////////////////////////////////////////

PosicionJuego Adso::posicionesPredef[3] = {
	PosicionJuego(ABAJO, 0x84, 0x4e, 0x02),	// posici?n en la iglesia
	PosicionJuego(ABAJO, 0x34, 0x39, 0x02),	// posici?n en el refectorio
	PosicionJuego(ABAJO, 0xa8, 0x18, 0x00)	// posici?n en la celda
};

/////////////////////////////////////////////////////////////////////////////
// inicializaci?n y limpieza
/////////////////////////////////////////////////////////////////////////////

Adso::Adso(Sprite *spr) : PersonajeConIA(spr)
{
	// asigna la tabla de animaci?n del personaje
	animacion = tablaAnimacion;
	numFotogramas = 8;

	mascarasPuertasBusqueda = 0x3c;

	// asigna las posiciones predefinidas
	posiciones = posicionesPredef;
}

Adso::~Adso()
{
}

/////////////////////////////////////////////////////////////////////////////
// movimiento
/////////////////////////////////////////////////////////////////////////////

void Adso::run()
{
	// inicialmente el personaje va a tratar de moverse
	elBuscadorDeRutas->seBuscaRuta = true;

	// ejecuta la l?gica del personaje
	piensa();

	// modifica el n?mero aleatorio generado
	laLogica->numeroAleatorio = bufAcciones[0];

	// modifica la tabla de conexiones de las habitaciones dependiendo de las puertas a las que se tenga acceso
	elBuscadorDeRutas->modificaPuertasRuta(mascarasPuertasBusqueda);

	// mueve el personaje
	mueve();

	if (aDondeVa != POS_GUILLERMO){
		// si se han terminado los comandos de movimiento, genera los comandos para ir a donde se quiere
		elBuscadorDeRutas->generaAccionesMovimiento(this);
	} else {
		// si va hacia donde se mueve guillermo

		// si la c?mara no sigue a guillermo o a adso, sale
		if (laLogica->numPersonajeCamara >= 2) return;

		if (!pensarNuevoMovimiento){
			// si tiene un movimiento pensado pero no se pudo mover hacia donde quer?a
			if (!laLogica->hayMovimiento){
				// si no ha podido moverse varias veces, piensa otro movimiento
				movimientosFrustados++;

				if (movimientosFrustados >= 10){
					movimientosFrustados = 0;

					posAcciones = 0;
					numBitAcciones = 0;
					bufAcciones[0] = 0x10;
				}
			}

			return;
		}

		// aqu? llega si adso no ten?a un movimiento pensado

		Personaje *guillermo = laLogica->guillermo;
		RejillaPantalla *rejilla = elMotorGrafico->rejilla;

		int posXRejilla, posYRejilla;

		// si adso est? en la pantalla que se muestra
		if (rejilla->estaEnRejillaCentral(this, posXRejilla, posYRejilla)){
			// si se puls? cursor arriba
			//if (losControles->estaSiendoPulsado(P1_UP)){
			if (sys->pad.up){

				int difAltura1 = 0, difAltura2, avanceX, avanceY;

				// obtiene las posiciones hacia las que se va a mover guillermo
				rejilla->obtenerAlturaPosicionesAvance2(guillermo, difAltura1, difAltura2, avanceX, avanceY);
					
				// comprueba si adso est? en alguna de las posiciones a las que va a moverse guillermo
				int aux = rejilla->bufCalculoAvance[0][1] | rejilla->bufCalculoAvance[0][2] | rejilla->bufCalculoAvance[1][1] | rejilla->bufCalculoAvance[1][2];

				// si adso impide el paso a guillermo, lo aparta
				if ((aux & 0x20) != 0){
					liberaPasoAGuillermo();
					return;
				}
			} 
				
			// si se puls? cursor abajo, trata de avanzar en la orientaci?n de guillermo
			//if (losControles->estaSiendoPulsado(P1_DOWN)){
			if (sys->pad.down){
				avanzaSegunGuillermo();
				return;
			}


			// limpia las posiciones que ocupan adso y guillermo en el buffer de alturas
			marcaPosicion(rejilla, 0);
			guillermo->marcaPosicion(rejilla, 0);

			// ajusta la posici?n de adso y guillermo a las coordenadas de rejilla
			int posXDest, posYDest;
			bool noHayError = rejilla->ajustaAPosRejilla(posX, posY, posXDest, posYDest);
			noHayError |= rejilla->ajustaAPosRejilla(guillermo->posX, guillermo->posY, elBuscadorDeRutas->posXIni, elBuscadorDeRutas->posYIni);
			assert(noHayError);

			// busca un camino para llegar a donde est? guillermo
			RejillaPantalla *temp = elBuscadorDeRutas->rejilla;
			elBuscadorDeRutas->rejilla = rejilla;
			bool encontrado = elBuscadorDeRutas->buscaEnPantalla(posXDest, posYDest);
			elBuscadorDeRutas->limpiaBitsBusquedaEnPantalla();
			elBuscadorDeRutas->rejilla = temp;

			// restaura las posiciones que ocupan adso y guillermo en el buffer de alturas
			marcaPosicion(rejilla, valorPosicion);
			guillermo->marcaPosicion(rejilla, guillermo->valorPosicion);

			// si no encontr? un camino para llegar a donde est? guillermo, sale
			if (!encontrado) return;

			int numIteraciones = 4;

			if (!enDesnivel){
				numIteraciones--;

				// si ninguna de las coordenadas es igual, se permite una iteraci?n m?s
				if (posX != guillermo->posX){
					if (posY != guillermo->posY){
						numIteraciones++;
					}
				}
			}

			// si el n?mero de iteraciones para llegar a guillermo es grande, avanza hacia guillermo
			if (elBuscadorDeRutas->nivelRecursion >= numIteraciones){ 
				// avanza hacia guillermo
				grabaComandosAvance(elBuscadorDeRutas->oriFinal);

				// ejecuta otra vez el comportamiento de adso
				run();
			}
		} else {
			// aqu? llega si adso no est? en la zona de la pantalla que se muestra
			elBuscadorDeRutas->alternativaActual = 0;
			elBuscadorDeRutas->numAlternativas = 1;

			// busca un camino para ir a por guillermo
			if (elBuscadorDeRutas->buscaCamino(this, guillermo) == -1){
				// si encuentra el camino, ejecuta otra vez el comportamiento de adso
				run();
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// comportamiento
/////////////////////////////////////////////////////////////////////////////

// Los estados en los que puede estar adso son:
//		0x00 -> estado incial
//		0x01 -> estado en prima y v?speras para ir a misa
//		0x04 -> estado dentro de su celda, esperando contestaci?n de si duerme o no
//		0x05 -> estado que se alcanza una vez que guillermo le dice que no se duerme
//		0x06 -> estado fuera de la celda por la noche o en completas para dirigirse a su celda
//		0x07 -> estado en sexta para ir al refectorio
void Adso::piensa()
{
	int numFrase = 0;
	Personaje *guillermo = laLogica->guillermo;
	
	if ((guillermo->objetos & PERGAMINO) == PERGAMINO){
		laLogica->pergaminoGuardado = false;
	}

	// si se est? acabando la noche, pone en el marcador la frase: PRONTO AMANECERA, MAESTRO
	if (laLogica->seAcabaLaNoche){
		elGestorFrases->muestraFrase(0x27);
	}

	// si hay un cambio de estado de la l?mpara, informa de ello
	if (laLogica->cambioEstadoLampara == 1){
		laLogica->cambioEstadoLampara = 0;
		elGestorFrases->muestraFraseYa(0x28);	// LA LAMPARA SE AGOTA
	} else if (laLogica->cambioEstadoLampara == 2){
		laLogica->cambioEstadoLampara = 0;
		laLogica->usandoLampara = false;
		elGestorFrases->muestraFraseYa(0x2a);	// SE HA AGOTADO LA LAMPARA

		// inicia el contador de tiempo que pueden ir a oscuras
		laLogica->cntTiempoAOscuras = 0x32;

		// pone la pantalla en negro y le quita la l?mpara a adso
		// CPC elMotorGrafico->genPant->limpiaPantalla(3);
		elMotorGrafico->genPant->limpiaPantalla(0); // VGA
		laLogica->reiniciaContadoresLampara();
	}

	// si guillermo no ha muerto
	if (!laLogica->haFracasado){
		// si se ha activado el contador del tiempo que pueden ir a oscuras
		if (laLogica->cntTiempoAOscuras >= 1){
			// si guillermo ha salido de la biblioteca, reinicia el contador
			if (guillermo->altura < 0x18){
				laLogica->cntTiempoAOscuras = 0;

				return;
			}

			// decrementa el tiempo que pueden ir a oscuras
			laLogica->cntTiempoAOscuras--;

			// si se termina el contador, muestra la frase JAMAS CONSEGUIREMOS SALIR DE AQUI y termina la partida
			if (laLogica->cntTiempoAOscuras == 1){
				laLogica->haFracasado = true;
				elGestorFrases->muestraFraseYa(0x2b);

				return;
			}
		} else {
			// si adso acaba de entrar en la biblioteca
			if (altura >= 0x18){
				// sigue a guillermo
				aDondeVa = POS_GUILLERMO;

				// si adso no tiene la l?mpara, muestra la frase: DEBEMOS ENCONTRAR UNA LAMPARA, MAESTRO
				if ((objetos & LAMPARA) != LAMPARA){
					elGestorFrases->muestraFraseYa(0x13);

					// inicia el contador de tiempo que pueden ir a oscuras
					laLogica->cntTiempoAOscuras = 0x64;

					return;
				} else {
					// en otro caso, enciende la l?mpara
					laLogica->usandoLampara = true;
				}
			} else {
				// si adso no est? en la biblioteca, apaga la l?mpara y reinicia el contador del tiempo que pueden ir a oscuras
				laLogica->usandoLampara = false;
				laLogica->cntTiempoAOscuras = 0;
			}
		}
	}

	// realiza acciones dependiendo del momento del d?a
	switch (laLogica->momentoDia){
		case SEXTA:	// va al refectorio
			aDondeVa = 1;
			oldEstado = 7;
			numFrase = 0x0c;	// frase DEBEMOS IR AL REFECTORIO, MAESTRO
			break;

		case PRIMA:	case VISPERAS:	// va a la iglesia
			aDondeVa = 0;
			oldEstado = 1;
			numFrase = 0x0b;	// frase DEBEMOS IR A LA IGLESIA, MAESTRO
			break;

		case COMPLETAS:	// va a su celda
			estado = 6;
			aDondeVa = 2;
			return;

		case NOCHE:
            // si est? esperando contestaci?n de si se duerme o no
			if (estado == 4){
				// si est? en la pantalla de fuera de la celda, indica que hay que avanzar el momento del d?a
				if (elMotorGrafico->numPantalla == 0x37){
					laLogica->avanzarMomentoDia = true;
				}

				// si no se est? reproduciendo una voz
				if (!elGestorFrases->mostrandoFrase){
					// si lleva mucho tiempo sin responder a la pregunta de dormir, duerme
					if (cntParaDormir >= 100){
						laLogica->avanzarMomentoDia = true;
					} else {
						// incrementa el contador para dormir
						cntParaDormir++;

						// escribe o limpia S:N
						escribeSN((cntParaDormir & 0x01) == 0);

						// comprueba si se pulso la S o la N
						if ((cntParaDormir & 0x01) == 0x01){
							//if (losControles->estaSiendoPulsado(KEYBOARD_S)){
							if (sys->pad.button3){
								laLogica->avanzarMomentoDia = true;
							}

							//if (losControles->estaSiendoPulsado(KEYBOARD_N)){
							if (sys->pad.button4){
								estado = 5;
							}
						}
					}
				}
				
				return;
			} else {
				aDondeVa = POS_GUILLERMO;

				// si no dormimos
				if (estado == 5){
					// si estamos en nuestra celda, sale
					if (elMotorGrafico->numPantalla == 0x3e){ 
						return;
					}

					// si ha salido de la celda, cambia el estado
					estado = 6;
				}

				// si estabamos fuera de la celda
				if (estado == 6){
					// si estamos cerca de guillermo y en nuestra celda
					if (estaCerca(guillermo) && (elMotorGrafico->numPantalla == 0x3e)){
						// reinicia el contador para dormir y pone la frase ?DORMIMOS?, MAESTRO
						cntParaDormir = 0;
						estado = 4;
						elGestorFrases->muestraFrase(0x12);
					}

					return;
				}
			}
                        break;
		default:
			// sigue a guillermo
			aDondeVa = POS_GUILLERMO;
		
			return;
	}

	// aqu? solo llega en SEXTA, PRIMA y VISPERAS

	// si ha cambiado el estado, muestra una frase
	if (estado != oldEstado){
		// si esta cerca de guillermo muestra la frase correspondiente
		if (estaCerca(guillermo)){
			elGestorFrases->muestraFrase(numFrase);
		}
		estado = oldEstado;
	}
}

// oculta o muestra el texto S:N
void Adso::escribeSN(bool muestra)
{
	if (muestra){
		// CPC elMarcador->imprimeFrase("S:N", 148, 164, 2, 3);
		elMarcador->imprimeFrase("S:N", 148, 164, 4, 0); // VGA
	} else {
        	// CPC elMarcador->imprimeFrase("   ", 148, 164, 2, 3);
	        elMarcador->imprimeFrase("   ", 148, 164, 4, 0); // VGA
	}
}

void Adso::grabaComandosAvance(int nuevaOri)
{
	int oldPosX = posX;
	int oldPosY = posY;
	int oldAltura = altura;
	int oldOri = orientacion;
	bool oldEnDesnivel = enDesnivel;
	bool oldGiradoEnDesnivel = giradoEnDesnivel;
	bool oldBajando = bajando;

	reiniciaPosicionBuffer();

	// comprueba si debe cambiar la orientaci?n del personaje
	if (orientacion != nuevaOri){
		modificaOrientacion(nuevaOri);
		orientacion = nuevaOri;
	}
	
	int difAltura1, difAltura2, avanceX, avanceY;

	// obtiene la altura de las posiciones hacia las que se va a mover
	elMotorGrafico->rejilla->obtenerAlturaPosicionesAvance(this, difAltura1, difAltura2, avanceX, avanceY);

	// escribe un comando dependiendo de si sube, baja o se mantiene
	avanzaPosicion(difAltura1, difAltura2, avanceX, avanceY);

	escribeComandos(0x1000, 12);
	reiniciaPosicionBuffer();

	bajando = oldBajando;
	giradoEnDesnivel = oldGiradoEnDesnivel;
	enDesnivel = oldEnDesnivel;
	orientacion = oldOri;
	altura = oldAltura;
	posY = oldPosY;
	posX = oldPosX;
}

// trata de avanzar en la orientaci?n de guillermo
void Adso::avanzaSegunGuillermo()
{
	// limpia la posici?n ocupada por adso
	marcaPosicion(elMotorGrafico->rejilla, 0);

	// calcula la entrada con las orientaciones a probar para moverse
	int numEntrada = laLogica->guillermo->orientacion + 1;
	if (numEntrada == 3) numEntrada = 7;
	
	// prueba a moverse hacia otra orientaci?n seg?n la entrada correspondiente
	pruebaMover(numEntrada);
}

// se aparta del avance de guillermo
void Adso::liberaPasoAGuillermo()
{
	// limpia la posici?n ocupada por adso
	marcaPosicion(elMotorGrafico->rejilla, 0);

	int numEntrada = 0;

	// calcula la distancia en x entre guillermo y adso
	int distX = posX - laLogica->guillermo->posX;

	// si adso est? a la izquierda de guillermo, modifica la entrada
	if (distX < 0){
		distX = -distX;
		numEntrada |= 0x04;
	}

	// calcula la distancia en y entre guillermo y adso
	int distY = posY - laLogica->guillermo->posY;

	// si adso est? a la izquierda de guillermo, modifica la entrada
	if (distY < 0){
		distY = -distY;
		numEntrada |= 0x02;
	}

	// si la distancia en y es menor que la distancia en x, modifica la entrada
	if (distY < distX){
		numEntrada++;
	}

	// prueba a moverse hacia otra orientaci?n seg?n la entrada correspondiente
	pruebaMover(numEntrada);
}

// prueba a moverse hacia otra orientaci?n seg?n la entrada correspondiente
void Adso::pruebaMover(int numEntrada)
{
	int posXRejilla, posYRejilla;

	// obtiene la posici?n del personaje dentro de la rejilla
	elMotorGrafico->rejilla->estaEnRejillaCentral(this, posXRejilla, posYRejilla);

	// obtiene la altura base del personaje
	int alturaBase = elMotorGrafico->rejilla->bufAlturas[posYRejilla][posXRejilla] & 0x0f;

	RejillaPantalla *temp = elBuscadorDeRutas->rejilla;
	elBuscadorDeRutas->rejilla = elMotorGrafico->rejilla;

	// recorre las 3 orientaciones a probar
	for (int i = 0; i < 3; i++){
		int ori = oriMovimiento[numEntrada][i];
		int posX = posXRejilla + despBufferSegunOri[ori][0];
		int posY = posYRejilla + despBufferSegunOri[ori][1];
		elMotorGrafico->rejilla->bufAlturas[posY][posX] &= 0x7f;

		// obtiene la altura si se mueve en esa orientaci?n
		int altura = elMotorGrafico->rejilla->bufAlturas[posY][posX];
		
		// si la posici?n es accesible, se mueve hacia esa posici?n
		if (elBuscadorDeRutas->esPosicionDestino(posX, posY, altura, alturaBase, false)){
			elMotorGrafico->rejilla->bufAlturas[posY][posX] &= 0x7f;
			grabaComandosAvance(ori);

			// marca la posici?n que ocupa adso
			marcaPosicion(elMotorGrafico->rejilla, valorPosicion);

			elBuscadorDeRutas->rejilla = temp;

			// ejecuta otra vez el comportamiento de adso
			run();

			return;
		}
	}

	// marca la posici?n que ocupa adso
	marcaPosicion(elMotorGrafico->rejilla, valorPosicion);

	elBuscadorDeRutas->rejilla = temp;
}

// BuscadorRutas.h
//
//	Clase que contiene los m�todos para buscar rutas y caminos entre posiciones
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _BUSCADOR_RUTAS_H_
#define _BUSCADOR_RUTAS_H_


#include "Singleton.h"
#include "Types.h"

#include "EntidadJuego.h"

namespace Abadia {

class FijarOrientacion;						// definido en FijarOrientacion.h
class PersonajeConIA;						// definido en PersonajeConIA.h
class RejillaPantalla;						// definido en RejillaPantalla.h

#define elBuscadorDeRutas BuscadorRutas::getSingletonPtr()

class BuscadorRutas : public Singleton<BuscadorRutas>
{
// campos
public:
	bool generadoCamino;					// indica si se ha generado alg�n camino en esta iteraci�n del bucle principal
	bool seBuscaRuta;						// indica si se ejecuta el buscador de rutas o no
	int contadorAnimGuillermo;				// contador de la animaci�n de guillermo al inicio de esta iteraci�n del bucle principal

	int numAlternativas;					// n�mero de alternativas generadas
	int alternativaActual;					// alternativa que se est� probando actualmente
	int nivelRecursion;						// nivel de recursi�n de la �ltima ejecuci�n del algoritmo de b�squeda de caminos
	int posXIni, posYIni;					// datos sobre la posici�n final de las b�squedas
	int posXFinal, posYFinal, oriFinal;		// datos sobre la posici�n final de las b�squedas

	RejillaPantalla *rejilla;				// objeto para realizar operaciones relacionadas con la rejilla de pantalla

protected:
	INT32 *buffer;							// buffer para la b�squeda de caminos
	int lgtudBuffer;						// longitud del buffer de b�squeda

	static UINT8 habitaciones[3][256];		// tabla con las habitaciones alcanzables desde una habitaci�n concreta
	static UINT8 habitacionesPuerta[6][4];	// tabla con las puertas y las habitaciones que comunican
	static PosicionJuego alternativas[5];	// posiciones alternativas para la b�squeda del camino
	static int despOrientacion[4][2];		// tabla de desplazamientos seg�n la orientaci�n
	static int posDestinoOrientacion[4][2];	// tabla con las posiciones de destino seg�n la orientaci�n
	
	int posPila;							// posici�n actual de la pila
	int posProcesadoPila;					// posici�n de la pila mientras se examinan las alternativas

	FijarOrientacion *fijaPosOri[4];		// objetos para indicar las posiciones a las que hay que ir seg�n la orientaci�n a coger

// m�todos
public:
	// inicializaci�n y limpieza
	BuscadorRutas(UINT8 *buf, int lgtud);
	~BuscadorRutas();

	void modificaPuertasRuta(int mascara);
	void generaAccionesMovimiento(PersonajeConIA *pers);

	int buscaCamino(PersonajeConIA *origen, PosicionJuego *destino);
	bool buscaEnPantalla(int posXDest, int posYDest);
	bool esPosicionDestino(int posX, int posY, int altura, int alturaBase, bool buscandoSolucion);
	void limpiaBitsBusquedaEnPantalla();

// m�todos de ayuda
protected:
	// m�todos de m�s alto nivel relacionados con la b�squeda y generaci�n de rutas
	void generaAlternativas(PosicionJuego *pos, int oriInicial, int oldOri);
	void generaAlternativa(PosicionJuego *pos, int orientacion, int oldOri);
	void procesaAlternativas(PersonajeConIA *pers, PosicionJuego *destino);
	
	int generaCaminoAPantalla(PersonajeConIA *pers, int numPlanta);
	
	void generaAlturasPantalla(PersonajeConIA *pers);
	bool estaCerca(int coord1, int coord2, int &distancia);

	int generaCaminoAPosicion(PersonajeConIA *pers, int posXDest, int posYDest);
	int generaCaminoAPosicionSiAlcanzable(PersonajeConIA *pers, int posXDest, int posYDest);
	int compruebaFinCamino(PersonajeConIA *pers, bool encontrado);

	void grabaComandosCamino(PersonajeConIA *pers);
	void reconstruyeCamino(PersonajeConIA *pers);

	// m�todos relacionados con la b�squeda de caminos en una pantalla
	bool buscaEnPantalla();
	bool buscaEnPantallaSiAlcanzable(int posXDest, int posYDest);
	bool buscaEnPantallaComun();
	bool esPosicionAlcanzable(int posX, int posY, int altura);
	bool esPosicionDestino(int posX, int posY, int alturaBase);

	// m�todos relacionados con la b�squeda de caminos entre pantallas
	bool buscaPantalla(int numPlanta, int mascara);
	bool buscaPantalla(int posXDest, int posYDest, int numPlanta);
	bool esPantallaDestino(int posX, int posY, int numPlanta, int mascara, int mascaraDestino);
	void limpiaBitsBusquedaPantalla(int numPlanta);

	// operaciones sobre la pila
	void push(INT32 val1, INT32 val2);
	void pop(INT32 &val1, INT32 &val2);
	void elem(int posicion, INT32 &val1, INT32 &val2);
	void pushInv(int &posicion, INT32 val1, INT32 val2);
	void pushInv(int &posicion, INT32 val1);
	void popInv(int &posicion, INT32 &val1);
	void popInv(int &posicion, INT32 &val1, INT32 &val2);
};


}

#endif	// _BUSCADOR_RUTAS_H_

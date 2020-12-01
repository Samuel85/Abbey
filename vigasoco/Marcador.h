// Marcador.h
//
//		Clase que encapsula las acciones relacionadas con el marcador
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _MARCADOR_H_
#define _MARCADOR_H_


#include <string>
#include "Singleton.h"

class CPC6128;					// definido en CPC6128.h

namespace Abadia {

#define elMarcador Marcador::getSingletonPtr()

class Marcador : public Singleton<Marcador>
{
// campos
public:
	CPC6128	*cpc6128;			// objeto que presta ayuda para realizar operaciones gr?ficas del cpc6128
	UINT8 *roms;				// puntero a las roms originales

	int numPosScrollDia;		// n?mero de posiciones para completar el scroll del nombre del d?a
	UINT8 *nombreMomentoDia;	// apunta al nombre del momento actual del d?a

protected:
	static int duracionEtapasDia[7][7];

// m?todos
public:
	void dibujaMarcador();
	void limpiaAreaMarcador();
	void limpiaAreaFrases();
	void dibujaObjetos(int objetos, int mascara);

	void muestraDiaYMomentoDia();
	void avanzaMomentoDia();
	void realizaScrollMomentoDia();

	void decrementaObsequium(int unidades);

	void imprimeFrase(std::string frase, int x, int y, int colorTexto, int colorFondo);
	void imprimirCaracter(int caracter, int x, int y, int colorTexto, int colorFondo);

	// inicializaci?n y limpieza
	Marcador();
	~Marcador();

protected:
	void dibujaDia(int numDia);
	void dibujaDigitoDia(int digito, int x, int y);
	void dibujaBarra(int lgtud, int color, int x, int y);
};


}

#endif	// _MARCADOR_H_

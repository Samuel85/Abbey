// GestorFrases.h
//
//		Clase que se encarga de mostrar las frases en el marcador
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _GESTOR_FRASES_H_
#define _GESTOR_FRASES_H_


#include "Singleton.h"
#include <string>

class CPC6128;					// definido en CPC6128.h

namespace Abadia {

#define elGestorFrases GestorFrases::getSingletonPtr()

class GestorFrases : public Singleton<GestorFrases>
{
// campos
public:
	volatile bool mostrandoFrase;// copia de reproduciendoFrase para que el otro thread consulte el estado de las frases

	std::string frasePergamino;	// frase del pergamino (necesario, porque en C++ no se puede modificar un static char *)

protected:
	CPC6128	*cpc6128;			// objeto que presta ayuda para realizar operaciones gr?ficas del cpc6128

	int contadorActualizacion;	// contador para actualizar la frase que se est? poniendo en el marcador
	int espaciosParaFin;		// n?mero de espacios para que la frase haya salido completamente del marcador
	bool fraseTerminada;		// indica si se termin? una frase
	bool reproduciendoFrase;	// indica que se est? mostrando una frase en el marcador
	const char *frase;				// apunta a la frase que se est? poniendo en el marcador

	// CPC
	//static char *frases[0x38];	// tabla de frases

	// VGA
	//Anyadimos una frase mas que en la version original
	//para limpiar el area de frases al cargar/grabar las partidas
		// convertimos en array para cada idioma
        static const char* frases[8][0x38+1];	// tabla de frases

// m?todos
public:
	void procesaFraseActual();
	void muestraFrase(int numFrase);
	void muestraFraseYa(int numFrase);
	void actualizaEstado();

	// inicializaci?n y limpieza
	GestorFrases();
	~GestorFrases();

protected:
	void scrollFrase();
	void dibujaFrase(int numFrase);
};


}

#endif	// _GESTOR_FRASES_H_

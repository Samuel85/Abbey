// Pergamino.h
//
//	Clase que representa el pergamino
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _PERGAMINO_H_
#define _PERGAMINO_H_


#include "Types.h"

class CPC6128;						// definido en CPC6128.h

namespace Abadia {


class Pergamino
{
// campos
public:
		// array por cada idioma
	//static const char *pergaminoInicio[8];	// texto del pergamino de la presentaci�n del juego
	static const unsigned char *pergaminoInicio[8];	// texto del pergamino de la presentaci�n del juego
	//static const char *pergaminoFinal[8];	// texto del pergamino del final del juego
	static const unsigned char *pergaminoFinal[8];	// texto del pergamino del final del juego
	static const unsigned char *pergaminoReferencias[8];	// texto del pergamino del final del juego
// no esta definido el caracter 5 en el pergamino y no podemos , por ejemplo,
// poner el texto para F5
//	static const unsigned char *pergaminoAyudasyMejoras[8];	// texto del pergamino del final del juego
	static const unsigned char *pergaminoManejo[8];	// texto del pergamino del manejo del teclado
	static const unsigned char *pergaminoIntroduccion[8];	// texto del pergamino del manejo del teclado


	int posX, posY;			// posici�n inicial del texto en el pergamino
	UINT16 *charTable;		// puntero a la tabla de punteros a los gr�ficos de los caracteres
	UINT8 const *pTrazosCaracter;
	
	int x;
	int y;
	int dim;
	int num;
	int pasaPaginaStep;


	bool writing;
	bool finished;

protected:
	CPC6128	*cpc6128;				// objeto que presta ayuda para realizar operaciones gr�ficas del cpc6128
	UINT8 *roms;

// m�todos
public:
	//void muestraTexto(const char *texto);
	const unsigned char *texto;
	void muestraTexto(const unsigned char *mensaje);

	// inicializaci�n y limpieza
	Pergamino();
	~Pergamino();

// m�todos de ayuda
protected:
	void dibuja();
	//void dibujaTexto(const char *texto);
	//void dibujaTexto(const unsigned char *texto);
	void dibujaTexto();

	void dibujaTiraHorizontal(int y, UINT8 *data);
	void dibujaTiraVertical(int x, UINT8 *data);

	void dibujaTriangulo(int x, int y, int lado, int color1, int color2);
	void restauraParteSuperiorYDerecha(int x, int y, int lado);
	void restauraParteInferior(int x, int y, int lado);
	bool pasaPagina();
private:
	int AdaptaColorAPaletaVGA(int a,int b); // VGA
	// los primeros 0x20 caracteres del ASCII no son imprimibles
	//UINT8 const *TablapTrazosCaracter[0x127-0x20]; 
	// Para cubrir los caracteres usados en las traducciones
	// la tabla cubre los 255 posibles caracteres del ASCII extendido
	UINT8 const *TablapTrazosCaracter[0xFF-0x20]; 
	static const UINT8 charD6[]; // �
	static const UINT8 charE0[]; // � 
	static const UINT8 charE1[]; // �
	static const UINT8 charE3[]; // � 
	static const UINT8 charE4[]; // �
	static const UINT8 charE7[]; // �
	static const UINT8 charE8[]; // �
	static const UINT8 charE9[]; // �
	static const UINT8 charEA[]; // �
	static const UINT8 charEC[]; // �
	static const UINT8 charED[]; // �
	static const UINT8 charEF[]; // �
	static const UINT8 charF1[]; // �
	static const UINT8 charF2[]; // �
	static const UINT8 charF3[]; // �
	static const UINT8 charF6[]; // �
	static const UINT8 charF9[]; // � 
	static const UINT8 charFA[]; // �
	static const UINT8 charFC[]; // �

	// W y w, que en el original no se usaban
	// y la w estaba intercambiada por la enye �
	static const UINT8 char27[]; // '
	static const UINT8 char42[]; // B
	static const UINT8 char46[]; // F
	static const UINT8 char49[]; // I
	static const UINT8 char4B[]; // K
	static const UINT8 char4E[]; // N
	static const UINT8 char51[]; // Q
	static const UINT8 char52[]; // R
	static const UINT8 char55[]; // U
	static const UINT8 char56[]; // V
	static const UINT8 char57[]; // W
	static const UINT8 char58[]; // X
	static const UINT8 char5A[]; // Z
	static const UINT8 char77[]; // w
};

}

#endif	// _PERGAMINO_H_

// MotorGrafico.h
//
//	Clase que contiene los m�todos de generaci�n de pantallas
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _MOTOR_GRAFICO_H_
#define _MOTOR_GRAFICO_H_


#include "Singleton.h"
#include "Types.h"

namespace Abadia {

class EntidadJuego;					// definido en EntidadJuego.h
class GeneradorPantallas;			// definido en GeneradorPantallas.h
class MezcladorSprites;				// definido en MezcladorSprites.h
class Personaje;					// definido en Personaje.h
class RejillaPantalla;				// definido en RejillaPantalla.h
class TransformacionCamara;			// definido en TransformacionesCamara.h

#define elMotorGrafico MotorGrafico::getSingletonPtr()

class MotorGrafico : public Singleton<MotorGrafico>
{
// campos
public:
	RejillaPantalla *rejilla;		// objeto para realizar operaciones relacionadas con la rejilla de pantalla
	GeneradorPantallas *genPant;	// generador de pantallas interpretando los bloques de construcci�n
	MezcladorSprites *mezclador;	// mezclador de los sprites con el resto de la pantalla

	int posXPantalla;				// posici�n x de la pantalla actual en coordenadas de mundo
	int posYPantalla;				// posici�n y de la pantalla actual en coordenadas de mundo
	int alturaBasePantalla;			// altura base de la planta que se muestra en la pantalla actual
	bool pantallaIluminada;			// indica si la pantalla est� iluminada o no
	bool hayQueRedibujar;			// indica que hay que redibujar la pantalla
	int numPantalla;				// n�mero de la pantalla que muestra la c�mara
	int oriCamara;					// orientaci�n de la c�mara para ver la pantalla actual

	Personaje *personaje;			// personaje al que sigue la c�mara

	static int tablaDespOri[4][2];	// tabla con los desplazamientos seg�n la orientaci�n
	static UINT8 plantas[3][256];	// mapa de las plantas de la abad�a

protected:
	UINT8 *roms;					// puntero a los datos del juego

	// objetos para la transformaci�n de coordenadas seg�n el tipo de c�mara de la pantalla
	TransformacionCamara *transCamara[4];

// m�todos
public:
	// comprobaci�n del cambio de pantalla
	void compruebaCambioPantalla(bool forzarCambio=false);

	// m�todos relacionados con la altura
	int obtenerPlanta(int alturaBase);
	int obtenerAlturaBasePlanta(int altura);

	// transformaciones relacionadas con la c�mara
	int ajustaOrientacionSegunCamara(int orientacion);
	void transCoordLocalesACoordCamara(int &x, int &y);
	int actualizaCoordCamara(EntidadJuego *entidad, int &posXPant, int &posYPant, int &sprPosY);

	// dibujo de la escena
	void dibujaPantalla();	
	void dibujaSprites();

	// inicializaci�n y limpieza
	MotorGrafico(UINT8 *buffer, int lgtudBuffer);
	~MotorGrafico();

// m�todos de ayuda
protected:
	int obtenerDirPantalla(int numPant);

	// actualizaci�n de las entidades del juego seg�n la c�mara
	void actualizaPuertas();
	void actualizaObjetos();
	void actualizaPersonajes();
};


}

#endif	// _MOTOR_GRAFICO_H_

// Logica.h
//
//		Clase que encapsula la l�gica del juego
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _LOGICA_H_
#define _LOGICA_H_


#include "Singleton.h"
#include "Types.h"

#include <iosfwd>

namespace Abadia {


// momentos del d�a
enum MomentosDia {
	NOCHE = 0,
	PRIMA = 1,
	TERCIA = 2,
	SEXTA = 3,
	NONA = 4,
	VISPERAS = 5,
	COMPLETAS = 6
};


// objetos del juego
enum ObjetosJuego {
	LIBRO = 0x80,
	GUANTES = 0x40,
	GAFAS = 0x20,
	PERGAMINO = 0x10,
	LLAVE1 = 0x08,
	LLAVE2 = 0x04,
	LLAVE3 = 0x02,
	LAMPARA = 0x01
};

class Abad;						// definido en Abad.h
class AccionesDia;				// definido en AccionesDia.h
class Adso;						// definido en Adso.h
class Berengario;				// definido en Berengario.h
class Bernardo;					// definido en Bernardo.h
class BuscadorRutas;			// definido en BuscadorRutas.h
class GestorFrases;				// definido en GestorFrases.h
class Guillermo;				// definido en Guillermo.h
class Jorge;					// definido en Jorge.h
class Malaquias;				// definido en Malaquias.h
class Personaje;				// definido en Personaje.h
class Severino;					// definido en Severino.h
class Sprite;					// definido en Sprite.h

#define laLogica Logica::getSingletonPtr()

class Logica : public Singleton<Logica>
{
// campos
public:
	UINT8 *roms;				// puntero a las roms originales

	AccionesDia *accionesDia;	// ejecutor de las acciones dependiendo del momento del d�a
	BuscadorRutas *buscRutas;	// buscador y generador de rutas
	GestorFrases *gestorFrases;	// gestor de las frases del juego

	Guillermo *guillermo;		// guillermo
	Adso *adso;					// adso
	Malaquias *malaquias;		// malaquias
	Abad *abad;					// el abad
	Berengario *berengario;		// berengario
	Severino *severino;			// severino
	Jorge *jorge;				// jorge
	Bernardo *bernardo;			// bernardo gui

	int dia;					// dia actual
	int momentoDia;				// momento del d�a
	int duracionMomentoDia;		// indica lo que falta para pasar al siguiente momento del d�a
	int oldMomentoDia;			// indica el momento del d�a de las �ltimas acciones programadas ejecutadas
	bool avanzarMomentoDia;		// indica si debe avanzar el momento del d�a
	int obsequium;				// nivel de obsequium (de 0 a 31)
	bool haFracasado;			// indica si guillermo ha fracasado en la investigaci�n
	bool investigacionCompleta;	// indica si se ha completado la investigaci�n
	int bonus;					// bonus que se han conseguido
	
	int mascaraPuertas;			// m�scara de las puertas que pueden abrirse

	bool espejoCerrado;			// indica si el espejo est� cerrado o se ha abierto
	int numeroRomano;			// indica el n�mero romano de la habitaci�n del espejo (en el caso de que se haya generado)
	int despDatosAlturaEspejo;	// desplazamiento hasta el final de los datos de altura de la habitaci�n del espejo
	int despBloqueEspejo;		// desplazamiento hasta los datos del bloque que forma el espejo

	bool seAcabaLaNoche;		// indica si falta poco para que se termine la noche
	bool haAmanecido;			// indica si ya ha amanecido
	bool usandoLampara;			// indica si se est� usando la l�mpara
	bool lamparaDesaparecida;	// indica si ha desaparecido la l�mpara
	int tiempoUsoLampara;		// contador del tiempo de uso de la l�mpara
	int cambioEstadoLampara;	// indica un cambio en el estado de la l�mpara
	int cntTiempoAOscuras;		// contador del tiempo que pueden ir a oscuras por la biblioteca

	int cntLeeLibroSinGuantes;	// contador para llevar un control del tiempo que lee guillermo el libro sin los guantes
	bool pergaminoGuardado;		// indica que el pergamino lo tiene el abad en su habitaci�n o est� detr�s de la habitaci�n del espejo

	int numeroAleatorio;		// n�mero aleatorio

	bool hayMovimiento;			// cuando hay alg�n movimiento de un personaje, esto se pone a true
	int cntMovimiento;			// contador que se pone a 0 con cada movimiento de guillermo (usado para cambios de c�mara)

	int numPersonajeCamara;		// indica el personaje al que sigue la c�mara actualmente
	int opcionPersonajeCamara;	// indica el personaje al que podr�a seguir la c�mara si no hay movimiento

// m�todos
public:
	void inicia();

	void compruebaLecturaLibro();
	void compruebaBonusYCambiosDeCamara();
	void compruebaCogerDejarObjetos();
	void compruebaCogerObjetos();
	void dejaObjeto(Personaje *pers);
	void compruebaAbrirCerrarPuertas();

	void actualizaVariablesDeTiempo();
	void compruebaFinMomentoDia();
	void compruebaFinLampara();
	void compruebaFinNoche();
	void reiniciaContadoresLampara();
	void ejecutaAccionesMomentoDia();

	int calculaPorcentajeMision();

	void compruebaAbreEspejo();
	int pulsadoQR();
	void realizaReflejoEspejo();
	void despHabitacionEspejo();

	// inicializaci�n y limpieza
	Logica(UINT8 *romData, UINT8 *buf, int lgtud);
	~Logica();


	friend std::ifstream& operator>>(std::ifstream& in,
			Logica* const logica);


// m�todos de ayuda
protected:
	// inicializaci�n
	void iniciaSprites();
	void iniciaPersonajes();
	void iniciaPuertas();
	void iniciaObjetos();
	void iniciaHabitacionEspejo();

	void generaNumeroRomano();
	bool reflejaPersonaje(Personaje *pers, Sprite *spr);

	void actualizaBonusYCamara();
};


}

#endif	// _LOGICA_H_

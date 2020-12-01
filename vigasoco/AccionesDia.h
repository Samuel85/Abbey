// AccionesDia.h
//
//	Clase para ejecutar las acciones programadas depnediendo del momento del día
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _ACCIONES_DIA_H_
#define _ACCIONES_DIA_H_


namespace Abadia {

class AccionesDia;

class AccionProgramada
{
// métodos
public:
	virtual void ejecuta(AccionesDia *ad) = 0;

	// inicialización y limpieza
	AccionProgramada(){}
	virtual ~AccionProgramada(){}
};

class Objeto;								// definido en Objeto.h
class Personaje;							// definido en Personaje.h

class AccionesDia
{
// campos
protected:
	static AccionProgramada *acciones[7];	// acciones programadas

// métodos
public:
	void ejecutaAccionesProgramadas();
	void dibujaEfectoEspiral();
	void colocaObjeto(Objeto *obj, int posX, int posY, int altura);
	void colocaPersonaje(Personaje *pers, int posX, int posY, int altura, int orientacion);

	// inicialización y limpieza
	AccionesDia();
	virtual ~AccionesDia();

// métodos de ayuda
protected:
	void dibujaEspiral(int color);
	void dibujaBloque(int posX, int posY, int color);
};


class AccionesNoche : public AccionProgramada
{
// métodos
public:
	virtual void ejecuta(AccionesDia *ad);
};

class AccionesPrima : public AccionProgramada
{
// métodos
public:
	virtual void ejecuta(AccionesDia *ad);
};

class AccionesTercia : public AccionProgramada
{
// métodos
public:
	virtual void ejecuta(AccionesDia *ad);
};

class AccionesSexta : public AccionProgramada
{
// métodos
public:
	virtual void ejecuta(AccionesDia *ad);
};

class AccionesNona : public AccionProgramada
{
// métodos
public:
	virtual void ejecuta(AccionesDia *ad);
};

class AccionesVisperas : public AccionProgramada
{
// métodos
public:
	virtual void ejecuta(AccionesDia *ad);
};

class AccionesCompletas : public AccionProgramada
{
// métodos
public:
	virtual void ejecuta(AccionesDia *ad);
};


}

#endif	// _ACCIONES_DIA_H_

// AccionesDia.h
//
//	Clase para ejecutar las acciones programadas depnediendo del momento del d�a
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _ACCIONES_DIA_H_
#define _ACCIONES_DIA_H_


namespace Abadia {

class AccionesDia;

class AccionProgramada
{
// m�todos
public:
	virtual void ejecuta([[maybe_unused]] AccionesDia *ad) = 0;

	// inicializaci�n y limpieza
	AccionProgramada()= default;
	virtual ~AccionProgramada()= default;
};

class Objeto;								// definido en Objeto.h
class Personaje;							// definido en Personaje.h

class AccionesDia
{
// campos
protected:
	static AccionProgramada *acciones[7];	// acciones programadas

// m�todos
public:
	void ejecutaAccionesProgramadas();
	void dibujaEfectoEspiral();
	void colocaObjeto(Objeto *obj, int posX, int posY, int altura);
	void colocaPersonaje(Personaje *pers, int posX, int posY, int altura, int orientacion);

	// inicializaci�n y limpieza
	AccionesDia();
	virtual ~AccionesDia();

// m�todos de ayuda
protected:
	void dibujaEspiral(int color);
	void dibujaBloque(int posX, int posY, int color);
};


class AccionesNoche : public AccionProgramada
{
// m�todos
public:
	void ejecuta(AccionesDia *ad) override;
};

class AccionesPrima : public AccionProgramada
{
// m�todos
public:
	void ejecuta(AccionesDia *ad) override;
};

class AccionesTercia : public AccionProgramada
{
// m�todos
public:
	void ejecuta(AccionesDia *ad) override;
};

class AccionesSexta : public AccionProgramada
{
// m�todos
public:
	void ejecuta(AccionesDia *ad) override;
};

class AccionesNona : public AccionProgramada
{
// m�todos
public:
	void ejecuta(AccionesDia *ad) override;
};

class AccionesVisperas : public AccionProgramada
{
// m�todos
public:
	void ejecuta(AccionesDia *ad) override;
};

class AccionesCompletas : public AccionProgramada
{
// m�todos
public:
	void ejecuta(AccionesDia *ad) override;
};


}

#endif	// _ACCIONES_DIA_H_

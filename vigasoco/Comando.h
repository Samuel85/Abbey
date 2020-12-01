// Comando.h
//
//	Clase que define el interfaz de los comandos que puede ejecutar el generador de bloques
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _COMANDO_H_
#define _COMANDO_H_


namespace Abadia {

class GeneradorPantallas;			// definido en GeneradorPantallas.h


class Comando
{
// métodos
public:
	virtual bool ejecutar(GeneradorPantallas *gen) = 0;

	// inicialización y limpieza
	Comando(){}
	virtual ~Comando(){}
};


}

#endif	// _COMANDO_H_

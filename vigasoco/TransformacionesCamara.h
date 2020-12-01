// TransformacionesCamara.h
//
//	Clases que definen las transformaciones de coordenadas según la posición de la cámara
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _TRANSFORMACIONES_CAMARA_H_
#define _TRANSFORMACIONES_CAMARA_H_


namespace Abadia {

class TransformacionCamara
{
// métodos
public:
	virtual void transforma(int &x, int &y) = 0;

	// inicialización y limpieza
	TransformacionCamara(){}
	virtual ~TransformacionCamara(){}
};


class Camara0 : public TransformacionCamara
{
// métodos
public:
	virtual void transforma(int &x, int &y);
};

class Camara1 : public TransformacionCamara
{
// métodos
public:
	virtual void transforma(int &x, int &y);
};

class Camara2 : public TransformacionCamara
{
// métodos
public:
	virtual void transforma(int &x, int &y);
};

class Camara3 : public TransformacionCamara
{
// métodos
public:
	virtual void transforma(int &x, int &y);
};

}

#endif	// _TRANSFORMACIONES_CAMARA_H_

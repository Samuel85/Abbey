// TransformacionesCamara.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "TransformacionesCamara.h"

using namespace Abadia;

/////////////////////////////////////////////////////////////////////////////
// transformaciones de la cámara
/////////////////////////////////////////////////////////////////////////////


void Camara0::transforma(int &x, int &y)
{
}

void Camara1::transforma(int &x, int &y)
{
	int temp = y;
	y = x;
	x = 40 - temp;
}

void Camara2::transforma(int &x, int &y)
{
	x = 40 - x;
	y = 40 - y;
}

void Camara3::transforma(int &x, int &y)
{
	int temp = x;
	x = y;
	y = 40 - temp;
}

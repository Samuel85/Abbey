// Serializar.h
//
//              Metodos sobrecargados de los
//              operadores << y >> 
//              para cargar/salvar la partida
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _SERIALIZAR_H
#define _SERIALIZAR_H

#include "fstream"
#include "EntidadJuego.h"
#include "Objeto.h"
#include "Puerta.h"
#include "Sprite.h"
#include "Personaje.h"
#include "Guillermo.h"
#include "Adso.h"
#include "Malaquias.h"
#include "Abad.h"
#include "Berengario.h"
#include "Severino.h"
#include "Jorge.h"
#include "Bernardo.h"
#include "Logica.h"

namespace Abadia {

std::ofstream& operator<< (
	std::ofstream& out,
	const PosicionJuego* const pos );

std::ifstream& operator>> (
	std::ifstream& in,
	PosicionJuego* const pos );

std::ofstream& operator<< (
	std::ofstream& out,
	const Objeto* const obj );

std::ifstream& operator>> (
	std::ifstream& in,
	Objeto* const obj );

std::ofstream& operator<< (
	std::ofstream& out,
	const Puerta* const puerta );

std::ifstream& operator>> (
	std::ifstream& in,
	Puerta* const puerta );

std::ofstream& operator<< (
	std::ofstream& out,
	const Sprite* const sprite );

std::ifstream& operator>> (
	std::ifstream& in,
	Sprite* const sprite );

std::ofstream& operator<< (
	std::ofstream& out,
	const Personaje* const pers );

std::ifstream& operator>> (
	std::ifstream& in,
	Personaje* const pers );

std::ofstream& operator<< (
	std::ofstream& out,
	const Guillermo* const guillermo );

std::ifstream& operator>> (
	std::ifstream& in,
	Guillermo* const guillermo );

std::ofstream& operator<< (
	std::ofstream& out,
	const PersonajeConIA* const persIA );

std::ifstream& operator>> (
	std::ifstream& in,
	PersonajeConIA* const persIA );

std::ifstream& operator>> (
	std::ifstream& in,
	Adso* const adso );

std::ofstream& operator<< (
	std::ofstream& out,
	const Adso* const adso );

std::ofstream& operator<< (
	std::ofstream& out,
	const Malaquias* const malaquias );

std::ifstream& operator>> (
	std::ifstream& in,
	Malaquias* const malaquias );

std::ofstream& operator<< (
	std::ofstream& out,
	const Abad* const abad );

std::ifstream& operator>> (
	std::ifstream& in,
	Abad* const abad );

std::ofstream& operator<< (
	std::ofstream& out,
	const Berengario* const berengario );

std::ifstream& operator>> (
	std::ifstream& in,
	Berengario* const berengario );

std::ofstream& operator<< (
	std::ofstream& out,
	const Severino* const severino );

std::ifstream& operator>> (
	std::ifstream& in,
	Severino* const severino );

std::ofstream& operator<< (
	std::ofstream& out,
	const Jorge* const jorge );

std::ifstream& operator>> (
	std::ifstream& in,
	Jorge* const jorge );

std::ofstream& operator<< (
	std::ofstream& out,
	const Bernardo* const bernardo );

std::ifstream& operator>> (
	std::ifstream& in,
	Bernardo* const bernardo );

std::ofstream& operator<< (
	std::ofstream& out,
	const Logica* const logica);

std::ifstream& operator>> (
	std::ifstream& in,
	Logica* const logica);

}; // namespace Abadia

#endif // _SERIALIZAR_H

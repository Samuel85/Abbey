// Serializar.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "Serializar.h"
#include "Juego.h"
#include "MotorGrafico.h"

//para std::numeric_limits
#include <limits>

std::ofstream& Abadia::operator<< (
		std::ofstream& out,
		const PosicionJuego* const pos )
{
	out << pos->orientacion << "// orientacion\n";
	out << pos->posX << "// posX\n";
	out << pos->posY << "// posY\n";
	out << pos->altura << "// altura\n";

	return out;
}

std::ifstream& Abadia::operator>> (
		std::ifstream& in,
		PosicionJuego* const pos )
{
	std::streamsize intMAX = std::numeric_limits<int>::max();

	in >>  pos->orientacion;
	in.ignore( intMAX, '\n' );

	in >>  pos->posX;
	in.ignore( intMAX, '\n' );

	in >>  pos->posY;
	in.ignore( intMAX, '\n' );

	in >>  pos->altura;
	in.ignore( intMAX, '\n' );

	return in;
}

std::ofstream& Abadia::operator<< (
		std::ofstream& out,
		const Objeto* const obj )
{
	int j;

	out << (PosicionJuego*)obj;

	out << obj->seEstaCogiendo << "// seEstaCogiendo\n";
	out << obj->seHaCogido  << "// seHaCogido\n";

	// No guardamos el puntero,
	// guardamos el numero de personaje,
	// o -1 , si el objeto no apunta a ningun
	// personaje
	for (	j = Juego::numPersonajes-1;
			( obj->personaje != elJuego->personajes[j] &&
			  j>=0 );
			j--) ;
	out << j << "// numPersonaje\n";

	return out;
}

std::ifstream& Abadia::operator>> (
		std::ifstream& in,
		Objeto* const obj )
{
	std::streamsize intMAX = std::numeric_limits<int>::max();
	int tmp;

	in >> (PosicionJuego*)obj;

	in >> obj->seEstaCogiendo;
	in.ignore( intMAX, '\n' );

	in >> obj->seHaCogido;
	in.ignore( intMAX, '\n' );

	in >> tmp;
	in.ignore( intMAX, '\n' );
	if ( tmp==-1 )
	{
		obj->personaje = NULL;
	}
	else
	{
		if ( tmp>=0 && tmp<=Juego::numPersonajes-1 )
		{
			obj->personaje=elJuego->personajes[tmp];
		}
		else
		{
			// el fichero de entrada no es bueno
			obj->personaje = NULL;
		}
	}

	return in;
}

std::ofstream& Abadia::operator<< (
		std::ofstream& out,
		const Puerta* const puerta )
{
	out << (PosicionJuego*)puerta;

	out << puerta->identificador <<  "// identificador\n";
	out << puerta->estaAbierta <<  "// estaAbierta\n";
	out <<  puerta->haciaDentro << "// haciaDentro\n";
	out << puerta->estaFija << "// estaFija\n";
	out << puerta->hayQueRedibujar << "// hayQueRedibujar\n";

	return out;
}

std::ifstream& Abadia::operator>> (
		std::ifstream& in,
		Puerta* const puerta )
{
	std::streamsize intMAX = std::numeric_limits<int>::max();

	in >> (PosicionJuego*)puerta;

	in >>  puerta->identificador;
	in.ignore( intMAX, '\n' );

	in >>  puerta->estaAbierta;
	in.ignore( intMAX, '\n' );

	in >>  puerta->haciaDentro;
	in.ignore( intMAX, '\n' );

	in >>  puerta->estaFija;
	in.ignore( intMAX, '\n' );

	in >>  puerta->hayQueRedibujar;
	in.ignore( intMAX, '\n' );

	return in;
}

std::ofstream& Abadia::operator<< (
		std::ofstream& out,
		const Sprite* const sprite )
{
	out << sprite->esVisible << "// esVisible\n";
	return out;
}

std::ifstream& Abadia::operator>> (
		std::ifstream& in,
		Sprite* const sprite )
{
	std::streamsize intMAX = std::numeric_limits<int>::max();

	in >>  sprite->esVisible;
	in.ignore( intMAX, '\n' );

	return in;
}

std::ofstream& Abadia::operator<< (
		std::ofstream& out,
		const Personaje* const pers)
{
	out << (PosicionJuego*)pers;

	out << pers->estado << "// estado\n";
	out << pers->contadorAnimacion << "// contadorAnimacion\n";
	out << pers->bajando << "// bajando\n";
	out << pers->orientacion << "// orientacion\n";
	out << pers->enDesnivel << "// enDesnivel\n";
	out << pers->giradoEnDesnivel << "// giradoEnDesnivel\n";
	out << pers->flipX << "// flipX\n";
	out << pers->despFlipX << "// despFlipX\n";
	out << pers->despX << "// despX\n";
	out << pers->despY << "// despY\n";
	out << pers->valorPosicion << "// valorPosicion\n";
	out << pers->puedeQuitarObjetos << "// puedeQuitarObjetos\n";
	out << pers->objetos << "// objetos\n";
	out << pers->mascaraObjetos << "// mascaraObjetos\n";
	out << pers->contadorObjetos << "// contadorObjetos\n";
	out << pers->permisosPuertas << "// permisosPuertas\n";
	out << pers->numFotogramas << "// numFotogramas\n";

	return out;
}

std::ifstream& Abadia::operator>> (
		std::ifstream& in,
		Personaje* const pers)
{
	std::streamsize intMAX = std::numeric_limits<int>::max();

	in >> (PosicionJuego*)pers;

	in >> pers->estado;
	in.ignore( intMAX, '\n' );

	in >> pers->contadorAnimacion;
	in.ignore( intMAX, '\n' );

	in >> pers->bajando;
	in.ignore( intMAX, '\n' );

	in >> pers->orientacion;
	in.ignore( intMAX, '\n' );

	in >> pers->enDesnivel;
	in.ignore( intMAX, '\n' );

	in >> pers->giradoEnDesnivel;
	in.ignore( intMAX, '\n' );

	in >> pers->flipX;
	in.ignore( intMAX, '\n' );

	in >> pers->despFlipX;
	in.ignore( intMAX, '\n' );

	in >> pers->despX;
	in.ignore( intMAX, '\n' );

	in >> pers->despY;
	in.ignore( intMAX, '\n' );

	in >> pers->valorPosicion;
	in.ignore( intMAX, '\n' );

	in >> pers->puedeQuitarObjetos;
	in.ignore( intMAX, '\n' );

	in >> pers->objetos;
	in.ignore( intMAX, '\n' );

	in >> pers->mascaraObjetos;
	in.ignore( intMAX, '\n' );

	in >> pers->contadorObjetos;
	in.ignore( intMAX, '\n' );

	in >> pers->permisosPuertas;
	in.ignore( intMAX, '\n' );

	in >> pers->numFotogramas; //?es necesario guardar esto??
	in.ignore( intMAX, '\n' );

	return in;
}

std::ofstream& Abadia::operator<< (
		std::ofstream& out,
		const Guillermo* const guillermo )
{
	out << "// GUILLERMO\n";
	out << (Personaje*)guillermo;
	out << guillermo->incrPosY << "// incrPosY\n";

	return out;
}

std::ifstream& Abadia::operator>> (
		std::ifstream& in,
		Guillermo* const guillermo )
{
	std::streamsize intMAX = std::numeric_limits<int>::max();

	in.ignore( intMAX, '\n' );
	in >> (Personaje*)guillermo;
	in >>  guillermo->incrPosY;
	in.ignore( intMAX, '\n' );

	return in;
}

std::ofstream& Abadia::operator<< (
	std::ofstream& out,
	const PersonajeConIA* const persIA )
{
	out << (Personaje*)persIA ;
	//?lugares??

	out << persIA->mascarasPuertasBusqueda << "// mascarasPuertasBusqueda\n";
	out << persIA->aDondeVa << "// aDondeVa\n";
	out << persIA->aDondeHaLlegado << "// aDondeHaLlegado\n";

	return out;
}

std::ifstream& Abadia::operator>> (
	std::ifstream& in,
	PersonajeConIA* const persIA )
{
	std::streamsize intMAX = std::numeric_limits<int>::max();
	in >> (Personaje*)persIA ;
	persIA->pensarNuevoMovimiento=true;

	//?lugares??

	in >> persIA->mascarasPuertasBusqueda; 
	in.ignore( intMAX, '\n' );

	in >> persIA->aDondeVa; 
	in.ignore( intMAX, '\n' );

	in >> persIA->aDondeHaLlegado; 
	in.ignore( intMAX, '\n' );

	return in;
}

std::ofstream& Abadia::operator<< (
	std::ofstream& out,
	const Adso* const adso )
{
	out << "// ADSO\n";
	out << (PersonajeConIA*)adso;

	out << adso->oldEstado << "// oldEstado\n";
	out << adso->movimientosFrustados << "// movimientosFrustados\n";
	out << adso->cntParaDormir << "// cntParaDormir\n";
	
	return out;
}

std::ifstream& Abadia::operator>> (
	std::ifstream& in,
	Adso* const adso )
{
	std::streamsize intMAX = std::numeric_limits<int>::max();

	in.ignore( intMAX, '\n' );
	in >> (PersonajeConIA*)adso;

	in >> adso->oldEstado;
	in.ignore( intMAX, '\n' );

	in >> adso->movimientosFrustados;
	in.ignore( intMAX, '\n' );

	in >> adso->cntParaDormir;
	in.ignore( intMAX, '\n' );
	
	return in;
}

std::ofstream& Abadia::operator<< (
		std::ofstream& out,
		const Malaquias* const malaquias )
{
	out << "// MALAQUIAS" << std::endl;
	out << (PersonajeConIA*)malaquias;

	out << malaquias->estaMuerto << "// estaMuerto\n";
	out << malaquias->estado2 << "// estado2\n";
	out << malaquias->contadorEnScriptorium << "// contadorEnScriptorium\n";

	return out;
}

std::ifstream& Abadia::operator>> (
		std::ifstream& in,
		Malaquias* const malaquias )
{
	std::streamsize intMAX = std::numeric_limits<int>::max();

	in.ignore( intMAX, '\n' );
	in >> (PersonajeConIA*)malaquias;

	in >> malaquias->estaMuerto;
	in.ignore( intMAX, '\n' );

	in >> malaquias->estado2;
	in.ignore( intMAX, '\n' );

	in >> malaquias->contadorEnScriptorium;
	in.ignore( intMAX, '\n' );

	return in;
}

std::ofstream& Abadia::operator<< (
	std::ofstream& out,
	const Abad* const abad )
{
	out << "// ABAD" << std::endl;
	out << (PersonajeConIA*)abad;

	out << abad->contador << "// contador\n";
	out << abad->numFrase << "// numFrase\n";
	out << abad->guillermoBienColocado << "// guillermoBienColocado\n";
	out << abad->lleganLosMonjes << "// lleganLosMonjes\n";
	out << abad->guillermoHaCogidoElPergamino << "// guillermoHaCogidoElPergamino\n";

	return out;
}

std::ifstream& Abadia::operator>> (
	std::ifstream& in,
	Abad* const abad )
{
	std::streamsize intMAX = std::numeric_limits<int>::max();

	in.ignore( intMAX, '\n' );
	in >> (PersonajeConIA*)abad;

	in >> abad->contador;
	in.ignore( intMAX, '\n' );

	in >> abad->numFrase;
	in.ignore( intMAX, '\n' );

	in >> abad->guillermoBienColocado;
	in.ignore( intMAX, '\n' );

	in >> abad->lleganLosMonjes;
	in.ignore( intMAX, '\n' );

	in >> abad->guillermoHaCogidoElPergamino;
	in.ignore( intMAX, '\n' );

	return in;
}

std::ofstream& Abadia::operator<< (
	std::ofstream& out,
	const Berengario* const berengario )
{
	out << "// BERENGARIO" << std::endl;
	out << (PersonajeConIA*)berengario;

	// CPC out << (berengario->datosCara[0] == 0xb35b ) << "// encapuchado\n";
	out << (berengario->datosCara[0] == 69308 ) << "// encapuchado\n";
	out << berengario->estado2 << "// estado2\n";
	out << berengario->estaVivo << "// estaVivo\n";
	out << berengario->contadorPergamino << "// contadorPergamino\n";

	return out;
}

std::ifstream& Abadia::operator>> (
	std::ifstream& in,
	Berengario* const berengario )
{
	bool encapuchado;
	std::streamsize intMAX = std::numeric_limits<int>::max();

	in.ignore( intMAX, '\n' );
	in >> (PersonajeConIA*)berengario;

	in >> encapuchado;
	in.ignore( intMAX, '\n' );
	berengario->fijaCapucha(encapuchado);

	in >> berengario->estado2;
	in.ignore( intMAX, '\n' );

	in >> berengario->estaVivo;
	in.ignore( intMAX, '\n' );

	in >> berengario->contadorPergamino;
	in.ignore( intMAX, '\n' );

	return in;
}

std::ofstream& Abadia::operator<< (
	std::ofstream& out,
	const Severino* const severino )
{
	out << "// SEVERINO" << std::endl;
	out << (PersonajeConIA*)severino;
	out << severino->estaVivo << "// estaVivo\n";

	return out;
}

std::ifstream& Abadia::operator>> (
	std::ifstream& in,
	Severino* const severino )
{
	std::streamsize intMAX = std::numeric_limits<int>::max();

	in.ignore( intMAX, '\n' );
	in >> (PersonajeConIA*)severino;

	in >> severino->estaVivo;
	in.ignore( intMAX, '\n' );

	return in;
}

std::ofstream& Abadia::operator<< (
	std::ofstream& out,
	const Jorge* const jorge )
{
	out << "// JORGE" << std::endl;
	out << (PersonajeConIA*)jorge;
	out << jorge->estaActivo << "// estaActivo\n";
	out << jorge->contadorHuida << "// contadorHuida\n";

	return out;
}

std::ifstream& Abadia::operator>> (
	std::ifstream& in,
	Jorge* const jorge )
{
	std::streamsize intMAX = std::numeric_limits<int>::max();

        in.ignore( intMAX, '\n' );
	in >> (PersonajeConIA*)jorge;

	in >> jorge->estaActivo;
        in.ignore( intMAX, '\n' );

	in >> jorge->contadorHuida;
        in.ignore( intMAX, '\n' );

	return in;
}

std::ofstream& Abadia::operator<< (
	std::ofstream& out,
	const Bernardo* const bernardo )
{
	out << "// BERNARDO" << std::endl;
	out << (PersonajeConIA*)bernardo;
	out << bernardo->estaEnLaAbadia << "// estaEnLaAbadia\n";

	return out;
}

std::ifstream& Abadia::operator>> (
	std::ifstream& in,
	Bernardo* const bernardo )
{
	std::streamsize intMAX = std::numeric_limits<int>::max();

	in.ignore( intMAX, '\n' );
	in >> (PersonajeConIA*)bernardo;

	in >> bernardo->estaEnLaAbadia;
	in.ignore( intMAX, '\n' );

	return in;
}

std::ofstream& Abadia::operator<< (
	std::ofstream& out,
	const Logica* const logica)
{
	Guillermo *guillermo;	// guillermo
	Adso *adso;		// adso
	Malaquias *malaquias;	// malaquias
	Abad *abad;		// el abad
	Berengario *berengario;	// berengario
	Severino *severino;	// severino
	Jorge *jorge;		// jorge
	Bernardo *bernardo;	// bernardo gui

	out << logica->dia << "// dia\n";
	out << logica->momentoDia << "// momentoDia\n";
	out << logica->duracionMomentoDia << "// duracionMomentoDia\n";
	out << logica->oldMomentoDia << "// oldMomentoDia\n";
	out << logica->avanzarMomentoDia << "// avanzarMomentoDia\n";
	out << logica->obsequium << "// obsequium\n";
	out << logica->haFracasado << "// haFracasado\n";
	out << logica->investigacionCompleta << "// investigacionCompleta\n";
	out << logica->bonus << "// bonus\n";
	out << logica->mascaraPuertas << "// mascaraPuertas\n";
	out << logica->espejoCerrado << "// espejoCerrado\n";
	out << logica->numeroRomano << "// numeroRomano\n";
	out << logica->despDatosAlturaEspejo << "// despDatosAlturaEspejo\n";
	out << logica->despBloqueEspejo << "// despBloqueEspejo\n";
	out << logica->seAcabaLaNoche << "// seAcabaLaNoche\n";
	out << logica->haAmanecido << "// haAmanecido\n";
	out << logica->usandoLampara << "// usandoLampara\n";
	out << logica->lamparaDesaparecida << "// lamparaDesaparecida\n";
	out << logica->tiempoUsoLampara << "// tiempoUsoLampara\n";
	out << logica->cambioEstadoLampara << "// cambioEstadoLampara\n";
	out << logica->cntTiempoAOscuras << "// cntTiempoAOscuras\n";
	out << logica->cntLeeLibroSinGuantes << "// cntLeeLibroSinGuantes\n";
	out << logica->pergaminoGuardado << "// pergaminoGuardado\n";
	out << logica->numeroAleatorio << "// numeroAleatorio\n";
	out << logica->hayMovimiento << "// hayMovimiento\n";
	out << logica->cntMovimiento << "// cntMovimiento\n";
	out << logica->numPersonajeCamara << "// numPersonajeCamara\n";
	out << logica->opcionPersonajeCamara << "// opcionPersonajeCamara\n";

	for (int i = 0; i < Juego::numSprites; i++){
		out << "// SPRITE " << i << "\n";
		out << elJuego->sprites[i];
	}
	// obtiene los personajes del juego
	guillermo = (Guillermo *)elJuego->personajes[0];
	adso = (Adso *)elJuego->personajes[1];
	malaquias = (Malaquias *)elJuego->personajes[2];
	abad = (Abad *)elJuego->personajes[3];
	berengario = (Berengario *)elJuego->personajes[4];
	severino = (Severino *)elJuego->personajes[5];
	jorge = (Jorge *)elJuego->personajes[6];
	bernardo = (Bernardo *)elJuego->personajes[7];

	out << guillermo;
	out << adso;
	out << malaquias;
	out << abad;
	out << berengario;
	out << severino;
	out << jorge;
	out << bernardo;

	Puerta **puertas = elJuego->puertas;

	for (int i = 0; i < Juego::numPuertas; i++){
		out << "// PUERTA " << i << "\n";
		out << puertas[i];
	}

	Objeto **objetos = elJuego->objetos;

	for (int i = 0; i < Juego::numObjetos; i++){
		out << "// OBJETO " << i << "\n";
		out << (Objeto*)objetos[i];
	}

	return out; // sin esto, falla en PNACL
}

std::ifstream& Abadia::operator>> (
	std::ifstream& in,
	Logica* const logica)
{
	Guillermo *guillermo;	// guillermo
	Adso *adso;		// adso
	Malaquias *malaquias;	// malaquias
	Abad *abad;		// el abad
	Berengario *berengario;	// berengario
	Severino *severino;	// severino
	Jorge *jorge;		// jorge
	Bernardo *bernardo;	// bernardo gui

	std::streamsize intMAX = std::numeric_limits<int>::max();

	in >> logica->dia;
	in.ignore( intMAX, '\n' );

	in >> logica->momentoDia;
	in.ignore( intMAX, '\n' );

	in >> logica->duracionMomentoDia;
	in.ignore( intMAX, '\n' );

	in >> logica->oldMomentoDia;
	in.ignore( intMAX, '\n' );

	in >> logica->avanzarMomentoDia;
	in.ignore( intMAX, '\n' );

	in >> logica->obsequium;
	in.ignore( intMAX, '\n' );

	in >> logica->haFracasado;
	in.ignore( intMAX, '\n' );

	in >> logica->investigacionCompleta;
	in.ignore( intMAX, '\n' );

	in >> logica->bonus;
	in.ignore( intMAX, '\n' );

	in >> logica->mascaraPuertas;
	in.ignore( intMAX, '\n' );

	in >> logica->espejoCerrado;
	in.ignore( intMAX, '\n' );

	in >> logica->numeroRomano;
	in.ignore( intMAX, '\n' );

	in >> logica->despDatosAlturaEspejo;
	in.ignore( intMAX, '\n' );

	in >> logica->despBloqueEspejo;
	in.ignore( intMAX, '\n' );

	if ( logica->espejoCerrado )
	{
		// preservar el numeroRomano
		// ya que iniciaHabitacionEspejo 
		// lo inicializa a 0	
		int tmp = logica->numeroRomano;
		logica->iniciaHabitacionEspejo();
		logica->numeroRomano = tmp;
	}
	else
	{
		// modifica los datos de altura de la habitación del espejo para que guillermo puede atravesarlo
                logica->roms[logica->despDatosAlturaEspejo] = 0xff;

		// cambia los datos de un bloque de la habitación del espejo para que el espejo esté abierto
                logica->roms[logica->despBloqueEspejo] = 0x51;

		// indica que se ha abierto el espejo y hay que la pantalla ha cambiado
	        elMotorGrafico->posXPantalla = elMotorGrafico->posYPantalla = -1;
	}

	in >> logica->seAcabaLaNoche;
	in.ignore( intMAX, '\n' );

	in >> logica->haAmanecido;
	in.ignore( intMAX, '\n' );

	in >> logica->usandoLampara;
	in.ignore( intMAX, '\n' );

	in >> logica->lamparaDesaparecida;
	in.ignore( intMAX, '\n' );

	in >> logica->tiempoUsoLampara;
	in.ignore( intMAX, '\n' );

	in >> logica->cambioEstadoLampara;
	in.ignore( intMAX, '\n' );

	in >> logica->cntTiempoAOscuras;
	in.ignore( intMAX, '\n' );

	in >> logica->cntLeeLibroSinGuantes;
	in.ignore( intMAX, '\n' );

	in >> logica->pergaminoGuardado;
	in.ignore( intMAX, '\n' );

	in >> logica->numeroAleatorio;
	in.ignore( intMAX, '\n' );

	in >> logica->hayMovimiento;
	in.ignore( intMAX, '\n' );

	in >> logica->cntMovimiento;
	in.ignore( intMAX, '\n' );

	in >> logica->numPersonajeCamara;
	in.ignore( intMAX, '\n' );

	in >> logica->opcionPersonajeCamara;
	in.ignore( intMAX, '\n' );

	for (int i = 0; i < Juego::numSprites; i++){
		in.ignore( intMAX, '\n' );
		in >> elJuego->sprites[i];
	} 

	// obtiene los personajes del juego
	guillermo = (Guillermo *)elJuego->personajes[0];
	adso = (Adso *)elJuego->personajes[1];
	malaquias = (Malaquias *)elJuego->personajes[2];
	abad = (Abad *)elJuego->personajes[3];
	berengario = (Berengario *)elJuego->personajes[4];
	severino = (Severino *)elJuego->personajes[5];
	jorge = (Jorge *)elJuego->personajes[6];
	bernardo = (Bernardo *)elJuego->personajes[7];

	in >> guillermo;
	in >> adso;
	in >> malaquias;
	in >> abad;
	in >> berengario;
	in >> severino;
	in >> jorge;
	in >> bernardo;

	Puerta **puertas = elJuego->puertas;

	for (int i = 0; i < Juego::numPuertas; i++){
		in.ignore( intMAX, '\n' );
		in >> puertas[i];
	}

	Objeto **objetos = elJuego->objetos;

	for (int i = 0; i < Juego::numObjetos; i++){
		in.ignore( intMAX, '\n' );
		in >> (Objeto*)objetos[i];
	}

	return in;
}

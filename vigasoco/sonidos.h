// SONIDOS.h
//
//	This class has helper methods for working with the sounds of the game
//	Sorry, using wav, the CPC464 sound is not emulated
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _Vigasoco_SONIDOS_H_
#define _Vigasoco_SONIDOS_H_



namespace Abadia {

	class SONIDOS {
		public:
			static const UINT8 Abrir = 0;
			static const UINT8 Aporrear = 1;
			static const UINT8 Campanas = 2;
			static const UINT8 Cerrar = 3;
			static const UINT8 Coger = 4;
			static const UINT8 Dejar = 5;
			static const UINT8 Espejo = 6;
			static const UINT8 Final = 7;
			static const UINT8 Fondo = 8;
			static const UINT8 Inicio = 9;
			static const UINT8 Pasos = 10;
			static const UINT8 Tintineo = 11;
	};

} // namespace Abadia

#endif	// _Vigasoco_SONIDOS_H_

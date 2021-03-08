

// Pergamino.cpp
//
/////////////////////////////////////////////////////////////////////////////


#include <cassert>

#include "cpc6128.h"

#include "Juego.h"
#include "Paleta.h"
#include "Pergamino.h"
#include "system.h"
#include "alphabet.h"

using namespace Abadia;

int Pergamino::AdaptaColorAPaletaVGA(int a,int b)
{
	int color=cpc6128->unpackPixelMode1(a, b);
	switch(color)
	{
		case 0: color=255; break;
		case 1: color=20; break;
		case 2: color=0; break;
		case 3: color=1; break;
		default: printf("Pergamino::AdaptaColorAPaletaVGA color %d\n",color);
	}
	return color;
}

Pergamino::Pergamino()
{
	cpc6128 = elJuego->cpc6128;
	roms = elJuego->roms;

	// puntero a la tabla de punteros a los gr???ficos de los caracteres
	UINT16* charTable = (UINT16*) &roms[0x680c];

	for (unsigned char c=0;c<255-0x20;c++) 
	{
		// si el caracter no est??? definido, muestra una 'z'
		TablapTrazosCaracter[c]=roms+SDL_SwapLE16(charTable['z'-0x20]);
	}
	// Recorre desde el caracter 0x21 hasta el 0x126 , apuntando
	// a los datos de la rom original donde se encuentran los
	// datos para dibujar los trazos
	// son los caracteres imprimibles del ASCII
	// a???n as??? algunos no est???n definidos porque el juego original no los usaba
	for (char c='!';c<'~';c++) 
	{
		// si el caracter no est??? definido, dejamos apuntando a la 'z'
		if (charTable[c - 0x20] != 0)
		{
			TablapTrazosCaracter[c-0x20]=roms+SDL_SwapLE16(charTable[c-0x20]);
		}
	}
	// Se a???aden los caracteres usados en las traducciones del remake PC
	// de los textos del pergamino

	//TablapTrazosCaracter['???'-0x20]=charD6; 
	TablapTrazosCaracter[0xD6-0x20]=charD6; 
	//TablapTrazosCaracter['???'-0x20]=charE0;
	TablapTrazosCaracter[0xE0-0x20]=charE0;
	//TablapTrazosCaracter['???'-0x20]=charE1;
	TablapTrazosCaracter[0xE1-0x20]=charE1;
	//TablapTrazosCaracter['???'-0x20]=charE3;
	TablapTrazosCaracter[0xE3-0x20]=charE3;
	//TablapTrazosCaracter['???'-0x20]=charE4;
	TablapTrazosCaracter[0xE4-0x20]=charE4;
	//TablapTrazosCaracter['???'-0x20]=charE7;
	TablapTrazosCaracter[0xE7-0x20]=charE7;
	//TablapTrazosCaracter['???'-0x20]=charE8;
	TablapTrazosCaracter[0xE8-0x20]=charE8;
	//TablapTrazosCaracter['???'-0x20]=charE9;
	TablapTrazosCaracter[0xE9-0x20]=charE9;
	//TablapTrazosCaracter['???'-0x20]=charEA;
	TablapTrazosCaracter[0xEA-0x20]=charEA;
	//TablapTrazosCaracter['???'-0x20]=charEC;
	TablapTrazosCaracter[0xEC-0x20]=charEC;
	//TablapTrazosCaracter['???'-0x20]=charED;
	TablapTrazosCaracter[0xED-0x20]=charED;
	//TablapTrazosCaracter['???'-0x20]=charEF;
	TablapTrazosCaracter[0xEF-0x20]=charEF;
	//TablapTrazosCaracter['???'-0x20]=charF1;
	TablapTrazosCaracter[0xF1-0x20]=charF1;
	//TablapTrazosCaracter['???'-0x20]=charF2;
	TablapTrazosCaracter[0xF2-0x20]=charF2;
	//TablapTrazosCaracter['???'-0x20]=charF3;
	TablapTrazosCaracter[0xF3-0x20]=charF3;
	//TablapTrazosCaracter['???'-0x20]=charF6;
	TablapTrazosCaracter[0xF6-0x20]=charF6;
	//TablapTrazosCaracter['???'-0x20]=charF9;
	TablapTrazosCaracter[0xF9-0x20]=charF9;
	//TablapTrazosCaracter['???'-0x20]=charFA;
	TablapTrazosCaracter[0xFA-0x20]=charFA;
	//TablapTrazosCaracter['???'-0x20]=charFC; 
	TablapTrazosCaracter[0xFC-0x20]=charFC; 

	// En el original, no se usaba la w,
	// as??? que la aprovechaban para la ???
	// aqu??? la ??? est???n en charF1
	// asi que dejamos la w en su sitio, 0x77
	//TablapTrazosCaracter['w'-0x20]=char77; 
	TablapTrazosCaracter[0x77-0x20]=char77; 

	// En la rom original hay muchos caracteres ASCII
	// no definidos
	//TablapTrazosCaracter['''-0x20]=char27; 
	TablapTrazosCaracter[0x27-0x20]=char27; 
	//TablapTrazosCaracter['B'-0x20]=char42; 
	TablapTrazosCaracter[0x42-0x20]=char42; 
	//TablapTrazosCaracter['F'-0x20]=char46; 
	TablapTrazosCaracter[0x46-0x20]=char46; 
	//TablapTrazosCaracter['I'-0x20]=char49; 
	TablapTrazosCaracter[0x49-0x20]=char49; 
	//TablapTrazosCaracter['K'-0x20]=char4B; 
	TablapTrazosCaracter[0x4B - 0x20]=char4B; 
	//TablapTrazosCaracter['N'-0x20]=char4E; 
	TablapTrazosCaracter[0x4E - 0x20]=char4E; 
	//TablapTrazosCaracter['Q'-0x20]=char51; 
	TablapTrazosCaracter[0x51-0x20]=char51; 
	//TablapTrazosCaracter['R'-0x20]=char52; 
	TablapTrazosCaracter[0x52-0x20]=char52; 
	//TablapTrazosCaracter['U'-0x20]=char55; 
	TablapTrazosCaracter[0x55-0x20]=char55; 
	//TablapTrazosCaracter['V'-0x20]=char56; 
	TablapTrazosCaracter[0x56-0x20]=char56; 
	//TablapTrazosCaracter['W'-0x20]=char57; 
	TablapTrazosCaracter[0x57-0x20]=char57; 
	//TablapTrazosCaracter['X'-0x20]=char58; 
	TablapTrazosCaracter[0x58-0x20]=char58; 
	//TablapTrazosCaracter['Z'-0x20]=char5A; 
	TablapTrazosCaracter[0x5A-0x20]=char5A; 
	
	pasaPaginaStep = 1;
	x = 240;
	y = 0;
	dim = 3;
	writing = false;
	num=0;
}

Pergamino::~Pergamino()
{
}

/////////////////////////////////////////////////////////////////////////////
// dibujo del pergamino
/////////////////////////////////////////////////////////////////////////////

// dibuja el pergamino
void Pergamino::dibuja()
{
	// limpia la memoria de video	
	cpc6128->fillMode1Rect(0, 0, 320, 200, 255); // VGA

	// limpia los bordes del rect???ngulo que formar??? el pergamino
	// VGA
	cpc6128->fillMode1Rect(0, 0, 64, 200, 20);
	cpc6128->fillMode1Rect(192 + 64, 0, 64, 200, 20);
	cpc6128->fillMode1Rect(0, 192, 320, 8, 20);

	// apunta a los datos del borde superior del pergamino
	UINT8* data = &roms[0x788a];

	// rellena la parte superior del pergamino
	dibujaTiraHorizontal(0, data);

	// rellena la parte derecha del pergamino
	data = &roms[0x7a0a];
	dibujaTiraVertical(248, data);

	// rellena la parte izquierda del pergamino
	data = &roms[0x7b8a];
	dibujaTiraVertical(64, data);

	// rellena la parte inferior del pergamino
	data = &roms[0x7d0a];
	dibujaTiraHorizontal(184, data);
}

// dibuja un borde horizontal del pergamino de 8 pixels de alto
void Pergamino::dibujaTiraHorizontal(int y, UINT8 *data)
{
	// recorre todo el ancho del pergamino
	for (int i = 0; i < 192/4; i++)
	{
		// la parte superior ocupa 8 pixels de alto
		for (int j = 0; j < 8; j++)
		{
			for (int k = 0; k < 4; k++)
			{				
				cpc6128->setMode1Pixel(64 + 4*i + k, j + y, AdaptaColorAPaletaVGA(*data, k));
			}
			data++;
		}
	}
}

// dibuja el pergamino
void Pergamino::dibujaTiraVertical(int x, UINT8 *data)
{
	// recorre el alto del pergamino
	for (int j = 0; j < 192; j++)
	{
		// lee 8 pixels y los escribe en pantalla
		for (int i = 0; i < 2; i++)
		{
			for (int k = 0; k < 4; k++)
			{
				cpc6128->setMode1Pixel(x + 4*i + k, j, AdaptaColorAPaletaVGA(*data, k));
			}
			data++;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// escritura de texto en el pergamino
/////////////////////////////////////////////////////////////////////////////
void Pergamino::muestraTexto(const unsigned char *mensaje)
{
	if (writing) {		
		// Tweak to speed up the writing process on the manuscript		
		for (int i=0;i<8;i++) dibujaTexto();
	}
	else {
		// pone la paleta negra
		elJuego->paleta->setGamePalette(0);

		// dibuja el pergamino
		dibuja();
		
		// posici???n inicial del texto en el pergamino
		posX = 76;
		posY = 16;
		
		// puntero a la tabla de punteros a los gr???ficos de los caracteres
		charTable = (UINT16*) &roms[0x680c];

		// pone la paleta del pergamino
		elJuego->paleta->setGamePalette(1); //CPC
		writing = true;
		finished = false;
		
		texto = mensaje;
		pTrazosCaracter = TablapTrazosCaracter[(*texto)-0x20];
	}
}

void Pergamino::dibujaTexto()
{
	// si se puls??? el bot???n 1 o espacio, termina	
	if (BUTTON_YES){		
		finished = true;
	} 
	else 
	{
		bool ret = false;		
		switch (*texto)
		{
			case 0x1a:			// f???n de pergamino					
				break;
			case 0x0d:			// salto de l???nea					
				if (posY > 148)
				{
					// si hay que pasar p???gina del pergamino					
					ret = pasaPagina();
					if (ret == false)
					{
						posX = 76;
						posY = 16;
					}						
				}
				else
				{
					posX = 76;
					posY += 16;					
				}
				
				// apunta al siguiente car???cter a imprimir			
				if ((*texto != 0x1a) && !ret){
					texto++;
					pTrazosCaracter = TablapTrazosCaracter[(*texto)-0x20];
				}
				break;
			case 0x20:			// espacio
				posX += 10;					
				if (*texto != 0x1a)
				{
					texto++;
					pTrazosCaracter = TablapTrazosCaracter[(*texto)-0x20];
				}
				break;
			case 0x0a:			// salto de p???gina
				posX = 76;
				posY = 16;								
				if ((*texto != 0x1a) && (!pasaPagina()))
				{
					texto++;
					pTrazosCaracter = TablapTrazosCaracter[(*texto)-0x20];
				}				
				break;
			default:			// car???cter imprimible
				//UINT8 const * pTrazosCaracter = TablapTrazosCaracter[(*texto)-0x20];
				//pTrazosCaracter = TablapTrazosCaracter[(*texto)-0x20];
				// elige un color dependiendo de si es may???sculas o min???sculas

				// la paleta CPC del pergamino es 07,28,20,12
				// o sea
				// el color 0 es el 07 que es pink
				// el color 1 es el 28 que es red
				// el color 2 es el 20 que es black
				// el color 3 es el 12 que es bright red

				//CPC					int color = (((*texto) & 0x60) == 0x40) ? 3 : 2;
				//Para VGA pongo el color 1?? para las mayusculas y el color 0?? para las minusculas
				int color = (((*texto) & 0x60) == 0x40) ? 1 : 0;

				// obtiene el desplazamiento a los datos de formaci???n del car???cter
				// transformando del dato nativo en litte_endian
				// al tipo del sistema (si es little_endian no hace nada,
				// y si es big_endian intercambia el orden)
				
				//int charOffset = 0;SDL_SwapLE16(charTable[(*texto) - 0x20]);

				// para alertar si nos hemos dejado algo sin definir
				//if (*texto!='z' && charOffset==SDL_SwapLE16(charTable['z'-0x20])) 	
				//	printf("????????? NOS HEMOS DEJADO ALGUN CARACTER SIN DEFINIR !!! %c\n",*texto);

				// si el caracter no est??? definido, muestra una 'z'
				//if (charTable[(*texto) - 0x20] == 0){
				//	charOffset = SDL_SwapLE16(charTable['z' - 0x20]);
				//}

				
				if ((*pTrazosCaracter & 0xf0) != 0xf0)
				{
					int newPosX = posX+(*pTrazosCaracter & 0x0f);
					int newPosy = posY+((*pTrazosCaracter>>4)&0x0f);

					// dibuja el trazo del car???cter
					cpc6128->setVGAPixel(newPosX, newPosy, color);
					pTrazosCaracter++;					
				}
				else 
				{				
					// avanza la posici???n hasta el siguiente car???cter
					// posX += roms[charOffset] & 0x0f;
					posX += *pTrazosCaracter & 0x0f;
					if (*texto != 0x1a)
					{
						texto++;
						pTrazosCaracter = TablapTrazosCaracter[(*texto)-0x20];
					}
				}

		}
	}	
}

/////////////////////////////////////////////////////////////////////////////
// paso de p???gina del pergamino
/////////////////////////////////////////////////////////////////////////////
// dibuja un tri???ngulo rect???ngulo de color1 con catetos paralelos a los ejes x e y, y limpia los 4 
//  pixels a la derecha de la hipotenusa del tri???ngulo con el color2
void Pergamino::dibujaTriangulo(int x, int y, int lado, int color1, int color2)
{
	lado = lado*4;

	for (int j = 0; j < lado; j++)
	{
		// dibuja el tri???ngulo
		for (int i = 0; i <= j; i++)
		{
			cpc6128->setMode1Pixel(x + i, y + j, color1);
		}

		// elimina restos de una ejecuci???n anterior
		for (int i = 0; i < 4; i++)
		{			
			cpc6128->setMode1Pixel(x + j + i + 1, y + j, 255); 
		}
	}
}

// restaura un trozo de 8x8 pixels de la parte superior y otro de la parte derecha del pergamino
void Pergamino::restauraParteSuperiorYDerecha(int x, int y, int lado)
{
	x = x + 4;

	// apunta a los datos borrados del borde superior del pergamino
	UINT8* data = &roms[0x788a + (48 - lado)*4*2];

	// 8 pixels de ancho
	for (int i = 0; i < 2; i++)
	{
		// 8 pixels de alto
		for (int j = 0; j < 8; j++)
		{
			for (int k = 0; k < 4; k++)
			{				
				cpc6128->setMode1Pixel(x + 4*i + k, y + j, AdaptaColorAPaletaVGA(*data, k));
			}
			data++;
		}
	}
	x = 248;
	y = (lado - 3)*4;

	// apunta a los datos borrados de la parte derecha del pergamino
	data = &roms[0x7a0a + y*2];

	// 8 pixels de alto
	for (int j = 0; j < 8; j++)
	{
		// 8 pixels de ancho
		for (int i = 0; i < 2; i++)
		{
			for (int k = 0; k < 4; k++)
			{				
				cpc6128->setMode1Pixel(x + 4*i + k, y + j, AdaptaColorAPaletaVGA(*data, k));
			}
			data++;
		}
	}
}

// restaura un trozo de 4x8 pixels de la parte inferior del pergamino
void Pergamino::restauraParteInferior(int x, int y, int lado)
{
	x = 64 + lado*4;
	y = 184;

	// apunta a los datos borrados del borde inferior del pergamino
	UINT8* data = &roms[0x7d0a + lado*4*2];

	// dibuja un trozo de 4x8 pixels de la parte inferior del pergamino
	for (int j = 0; j < 8; j++)
	{
		for (int k = 0; k < 4; k++)
		{			
			cpc6128->setMode1Pixel(x + k, y + j, AdaptaColorAPaletaVGA(*data, k));
		}
		data++;
	}
}

// realiza el efecto de pasar una p???gina del pergamino
bool Pergamino::pasaPagina()
{	
	bool ret = true;
	switch(pasaPaginaStep)
	{
		case 1:
			// step 1
			// realiza el efecto del paso de p???gina desde la esquina superior derecha hasta la mitad de la p???gina
			if (num < 45)
			{
				dibujaTriangulo(x, y, dim, 1, 0);
				restauraParteSuperiorYDerecha(x, y, dim);

				x = x - 4;
				dim++;
				num++;
				
			}
			else
			{
				pasaPaginaStep = 2;
			}
			break;
		case 2:
			// step 2
			restauraParteSuperiorYDerecha(x, y, dim);
			x = 64;
			y = 4;
			dim = 47;
			num = 0;
			pasaPaginaStep = 3;
			break;
		case 3:	
			// step 3
			// realiza el efecto del paso de p???gina desde la mitad de la p???gina hasta terminar en la esquina inferior izquierda
			if (num < 46)
			{
				dibujaTriangulo(x, y, dim, 1, 0);
				y = y - 4;

				// apunta a los datos borrados del borde izquierdo del pergamino
				UINT8* data = &roms[0x7b8a + y*2];

				// dibuja un trozo de 8x4 de la parte izquierda del pergamino
				for (int j = 0; j < 4; j++)
				{
					for (int i = 0; i < 2; i++)
					{
						for (int k = 0; k < 4; k++)
						{							
							cpc6128->setMode1Pixel(x + 4*i + k, y + j, AdaptaColorAPaletaVGA(*data, k));
						}
						data++;
					}
				}

				// restaura un trozo de 4x8 pixels de la parte inferior del pergamino
				restauraParteInferior(x, y, dim);
				y = y + 8;
				dim--;
				num++;
				
			}
			else 
			{
				pasaPaginaStep = 4;
			}
			break;
		case 4:	
			// step 4
			restauraParteInferior(x, y, 1);
			restauraParteInferior(x, y, 0);
			pasaPaginaStep = 1;
			num = 0;
			x = 240;
			y = 0;
			dim = 3;		
			ret = false;			
			break;
	}
	return ret;
}
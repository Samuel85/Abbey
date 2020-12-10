#ifndef __TEXTS_H__
#define __TEXTS_H__

#include <string>

enum LANGUAGE
{	
	CASTELLANO,
	ENGLISH,
	
	TOTAL_LANGUAGES
};

const char * menu[2][5] =
{
	{	// Castellano
		"NUEVA PARTIDA",
		"CARGAR",
		"GUARDAR",
		"IDIOMA",
		"CONTINUAR"
	},
	{	// English
		"NEW GAME",
		"LOAD",
		"SAVE",
		"LANGUAGE",
		"CONTINUE"
	}
};

const char * menuText[8][9] = 
{ 
	{ // 0 Castellano
		"IDIOMA",
		"CARGAR PARTIDA" ,
		"GRABAR PARTIDA" ,
		"GR??FICOS VGA-CPC" ,
		"AYUDA" ,
		"TUTORIAL",
		"REINICIAR",
		"SONIDO", 
		"JUGAR"
	},
	{ // 1 INGLES
		"LANGUAGE" , 
		"LOAD GAME" ,
		"SAVE GAME" ,
		"GRAPHICS VGA-CPC" ,
		"HELP" ,
		"TUTORIAL",
		"RESTART",
		"SOUND",
		"PLAY"
	},
	{ // 2 PORTUGUES BRASIL
		"L?NGUA" ,
		"CARREGAR JOGO" ,
		"SALVAR JOGO",
		"GRAPHICS VGA-CPC",
		"SOCORRO" ,
		"TUTORIAL",
		"REINICIAR",
		"SOM",
		"JOGAR"
	},
	{ // 3 CATALAN
		"LLENGUATGE" ,
		"CARREGAR EL JOC" ,
		"GUARDAR LA PARTIDA" ,
		"GRAPHICS VGA-CPC",
		"AJUDA",
		"TUTORIAL",
		"REINICIA",
		"SO",
		"JUGAR"
	},
	{ // 4 GALLEGO
		"LINGUA" ,
		"XOGO DE CARGA",
		"GARDAR XOGO",
		"GRAPHICS VGA-CPC",
		"AXUDA",
		"TITOR?A",
		"REINICIA",
		"SON",
		"XOGAR"
	},
	{ // 5 ITALIANO
		"LINGUAGGIO" ,
		"CARICA GIOCO",
		"SALVA IL GIOCO" ,
		"GRAPHICS VGA-CPC",
		"AIUTO" ,
		"TUTORIAL",
		"RICOMINCIA",
		"SUONO",
		"GIOCARE"
	},
	{ // 6 FINES
		"KIELI" ,
		"LATAA PELI" ,
		"TALLENTAA PELIN" ,
		"GRAPHICS VGA-CPC",
		"AUTA" ,
		"TUTORIAL",
		"UUDELLEENK?YNNIST??",
		"??I",
		"PELATA"
	},
	{ // 7 PORTUGUES
		"LÍNGUA" ,
		"CARREGAR JOGO" ,
		"SALVAR JOGO",
		"GRAPHICS VGA-CPC",
		"SOCORRO" ,
		"TUTORIAL",
		"REINICIAR",
		"SOM",
		"JOGAR"
	}
};


const char * textSave[8][8] = 
{ 
	{ // 0 Castellano
		"SLOT 0", 
		"SLOT 1", 
		"SLOT 2", 
		"SLOT 3", 
		"SLOT 4", 
		"SLOT 5", 
		"SLOT 6",
		"VOLVER"
	},
	{ // 1 INGLES
		"0 SLOT 0",
		"1 SLOT 1", 
		"2 SLOT 2", 
		"3 SLOT 3", 
		"4 SLOT 4", 
		"5 SLOT 5", 
		"6 SLOT 6",
		"7 RETURN TO PREVIOUS MENU"
	},
	{ // 2 PORTUGUES BRASIL
		"0 SLOT 0", 
		"1 SLOT 1", 
		"2 SLOT 2", 
		"3 SLOT 3", 
		"4 SLOT 4", 
		"5 SLOT 5", 
		"6 SLOT 6",
		"7 VOLVER AL MENU ANTERIOR"
	},
	{ // 3 CATALAN
		"0 SLOT 0", 
		"1 SLOT 1", 
		"2 SLOT 2", 
		"3 SLOT 3", 
		"4 SLOT 4", 
		"5 SLOT 5", 
		"6 SLOT 6",
		"7 VOLVER AL MENU ANTERIOR"
	},
	{ // 4 GALLEGO
		"0 SLOT 0", 
		"1 SLOT 1", 
		"2 SLOT 2", 
		"3 SLOT 3", 
		"4 SLOT 4", 
		"5 SLOT 5", 
		"6 SLOT 6",
		"7 VOLVER AL MENU ANTERIOR"
	},
	{ // 5 ITALIANO
		"0 SLOT 0", 
		"1 SLOT 1", 
		"2 SLOT 2", 
		"3 SLOT 3", 
		"4 SLOT 4", 
		"5 SLOT 5", 
		"6 SLOT 6",
		"7 VOLVER AL MENU ANTERIOR"
	},
	{ // 6 FINES
		"0 SLOT 0",
		"1 SLOT 1", 
		"2 SLOT 2", 
		"3 SLOT 3", 
		"4 SLOT 4", 
		"5 SLOT 5", 
		"6 SLOT 6",
		"7 RETURN TO PREVIOUS MENU"
	},
	{ // 7 PORTUGUES
		"0 SLOT 0", 
		"1 SLOT 1", 
		"2 SLOT 2", 
		"3 SLOT 3", 
		"4 SLOT 4", 
		"5 SLOT 5", 
		"6 SLOT 6",
		"7 VOLVER AL MENU ANTERIOR"
	}
};

 const char * textLanguage[8][8] = 
	{ 
		{ // 0 Castellano
			"CASTELLANO" , 
			"ENGLISH" ,
			"PORTUGU??S BRASIL" ,
			"CATAL??N" ,
			"GALLEGO" ,
			"ITALIANO",
			"FINES",
			"PORTUGU??S"
		},
		{ // 1 INGLES
			"CASTELLANO" , 
			"ENGLISH" ,
			"PORTUGU??S BRASIL" ,
			"CATAL??N" ,
			"GALLEGO" ,
			"ITALIANO",
			"FINES",
			"PORTUGU??S"
		},
		{ // 2 PORTUGUES BRASIL
			"CASTELLANO" , 
			"ENGLISH" ,
			"PORTUGU??S BRASIL" ,
			"CATAL??N" ,
			"GALLEGO" ,
			"ITALIANO",
			"FINES",
			"PORTUGU??S"
		},
		{ // 3 CATALAN
			"CASTELLANO" , 
			"ENGLISH" ,
			"PORTUGU??S BRASIL" ,
			"CATAL??N" ,
			"GALLEGO" ,
			"ITALIANO",
			"FINES",
			"PORTUGU??S"
		},
		{ // 4 GALLEGO
			"CASTELLANO" , 
			"ENGLISH" ,
			"PORTUGU??S BRASIL" ,
			"CATAL??N" ,
			"GALLEGO" ,
			"ITALIANO",
			"FINES",
			"PORTUGU??S"
		},
		{ // 5 ITALIANO
			"CASTELLANO" , 
			"ENGLISH" ,
			"PORTUGU??S BRASIL" ,
			"CATAL??N" ,
			"GALLEGO" ,
			"ITALIANO",
			"FINES",
			"PORTUGU??S"
		},
		{ // 6 FINES
			"CASTELLANO" , 
			"ENGLISH" ,
			"PORTUGU??S BRASIL" ,
			"CATAL??N" ,
			"GALLEGO" ,
			"ITALIANO",
			"FINES",
			"PORTUGU??S"
		},
		{ // 7 PORTUGUES
			"CASTELLANO" , 
			"ENGLISH" ,
			"PORTUGU??S BRASIL" ,
			"CATAL??N" ,
			"GALLEGO" ,
			"ITALIANO",
			"FINES",
			"PORTUGU??S"
		}
	};

	static const char * textCameras[8][9] = 
	{ 
		{ // 0 Castellano
			"   USA ESTAS TECLAS DURANTE EL JUEGO   " , 
			"1 DEJAD PULSADO PARA SEGUIR AL ABAD",
			"2 DEJAD PULSADO PARA SEGUIR A SEVERINO",
			"3 DEJAD PULSADO PARA SEGUIR A MALAQUIAS",
			"4 DEJAD PULSADO PARA SEGUIR A BERENGARIO",
			"5 DEJAD PULSADO PARA SEGUIR A JORGE",
			"6 DEJAD PULSADO PARA SEGUIR A BERNARDO",
			"7 DEJAD PULSADO PARA SEGUIR A ADSO",
			"    -PULSA ESPACIO PARA CONTINUAR-"
			
		},
		{ // 1 INGLES
			"ERR",
			"",
			"",
			"",
			"" ,
			"",
			"",
			"",
			"PRESS SPACE TO CONTINUE"
		},
		{ // 2 PORTUGUES BRASIL
			"TODO" , 
			"",
			"",
			"",
			"" ,
			"",
			"",
			"",
			"PRESS SPACE TO CONTINUE"
		},
		{ // 3 CATALAN
			"TODO" , 
			"" ,
			"" ,
			"" ,
			"" ,
			"",
			"",
			"",
			"PRESS SPACE TO CONTINUE"
		},
		{ // 4 GALLEGO
			"TODO" , 
			"" ,
			"" ,
			"" ,
			"" ,
			"",
			"",
			"",
			"PRESS SPACE TO CONTINUE"
		},
		{ // 5 ITALIANO
			"TODO",
			"" ,
			"" ,
			"" ,
			"" ,
			"",
			"",
			"",
			"PRESS SPACE TO CONTINUE"
		},
		{ // 6 FINES
			"TODO",
			"" ,
			"" ,
			"" ,
			"" ,
			"",
			"",
			"",
			"PRESS SPACE TO CONTINUE"
		},
		{ // 7 PORTUGUES
			"TODO" ,
			"" ,
			"" ,
			"" ,
			"" ,
			"",
			"",
			"",
			"PRESS SPACE TO CONTINUE"
		}
	};

	static const char * textEnhancements[8][9] = 
	{ 
		{ // 0 Castellano
			"   USA ESTAS TECLAS DURANTE EL JUEGO   " , 
			"G GRABAR LA PARTIDA" , 
			"C CARGAR LA PARTIDA" ,
			"F2 CAMBIAR ENTRE GR??FICOS VGA O CPC", 
			"F3 PANTALLA COMPLETA",
			"F5 MOSTRAR MAPAS",
			"SUPR PAUSA",
			"",
			"    -PULSA ESPACIO PARA CONTINUAR-"
		},
		{ // 1 INGLES
			"            KEYBOARD SHORTCUTS         ",
			"G SAVE GAME",
			"C LOAD GAME",
			"F2 SWITCH VGA CPC GRAPHICS",
			"F3 FULLSCREEN" ,
			"F5 SHOW MAPS",
			"SUPR PAUSE",
			"",
			"           -PRESS SPACE-          " 
		},
		{ // 2 PORTUGUES BRASIL
			"   USA ESTAS TECLAS DURANTE EL JUEGO   " , 
			"G GRABAR LA PARTIDA" , 
			"C CARGAR LA PARTIDA" ,
			"F2 CAMBIAR ENTRE GR??FICOS VGA O CPC", 
			"F3 PANTALLA COMPLETA",
			"F5 MOSTRAR MAPAS",
			"SUPR PAUSA",
			"",
			"    -PULSA ESPACIO PARA CONTINUAR-"
		},
		{ // 3 CATALAN
			"   USA ESTAS TECLAS DURANTE EL JUEGO   " , 
			"G GRABAR LA PARTIDA" , 
			"C CARGAR LA PARTIDA" ,
			"F2 CAMBIAR ENTRE GR??FICOS VGA O CPC", 
			"F3 PANTALLA COMPLETA",
			"F5 MOSTRAR MAPAS",
			"SUPR PAUSA",
			"",
			"    -PULSA ESPACIO PARA CONTINUAR-"
		},
		{ // 4 GALLEGO
			"   USA ESTAS TECLAS DURANTE EL JUEGO   " , 
			"G GRABAR LA PARTIDA" , 
			"C CARGAR LA PARTIDA" ,
			"F2 CAMBIAR ENTRE GR??FICOS VGA O CPC", 
			"F3 PANTALLA COMPLETA",
			"F5 MOSTRAR MAPAS",
			"SUPR PAUSA",
			"",
			"    -PULSA ESPACIO PARA CONTINUAR-"
		},
		{ // 5 ITALIANO
			"   USA ESTAS TECLAS DURANTE EL JUEGO   " , 
			"G GRABAR LA PARTIDA" , 
			"C CARGAR LA PARTIDA" ,
			"F2 CAMBIAR ENTRE GR??FICOS VGA O CPC", 
			"F3 PANTALLA COMPLETA",
			"F5 MOSTRAR MAPAS",
			"SUPR PAUSA",
			"",
			"    -PULSA ESPACIO PARA CONTINUAR-"
		},
		{ // 6 FINES
			"            KEYBOARD SHORTCUTS         ",
			"G SAVE GAME",
			"C LOAD GAME",
			"F2 SWITCH VGA CPC GRAPHICS",
			"F3 FULLSCREEN" ,
			"F5 SHOW MAPS",
			"SUPR PAUSE",
			"",
			"           -PRESS SPACE-          " 
		},
		{ // 7 PORTUGUES
			"   USA ESTAS TECLAS DURANTE EL JUEGO   " , 
			"G GRABAR LA PARTIDA" , 
			"C CARGAR LA PARTIDA" ,
			"F2 CAMBIAR ENTRE GR??FICOS VGA O CPC", 
			"F3 PANTALLA COMPLETA",
			"F5 MOSTRAR MAPAS",
			"SUPR PAUSA",
			"",
			"    -PULSA ESPACIO PARA CONTINUAR-"
		}
	};

const std::string frase1[8] = {
	"HAS RESUELTO EL", // 0 castellano
	"YOU HAVE SOLVED", // 1 INGLES
	"VOC?? RESOLVEU", // 2 PORTUGUES BRASIL
	"HAS  RESOLT EL", // 3 CATAL??N
	"RESOLVICHELO", // 4 GALLEGO
	"HAI RISOLTO IL", // 5 ITALIANO
	"YOU HAVE SOLVED", // 6 FINES
	"RESOLVESTE " // 7 PORTUGUES
};
/*
const std::string porcentaje[8] = {
	"  XX POR CIENTO DE", // 0 castellano
	"  XX  PER  CENT", // 1 ingles
	"  XX POR CENTO DA", // 2 portugues brasil
	"  XX PER CENT DE", // 3 CATAL??N
	"  XX POR CENTO DA", // 4 GALLEGO
	"  XX PER CENTO", // 5 ITALIANO
	"  XX  PER  CENT", // 6 fines
	"  XX POR CENTO DA" // 7 PORTUGUES
};
*/
const std::string frase3[8] = {
	"LA INVESTIGACI??N", // 0 castellano
	"OF THE RESEARCH", // 1 INGLES
	"INVESTIGA????O", // 2 PORTUGUES BRASIL
	"LA INVESTIGACI??", // 3 CATAL??N
	"INVESTIGACI??N", // 4 GALLEGO
	"DELL'INDAGINE", // 5 italiano
	"OF THE RESEARCH", // 6 FINES
	"INVESTIGA????O" // 7 PORTUGUES
};
const std::string frase4[8] = {
	"PRESIONA CUALQUIER BOT?N PARA REINICIAR", // 0 castellano
	"PRESS ANY BUTTON TO RESTART", // 1 ingles
	"PRESSIONE QUALQUER BOT?O PARA REINICIAR", // 2 portugues brasil
	"PREMEU QUALSEVOL BOT? PER REINICIAR", // 3 catal??n
	"PRESSA CALQUERA BOT?N PARA REINICIAR", // 4 gallego
	"PREMERE UN TASTO QUALSIASI PER RIAVVIARE", // 5 italiano
	"PAINA KAIKKI PAINIKET UUDELLEEN", // 6 fines
	"PRESSIONE QUALQUER BOT?O PARA REINICIAR" // 7 portugues
	
};

#endif
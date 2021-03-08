#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <iostream>
#include <cassert>
#include <vector>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#ifdef ANDROID
#include <android/log.h>
#endif

#define WINDOW_WIDTH 1280  
#define WINDOW_HEIGHT 800
#define TEXTURE_WIDTH 640
#define TEXTURE_HEIGHT 400
#define WINDOW_TITLE "Abbey"
#define GAME_FRAME_TIME 130
#define SCROLL_FRAME_TIME 60

enum MUSICFILES{
  START,
  END,
  BACKGROUND,

  TOTAL_MUSIC_FILES
};
#ifdef ANDROID
static const char* const
musicPathList[] = {
	"roms/abadia/inicio.wav",
	"roms/abadia/final.wav",
	"roms/abadia/fondo.wav",
};
#else
static const char* const
musicPathList[] = {
	"./roms/abadia/inicio.wav",
	"./roms/abadia/final.wav",
	"./roms/abadia/fondo.wav",
};
#endif

enum SOUNDFILES{
  OPEN,
  HIT,
  BELLS,
  CLOSE,
  GET,
  LET,
  MIRROR,
  STEPS,
  JINGLE,

  TOTAL_SOUND_FILES
};	

enum STATES{
	INTRO,
	SCROLL,
	MENU,
	LANGUAGE,
	LOAD,
	SAVE,	
	PLAY,
	ASK_NEW_GAME,
	ASK_CONTINUE,
	ASK_EXIT,
	ENDING
};

enum TILEMAP{
	DAY,
	NIGHT,
	LAMP,

	TOTAL_TILEMAP_FILES
};

#ifdef ANDROID
static const char* 
soundsPathList[] = {
	"roms/abadia/abrir.wav",
	"roms/abadia/aporrear.wav",
	"roms/abadia/campanas.wav",
	"roms/abadia/cerrar.wav",
	"roms/abadia/coger.wav",
	"roms/abadia/dejar.wav",
	"roms/abadia/espejo.wav",		
	"roms/abadia/pasos.wav",
	"roms/abadia/tintineo.wav",
	};
#else
static const char* 
soundsPathList[] = {
	"./roms/abadia/abrir.wav",
	"./roms/abadia/aporrear.wav",
	"./roms/abadia/campanas.wav",
	"./roms/abadia/cerrar.wav",
	"./roms/abadia/coger.wav",
	"./roms/abadia/dejar.wav",
	"./roms/abadia/espejo.wav",		
	"./roms/abadia/pasos.wav",
	"./roms/abadia/tintineo.wav",
	};
#endif

static const char*
tilemapList[] = {
	"res/Mosaico.bmp",
	"res/Noche.bmp",
	"res/Lampara.bmp"
};

struct PlayerInput
{
	bool up, down, left, right, button1, button2, button3, button4;
	bool start;
};

#define BUTTON_YES sys->pad.button3
#define BUTTON_NO sys->pad.button2

struct System
{	
	PlayerInput pad;
	bool exit = false;
	bool informationMode = false;	
	bool enableJoystick = true;
	bool fullscreen = false;
	bool haveHapticDevice = false;
	int w = WINDOW_WIDTH;
	int h = WINDOW_HEIGHT;
	int minimumFrameTime = GAME_FRAME_TIME;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
	const Uint32 rmask = 0xff000000;
	const Uint32 gmask = 0x00ff0000;
	const Uint32 bmask = 0x0000ff00;
	const Uint32 amask = 0x000000ff;
#else
	const Uint32 rmask = 0x000000ff;
	const Uint32 gmask = 0x0000ff00;
	const Uint32 bmask = 0x00ff0000;
	const Uint32 amask = 0xff000000;
#endif

	SDL_Surface *surface;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	SDL_Window *window;
	SDL_GameController *gamepad;
	SDL_Haptic *hapticDevice;

	SDL_Surface *tilemap[TOTAL_TILEMAP_FILES];

	std::vector<Mix_Chunk*>sounds;
	std::vector<Mix_Chunk*>music;
	
	void init();
	void quit();
	void playMusic(int i);
	void stopMusic();
	void playSound(int i);
	void updateScreen();
	void handleEvents();
	void hapticFeedback();

	SDL_Surface* flipSurfaceHorizontally(SDL_Surface *src);

	void setFastSpeed();
	void setNormalSpeed();
	Uint32 RGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	void updateTexture();
	void exitGame();
	void print(const std::string message);
};
extern System *const sys;

#endif

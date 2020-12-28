#ifndef SYSTEM_H
#define SYSTEM_H

#include <SDL.h>
#include <SDL_mixer.h>
#include <string>
#include <iostream>

#ifdef ANDROID
#include <android/log.h>
#else
#include <SDL_ttf.h>
#endif

#include <vector>

#define WINDOW_WIDTH 1280  
#define WINDOW_HEIGHT 800
#define TEXTURE_WIDTH 640
#define TEXTURE_HEIGHT 400
#define WINDOW_TITLE "Abbey"
#define GAME_FRAME_TIME 130
#define SCROLL_FRAME_TIME 10

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

#ifdef ANDROID
static const char* const
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
static const char* const
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
	int w = WINDOW_WIDTH;
	int h = WINDOW_HEIGHT;
	int minimumFrameTime = GAME_FRAME_TIME;

	SDL_Surface *surface;
	SDL_Renderer *renderer;
	SDL_Texture *texture;
	SDL_Window *window;
	SDL_GameController *gamepad;
	SDL_Haptic *hapticDevice;

	#ifndef ANDROID
	TTF_Font *font;
	#endif
	std::vector<Mix_Chunk*>sounds;
	//std::vector<Mix_Music*>music;
    std::vector<Mix_Chunk*>music;
	
	void init();
	void quit();
	void playMusic(int i);
	void stopMusic();
	void playSound(int i);
	void updateScreen();
	void handleEvents();
	void hapticFeedback();

	void setFastSpeed()
	{
		minimumFrameTime = SCROLL_FRAME_TIME;
	}	
	void setNormalSpeed()
	{
		minimumFrameTime = GAME_FRAME_TIME;
	}
	inline Uint32 RGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
	{
		return SDL_MapRGBA(surface->format, r,g,b,a);
	}
	inline void updateTexture()
	{
		SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);
	}
	void exitGame()
	{
		exit = true;
	}
	void print(const std::string message)
    {
        #ifdef ANDROID
		 __android_log_print(ANDROID_LOG_DEBUG, "ABBEY", "%s\n", message.c_str());
		#else
         std::cout << message;
        #endif
    }
};
extern System *const sys;

#endif

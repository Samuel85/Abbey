#include <iostream>
#include <vector>
#include "system.h"

static System tmpSys;
System *const sys = &tmpSys;

void System::init()
{
	pad.up = false;
	pad.down = false;
	pad.left = false;
	pad.right = false;
	pad.button1 = false;
	pad.button2 = false;
	pad.button3 = false;
	pad.button4 = false;
	pad.start = false;

	gamepad = NULL;
	hapticDevice = NULL;
	Uint32 windowFlags = SDL_WINDOW_SHOWN;
	Uint32 initFlags = SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER;  

	SDL_Init(SDL_INIT_VIDEO | initFlags | SDL_INIT_AUDIO);    
	
#ifdef ANDROID
	windowFlags = windowFlags | SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN | SDL_WINDOW_RESIZABLE;
#endif

	// Joypad
	if( SDL_NumJoysticks() < 1 ){		
		print("Warning: controller not found.\n");		
	}
	else{
		gamepad = SDL_GameControllerOpen(0);
		if (gamepad == NULL){
			print("Warning: Can't open SDL_GameControllerOpen(0)\n");
		}
		SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt");
	}
	if (SDL_InitSubSystem(SDL_INIT_HAPTIC) < 0){        
		print ("Error: Can't init SDL_InitSubSystem(SDL_INIT_HAPTIC).\n");
	}
  	else if (SDL_NumHaptics() > 0){
		haveHapticDevice = true;
		hapticDevice = SDL_HapticOpen(0);
		if (hapticDevice == NULL){
			print("Warning: SDL_HapticOpen(0) failed (non-fatal)\n");
		}
		else if (SDL_HapticRumbleInit(hapticDevice) != 0){			
			print("Warning: SDL_HapticRumbleInit failed (non-fatal)\n");
			SDL_HapticClose(hapticDevice);
			hapticDevice = NULL;
		}
	}

#ifdef RG350
	// soft haptic response
	if (SDL_HapticRumblePlay(hapticDevice, 0.30f /* Strength */, 10 /* Time */) < 0){		
        print("Warning: SDL_HapticRumbleStop failed\n");
	}
#endif
	
#ifdef ANDROID
	window = SDL_CreateWindow(WINDOW_TITLE, 0, 0, 0, 0, windowFlags);
	SDL_GetWindowSize(window, &w, &h);
#else
	window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, windowFlags);
#endif
	
	if (window == NULL){		
        print("ERROR: Could not create window.\n");
	}

	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
	if( renderer == NULL ){
        print("ERROR: Could not create renderer.\n");		
	}

	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ){
		print("ERROR: Mix_OpenAudio.\n");
	}
	for (int i=0;i<TOTAL_SOUND_FILES;i++)
	{
		sounds.push_back(Mix_LoadWAV(soundsPathList[i]));
		if (sounds[i] == NULL){
			print("Error: can't read sound file\n");			
		}
	}

	for (int i=0;i<TOTAL_MUSIC_FILES;i++){
		music.push_back(Mix_LoadWAV(musicPathList[i]));
		if (music[i] == NULL){
			print("Error: can't read music file.\n");			
		}
	}
	
	surface = SDL_CreateRGBSurface(0, TEXTURE_WIDTH,TEXTURE_HEIGHT,32, rmask, gmask,bmask, amask);
	if (surface == NULL){		
        print ("Error: Can't create surface.\n");
	}
	texture = SDL_CreateTextureFromSurface(renderer, surface);	
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
}

void System::hapticFeedback()
{
	if (!haveHapticDevice) return;
    
	if (SDL_HapticRumblePlay(hapticDevice, 0.80f /* Strength */, 200 /* Time */) < 0){
		print("Error: SDL_HapticRumblePlay failed\n");
	}
}

void System::quit()
{
	SDL_GameControllerClose(gamepad);
	SDL_HapticClose(hapticDevice);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);	

	// music
	for (int i=0;i<TOTAL_MUSIC_FILES;i++){
        Mix_FreeChunk(music[i]);
	}
	music.clear();
	for (int i=0;i<TOTAL_SOUND_FILES;i++){
		Mix_FreeChunk(sounds[i]);
	}
	sounds.clear(); 

	SDL_Quit();	
}

void System::updateScreen()
{
	// Test this line with all the supported platforms. Fixes video problems with 
	// the raspberry pi with KMSDRM
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}
void System::playMusic(int i)
{	
    Mix_PlayChannel( -1, music[i], -1);
}
void System::stopMusic()
{
    Mix_HaltChannel(-1);
}
void System::playSound(int i)
{
	Mix_PlayChannel( -1, sounds[i], 0);
}
void System::handleEvents()
{
	SDL_Event event;	
	while(SDL_PollEvent(&event))
	{
		SDL_ControllerButtonEvent ev = event.cbutton;
		if( event.type == SDL_QUIT ){
			exit = true;
		}
		else if(event.type == SDL_KEYDOWN)
		{
			switch(event.key.keysym.sym)
			{
#if RG350                
				case SDLK_ESCAPE:
#else
                case SDLK_1:
#endif                    
					informationMode = !informationMode;
					break;
				case SDLK_LEFT:
					pad.left = true;
					break;
				case SDLK_RIGHT:
					pad.right = true;
					break;
				case SDLK_UP:
					pad.up = true;
					break;
				case SDLK_DOWN:
					pad.down = true;
					break;				
#if RG350                    
				case SDLK_LSHIFT: //Y (S)
#else
                case SDLK_RETURN:
#endif		
					pad.button3 = true;
					break;
#if RG350                    
				case SDLK_LALT: //B (N)
#else
                case SDLK_n:
#endif                    
					pad.button4 = true;
					break;	
				case SDLK_SPACE: //X (Drop)
					pad.button1 = true;
					break;
				case SDLK_LCTRL: //A (?)
					pad.button2 = true;
					break;
#if RG350
				case SDLK_RETURN:
#else
                case SDLK_2:
#endif 
					pad.start = true;
					break;									
				default:
					break;
			}
		}
		else if(event.type == SDL_KEYUP)
		{
			switch(event.key.keysym.sym)
			{
				case SDLK_LEFT:
					pad.left = false;
					break;
				case SDLK_RIGHT:
					pad.right = false;
					break;
				case SDLK_UP:
					pad.up = false;
					break;
				case SDLK_DOWN:
					pad.down = false;
					break;
#if RG350                    
				case SDLK_LSHIFT: //Y (S)
#else
                case SDLK_RETURN:
#endif                    
					pad.button3 = false;
					break;
#if RG350                    
				case SDLK_LALT: //B (N)
#else
                case SDLK_n:
#endif 
					pad.button4 = false;
					break;	
				case SDLK_SPACE: //X (Drop)
					pad.button1 = false;
					break;
				case SDLK_LCTRL: //A (?)
					pad.button2 = false;
					break;
#if RG350
				case SDLK_RETURN:
#else
                case SDLK_2:
#endif                    
					pad.start = true;
					break;		
				default:
					break;
			}
		}
		else if (event.type == SDL_CONTROLLERBUTTONDOWN)
		{
			
			switch(ev.button)
			{
				case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
					pad.left = true;
					break;
				case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
					pad.right = true;
					break;
				case SDL_CONTROLLER_BUTTON_DPAD_UP:
					pad.up = true;
					break;
				case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
					pad.down = true;
					break;
				case SDL_CONTROLLER_BUTTON_A: //Y (S)
					pad.button3 = true;
					break;
				case SDL_CONTROLLER_BUTTON_X: //B (N)
					pad.button4 = true;
					break;	
				case SDL_CONTROLLER_BUTTON_Y: //X (Drop)
					pad.button1 = true;
					break;
				case SDL_CONTROLLER_BUTTON_B: //A (?)
					pad.button2 = true;
					break;
				case SDL_CONTROLLER_BUTTON_START:
					pad.start = true;
					break;
				case SDL_CONTROLLER_BUTTON_BACK :
					informationMode = !informationMode;					
					break;		
				default:
					break;
			}
		}
		else if (event.type == SDL_CONTROLLERBUTTONUP)
		{			
			switch(ev.button)
			{
				case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
					pad.left = false;
					break;
				case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
					pad.right = false;
					break;
				case SDL_CONTROLLER_BUTTON_DPAD_UP:
					pad.up = false;
					break;
				case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
					pad.down = false;
					break;
				case SDL_CONTROLLER_BUTTON_A: //Y (S)
					pad.button3 = false;
					break;
				case SDL_CONTROLLER_BUTTON_X: //B (N)
					pad.button4 = false;
					break;	
				case SDL_CONTROLLER_BUTTON_Y: //X (Drop)
					pad.button1 = false;
					break;
				case SDL_CONTROLLER_BUTTON_B: //A (?)
					pad.button2 = false;
					break;
				case SDL_CONTROLLER_BUTTON_START:
					pad.start = false;
					break;
				case SDL_CONTROLLER_BUTTON_BACK :					
				default:
					break;
			}
		}		
	}
}

void System::setFastSpeed()
{
	minimumFrameTime = SCROLL_FRAME_TIME;
}	
void System::setNormalSpeed()
{
	minimumFrameTime = GAME_FRAME_TIME;
}
Uint32 System::RGBA(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	return SDL_MapRGBA(surface->format, r,g,b,a);
}
void System::updateTexture()
{
	SDL_UpdateTexture(texture, NULL, surface->pixels, surface->pitch);
}
void System::exitGame()
{
	exit = true;
}
void System::print(const std::string message)
{
	#ifdef ANDROID
		__android_log_print(ANDROID_LOG_DEBUG, "ABBEY", "%s\n", message.c_str());
	#else
		std::cout << message;
	#endif
}

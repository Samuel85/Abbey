#include <iostream>
#include <SDL.h>
#include <SDL_mixer.h>
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
	Uint32 initFlags = SDL_INIT_VIDEO  | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER;  

	SDL_Init(SDL_INIT_VIDEO | initFlags | SDL_INIT_AUDIO);    
	if (fullscreen) {
		windowFlags = windowFlags | SDL_WINDOW_FULLSCREEN;
	}

	// Joypad
	if( SDL_NumJoysticks() < 1 ){
        std::cout << "Warning: No joysticks connected!\n";
    }
	else
	{
		gamepad = SDL_GameControllerOpen(0);
		if (gamepad == NULL){
			std::cout << "Warning: No joysticks connected!\n";
		}
		SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt");
	}
	if (SDL_InitSubSystem(SDL_INIT_HAPTIC) < 0){
		std::cout << "SDL force feedback initialisation failed (non-fatal): %s\n" << SDL_GetError() << std::endl;
	}
  	else if (SDL_NumHaptics() > 0){
		hapticDevice = SDL_HapticOpen(0);
		if (hapticDevice == NULL){
			std::cout << "SDL_HapticOpen(0) failed (non-fatal): %s\n" << SDL_GetError() << std::endl;
		}
		else if (SDL_HapticRumbleInit(hapticDevice) != 0){
			std::cout << "SDL_HapticRumbleInit failed (non-fatal): %s\n"<< SDL_GetError() << std::endl;
			SDL_HapticClose(hapticDevice);
			hapticDevice = NULL;
		}
	}
	#ifdef RG350
	// soft haptic response
	if (SDL_HapticRumblePlay(hapticDevice, 0.30f /* Strength */, 10 /* Time */) < 0){
		std::cout <<  "SDL_HapticRumbleStop failed: %s\n"<< SDL_GetError() << std::endl;
	}
	#endif
	//

	window = SDL_CreateWindow(WINDOW_TITLE, 0, 0, w, h, windowFlags);                            
	if (window == NULL){
		std::cout << "ERROR: Could not create window: " << SDL_GetError() << std::endl;    
	}

	renderer = SDL_CreateRenderer( window, -1, SDL_RENDERER_ACCELERATED );
	if( renderer == NULL ){
		std::cout << "ERROR: Could not create renderer: " << SDL_GetError() << std::endl;    
	}

	if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 ){
		std::cout << "ERROR: Mix_OpenAudio" << std::endl;
	}
	for (int i=0;i<TOTAL_SOUND_FILES;i++)
	{
		sounds.push_back(Mix_LoadWAV(soundsPathList[i]));
		if (sounds[i] == NULL){
			std::cout << "Error: can't read " << soundsPathList[i] <<std::endl;	
		}
	}

	for (int i=0;i<TOTAL_MUSIC_FILES;i++)
	{
		music.push_back(Mix_LoadMUS(musicPathList[i]));
		if (music[i] == NULL){
			std::cout << "Error: can't read " << musicPathList[i] <<std::endl;
		}
	}

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

	//surface = SDL_CreateRGBSurface(0, TEXTURE_WIDTH,TEXTURE_HEIGHT>>1,32, rmask, gmask,bmask, amask);
	surface = SDL_CreateRGBSurface(0, TEXTURE_WIDTH,TEXTURE_HEIGHT,32, rmask, gmask,bmask, amask);
	if (surface == NULL){
		std::cout << "Surface Error:" << SDL_GetError() << std::endl;
	}
	texture = SDL_CreateTextureFromSurface(renderer, surface);	
	SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
	
	// Init Fonts
	if (TTF_Init() < 0){
		std::cout << "Error initializing TTF library" << std::endl;
	}
	font = TTF_OpenFont("fonts/arial.ttf", 10);
	if (font == NULL){
		std::cout << "Error loading fonts" << std::endl;
	}
}

void System::hapticFeedback()
{
	if (SDL_HapticRumblePlay(hapticDevice, 0.80f /* Strength */, 200 /* Time */) < 0){
		std::cout <<  "SDL_HapticRumbleStop failed: %s\n"<< SDL_GetError() << std::endl;
	}
}

void System::quit()
{
	SDL_GameControllerClose(gamepad);
	SDL_HapticClose(hapticDevice);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);	

	for (int i=0;i<TOTAL_MUSIC_FILES;i++){
		Mix_FreeMusic(music[i]);
	}
	music.clear();

	for (int i=0;i<TOTAL_SOUND_FILES;i++){
		Mix_FreeChunk(sounds[i]);
	}
	sounds.clear();  
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_Quit();	
}

void System::updateScreen()
{
	//SDL_Rect src;
	//src.x = 64;
	//src.y = 0;
	//src.w = 512;
	//src.h = 160;
	//SDL_RenderCopy(renderer, texture, &src, NULL);
	SDL_RenderCopy(renderer, texture, NULL, NULL);
	SDL_RenderPresent(renderer);
}
void System::playMusic(int i)
{
	Mix_PlayMusic( music[i], -1);
}
void System::stopMusic()
{
	Mix_HaltMusic();
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
		if( event.type == SDL_QUIT )
		{
			exit = true;
		}
		else if(event.type == SDL_KEYDOWN)
		{
			switch(event.key.keysym.sym)
			{
				case SDLK_ESCAPE:					
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
				case SDLK_LSHIFT: //Y (S)
					pad.button3 = true;
					break;
				case SDLK_LALT: //B (N)
					pad.button4 = true;
					break;	
				case SDLK_SPACE: //X (Drop)
					pad.button1 = true;
					break;
				case SDLK_LCTRL: //A (?)
					pad.button2 = true;
					break;
				case SDLK_RETURN:
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
				case SDLK_LSHIFT: //Y (S)
					pad.button3 = false;
					break;
				case SDLK_LALT: //B (N)
					pad.button4 = false;
					break;	
				case SDLK_SPACE: //X (Drop)
					pad.button1 = false;
					break;
				case SDLK_LCTRL: //A (?)
					pad.button2 = false;
					break;
				case SDLK_RETURN:
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
		/*		
		else if( event.type == SDL_JOYBUTTONUP)
		{ 
	        switch(event.jbutton.button)
			{

				
			}
    	}
		else if(event.type == SDL_JOYBUTTONDOWN)
		{
			switch(event.jbutton.button)
			{
				
			}
		}
		*/
	}
}
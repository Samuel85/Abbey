// VigasocoSDL.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "IDrawPlugin.h"
#include "SDLPalette.h"
#include "FileLoader.h"
#include "FontManager.h"
#include "IInputPlugin.h"

#include "VigasocoSDL.h"


// para los eventos y para poner el titulo de la ventana
#include "SDL.h"

#ifdef _EE
bool PS2SpecificInit(void);
#endif

// current plugin versions
int VigasocoSDL::g_currentVideoPluginVersion = 1;
int VigasocoSDL::g_currentInputPluginVersion = 1;
int VigasocoSDL::g_currentLoaderPluginVersion = 1;
int VigasocoSDL::g_currentAudioPluginVersion = 1;

// paths for the plugins
std::string VigasocoSDL::g_videoPluginPath = "video/";
std::string VigasocoSDL::g_inputPluginPath = "input/";
std::string VigasocoSDL::g_loaderPluginPath = "loaders/";
std::string VigasocoSDL::g_audioPluginPath = "audio/";

/////////////////////////////////////////////////////////////////////////////
// initialization and cleanup
/////////////////////////////////////////////////////////////////////////////

//VigasocoSDL::VigasocoSDL(std::string game,
//				std::string drawPluginsDLL,
//				std::string drawPlugin,
//				Strings inputPluginsDLLs,
//				Strings inputPlugins,
//				std::string audioPluginsDLL,
//				std::string audioPlugin,
//				Strings paths)
VigasocoSDL::VigasocoSDL()
{
//	_pluginHandler = 0;
//	_game = game;
//
//	_sDrawPluginsDLL = drawPluginsDLL;
//	_sDrawPlugin = drawPlugin;
//	_sInputPluginsDLLs = inputPluginsDLLs;
//	_sInputPlugins = inputPlugins;
//	_sAudioPluginsDLL = audioPluginsDLL;
//	_sAudioPlugin = audioPlugin;
//	_sPaths = paths;
}

VigasocoSDL::~VigasocoSDL()
{
}

/////////////////////////////////////////////////////////////////////////////
// platform services
/////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////
// construction template methods overrides
/////////////////////////////////////////////////////////////////////////////

bool VigasocoSDL::platformSpecificInit()
{
	// creates the plugin handler
	// TODO:
	// esto no compila en Linux y no lo usamos en la version SDL
	//_pluginHandler = new PluginHandler(_settings); 

#ifdef _EE
	return PS2SpecificInit();
#else
	return true;
#endif
}

void VigasocoSDL::createPalette()
{
	_palette = new SDLPalette();
}

void VigasocoSDL::addCustomLoaders(FileLoader *fl)
{
// TODO
/* !!! FALTA POR IMPLEMENTAR EN LINUX 
	HANDLE hSearch;
	WIN32_FIND_DATA findData;

	SetCurrentDirectory(g_loaderPluginPath.substr(0, g_loaderPluginPath.size() - 1).c_str());

	// traverse all the files in the loader directory searching for plugins
	hSearch = FindFirstFile("*.dll", &findData);
	if (hSearch != INVALID_HANDLE_VALUE){
		do {
			ILoader *customLoader = 0;
			DLLEntry entry;

			// try to load the plugin from the DLL
			if (_pluginHandler->loadPlugin(findData.cFileName, 
				"CustomLoader", LOADER_PLUGIN, g_currentLoaderPluginVersion, &entry)){
				customLoader = (ILoader *)entry.plugin;
			}

			if (customLoader != 0){
				// save DLL reference for later
				_loaderPluginsInfo.push_back(entry);

				// add the plugin to the fileLoader
				fl->addLoader(customLoader);
			}
		} while (FindNextFile(hSearch, &findData));
	}
	FindClose(hSearch);

	SetCurrentDirectory("..");

	// add optional paths to the file loader
	for (Strings::size_type i = 0; i < _sPaths.size(); i++){
		fl->addPath(_sPaths[i]);
	}
*/
}

void VigasocoSDL::createDrawPlugin()
{
	
}

void VigasocoSDL::createAudioPlugin()
{
	// load the plugin from a DLL
	
}

void VigasocoSDL::addCustomInputPlugins()
{
	
}

void VigasocoSDL::createTimer()
{

}

void VigasocoSDL::createAsyncThread()
{
	//_asyncThread = new SDLThread();	
}

void VigasocoSDL::initCompleted()
{

}

/////////////////////////////////////////////////////////////////////////////
// destruction template methods overrides
/////////////////////////////////////////////////////////////////////////////

void VigasocoSDL::destroyAsyncThread()
{
	//delete _asyncThread;
	//_asyncThread = 0;
}

void VigasocoSDL::destroyTimer()
{
	//delete _timer;
	//_timer = 0;
}

void VigasocoSDL::removeCustomInputPlugins()
{
	
}

void VigasocoSDL::destroyDrawPlugin()
{
	
}

void VigasocoSDL::destroyAudioPlugin()
{
	
}

void VigasocoSDL::removeCustomLoaders(FileLoader *fl)
{
	
}

void VigasocoSDL::destroyPalette()
{
	delete _palette;
	_palette = 0;
}

void VigasocoSDL::platformSpecificEnd()
{
	

	
} 

/////////////////////////////////////////////////////////////////////////////
// main loop template methods overrides
/////////////////////////////////////////////////////////////////////////////

bool VigasocoSDL::processEvents()
{
	
}

/////////////////////////////////////////////////////////////////////////////
// window procedure
/////////////////////////////////////////////////////////////////////////////

// windows function
//LRESULT CALLBACK VigasocoWin32::wndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
// TODO
// No hay nada parecido por ahora en la version Linux

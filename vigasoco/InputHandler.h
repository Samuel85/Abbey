// InputHandler.h
//
//	Singleton class that handles all the inputs of the aplication. 
//
//	The inputs are stored on the _inputs array. The _oldInputs array stores last 
//	4 input values. If a game doesn't use has input ports (for example, a CPC
//	game), the _inputs array will be empty and the core scans for all inputs.
//	
//	The class has a collection of plugins that are called to update the _inputs 
//	array. All plugins should be added before calling the init method. 
//
//	In a game not all inputs are needed, so the plugins only update the 
//	entries >= 0. After all plugins have modified the inputs array, the class 
//	updates the game's input ports to reflect the current inputs.
//
//	TODO: implement DIPSW. Now it's returning pacman's default values.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _INPUT_HANDLER_H_
#define _INPUT_HANDLER_H_


#include "GameDriver.h"
#include "InputPort.h"
#include <list>
#include <string>
#include "Types.h"
#include "Singleton.h"

class IInputPlugin;			// defined in IInputPlugin.h

#define theInputHandler InputHandler::getSingletonPtr()

class InputHandler : public Singleton<InputHandler>
{
// types
protected:
	typedef std::list<IInputPlugin *> Plugins;

// fields
protected:
	Plugins _plugins;						// collection of input plugins
	GameDriver::InputPorts *_inputPorts;	// game's input ports
	int _inputs[END_OF_INPUTS];				// final game inputs
	int _oldInputs[END_OF_INPUTS];			// previous frames inputs

	bool _isInitialized;

// methods
public:
	InputHandler(); 
	virtual ~InputHandler();

	// plugin management
	void addInputPlugin(IInputPlugin *ip);
	void removeInputPlugin(IInputPlugin *ip);

	// initialization and cleanup
	bool init(GameDriver *gd);
	void end();

	void copyInputsState(int *dest);

	// getters
	bool isInitialized() const { return _isInitialized; }
	UINT32 getInput(int number) const { return _inputPorts->at(number)->getValue(); }
	UINT32 getDIPSW(int number) const { return 0xd9; }
	bool isPressed(Inputs input) const { return (_inputs[input] > 0); }
	bool hasBeenPressed(Inputs input);
	bool hasBeenReleased(Inputs input);

	void acquire();
	void unAcquire();
	void process();

// helper methods
protected:
	void enableFullKeyboard();
	void enableNonGameInputs();
};

#endif	// _INPUT_HANDLER_H_

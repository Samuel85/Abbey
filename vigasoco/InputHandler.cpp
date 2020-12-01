// InputHandler.cpp
//
/////////////////////////////////////////////////////////////////////////////

#include "InputHandler.h"
#include "IInputPlugin.h"
#include "InputPort.h"

//para memset
#include <string.h>

/////////////////////////////////////////////////////////////////////////////
// initialization and cleanup
/////////////////////////////////////////////////////////////////////////////

InputHandler::InputHandler()
{
	_inputPorts = 0;
	_isInitialized = false;
}

InputHandler::~InputHandler()
{
	// deallocate input plugins
	for (Plugins::iterator i = _plugins.begin(); i != _plugins.end(); i++){
		delete *i;
	}
}

/////////////////////////////////////////////////////////////////////////////
// driver input plugin management
/////////////////////////////////////////////////////////////////////////////

bool InputHandler::init(GameDriver *gd)
{
	// get game's input ports
	_inputPorts = gd->getInputs();

	// disable unneeded inputs
	memset(_inputs, -1, sizeof(_inputs));

	// iterate through the input ports enabling associated inputs
	for (GameDriver::InputPorts::size_type j = 0; j < _inputPorts->size(); j++){
		InputPort *ip = _inputPorts->at(j);

		InputPort::InputBits *ib = ip->getBits();

		for (InputPort::InputBits::size_type i = 0; i < ib->size(); i++){
			// if that key is pressed, update value
			if (ib->at(i).input != UNMAPPED){
				_inputs[ib->at(i).input] = 0;
			}
		}
	}

	// if there aren't any input ports defined, the game uses the full keyboard
	if (_inputPorts->size() == 0){
		enableFullKeyboard();
	}

	// enable inputs used by the core
	enableNonGameInputs();

	// reset inputs history
	memset(_oldInputs, 0, sizeof(_oldInputs));

	// if there aren't any input plugins, error
	if (_plugins.size() == 0){
		return false;
	}

	// init the plugins
	for (Plugins::iterator i = _plugins.begin(); i != _plugins.end(); i++){
		if (!(*i)->init()){
			return false;
		}
	}

	_isInitialized = true;

	return true;
}

void InputHandler::end()
{
	_isInitialized = false;

	// finish the plugins
	for (Plugins::iterator i = _plugins.begin(); i != _plugins.end(); i++){
		(*i)->end();
	}
}

/////////////////////////////////////////////////////////////////////////////
// input plugin management
/////////////////////////////////////////////////////////////////////////////

void InputHandler::addInputPlugin(IInputPlugin *ip)
{
	_plugins.push_back(ip);
}

void InputHandler::removeInputPlugin(IInputPlugin *ip)
{
	_plugins.remove(ip);
}

void InputHandler::acquire()
{
	for (Plugins::iterator i = _plugins.begin(); i != _plugins.end(); i++){
		(*i)->acquire();
	}
}

void InputHandler::unAcquire()
{
	for (Plugins::iterator i = _plugins.begin(); i != _plugins.end(); i++){
		(*i)->unAcquire();
	}
}

void InputHandler::process()
{
	GameDriver::InputPorts &inputPorts = *_inputPorts;

	// reset the _inputs array and update the _oldInputs array
	for (int i = 0; i < END_OF_INPUTS; i++){
		_oldInputs[i] = (_oldInputs[i] << 1) & 0x0f;
		if (_inputs[i] > 0){
			_inputs[i] = 0;
			_oldInputs[i] |= 1;
		}
	}

	// for each plugin, modify _inputs array if the user has pressed an input
	for (Plugins::iterator i = _plugins.begin(); i != _plugins.end(); i++){
		(*i)->process(_inputs);
	}

	// process the _inputs array modifying the input ports' values

	// iterate through the input ports checking associated inputs
	for (GameDriver::InputPorts::size_type j = 0; j < inputPorts.size(); j++){
		InputPort &ip = *inputPorts[j];

		// reset input port value
		ip.reset();

		InputPort::InputBits &ib = *ip.getBits();

		// get actual input port value
		UINT32 value = ip.getValue();

		// check if the input was pressed
		for (InputPort::InputBits::size_type i = 0; i < ib.size(); i++){
			// if that input is pressed, update value
			if (_inputs[ib[i].input] > 0){
				if (ib[i].mode == ACTIVE_HIGH){
					value |= 1 << i;
				} else {
					value &= ~(1 << i);
				}
			}
		}

		// update input port value
		ip.setValue(value);
	}
}

void InputHandler::copyInputsState(int *dest)
{
	for (int i = 0; i < END_OF_INPUTS; i++){
		dest[i] = _inputs[i];
	}
}

/////////////////////////////////////////////////////////////////////////////
// input test
/////////////////////////////////////////////////////////////////////////////

bool InputHandler::hasBeenPressed(Inputs input)
{
	// detect 0->1 transitions
	return _oldInputs[input] == 0x03;
}

bool InputHandler::hasBeenReleased(Inputs input)
{
	// detect 1->0 transitions
	return _oldInputs[input] == 0x0c;
}

/////////////////////////////////////////////////////////////////////////////
// helper methods
/////////////////////////////////////////////////////////////////////////////

void InputHandler::enableFullKeyboard()
{
	for (int i = 0; i < CORE_INPUTS; i++){
		_inputs[i] = 0;
	}
}

void InputHandler::enableNonGameInputs()
{
	for (int i = CORE_INPUTS; i < END_OF_INPUTS; i++){
		_inputs[i] = 0;
	}
}

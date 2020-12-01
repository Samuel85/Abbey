// InputPort.h
//
//	Class that models a game input port
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _INPUT_PORT_H_
#define _INPUT_PORT_H_


#include "Types.h"
#include <vector>


enum InputMode {
	ACTIVE_HIGH,
	ACTIVE_LOW
};

struct InputBit {
	Inputs input;
	InputMode mode;
};

class InputPort
{
// types
public:
	typedef std::vector<InputBit> InputBits;

// fields
protected:
	UINT32 _value;
	InputBits _bits;

// methods
public:
	// getters & setters
	void addBit(int bit, Inputs input, InputMode mode);
	InputBits *getBits() { return &_bits; }
	UINT32 getValue() const { return _value; }
	void setValue(UINT32 value) { _value = value; }

	void reset();

	// initialization and cleanup
	InputPort(int numBits);
	~InputPort();
};

#endif	// _INPUT_PORT_H_

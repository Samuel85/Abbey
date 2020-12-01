// IInputPlugin.h
//
//	Abstract class that defines the interface to handle game's input with a 
//	specific device.
//
//	The inputs are updates in the process method. When an input is pressed, the
//	input plugin should increment the associated input entry, but only if the
//	entry is active (!= -1).
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _IINPUT_PLUGIN_H_
#define _IINPUT_PLUGIN_H_

#include <string>
#include "Types.h"

class IInputPlugin
{
// methods
public:
	// initialization and cleanup
	IInputPlugin(){}
	virtual ~IInputPlugin(){}
	virtual bool init() = 0;
	virtual void end() = 0;

	virtual void acquire() = 0;
	virtual void unAcquire() = 0;

	virtual void process(int *inputs) = 0;

	// access to custom plugin properties
	virtual const std::string *getProperties(int *num) const = 0;
	virtual const int *getPropertiesType() const = 0;
	virtual void setProperty(std::string prop, int data) = 0;
	virtual void setProperty(std::string prop, int index, int data) = 0;
	virtual int getProperty(std::string prop) const = 0;
	virtual int getProperty(std::string prop, int index) const = 0;
};


#endif // _IINPUT_PLUGIN_H_

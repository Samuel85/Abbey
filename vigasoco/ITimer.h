// ITimer.h
//
//	Abstract class that defines the interface for a high resolution timer
//
//	A timer should implement 3 methods:
//		* getTime, that returns the actual number of ticks.
//		* getTicksPerSecond, that returns the number of ticks in a second.
//		* sleep, that stops execution for a specific time.
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _ITIMER_H_
#define _ITIMER_H_


#include "Types.h"

class ITimer
{
// abstract methods
public:
	virtual bool init() = 0;
	virtual void end() = 0;

	virtual INT64 getTime() = 0;
	virtual INT64 getTicksPerSecond() = 0;
	virtual void sleep(UINT32 milliseconds) = 0;

	// initialization and cleanup
	ITimer(){}
	virtual ~ITimer(){}
};

#endif	// _ITIMER_H_

// IPalette.h
//
//	Abstract class that defines the interface of a palette
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _IPALETTE_H_
#define _IPALETTE_H_


#include "Types.h"
#include "NotificationProvider.h"

class IPalette : public NotificationProvider<IPalette>
{
// abstract methods:
public:
	virtual ~IPalette(){}

	virtual void init(int colors) = 0;
	virtual void end() = 0;

	// getters & setters
	virtual int getTotalColors() const = 0;
	virtual void setColor(int index, UINT8 r, UINT8 g, UINT8 b) = 0;
	virtual void getColor(int index, UINT8 &r, UINT8 &g, UINT8 &b) = 0;
	virtual UINT8* getRawPalette() = 0;

	// notification
	virtual void notifyChange();
};


#endif	// _IPALETTE_H_

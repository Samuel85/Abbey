// ICriticalSection.h
//
//	Abstract class that defines the interface of a critical section
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _ICRITICAL_SECTION_H_
#define _ICRITICAL_SECTION_H_


class ICriticalSection
{
public:
	virtual ~ICriticalSection();

	// abstract methods
	virtual void init() = 0;
	virtual void destroy() = 0;

	virtual void enter() = 0;
	virtual void leave() = 0;

protected:
	ICriticalSection();
};

#endif	// _ICRITICAL_SECTION_H_

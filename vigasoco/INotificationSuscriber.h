// INotificationSuscriber.h
//
//	Observer interface used to notify subject changes (Observer pattern)
//
//	How to use it: class MyObserver: public INotificationSuscriber<MySubject>
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _INOTIFICATION_SUSCRIBER_H_
#define _INOTIFICATION_SUSCRIBER_H_


template <typename T>
class INotificationSuscriber
{
// methods
public:
	INotificationSuscriber(){}
	virtual ~INotificationSuscriber(){}
	virtual void update(T* subject, int data) = 0;
};


#endif	// _INOTIFICATION_SUSCRIBER_H_

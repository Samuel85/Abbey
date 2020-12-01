// NotificationProvider.h
//
//	Class that provides the functionality to add and remove observers, and
//	to notify observers of subject's changes (Observer pattern)
//
//	How to use it: class MySubject: public NotificationProvider<MySubject>
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _NOTIFICATION_PROVIDER_H_
#define _NOTIFICATION_PROVIDER_H_

#include "INotificationSuscriber.h"
#include <list>
#include <cassert>

template <typename T>
class NotificationProvider
{
// types
protected:
	typedef std::list<INotificationSuscriber<T> *> Observers;

// fields
protected:
	Observers _observers;

// methods
public:
	NotificationProvider();
	virtual ~NotificationProvider();
	virtual void attach(INotificationSuscriber<T> *o);
	virtual void detach(INotificationSuscriber<T> *o);
	virtual void notify(int data);
};

//#include "NotificationProvider.cpp"
template <typename T> NotificationProvider<T>::NotificationProvider()
{
}

template <typename T> NotificationProvider<T>::~NotificationProvider()
{
	assert(_observers.size() == 0);
}

/////////////////////////////////////////////////////////////////////////////
// notification
/////////////////////////////////////////////////////////////////////////////

template <typename T> void NotificationProvider<T>::attach(INotificationSuscriber<T> *o)
{
	_observers.push_back(o);
}

template <typename T> void NotificationProvider<T>::detach(INotificationSuscriber<T> *o)
{
	_observers.remove(o);
}

template <typename T> void NotificationProvider<T>::notify(int data)
{
	typename Observers::iterator i; 

	// notify all observers of the change
	for (i = _observers.begin(); i != _observers.end(); i++){
		(*i)->update((T *)(this), data);
	}
}


#endif	// _NOTIFICATION_PROVIDER_H_

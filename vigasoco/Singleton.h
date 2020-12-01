// Singleton.h
//
//	Automatic singleton utility class (Singleton pattern)
//	(Presented by Scott Bilas in Game Programming Gems)
//
//	How to use it: class MySingleton: public Singleton<MySingleton>
//
/////////////////////////////////////////////////////////////////////////////

#ifndef _SINGLETON_H_
#define _SINGLETON_H_

#ifdef _MSC_VER
#pragma warning (disable : 4311)
#pragma warning (disable : 4312)
#endif

#include <cassert>


template <typename T>
class Singleton
{
	static T *g_singleton;	// the singleton object

// methods
public:
	// constructor
	Singleton()
	{
		assert(!g_singleton);

		// get the correct pointer in case of multiple inheritance
		g_singleton = static_cast<T*>(this);
	}

	// destructor
	~Singleton()
	{
		assert(g_singleton != 0);

		g_singleton = 0;
	}

	// getters
	static T& getSingleton() { assert(g_singleton); return *g_singleton; }
	static T *getSingletonPtr() { return g_singleton; }
};


template <typename T> T* Singleton<T>::g_singleton = 0;


#endif	// _SINGLETON_H_

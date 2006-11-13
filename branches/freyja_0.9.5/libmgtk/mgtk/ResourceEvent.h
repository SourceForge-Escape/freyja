/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja/
 * Email   : mongoose@icculus.org
 * Object  : ResourceEvent
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This is the plugin event class
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_ResourceEvent - Builds ResourceEvent class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.08.02:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_ResourceEvent_H_
#define GUARD__FREYJA_MONGOOSE_ResourceEvent_H_


#include <mstl/Vector.h>
#include <hel/math.h>
#include "Resource.h"
#include "mgtk_events.h"

using namespace mstl;

class ResourceEvent
{
 public:

	static const unsigned int eBaseEvent = 10000;


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	ResourceEvent(const char *name);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of ResourceEvent
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.02: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual ~ResourceEvent();
	/*------------------------------------------------------
	 * Pre  : ResourceEvent object is allocated
	 * Post : Deconstructs an object of ResourceEvent
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.02: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	unsigned int getUID() { return mUID; }

	char *getName() { return mName;	}

	static int GetResourceIdBySymbol(char *symbol)
	{
		int id = -1;
		
		if (mResource)
		{
			mResource->Lookup(symbol, &id);
		}
		
		return id;
	}


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	virtual bool action() = 0;
	virtual bool action(unsigned int value) { return false; };
	virtual bool action(unsigned int value, unsigned int value2);
	virtual bool action(long value);
	virtual bool action(long *value, unsigned long size);
	virtual bool action(float value);
	virtual bool action(float *value, unsigned long size);
	virtual bool action(char *value);
	virtual bool action(mgtk_mouse_event_t &mouse_event);
	virtual bool action(void (*func)());
	virtual bool action(void (*func)(mgtk_mouse_event_t &));
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Performs action for an event
	 *
	 * NOTE : Only action() required in child, since you may
	 *        not want to handle all listen types.  By default
	 *        these implementations just return failure (false).
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.02:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	static bool listen(unsigned long event);
	static bool listen(unsigned long event, long value);
	static bool listen(unsigned long event, unsigned int value);
	static bool listen(unsigned long event, unsigned int value, unsigned int value);
	static bool listen(unsigned long event, long *value, unsigned long size);
	static bool listen(unsigned long event, float value);
	static bool listen(unsigned long event, float *value, unsigned long size);
	static bool listen(unsigned long event, char *value);
	static bool listen(unsigned long event, mgtk_mouse_event_t &mouse_event);
	static bool listen(unsigned long event, void (*func)());
	static bool listen(unsigned long event, void (*func)(mgtk_mouse_event_t &));
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Action listeners for _all_ events
	 *        Returns false if unable to handle event
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.02.08:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setHandler(void (*handler)());
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	static void setResource(Resource *rcSys);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	virtual bool redo();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Performs last action for an event in undo stack
	 *        Returns false if unable to redo
	 *
	 * NOTE : Base implementation just returns (false)
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.02.08:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual bool undo();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Undoes last action from event
	 *        Returns false if unable to undo
	 *
	 * NOTE : Base implementation just returns (false)
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.02.08:
	 * Mongoose - Created
	 ------------------------------------------------------*/


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////

	static ResourceEvent *getEventById(unsigned long id);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns event for id, or NULL ( 0x0 ) if fails
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.02.08:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	void setName(const char *name);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets name and exports it to resource system
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.02:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned int mUID;                   /* Numeric id of event */

	char *mName;                         /* Symbolic name of event */

	static unsigned int mCounter;        /* Avoid reserve algorithm ids */

	static Resource *mResource;          /* Resource system pointer */

	static Vector<ResourceEvent*> mEvents; /* Event store for control use */
};


class ResourceEventCallback : public ResourceEvent
{
public:

	ResourceEventCallback(const char *name, void (*func)()) : ResourceEvent(name)
	{
		setHandler(func);
	}

	static void add(const char *name, void (*func)())
	{
		ResourceEventCallback *e = new ResourceEventCallback(name, func);

		if (e)
		{
		}
	}

	virtual bool action()
	{
		if (mHandler)
		{
			(*mHandler)();
			return true;
		}

		return false;
	}

	void setHandler(void (*func)())
	{
		mHandler = func;
	}


private:

	void (*mHandler)();                  /* Function pointer callback */
};


class ResourceEventCallback2 : public ResourceEvent
{
public:

	ResourceEventCallback2(const char *name, void (*func)(ResourceEvent *e)) : ResourceEvent(name)
	{
		setHandler(func);
	}

	static void add(const char *name, void (*func)(ResourceEvent *e))
	{
		ResourceEventCallback2 *e = new ResourceEventCallback2(name, func);

		if (e)
		{
		}
	}

	virtual bool action()
	{
		if (mHandler)
		{
			(*mHandler)(this);
			return true;
		}

		return false;
	}

	void setHandler(void (*func)(ResourceEvent *e))
	{
		mHandler = func;
	}


private:

	void (*mHandler)(ResourceEvent *e);       /* Function pointer callback */
};


class ResourceEventCallbackUInt : public ResourceEvent
{
public:

	ResourceEventCallbackUInt(const char *name, void (*func)(unsigned int)) : ResourceEvent(name)
	{
		setHandler(func);
	}

	static void add(const char *name, void (*func)(unsigned int))
	{
		ResourceEventCallbackUInt *e = new ResourceEventCallbackUInt(name, func);

		if (e)
		{
		}
	}

	virtual bool action()
	{
		return false;
	}

	virtual bool action(unsigned int value)
	{
		if (mHandler)
		{
			(*mHandler)(value);
			return true;
		}

		return false;
	}

	void setHandler(void (*func)(unsigned int))
	{
		mHandler = func;
	}


private:

	void (*mHandler)(unsigned int);       /* Function pointer callback */
};


class ResourceEventCallbackUInt2 : public ResourceEvent
{
public:

	ResourceEventCallbackUInt2(const char *name, void (*func)(unsigned int, unsigned int)) : ResourceEvent(name)
	{
		setHandler(func);
	}

	static void add(const char *name, void (*func)(unsigned int, unsigned int))
	{
		ResourceEventCallbackUInt2 *e = new ResourceEventCallbackUInt2(name, func);

		if (e)
		{
		}
	}

	virtual bool action()
	{
		return false;
	}

	virtual bool action(unsigned int value, unsigned int value2)
	{
		if (mHandler)
		{
			(*mHandler)(value, value2);
			return true;
		}

		return false;
	}

	void setHandler(void (*func)(unsigned int, unsigned int))
	{
		mHandler = func;
	}


private:

	void (*mHandler)(unsigned int, unsigned int);       /* Function pointer callback */
};


class ResourceEventCallbackString : public ResourceEvent
{
public:

	ResourceEventCallbackString(const char *name, void (*func)(char*)) : ResourceEvent(name)
	{
		setHandler(func);
	}

	static void add(const char *name, void (*func)(char*))
	{
		ResourceEventCallbackString *e = new ResourceEventCallbackString(name, func);

		if (e)
		{
		}
	}

	virtual bool action()
	{
		return false;
	}

	virtual bool action(char *value)
	{
		if (mHandler)
		{
			(*mHandler)(value);
			return true;
		}

		return false;
	}

	void setHandler(void (*func)(char*))
	{
		mHandler = func;
	}


private:

	void (*mHandler)(char*);       /* Function pointer callback */
};


class ResourceEventCallbackVec : public ResourceEvent
{
public:

	ResourceEventCallbackVec(const char *name, void (*func)(vec_t)) : ResourceEvent(name)
	{
		setHandler(func);
	}

	static void add(const char *name, void (*func)(vec_t))
	{
		ResourceEventCallbackVec *e = new ResourceEventCallbackVec(name, func);

		if (e)
		{
		}
	}

	virtual bool action()
	{
		return false;
	}

	virtual bool action(float value)
	{
		if (mHandler)
		{
			(*mHandler)(value);
			return true;
		}

		return false;
	}

	void setHandler(void (*func)(vec_t))
	{
		mHandler = func;
	}


private:

	void (*mHandler)(vec_t);       /* Function pointer callback */
};


class ResourceEventCallbackVecArray : public ResourceEvent
{
public:

	ResourceEventCallbackVecArray(const char *name, void (*func)(vec_t*, unsigned long)) : ResourceEvent(name)
	{
		setHandler(func);
	}

	static void add(const char *name, void (*func)(vec_t*, unsigned long))
	{
		ResourceEventCallbackVecArray *e = new ResourceEventCallbackVecArray(name, func);

		if (e)
		{
		}
	}

	virtual bool action()
	{
		return false;
	}

	virtual bool action(float *value, unsigned long count)
	{
		if (mHandler)
		{
			(*mHandler)(value, count);
			return true;
		}

		return false;
	}

	void setHandler(void (*func)(vec_t*, unsigned long))
	{
		mHandler = func;
	}


private:

	void (*mHandler)(vec_t*, unsigned long);       /* Function pointer callback */
};


class ResourceEventCallbackMouse : public ResourceEvent
{
public:

	ResourceEventCallbackMouse(const char *name, void (*func)(mgtk_mouse_event_t&)) : ResourceEvent(name)
	{
		setHandler(func);
	}

	static void add(const char *name, void (*func)(mgtk_mouse_event_t&))
	{
		ResourceEventCallbackMouse *e = new ResourceEventCallbackMouse(name, func);

		if (e)
		{
		}
	}

	virtual bool action()
	{
		return false;
	}

	virtual bool action(mgtk_mouse_event_t &e)
	{
		if (mHandler)
		{
			(*mHandler)(e);
			return true;
		}

		return false;
	}

	void setHandler(void (*func)(mgtk_mouse_event_t&))
	{
		mHandler = func;
	}


private:

	void (*mHandler)(mgtk_mouse_event_t&);       /* Function pointer callback */
};


class ResourceEventIndirectCallback : public ResourceEvent
{
public:

	ResourceEventIndirectCallback(const char *name, void (*func)(void (*func)())) : ResourceEvent(name)
	{
		setHandler(func);
	}

	static void add(const char *name, void (*func)(void (*func)()))
	{
		ResourceEventIndirectCallback *e = new ResourceEventIndirectCallback(name, func);

		if (e)
		{
		}
	}

	virtual bool action()
	{
		return false;
	}

	virtual bool action(void (*func)())
	{
		if (mHandler)
		{
			(*mHandler)(func);
			return true;
		}

		return false;
	}

	void setHandler(void (*func)(void (*func)()))
	{
		mHandler = func;
	}


private:

	void (*mHandler)(void (*func)());                  /* Function pointer callback */
};



class ResourceEventIndirectMouseCallback : public ResourceEvent
{
public:

	ResourceEventIndirectMouseCallback(const char *name, void (*func)(void (*func)(mgtk_mouse_event_t&))) : ResourceEvent(name)
	{
		setHandler(func);
	}

	static void add(const char *name, void (*func)(void (*func)(mgtk_mouse_event_t&)))
	{
		ResourceEventIndirectMouseCallback *e = new ResourceEventIndirectMouseCallback(name, func);

		if (e)
		{
		}
	}

	virtual bool action()
	{
		return false;
	}

	virtual bool action(void (*func)(mgtk_mouse_event_t&))
	{
		if (mHandler)
		{
			(*mHandler)(func);
			return true;
		}

		return false;
	}

	void setHandler(void (*func)(void (*func)(mgtk_mouse_event_t&)))
	{
		mHandler = func;
	}


private:

	void (*mHandler)(void (*func)(mgtk_mouse_event_t&));                  /* Function pointer callback */
};


class ResourceAppPluginTest
{
public:
	ResourceAppPluginTest(void (*rc_func)(), void (*gui_func)()) 
	{
		mEventsAttach = rc_func;
		mGUIAttach = gui_func;

		mPlugins.pushBack(this);
	}

	~ResourceAppPluginTest()
	{
		//FIXME mPlugins.erase();
	}

	static Vector<ResourceAppPluginTest*> mPlugins;
	void (*mEventsAttach)();
	void (*mGUIAttach)();
};

#endif

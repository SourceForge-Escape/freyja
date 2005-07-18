/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : FreyjaEvent
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This is the plugin event class
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_FREYJAEVENT - Builds FreyjaEvent class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.08.02:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_FREYJAEVENT_H_
#define GUARD__FREYJA_MONGOOSE_FREYJAEVENT_H_


#include <mstl/Vector.h>
#include <hel/math.h>
#include <mgtk/Resource.h>


class FreyjaEvent
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	FreyjaEvent(const char *name);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of FreyjaEvent
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.02: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual ~FreyjaEvent();
	/*------------------------------------------------------
	 * Pre  : FreyjaEvent object is allocated
	 * Post : Deconstructs an object of FreyjaEvent
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.02: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	char *getName()
	{
		return mName;
	}


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	virtual bool action() = 0;
	virtual bool action(unsigned int value) { return false; };
	virtual bool action(long value);
	virtual bool action(long *value, unsigned long size);
	virtual bool action(float value);
	virtual bool action(float *value, unsigned long size);
	virtual bool action(char *value);
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
	static bool listen(unsigned long event, long *value, unsigned long size);
	static bool listen(unsigned long event, float value);
	static bool listen(unsigned long event, float *value, unsigned long size);
	static bool listen(unsigned long event, char *value);
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

	static FreyjaEvent *getEventById(unsigned long id);
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

	static Vector<FreyjaEvent*> mEvents; /* Event store for control use */
};


class FreyjaEventCallback : public FreyjaEvent
{
public:

	FreyjaEventCallback(const char *name, void (*func)()) : FreyjaEvent(name)
	{
		setHandler(func);
	}

	static void add(const char *name, void (*func)())
	{
		FreyjaEventCallback *e = new FreyjaEventCallback(name, func);

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


class FreyjaEventCallback2 : public FreyjaEvent
{
public:

	FreyjaEventCallback2(const char *name, void (*func)(FreyjaEvent *e)) : FreyjaEvent(name)
	{
		setHandler(func);
	}

	static void add(const char *name, void (*func)(FreyjaEvent *e))
	{
		FreyjaEventCallback2 *e = new FreyjaEventCallback2(name, func);

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

	void setHandler(void (*func)(FreyjaEvent *e))
	{
		mHandler = func;
	}


private:

	void (*mHandler)(FreyjaEvent *e);       /* Function pointer callback */
};


class FreyjaEventCallbackUInt : public FreyjaEvent
{
public:

	FreyjaEventCallbackUInt(const char *name, void (*func)(unsigned int)) : FreyjaEvent(name)
	{
		setHandler(func);
	}

	static void add(const char *name, void (*func)(unsigned int))
	{
		FreyjaEventCallbackUInt *e = new FreyjaEventCallbackUInt(name, func);

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


class FreyjaEventCallbackVec : public FreyjaEvent
{
public:

	FreyjaEventCallbackVec(const char *name, void (*func)(vec_t)) : FreyjaEvent(name)
	{
		setHandler(func);
	}

	static void add(const char *name, void (*func)(vec_t))
	{
		FreyjaEventCallbackVec *e = new FreyjaEventCallbackVec(name, func);

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

#endif

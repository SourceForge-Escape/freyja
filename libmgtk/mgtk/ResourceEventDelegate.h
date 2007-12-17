/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : ResourceEventDelegate
 * License : LGPL, (C) 2004-2007 Mongoose
 * Comments: 
 *
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.04.01:
 * Mongoose - Created, based on older event system and older delegates.
 ==========================================================================*/

#ifndef GUARD__MGTK_RESOURCEEVENTDELEGATE_H_
#define GUARD__MGTK_RESOURCEEVENTDELEGATE_H_

#include <mstl/Delegate.h>
#include <mstl/String.h>
#include <mstl/Vector.h>
#include <hel/math.h>
#include "ResourceEvent.h"

using namespace mstl;

class ResourceEventDelegate : public ResourceEvent
{
public:

	ResourceEventDelegate(const char *name, MethodDelegate *d) : 
		ResourceEvent(name), mDelegate(d)
	{
	}

	~ResourceEventDelegate()
	{
		delete mDelegate;
	}

	static void add(const char *name, MethodDelegate *d)
	{
		ResourceEventDelegate *e = new ResourceEventDelegate(name, d);

		if (e)
		{
		}
	}


	bool action()
	{
		if (mDelegate)
			mDelegate->Execute();

		return true;
	}


	bool action(unsigned int value)
	{
		if (mDelegate)
		{
			ArgList1<unsigned int> arg(value);
			mDelegate->Execute(arg);
		}

		return true;
	}


	bool action(float value)
	{
		if (mDelegate)
		{
			ArgList1<float> arg(value);
			mDelegate->Execute(arg);
		}

		return true;
	}


	bool action(char *value)
	{
		if (mDelegate)
		{
			ArgList1<char *> arg(value);
			mDelegate->Execute(arg);
		}

		return true;
	}


	virtual bool action(char *value, char *value2)
	{
		if (mDelegate)
		{
			ArgList2<char *, char *> arg(value, value2);
			mDelegate->Execute(arg);
		}

		return false;
	}


private:

	MethodDelegate *mDelegate;
};

#endif // GUARD__MGTK_RESOURCEEVENTDELEDATE_H_

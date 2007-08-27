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
 *-- History ------------------------------------------------- 
 *
 * 2004.08.02:
 * Mongoose - Created
 ==========================================================================*/

#include <string.h>
#include <stdio.h>

#include "ResourceEvent.h"

using namespace mgtk;
using namespace mstl;

mstl::Vector<ResourcePlugin*> ResourcePlugin::mPlugins;
unsigned int ResourceEvent::mCounter = 0; 
Resource *ResourceEvent::mResource = NULL; 
Vector<ResourceEvent*>  ResourceEvent::mEvents;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

ResourcePlugin::ResourcePlugin(void (*rc_func)(), void (*gui_func)()) : 
	mEventsAttach(rc_func),	
	mGUIAttach(gui_func), 
	mDrawCB(NULL)
{
	mPlugins.push_back(this);
}


ResourcePlugin::~ResourcePlugin()
{
}


void ResourcePlugin::Draw()
{
	if (mDrawCB)
	{
		mDrawCB();
	}
}


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

ResourceEvent::ResourceEvent(const char *name)
{
	unsigned int i, count;
	bool found = false;

	mName = 0x0;
	setName(name);

	/* Setup UID and class container reference */
	mUID = count = mEvents.size();

	for (i = 0; i < count; ++i)
	{
		if (mEvents[i] == 0x0)
		{
			mUID = i;
			mEvents.assign(mUID, this);
			printf("= %u : '%s'\n", mUID, mName);
			found = true;
		}	
	}

	if (!found)
	{
		mEvents.pushBack(this);
		printf("+ %u : '%s'\n", mUID, mName);
	}

	mResource->RegisterInt(mName, eBaseEvent + mUID);
	++mCounter;
}


ResourceEvent::~ResourceEvent()
{
	/* Mark NULL in container, marking free slot */
	mEvents.assign(mUID, 0x0);
	--mCounter;
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

int ResourceEvent::GetResourceIdBySymbol(const char *symbol)
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

bool ResourceEvent::action(long value)
{
	return false;
}


bool ResourceEvent::action(unsigned int value, unsigned int value2)
{
	return false;
}


bool ResourceEvent::action(long *value, unsigned long size)
{
	return false;
}


bool ResourceEvent::action(float value)
{
	return false;
}


bool ResourceEvent::action(float *value, unsigned long size)
{
	return false;
}


bool ResourceEvent::action(char *value)
{
	return false;
}


bool ResourceEvent::action(char *value, char *value2)
{
	return false;
}


bool ResourceEvent::action(mgtk_mouse_event_t &e)
{
	return false;
}


bool ResourceEvent::action(void (*func)())
{
	return false;
}



bool ResourceEvent::action(void (*func)(mgtk_mouse_event_t &))
{
	return false;
}


bool ResourceEvent::drop(unsigned long event)
{
	ResourceEvent *e = getEventById(event);
	delete e;
	return true;
}


bool ResourceEvent::listen(unsigned long event)
{
	ResourceEvent *e = getEventById(event);

	if (e)
	{
		return e->action();
	}

	return false;
}


bool ResourceEvent::listen(unsigned long event, unsigned int value, 
						   unsigned int value2)
{
	ResourceEvent *e = getEventById(event);

	if (e)
	{
		return e->action(value, value2);
	}

	return false;
}


bool ResourceEvent::listen(unsigned long event, unsigned int value)
{
	ResourceEvent *e = getEventById(event);

	if (e)
	{
		return e->action(value);
	}

	return false;
}


bool ResourceEvent::listen(unsigned long event, long value)
{
	ResourceEvent *e = getEventById(event);

	if (e)
	{
		return e->action(value);
	}

	return false;
}


bool ResourceEvent::listen(unsigned long event, long *value, unsigned long size)
{
	ResourceEvent *e = getEventById(event);

	if (e)
	{
		return e->action(value, size);
	}

	return false;
}


bool ResourceEvent::listen(unsigned long event, float value)
{
	ResourceEvent *e = getEventById(event);

	if (e)
	{
		return e->action(value);
	}

	return false;
}


bool ResourceEvent::listen(unsigned long event, float *value, unsigned long size)
{
	ResourceEvent *e = getEventById(event);

	if (e)
	{
		return e->action(value, size);
	}

	return false;
}


bool ResourceEvent::listen(unsigned long event, char *value)
{
	ResourceEvent *e = getEventById(event);

	if (e)
	{
		return e->action(value);
	}

	return false;
}


bool ResourceEvent::listen(unsigned long event, char *value, char *value2)
{
	ResourceEvent *e = getEventById(event);

	if (e)
	{
		return e->action(value, value2);
	}

	return false;
}


bool ResourceEvent::listen(unsigned long event, void(*func)())
{
	ResourceEvent *e = getEventById(event);

	if (e)
	{
		return e->action(func);
	}

	return false;
}


bool ResourceEvent::listen(unsigned long event, void(*func)(mgtk_mouse_event_t&))
{
	ResourceEvent *e = getEventById(event);

	if (e)
	{
		return e->action(func);
	}

	return false;
}


bool ResourceEvent::listen(unsigned long event, mgtk_mouse_event_t &mouse_event)
{
	ResourceEvent *e = getEventById(event);

	if (e)
	{
		return e->action(mouse_event);
	}

	return false;
}


void ResourceEvent::setResource(Resource *rcSys)
{
	mResource = rcSys;
}


bool ResourceEvent::redo()
{
	return false;
}


bool ResourceEvent::undo()
{
	return false;
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////

ResourceEvent *ResourceEvent::getEventById(unsigned long id)
{
	if (id < mEvents.size())
	{
		return mEvents[id];
	}

	return 0x0;
}


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////

void ResourceEvent::setName(const char *name)
{
	unsigned int l;


	if (mName)
	{
		printf("ERROR: Event already has a symbol entry.\n");
		return;
	}

	if (!name || !name[0])
	{
		printf("ERROR: Event can not use invalid symbol entry.\n");
		return;
	}

	l = strlen(name);

	mName = new char[l+1];
	strncpy(mName, name, l);
	mName[l] = 0;
}


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_ResourceEvent
int runResourceEventUnitTest(int argc, char *argv[])
{
	ResourceEvent test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[ResourceEvent class test]\n");

	return runResourceEventUnitTest(argc, argv);
}
#endif



 

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
 *-- History ------------------------------------------------- 
 *
 * 2004.08.02:
 * Mongoose - Created
 ==========================================================================*/

#include <string.h>
#include <stdio.h>

#include "FreyjaEvent.h"


unsigned int FreyjaEvent::mNextId = 0;
Vector<FreyjaEvent*> FreyjaEvent::mEventStore;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

FreyjaEvent::FreyjaEvent(FreyjaResource *rcSys, char *name)
{
	mResource = rcSys;

	mId = ++mNextId;
	mName = 0x0;
	setName(name);

	mEventStore.pushBack(this);
}


FreyjaEvent::~FreyjaEvent()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void FreyjaEvent::setName(char *name)
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
	mResource->RegisterCPPInt(mName, mId);
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_FREYJAEVENT
int runFreyjaEventUnitTest(int argc, char *argv[])
{
	FreyjaEvent test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[FreyjaEvent class test]\n");

	return runFreyjaEventUnitTest(argc, argv);
}
#endif

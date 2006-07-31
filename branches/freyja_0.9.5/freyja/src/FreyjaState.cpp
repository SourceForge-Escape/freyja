/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : FreyjaState
 * License : No use w/o permission (C) 2006 Mongoose
 * Comments: This is a simple action event stack for undo/redo
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2006.03.05:
 * Mongoose - Created
 ==========================================================================*/

#include "FreyjaState.h"


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

FreyjaState::FreyjaState(int event, int objIndex, int mode) :
	mEvent(event),
	mMode(mode),
	mIndex(objIndex)
{
}


FreyjaState::~FreyjaState()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void FreyjaState::operator =(FreyjaState s) 
{
	mEvent = s.mEvent; 
	mIndex = s.mIndex; 
	mMode = s.mMode; 
}


bool FreyjaState::operator ==(FreyjaState b) 
{
	return ( b.mEvent == mEvent && b.mIndex == mIndex && b.mMode == mMode ); 
}


bool FreyjaState::operator !=(FreyjaState b) 
{
	return ( b.mEvent != mEvent || b.mIndex != mIndex || b.mMode != mMode ); 
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

#ifdef UNIT_TEST_FREYJASTATE
int runFreyjaStateUnitTest(int argc, char *argv[])
{
	FreyjaState test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[FreyjaState class test]\n");

	return runFreyjaStateUnitTest(argc, argv);
}
#endif

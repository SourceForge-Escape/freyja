/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : Action
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the 'event reel atom' for freyja undo/redo/macro usage
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2005.05.06:
 * Mongoose - Created
 ==========================================================================*/

#include "Action.h"

using namespace freyja;

////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Action::Action()
{
}


Action::~Action()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

bool Action::serialize(FileWriter &r)
{
	return false;
}



////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void Action::redo()
{
}


bool Action::serialize(FileReader &r)
{
	return false;
}


void Action::undo()
{
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

#ifdef UNIT_TEST_ACTION
int runActionUnitTest(int argc, char *argv[])
{
	Action test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[Action class test]\n");

	return runActionUnitTest(argc, argv);
}
#endif

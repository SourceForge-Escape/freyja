/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : FreyjaSkeleton
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This is the FreyjaSkeleton class and the classes that compose it.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.10.22:
 * Mongoose - Created
 ==========================================================================*/

#include "FreyjaSkeleton.h"

unsigned int FreyjaSkeleton::mCounter = 0;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

FreyjaSkeleton::FreyjaSkeleton()
{
	mId = mCounter++;
}


FreyjaSkeleton::~FreyjaSkeleton()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_FREYJASKELETON
int runFreyjaSkeletonUnitTest(int argc, char *argv[])
{
	FreyjaSkeleton test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[FreyjaSkeleton class test]\n");

	return runFreyjaSkeletonUnitTest(argc, argv);
}
#endif

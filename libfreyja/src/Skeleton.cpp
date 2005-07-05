/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : Skeleton
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the skeletal animation subsystem
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2005.06.29:
 * Mongoose - Created
 ==========================================================================*/

#include "Skeleton.h"

using namespace freyja;

uint32 Bone::mUIDCount = 0;

Vector<Bone *> gBones;


Bone::Bone()
{
	/* Set gobal index via mUID, and store a pointer using a vector */
	mUID = gBones.size(); 
	gBones.pushBack(this);
}


Bone *Bone::getGobalBoneByUID(uint32 uid)
{
	if (uid < gBones.size())
		return gBones[uid];

	return 0x0;
}


uint32 Bone::getUID()
{
	return mUID;
}


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Skeleton::Skeleton()
{
}


Skeleton::~Skeleton()
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

#ifdef UNIT_TEST_SKELETON
int runSkeletonUnitTest(int argc, char *argv[])
{
	Skeleton test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[Skeleton class test]\n");

	return runSkeletonUnitTest(argc, argv);
}
#endif

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

#include "SkeletonABI.h"
#include "Skeleton.h"

using namespace freyja;

uint32 Skeleton::mUIDCount = 0;

Vector<Skeleton *> gSkeletons;


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


////////////////////////////////////////////////////////////
// ABI
////////////////////////////////////////////////////////////
#ifndef BONE_0_9_3_API
index_t freyjaGetCurrentSkeletonIndex()
{
	return 0; // FIXME
}


index_t freyjaSkeletonCreate()
{
	return 0; // FIXME
}


void freyjaSkeletonDelete(index_t skeletonIndex)
{
}

#include "BoneABI.h"

void freyjaSkeletonUpdateBones(index_t skeletonIndex)
{
	// FIXME
}


void freyjaSkeletonAddBone(index_t skeletonIndex, index_t boneIndex)
{
	// FIXME
}

// ABI Accessors ///////////////////////

uint32 freyjaGetSkeletonCount()
{
	return 1; // FIXME
}


uint32 freyjaGetSkeletonBoneCount(index_t skeletonIndex)
{
	return freyjaGetBoneCount(); // FIXME
}


index_t freyjaGetSkeletonBoneIndex(index_t skeletonIndex, uint32 element)
{
	return element; // FIXME
}


index_t freyjaGetSkeletonRootIndex(index_t skeletonIndex)
{
	return 0; // FIXME
}
#endif

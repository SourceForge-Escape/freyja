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
 *           Skeletons and their animations are seperate data structures, so
 *           reuse and blending is possible.  Not to mention more flexible.
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_SKELETON - Builds Skeleton class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2005.06.29:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_SKELETON_H_
#define GUARD__FREYJA_MONGOOSE_SKELETON_H_

#include <hel/math.h>
#include <hel/Vector3d.h>
#include <hel/Matrix.h>
#include <hel/Quaternion.h>

#include <mstl/Vector.h>


namespace freyja {


class BoneTransform
{
public:

	BoneTransform();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	~BoneTransform();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	uint32 mSkeletalKeyFrameUID;     /* UID of skeletal keyframe */

	uint32 mRestBoneUID;             /* UID of rest bone */

	Quaternion mRotation;            /* Oreintation of this bone */

	Vector3d mTranslation;           /* Offset of this bone from parent */

	Matrix mBoneToWorld;             /* Transform vertices to world coords with
									  * this cache of the current orientation 
									  * and translation in matrix form */

	Matrix mCombined;                /* Cached result of transform matrices from
									  * rest position bone, and this transform:
									  * rest.mBoneToWorld * this.mBboneToWorld,
									  * which is updated in sync with changes
									  * to rest bone and this transform */
};



class Bone 
{
public:

	Bone();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	~Bone();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	uint32 getUID();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	static Bone *getGobalBoneByUID(uint32 uid);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/


	uint32 mSkeletonIndex;           /* The skeleton that owns this bone */

	uint32 mIndex;                   /* Unique identifier in this skeleton */

	uint32 mFlags;                   /* Options bitmap */

	char mName[64];                  /* Human readable identifier */

	int32 mParent;                   /* Parent of this bone */

	Vector <uint32> mChildren;       /* Children bones */

	Quaternion mRotation;            /* Oreintation of this bone */

	Vector3d mTranslation;           /* Offset of this bone from parent */

	Matrix mBoneToWorld;             /* Transform vertices to world coords with
									  * this cache of the current orientation 
									  * and translation in matrix form */


private:
	
	uint32 mUID;                     /* System for allowing gobal references, 
									  * which are mostly for C API callbacks */

	static uint32 mUIDCount;
};


class Skeleton
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Skeleton();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Skeleton
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.06.29: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~Skeleton();
	/*------------------------------------------------------
	 * Pre  : Skeleton object is allocated
	 * Post : Deconstructs an object of Skeleton
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.06.29: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	char mName[64];                  /* Human readable identifier */

	Vector<Bone *> mBones;           /* Bones in this skeleton */

	uint32 mRoot;                    /* Root bone index */


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	uint32 mUID;                     /* System for allowing gobal references, 
									  * which are mostly for C API callbacks */

	static uint32 mUIDCount;
};



class SkeletalKeyFrame
{
public:

	vec_t time;

	vec_t lastTime;

	Vector<BoneTransform *> mTransforms;
};



class SkeletalAnimation
{
public:

	SkeletalAnimation();

	~SkeletalAnimation();

	char mName[64];

	Vector<SkeletalKeyFrame *> mKeyFrames;

private:
	
	uint32 mUID;                     /* System for allowing gobal references, 
									  * which are mostly for C API callbacks */

	static uint32 mUIDCount;
	
};


}

#endif

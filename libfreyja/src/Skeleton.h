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
#include "freyja.h"
#include "Bone.h"


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

	index_t mSkeletalKeyFrameUID;    /* UID of skeletal keyframe */

	index_t mRestBoneUID;            /* UID of rest bone */

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

	index_t getUID();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	static uint32 getCount();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	static Skeleton *getSkeleton(index_t skeletonIndex);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void addBone(index_t bone);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	index_t addToPool();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	void removeBone(index_t bone);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	void removeFromPool();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	static void resetPool();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	void setName(const char *name);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	char mName[64];                  /* Human readable identifier */

	Vector<index_t> mBones;          /* Bones in this skeleton */

	index_t mRoot;                   /* Root bone index */


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	index_t mUID;                     /* System for allowing gobal references, 
									   * which are mostly for C API callbacks */

	static Vector<Skeleton *> mGobalPool; /* Storage for gobal access */
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

	Skeleton *mRestPose;

	Vector<SkeletalKeyFrame *> mKeyFrames;

private:
	
	index_t mUID;                    /* System for allowing gobal references, 
									  * which are mostly for C API callbacks */

	static Vector<SkeletalAnimation *> mGobalPool; /* Storage for gobal access */
	
};


}

#endif

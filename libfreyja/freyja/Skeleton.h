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

	BoneTransform() :
		mSkeletalKeyFrameUID(INDEX_INVALID),
		mRestBoneUID(INDEX_INVALID),
		mRotate(),
		mTranslate(),
		mBoneToWorld(),
		mCombined()
	{
	}
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	~BoneTransform() { }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	index_t mSkeletalKeyFrameUID;    /* UID of skeletal keyframe */

	index_t mRestBoneUID;            /* UID of rest bone */

	Quaternion mRotate;              /* Oreintation of this bone */

	Vector3d mTranslate;             /* Offset of this bone from parent */

	Matrix mBoneToWorld;             /* Transform vertices to world coords with
									  * this cache of the current orientation 
									  * and translation in matrix form */

	Matrix mCombined;                /* Cached result of transform matrices from
									  * rest position bone, and this transform:
									  * rest.mBoneToWorld * this.mBboneToWorld,
									  * which is updated in sync with changes
									  * to rest bone and this transform */
};


class SkeletalKeyFrame
{
public:

	SkeletalKeyFrame() : 
		mAnimation(INDEX_INVALID),
		time(0.0f),
		lastTime(0.0f),
		mBones()
	{
	}

	~SkeletalKeyFrame()
	{
		mBones.erase();
	}

	index_t mAnimation;         /* The animation which owns this keyframe */

	vec_t time;

	vec_t lastTime;

	Vector<BoneTransform *> mBones;
};


class SkeletalAnimation
{
public:

	SkeletalAnimation() :
		mId(INDEX_INVALID),
		mSkeleton(INDEX_INVALID),
		mKeyFrames()
 	{
		mName[0] = 0;
	}

	~SkeletalAnimation()
	{
		mKeyFrames.erase();
	}

	index_t mId;            /* Local UID ( local to this skeleton ) */

	index_t mSkeleton;      /* Skeleton which owns this animation */

	char mName[64];

	//char mSkeletonName[64]; /* 'Symbolic pointer' to link to skeleton by name */

	//bool mHasRootAnimation;

	Vector<SkeletalKeyFrame *> mKeyFrames;
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

	SkeletalAnimation *getAnimation(index_t animationIndex);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

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

	index_t createAnimation();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	void deleteAnimation(index_t animationIndex);
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

	Vector<SkeletalAnimation *> mAnimations; /* Animations for the skeleton */

	index_t mUID;                 /* System for allowing gobal references, 
	                               * which are mostly for C API callbacks */

	static Vector<Skeleton *> mGobalPool; /* Storage for gobal access */
};

}

#endif

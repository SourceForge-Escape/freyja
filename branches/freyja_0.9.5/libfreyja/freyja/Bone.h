/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : Bone
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is part of the skeletal animation subsystem
 *
 *           Skeletons and their animations are seperate data structures, so
 *           reuse and blending is possible.  Not to mention more flexible.
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_BONE - Builds Bone class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2005.06.29:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_BONE_H_
#define GUARD__FREYJA_MONGOOSE_BONE_H_

#include <hel/math.h>
#include <hel/Vector3d.h>
#include <hel/Matrix.h>
#include <hel/Quaternion.h>
#include <mstl/Vector.h>

#include "KeyFrame.h"
#include "Track.h"
#include "freyja.h"

using namespace mstl;


namespace freyja {

class BoneKeyFrame : public KeyFrame
{
 public:

	BoneKeyFrame() : KeyFrame(), mLoc(), mRot(), mWorldPose(), mCombined() {}

	virtual ~BoneKeyFrame() {}

	virtual uint32 GetSerializedSize() { return 4*4+4*3; }
	/*------------------------------------------------------
	 * Pre  :
	 * Post : 
	 ------------------------------------------------------*/

	virtual bool Serialize(SystemIO::FileWriter &w) 
	{
		w.WriteFloat32(mLoc.mVec[0]);
		w.WriteFloat32(mLoc.mVec[1]);
		w.WriteFloat32(mLoc.mVec[2]);

		vec4_t wxyz;
		mRot.getQuaternion4fv(wxyz);
		w.WriteFloat32(wxyz[0]);
		w.WriteFloat32(wxyz[1]);
		w.WriteFloat32(wxyz[2]);
		w.WriteFloat32(wxyz[3]);
		return true;
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Write data from this object to disk
	 ------------------------------------------------------*/

	virtual bool Serialize(SystemIO::FileReader &r) 
	{ 
		mLoc.mVec[0] = r.ReadFloat32();
		mLoc.mVec[1] = r.ReadFloat32();
		mLoc.mVec[2] = r.ReadFloat32(); 

		mRot = Quaternion(r.ReadFloat32(), r.ReadFloat32(), 
						  r.ReadFloat32(), r.ReadFloat32());
		return true; 
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Read data into this object from disk
	 ------------------------------------------------------*/

	Vec3 mLoc;                       /* Keyframe data */

	Quaternion mRot;

	Matrix mWorldPose;               /* This is here for ease of use only */

	Matrix mCombined;                /* <= mWorldPose * mBindToWorld, 
									  * Convenice storage of deformation  */
};


class BoneTrack : public Track
{
public:
	BoneTrack() : Track() { mName = "Bone"; }
	
	~BoneTrack() {}
	
	virtual KeyFrame *NewTrackKeyFrame(vec_t time)
	{
		BoneKeyFrame *key = new BoneKeyFrame();
		key->mTime = time;
		return key;
	}

	virtual BoneKeyFrame *GetKeyframe(index_t idx) 
	{
		return (BoneKeyFrame *)Track::GetKeyframe(idx);
	}
};


class Bone 
{
public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

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


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	index_t GetUID();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	static uint32 GetCount();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	static Bone *GetBone(index_t uid);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void AddChild(index_t child);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	index_t AddToPool();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	void RemoveChild(index_t child);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	void RemoveFromPool();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	static void ResetPool();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	void SetName(const char *name);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	void UpdateBindPose();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Pass transform changes up the heirarchy
	 ------------------------------------------------------*/

	void UpdateWorldPose();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Pass transform changes up the heirarchy
	 ------------------------------------------------------*/


	byte mFlags;                     /* Options bitmap */

	char mName[64];                  /* Human readable identifier */

	index_t mSkeleton;               /* Skeleton to which this bone belongs */

	index_t mParent;                 /* Parent bone of this bone */

	Vector<index_t> mChildren;       /* Children bones of this bone */

	Quaternion mRotation;            /* Orientation of this bone */

	Vec3 mTranslation;               /* Offset of this bone from parent */

	Matrix mBindPose;                /* Store the bind transform from the 
									  * origin ( this bulids off parents ).
									  * This transforms vertices to rest pose */

	Matrix mBindToWorld;             /* Cached inverse of mBindTransform:
									  * Transform vertices to world coords with
									  * this cache of the current orientation 
									  * and translation in matrix form */

	BoneTrack mKeyframes;   // Test, only supporting one 'range/anim' in test

#if OBSOLETE
	// Starting in 0.9.5 moved world transforms here from keyframing,
	// Makes it easier to control the beast, and stops bone sharing schemes.
	// The actual transforms 'we care about' in the keyframes.
	// This is for use as a callback a bone to do rendering/pose transforms.

	Quaternion mFrameRotation;       /* AnimFrame Orientation of this bone */

	Vec3 mFrameTranslation;          /* AnimFrame Offset of this bone from parent */

	Matrix mWorldPose;               /* This is the animated frame transform 
									  * from origin */

	Matrix mCombined;                /* <= mWorldPose * mBindToWorld, 
									  * Convenice storage of deformation  */
#endif

private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////
	
	index_t mUID;                     /* System for allowing gobal references, 
									   * which are mostly for C API callbacks */

	static Vector<Bone *> mGobalPool; /* Storage for gobal access */
};

}

#endif


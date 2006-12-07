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

class BoneTrack // : public InterfaceTrack
{
public:

	BoneTrack() : mRot(), mLoc() { }

	~BoneTrack() { }

	index_t GetKeyfameIndex(vec_t time) // mRot and mLoc have same rate/duration
	{ return mRot.GetKeyfameIndexFromTime(time); }

	Vec3KeyFrame *GetRotKeyframe(index_t key) // 'New' will return prev allocated as well
	//{ return mRot.GetKeyframe(key); }
	{ return (Vec3KeyFrame *)mRot.NewKeyframeByIndex(key); }
	

	index_t NewRotKeyframe(vec_t time) { return mRot.NewKeyframe(time); }

	Vec3KeyFrame *GetLocKeyframe(index_t key) // 'New' will return prev allocated as well
	//{ return mLoc.GetKeyframe(key); }
	{ return (Vec3KeyFrame *)mLoc.NewKeyframeByIndex(key); }

	index_t NewLocKeyframe(vec_t time) { return mLoc.NewKeyframe(time); }

	
	vec_t GetDuration() { return mRot.GetDuration(); }

	void SetDuration(vec_t d) { mRot.SetDuration(d); mLoc.SetDuration(d); }

	vec_t GetRate() { return mRot.GetRate(); }

	void SetRate(vec_t fps) { mRot.SetRate(fps); mLoc.SetRate(fps); }

	uint32 GetRotKeyframeCount() { return mRot.GetKeyframeCount(); }

	uint32 GetLocKeyframeCount() { return mLoc.GetKeyframeCount(); }


	Vec3 GetRot(vec_t time) { return mRot.GetTransform(time); }

	bool GetRot(vec_t t, Vec3 &v)  { return mRot.GetTransform(t, v); }


	Vec3 GetLoc(vec_t time) { return mLoc.GetTransform(time); }

	bool GetLoc(vec_t t, Vec3 &v)  { return mLoc.GetTransform(t, v); }


	Vec3Track mRot;

	Vec3Track mLoc;
};


class Bone 
{
public:

	typedef enum {
		fNone        =  0,
		fHighlighted =  1,
		fDof         =  2,
		fSelected    =  4,
		fHidden      =  8,
		fRayHit      = 16,
		fRoot        = 32
	} BoneFlags;


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
	// Gobal pool API
	////////////////////////////////////////////////////////////

	static uint32 GetCount();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns number of bones in gobal store
	 ------------------------------------------------------*/

	static Bone *GetBone(index_t uid);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns bone matching gobal store UID
	 ------------------------------------------------------*/

	static void ResetPool();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Erase the gobal bone store
	 ------------------------------------------------------*/

	index_t AddToPool();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Add this bone to the gobal store
	 *        Return UID
	 ------------------------------------------------------*/

	index_t GetUID();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns this bone's gobal store UID
	 ------------------------------------------------------*/

	void RemoveFromPool();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Remove this bone from gobal store
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	Matrix &GetBindPose() { return mBindPose; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns this bone's rest/bind pose transform
	 ------------------------------------------------------*/

	Matrix &GetInverseBindPose() { return mBindToWorld; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns this bone's 'rest to world' transform
	 ------------------------------------------------------*/

	byte GetFlags() { return mFlags; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Get this bone's option flags
	 ------------------------------------------------------*/

	const char *GetName() { return mName; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Get the human readable name of this bone
	 ------------------------------------------------------*/

	index_t GetParent() { return mParent; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Get the UID of this bone's parent bone or 
	 *        INDEX_INVALID if it has no parent
	 ------------------------------------------------------*/

	BoneTrack &GetTrack(uint32 track) { return mTrack; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Only supporting one 'range/anim' in test: F(track) <- F(0)
	 ------------------------------------------------------*/

	bool Serialize(SystemIO::TextFileWriter &w);
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
	 * Post : Add child bone to this bone by by UID
	 ------------------------------------------------------*/

	index_t NewTrack() {return 0;}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Only supporting one 'range/anim' in test: F(track) <- F(0) 
	 ------------------------------------------------------*/

	void RemoveChild(index_t child);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Remove child bone of this bone by by UID
	 ------------------------------------------------------*/

	bool Serialize(SystemIO::TextFileReader &r);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	void SetName(const char *name);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Set human readable name
	 ------------------------------------------------------*/

	void SetLoc(const Vec3 &v);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Set parent relative translation
	 ------------------------------------------------------*/

	void SetRot(const Quaternion &q);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Set parent relative rotation
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

	void ClearFlag(BoneFlags f) { mFlags ^= f; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post :Set a single flag 'off' 
	 ------------------------------------------------------*/

	void SetFlag(BoneFlags f) { mFlags |= f; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Set a single flag 'on'
	 ------------------------------------------------------*/

	void UpdateBindPose(const Matrix &m);
	/*------------------------------------------------------
	 * Pre  : Pass in parent's bind transform
	 * Post : Updates this bone and children bind transforms
	 ------------------------------------------------------*/

	static void UpdateBindPose(index_t boneIndex, Matrix &m);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
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

	BoneTrack mTrack;                /* Animation track(s) - only one in test */


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


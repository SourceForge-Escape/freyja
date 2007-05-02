/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : Bone
 * License : No use w/o permission (C) 2005-2007 Mongoose
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

#ifndef GUARD__FREYJA_BONE_H_
#define GUARD__FREYJA_BONE_H_

#include <hel/math.h>
#include <hel/Vec3.h>
#include <hel/Mat44.h>
#include <hel/Quat.h>
#include <mstl/Vector.h>
#include <mstl/String.h>

#include "BoneTrack.h"
#include "KeyFrame.h"
#include "Track.h"
#include "freyja.h"


namespace freyja {

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

	hel::Mat44 &GetBindPose() { return mBindPose; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns this bone's rest/bind pose transform
	 ------------------------------------------------------*/

	hel::Mat44 &GetInverseBindPose() { return mBindToWorld; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns this bone's 'rest to world' transform
	 ------------------------------------------------------*/

	hel::Mat44 &GetWorldPose() { return mTrack.mWorld; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Returns this bone's 'world' transform
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

	uint32 GetTrackCount() { return mTrackCount; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
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
	 * Post : Add child bone to this bone by <child> UID.
	 *
	 ------------------------------------------------------*/

	index_t NewTrack() { mTrackCount = 1; return 0;}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Only supporting one 'range/anim' in test: F(track) <- F(0) 
	 *
	 ------------------------------------------------------*/

	void RemoveChild(index_t child);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Remove child bone of this bone by <child> UID.
	 *
	 ------------------------------------------------------*/

	bool Serialize(SystemIO::TextFileReader &r);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void SetName(const char *name);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Set human readable name.
	 *
	 ------------------------------------------------------*/

	void SetLoc(const hel::Vec3 &v);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Set parent relative translation.
	 *
	 ------------------------------------------------------*/

	void SetRot(const hel::Quat &q);
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Set parent relative rotation.
	 *
	 ------------------------------------------------------*/

	void UpdateBindPose();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Only updates bind pose for this bone using
	 *        parent bind transform and a newly computed
	 *        local transform.
	 *
	 ------------------------------------------------------*/

	void UpdateBindPoseForParent();
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Traces back the heirarchy to get a full transform.
	 *        Alters/updates parents as well as this bone.
	 *
	 ------------------------------------------------------*/

	void UpdateBindPoseForChildren();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Updates this bone's child bind transforms only.
	 *
	 ------------------------------------------------------*/

	void UpdateWorldPose(index_t track, vec_t time);
	/*------------------------------------------------------
	 * Pre  : Input <track> and <time> is a valid animation.
	 * Post : Traces back the heirarchy to get a full transform.
	 *        Alters/updates parents as well.
	 *
	 ------------------------------------------------------*/

	void ClearFlag(BoneFlags f) { mFlags &= ~f; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post :Set a single flag 'off' 
	 *
	 ------------------------------------------------------*/

	void SetFlag(BoneFlags f) { mFlags |= f; }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Set a single flag 'on'
	 *
	 ------------------------------------------------------*/


	mstl::String mMetaData;          /* Metadata for bone */

	byte mFlags;                     /* Options bitmap */

	char mName[64];                  /* Human readable identifier */

	index_t mSkeleton;               /* Skeleton to which this bone belongs */

	index_t mParent;                 /* Parent bone of this bone */

	Vector<index_t> mChildren;       /* Children bones of this bone */

	hel::Quat mRotation;             /* Orientation of this bone */

	hel::Vec3 mTranslation;          /* Offset of this bone from parent */

	hel::Mat44 mLocalTransform;      /* Might as well cache local transform */

	hel::Mat44 mBindPose;            /* Store the bind transform from the 
									  * origin ( this bulids off parents ).
									  * This transforms vertices to rest pose */

	hel::Mat44 mBindToWorld;         /* Cached inverse of mBindTransform:
									  * Transform vertices to world coords with
									  * this cache of the current orientation 
									  * and translation in matrix form */

	BoneTrack mTrack;                /* Animation track(s) - only one in test */

	uint32 mTrackCount;              /* How many tracks are loaded? */

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


} // namespace freyja

#endif // GUARD__FREYJA_BONE_H_


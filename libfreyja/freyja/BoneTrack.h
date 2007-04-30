/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : BoneTrack
 * License : No use w/o permission (C) 2005-2007 Mongoose
 * Comments: This is part of the skeletal animation subsystem
 *
 *           Skeletal animation track prototype.
 *
 *-- History ------------------------------------------------ 
 *
 * 2005.06.29:
 * Mongoose - Created, forked from Bone.h
 ==========================================================================*/

#ifndef GUARD__FREYJA_BONETRACK_H_
#define GUARD__FREYJA_BONETRACK_H_

#include <hel/math.h>
#include <hel/Vec3.h>
#include <hel/Mat44.h>
#include <mstl/Vector.h>
#include <mstl/String.h>

#include "KeyFrame.h"
#include "Track.h"
#include "freyja.h"


namespace freyja {

class BoneTrack
{
public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	BoneTrack() : mRot(), mLoc() {}
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	~BoneTrack() {}
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	index_t GetKeyfameIndex(vec_t time)
	{ return mRot.GetKeyfameIndexFromTime(time); }
	/*------------------------------------------------------
	 * Pre  : mRot and mLoc have same rate/duration
	 * Post : 
	 ------------------------------------------------------*/

	Vec3KeyFrame *GetRotKeyframe(index_t key)
	{ return (Vec3KeyFrame *)mRot.NewKeyframeByIndex(key); }
	/*------------------------------------------------------
	 * Pre  :  'New' will return prev allocated as well
	 * Post : 
	 ------------------------------------------------------*/

	index_t NewRotKeyframe(vec_t time) { return mRot.NewKeyframe(time); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	Vec3KeyFrame *GetLocKeyframe(index_t key)
	{ return (Vec3KeyFrame *)mLoc.NewKeyframeByIndex(key); }
	/*------------------------------------------------------
	 * Pre  : 'New' will return prev allocated as well
	 * Post : 
	 ------------------------------------------------------*/

	index_t NewLocKeyframe(vec_t time) { return mLoc.NewKeyframe(time); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/
	
	vec_t GetDuration() { return mRot.GetDuration(); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	void SetDuration(vec_t d) { mRot.SetDuration(d); mLoc.SetDuration(d); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	vec_t GetRate() { return mRot.GetRate(); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	void SetRate(vec_t fps) { mRot.SetRate(fps); mLoc.SetRate(fps); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	uint32 GetRotKeyframeCount() { return mRot.GetKeyframeCount(); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	uint32 GetLocKeyframeCount() { return mLoc.GetKeyframeCount(); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	vec_t GetRotKeyframeTime(uint32 key) { return mRot.GetTime(key); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	vec_t GetLocKeyframeTime(uint32 key) { return mLoc.GetTime(key); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/


	void GetRotKeyframe(uint32 key, vec3_t v) { mRot.GetKey(key, v); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	void GetLocKeyframe(uint32 key, vec3_t v) { mLoc.GetKey(key, v); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	hel::Vec3 GetRot(vec_t time) { return mRot.GetTransform(time); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	bool GetRot(vec_t t, hel::Vec3 &v)  { return mRot.GetTransform(t, v); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	hel::Vec3 GetLoc(vec_t time) { return mLoc.GetTransform(time); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	bool GetLoc(vec_t t, hel::Vec3 &v)  { return mLoc.GetTransform(t, v); }
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	bool Serialize(SystemIO::TextFileWriter &w)
	{
		mRot.Serialize(w);
		mLoc.Serialize(w);
		return true;
	}
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/

	bool Serialize(SystemIO::TextFileReader &r)
	{
		mRot.Serialize(r);
		mLoc.Serialize(r);
		return true;
	}
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : 
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	//mstl::Vector<hel::Mat44> mLocalTransform;

	//mstl::Vector<hel::Mat44> mPose;

	hel::Mat44 mLocal; // Cache last requested time transform here

	hel::Mat44 mWorld; // Cache last requested time transform here

	Vec3Track mRot;

	Vec3Track mLoc;
};


} // namespace freyja

#endif // GUARD__FREYJA_BONETRACK_H_


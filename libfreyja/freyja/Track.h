/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : Track
 * License : No use w/o permission (C) 2006 Mongoose
 * Comments: Keyframe animation track
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_TRACK - Builds Track class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2006.11.10:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_TRACK_H_
#define GUARD__FREYJA_MONGOOSE_TRACK_H_


#include <math.h>
#include <hel/math.h>
#include <hel/Vector3d.h>
#include <hel/Matrix.h>
#include <hel/Quaternion.h>
#include <mstl/Vector.h>
#include <mstl/SystemIO.h>
#include <mstl/String.h>

#include "KeyFrame.h"
#include "freyja.h"

using namespace mstl;
using namespace freyja;

namespace freyja {

class Track
{
public:

	typedef enum {
		fCreateMissingKeys = 1
		
	} flags_t;


	Track() : 
		mKeyFrames(),
		mName("UnNamed"), 
		mStart(0.0f), 
		mRate(30.0f), 
		mDuration(10.0f),
		mFlags(0x0)
	{
		UpdateKeyframes();
	}

	virtual ~Track() { }


	void DeleteKeyFrame(index_t idx)
	{
		KeyFrame **array = mKeyFrames.getVectorArray();
		
		if ( idx < mKeyFrames.size() && array[idx] )
		{
			delete array[idx];
			array[idx] = NULL;
		}
	}

	virtual KeyFrame *NewTrackKeyFrame(vec_t time) = 0;
	/*------------------------------------------------------
	 * Pre  : <time> > 0.0f
	 *        No generics, we want specialized Track interfaces 
	 * Post : Returns keyframe type specific to Track type
	 ------------------------------------------------------*/

	virtual void UpdateKeyframes()
	{
		// FIXME: ATM this _WIPES_ all keyframes, we should
		// shuffle the old keyframes into the new list matching
		// their new index ( after all we just shuffle pointers )
		
		// Don't screw around rebuild a pointer array every time, 
		// so we can just use direct indices in the interface
		for ( uint32 i = 0, count = GetKeyframeCount(); i < count; ++i )
		{
			mKeyFrames.pushBack(NULL);
		}
	}


	index_t NewKeyframe(vec_t time)
	{
		KeyFrame **array = mKeyFrames.getVectorArray();
		uint32 keyframe = GetKeyfameIndexFromTime(time);

		if ( keyframe < mKeyFrames.size() )
		{
			if ( array[keyframe] == NULL )
				array[keyframe] = NewTrackKeyFrame(time);

			return keyframe;
		}
		
		return INDEX_INVALID;
	}

	virtual KeyFrame *GetKeyframe(index_t idx) 
	{ return (idx < mKeyFrames.size()) ? 
		(mKeyFrames.getVectorArray())[idx] : NULL; }
	/*------------------------------------------------------
	 * Pre  : <time> > 0.0f
	 * Post : Returns keyframe if it exists or NULL
	 ------------------------------------------------------*/

	int32 GetKeyfameIndexFromTime(vec_t time) { return (int32)(mRate * time); }
	/*------------------------------------------------------
	 * Pre  : <time> > 0.0f
	 * Post : Returns claculated keyframe
	 ------------------------------------------------------*/

	uint32 GetKeyframeCount() { return (uint32)(mRate * mDuration); } 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns max number of keyframes for duration
	 ------------------------------------------------------*/

	vec_t GetDuration() { return mDuration; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets track duration
	 ------------------------------------------------------*/

	void SetDuration(vec_t lenght) { mDuration = lenght; UpdateKeyframes(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Updates animation duration and adjusts keyframe list 
	 ------------------------------------------------------*/

	vec_t GetRate() { return mRate; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Updates track rate
	 ------------------------------------------------------*/

	void SetRate(vec_t fps) { mRate = fps; UpdateKeyframes(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Updates track rate and adjusts keyframe list 
	 ------------------------------------------------------*/

	// FIXME: Add serialzation support for Track
	virtual bool Serialize(SystemIO::FileWriter &w) { return false; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Write data from this object to disk
	 ------------------------------------------------------*/

	virtual bool Serialize(SystemIO::FileReader &r) { return false; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Read data into this object from disk
	 ------------------------------------------------------*/

	Vector<KeyFrame *> mKeyFrames;   /* Keyframe data */

	String mName;		             /* Human readable name for this track */

	vec_t mStart;                    /* Whence track starts ( playback only ) */

	vec_t mRate;                     /* Playback rate */

	vec_t mDuration;		         /* How long is this track */	

	byte mFlags;                     /* Control flags */
};


class VertexAnimTrack : public Track
{
public:
	VertexAnimTrack() : Track() { mName = "VertexAnim"; }
	
	~VertexAnimTrack() {}

	
	virtual KeyFrame *NewTrackKeyFrame(vec_t time)
	{
		VertexAnimKeyFrame *key = new VertexAnimKeyFrame();
		return key;
	}

	virtual VertexAnimKeyFrame *GetKeyframe(index_t idx) 
	{
		return (VertexAnimKeyFrame *)Track::GetKeyframe(idx);
	}
};


class TransformTrack : public Track
{
public:

	TransformTrack() : Track() { mName = "Transform"; }

	~TransformTrack() {}

	virtual KeyFrame *NewTrackKeyFrame(vec_t time)
	{
		Vec3x3KeyFrame *key = new Vec3x3KeyFrame();

		key->mData[0] = Vec3(0,0,0); // Rotate
		key->mData[1] = Vec3(1,1,1); // Scale
		key->mData[2] = Vec3(0,0,0); // Translate
		return key;
	}


	virtual Vec3x3KeyFrame *GetKeyframe(index_t idx) 
	{
		return (Vec3x3KeyFrame *)Track::GetKeyframe(idx);
	}


	virtual void GetTransform(vec_t time, Vec3 &pos, Vec3 &rot, Vec3 &scale);


	Vec3 GetRotation(index_t idx) 
	{ 
		Vec3 v;
		Vec3x3KeyFrame *key = GetKeyframe(idx);

		if (key)
		{
			v = key->mData[0];
		}

		return v;
	}


	void SetRotation(index_t idx, const Vec3 &v) 
	{ 
		Vec3x3KeyFrame *key = GetKeyframe(idx);

		if (key)
		{
			key->mData[0] = v;
		}
	}


	Vec3 GetScale(index_t idx) 
	{ 
		Vec3 v;
		Vec3x3KeyFrame *key = GetKeyframe(idx);

		if (key)
		{
			v = key->mData[1];
		}

		return v;
	}


	void SetScale(index_t idx, const Vec3 &v) 
	{ 
		Vec3x3KeyFrame *key = GetKeyframe(idx);

		if (mFlags & fCreateMissingKeys && key == NULL)
		{
			// FIXME: Not like this  =0
		}

		if (key)
		{
			for (uint32 i = 0; i < 3; ++i)
			{
				// Scale = 0.0f undefined property, so filter it out
				if (v.mVec[i] > 0.0f)
					key->mData[1].mVec[i] = v.mVec[i]; 
			}
		}
	}


	Vec3 GetPosition(index_t idx) 
	{
		Vec3 v;
		Vec3x3KeyFrame *key = GetKeyframe(idx);

		if (key)
		{
			v = key->mData[2];
		}

		return v;
	}


	void SetPosition(index_t idx, const Vec3 &v) 
	{ 
		Vec3x3KeyFrame *key = GetKeyframe(idx);

		if (key)
		{
			key->mData[2] = v; 
		}
	}


	void Scale(index_t idx, Vec3 v) { SetScale(idx, v); }	

	void Scale(index_t idx, vec3_t xyz) { SetScale(idx, Vec3(xyz)); }	

	void Scale(index_t idx, vec_t s) { SetScale(idx, Vec3(s, s, s)); }

	void ScaleX(index_t idx, vec_t x) { SetScale(idx, Vec3(x, 1.0f, 1.0f)); }

	void ScaleY(index_t idx, vec_t y) { SetScale(idx, Vec3(1.0f, y, 1.0f)); }

	void ScaleZ(index_t idx, vec_t z) { SetScale(idx, Vec3(1.0f, 1.0f, z)); }


	//Matrix mTransform;                /* Cached transform */

	//Matrix mInverse;                  /* Cached inverse of transform */
};


#ifdef OBSOLETE

class InterfaceTrack
{
	////////////////////////////////////////////////////////////
	// This class is the base for keyframe 'typing' aggregate.
	// Notice to add a new kind of transform it must be added 
	// here first.  This helps encourage more uniform extension.
	// However feel free to use the interface however works best.
	//
	// The methods here should drive your UI / curve control design.
	// By providing commonly used curve transforms here, you shouldn't
	// have to hack around anything.  
	//
	////////////////////////////////////////////////////////////

 public:

	InterfaceTrack() { }

	virtual ~InterfaceTrack() { }

	// Might want to put 'tmp/edit' keyframe support in here instead of UI
	// for use when creating a keyframe using auto

	// 'Location' interface
	virtual void SetPosition(vec3_t xyz) {}
	virtual void SetPositionX(float x) {}
	virtual void SetPositionY(float y) {}
	virtual void SetPositionZ(float z) {}
	virtual void SetDeltaPosition(vec3_t xyz) {}
	virtual void SetDeltaPositionX(float x) {}
	virtual void SetDeltaPositionY(float y) {}
	virtual void SetDeltaPositionZ(float z) {}

	// 'Rotation' Quaternion interface 
	virtual void SetRotationQuat(vec4_t wxyz) {}
	virtual void SetDeltaRotationQuat(vec4_t wxyz) {}

	// 'Rotation'Euler angle interface 
	virtual void SetRotationEuler(vec3_t xyz) {}
	virtual void SetRotationEulerX(float x) {}
	virtual void SetRotationEulerY(float y) {}
	virtual void SetRotationEulerZ(float z) {}
	virtual void SetDeltaRotationEuler(vec3_t xyz) {}	
	virtual void SetDeltaRotationEulerX(float x) {}
	virtual void SetDeltaRotationEulerY(float y) {}
	virtual void SetDeltaRotationEulerZ(float z) {}

	// 'Size' interface
	virtual void SetScale(vec_t s) {}
	virtual void SetScale(vec3_t xyz) {}	
	virtual void SetScaleX(float x) {}
	virtual void SetScaleY(float y) {}
	virtual void SetScaleZ(float z) {}
	virtual void SetDeltaScale(vec3_t xyz) {}	
	virtual void SetDeltaScaleX(float x) {}
	virtual void SetDeltaScaleY(float y) {}
	virtual void SetDeltaScaleZ(float z) {}

	// 'Color' interface
	virtual void SetColorARGB(vec4_t argb) {}
	virtual void SetColorRGB(vec3_t rgb) {}
	virtual void SetColorRGBA(vec4_t rgba) {}
	virtual void SetColorR(vec_t r) {}
	virtual void SetColorB(vec_t b) {}
	virtual void SetColorG(vec_t g) {}
	virtual void SetColorA(vec_t a) {}
	virtual void SetTransparentcy(vec_t t) {}
	virtual void SetVisibility(vec_t v) {}

	// 'Light' interface
	virtual void SetFalloff(vec_t d) {}

};
#endif

} // namespace freyja

#endif

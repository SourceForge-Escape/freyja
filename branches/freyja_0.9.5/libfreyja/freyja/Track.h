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

#ifndef GUARD__FREYJA_TRACK_H_
#define GUARD__FREYJA_TRACK_H_

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
		// Copy old keyframes if they exist
		Vector<KeyFrame *> tmp(mKeyFrames);
		
		// Don't screw around rebuild a pointer array every time, 
		// so we can just use direct indices in the interface
		mKeyFrames.resize(0);
		for ( uint32 i = 0, count = GetKeyframeCount(); i < count; ++i )
		{
			mKeyFrames.pushBack(NULL);
		}

		// FIXME: Since, this design is about to be axed don't bother updating
		//        the keyframe id for rate/duration unless this is release. =/
		//
		//        Truncate off if you shrink for now... might just move this
		//        to a tree/list later since N is always small and list sparse.
		uint32 i;
		foreach(tmp, i)
		{
			if ( i < mKeyFrames.size() )
				mKeyFrames[i] = tmp[i];
		}
	}


	virtual uint32 GetNextKeyframe(uint32 frame)
	{
		KeyFrame **array = mKeyFrames.getVectorArray();
		for ( uint32 count = mKeyFrames.size()-1; frame < count; ++frame )
		{
			if (array[frame])
			{
				break;
			}
		}

		return frame;
	}


	virtual KeyFrame *GetPrevKey(vec_t time)
	{
		KeyFrame *key = NULL;
		KeyFrame **array = mKeyFrames.getVectorArray();
		uint32 keyframe = GetKeyfameIndexFromTime(time);
		uint32 size = mKeyFrames.size();

		for ( ; keyframe > 0; --keyframe )
		{
			if (keyframe < size && array[keyframe])
			{
				key = array[keyframe];
				break;
			}
		}

		return key;
	}

	virtual uint32 GetPrevKeyframe(uint32 keyframe)
	{
		KeyFrame **array = mKeyFrames.getVectorArray();
		for ( ; keyframe > 0; --keyframe )
		{
			if (array[keyframe])
			{
				break;
			}
		}

		return keyframe;
	}


	// Returns existing keyframe if allocated
	KeyFrame *NewKeyframeByIndex(uint32 keyframe)
	{
		if ( keyframe < mKeyFrames.size() )
		{
			KeyFrame **array = mKeyFrames.getVectorArray();

			if ( array[keyframe] == NULL )
			{
				vec_t time = (vec_t)keyframe / mRate;
				array[keyframe] = NewTrackKeyFrame(time);
			}

			return array[keyframe];
		}

		return NULL;
	}


	// Returns existing keyframe if allocated
	index_t NewKeyframe(vec_t time)
	{
		uint32 keyframe = GetKeyfameIndexFromTime(time);

		if ( keyframe < mKeyFrames.size() )
		{
			KeyFrame **array = mKeyFrames.getVectorArray();

			if ( array[keyframe] == NULL )
				array[keyframe] = NewTrackKeyFrame(time);

			return keyframe;
		}
		
		return INDEX_INVALID;
	}

	virtual KeyFrame *GetKeyframe(index_t idx) 
	{ 
		return (idx < mKeyFrames.size()) ? mKeyFrames[idx] : NULL; 
	}
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

	void SetRate(vec_t fps) 
	{ if ( fps > 1.0f ) { mRate = fps; UpdateKeyframes(); } }
	/*------------------------------------------------------
	 * Pre  : <fps? must be at least 1 frame / second
	 * Post : Updates track rate and adjusts keyframe list 
	 ------------------------------------------------------*/

	uint32 ComputeSerializedSize()
	{
		uint32 i, sz = 0;

		sz += 4 + 4;
		foreach(mKeyFrames, i)
		{
			if (mKeyFrames[i])
			{
				sz = 4 + mKeyFrames[i]->GetSerializedSize();
			}
		}

		sz += 4 + mName.GetLength();
		sz += 4 + 4 + 4 + 1;

		return sz;
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	virtual bool Serialize(SystemIO::FileWriter &w) 
	{		
		freyja_file_chunk_t chunk;

		chunk.type = FREYJA_CHUNK_TRACK;
		chunk.size = ComputeSerializedSize();
		chunk.flags = 0x0;
		chunk.version = 1;

		/* Write chunk header to diskfile */
		w.WriteLong(FREYJA_CHUNK_MESH);
		w.WriteLong(chunk.size);
		w.WriteLong(chunk.flags);
		w.WriteLong(chunk.version);

		w.WriteLong(mName.GetLength());
		w.WriteString(mName.GetLength(), mName.c_str());
		w.WriteFloat32(mStart);
		w.WriteFloat32(mRate);
		w.WriteFloat32(mDuration);
		w.WriteInt8U(mFlags);

		uint32 i = 0;
		w.WriteLong(mKeyFrames.end());
		foreach(mKeyFrames, i)
		{
			if (mKeyFrames[i])
			{
				w.WriteInt32(i);
				mKeyFrames[i]->Serialize(w);
			}
		}

		w.WriteInt32(-1);

		return true; 
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Write data from this object to disk
	 ------------------------------------------------------*/

	virtual bool Serialize(SystemIO::FileReader &r) 
	{		
		freyja_file_chunk_t chunk;

		// This class expects:
		// chunk.type = FREYJA_CHUNK_TRACK
		// chunk.version = 1

		chunk.type = r.ReadLong();
		chunk.size = r.ReadLong();
		chunk.flags = r.ReadLong();
		chunk.version = r.ReadLong();

		if (chunk.type != FREYJA_CHUNK_TRACK || chunk.version != 1)
			return false;

		long sz = r.ReadLong();
		char name[sz];
		r.ReadString(sz, name);
		name[sz - 1] = 0;
		mName.Set(name);

		mStart = r.ReadFloat32();
		mRate = r.ReadFloat32();
		mDuration = r.ReadFloat32();
		mFlags = r.ReadInt8U();

		uint32 i, count = r.ReadLong();
		for (i = 0; i < count; ++i)
		{
			mKeyFrames.pushBack(NULL);
		}

		int32 j = r.ReadInt32();

		while ( j != -1 )
		{
			KeyFrame *key = NewKeyframeByIndex(j);

			if (key)
			{
				key->Serialize(r);
			}

			j = r.ReadInt32();
		}
		
		return true;
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Read data into this object from disk
	 ------------------------------------------------------*/

	bool Serialize(SystemIO::TextFileWriter &w)
	{
		w.Print("Track\n");
		w.Print("\t mVersion 1\n");

		w.Print("\t mName \"%s\"\n", mName.c_str());

		w.Print("\t mFlags %u\n", mFlags);

		w.Print("\t mDuration %f\n", mDuration);

		w.Print("\t mStart %f\n", mStart);

		w.Print("\t mRate %f\n", mRate);

		// Weed out this junky sparse array...
		uint32 i = 0, count = 0;
		foreach(mKeyFrames, i)
		{
			if (mKeyFrames[i])
			{
				++count;
			}
		}

		w.Print("\t mKeyFrames %u\n", count);

		i = 0;
		foreach(mKeyFrames, i)
		{
			if (mKeyFrames[i])
			{
				w.Print("\t Key %u ", i);
				mKeyFrames[i]->Serialize(w);
				w.Print("\n");
			}
		}

		w.Print("END\n");

		return true; 
	}


	bool Serialize(SystemIO::TextFileReader &r)
	{		
		r.ParseSymbol(); // Track

		r.ParseSymbol(); // mVersion
		r.ParseInteger(); // == 1

		r.ParseSymbol(); // mName
		mName = r.ParseStringLiteral();

		r.ParseSymbol(); // mFlags
		mFlags = r.ParseInteger();

		r.ParseSymbol(); // mDuration
		mDuration = r.ParseFloat();

		r.ParseSymbol(); // mStart
		mStart = r.ParseFloat();

		r.ParseSymbol(); // mRate
		mRate = r.ParseFloat();

		UpdateKeyframes(); // Allocate junky sparse array...

		r.ParseSymbol(); // mKeyFrames
		uint32 count = r.ParseInteger();
		while (count > 0)
		{
			r.ParseSymbol(); // Key
			uint32 key = r.ParseInteger();
			NewKeyframeByIndex(key)->Serialize(r);

			--count;
		}

		r.ParseSymbol(); // END

		return true;
	}


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


class Vec3Track : public Track
{
public:
	Vec3Track() : Track() { mName = "Vec3"; }
	
	~Vec3Track() {}
	
	virtual KeyFrame *NewTrackKeyFrame(vec_t time)
	{
		return new Vec3KeyFrame(time);
	}


	virtual vec_t GetTime(index_t idx)
	{
		Vec3KeyFrame *key = GetKeyframe(idx);

		if (key)
		{
			return key->GetTime();
		}

		return 0.0f;
	}


	virtual void GetKey(index_t idx, vec3_t v)
	{
		Vec3KeyFrame *key = GetKeyframe(idx);
		Vec3 u;

		if (key)
		{
			u = key->GetData();
		}

		u.Get(v);
	}


	virtual Vec3KeyFrame *GetKeyframe(index_t idx) 
	{
		return (Vec3KeyFrame *)Track::GetKeyframe(idx);
	}


	Vec3 GetTransform(vec_t time)
	{
		Vec3 v;
		GetTransform(time, v);
		return v;
	}


	bool GetTransform(vec_t time, Vec3 &v) 
	{
		// Don't alter v, if out of bounds
		//v = Vec3(0,0,0);

		// Ran off the rails here... could wrap around, but just return for now
		if (time > GetDuration() || time < 0.0f)
		{
			return false;
		}
	
		// Given time assertion this should be a 'valid' frame here...
		int32 frame = GetKeyfameIndexFromTime(time);
		Vec3KeyFrame *key = GetKeyframe(frame);
	
		if (key)
		{
			v = key->mData;
		}
		else
		{
			// Find last keyframe ( not last played, that's for a game engine )
			uint32 prev = GetPrevKeyframe(frame);
			uint32 next = GetNextKeyframe(frame);

			// <= Interpolate(prev, next, time);

			key = GetKeyframe(prev);

			// Found a 'start' keyframe, or if you don't you'll use a
			// dummy keyframe with reflexive transforms @ 0.0 seconds
			vec_t start = 0.0f;
			if (key)
			{
				// Prev bundle
				v = key->mData;
				start = (vec_t)prev / GetRate();
			}
		
			key = GetKeyframe(next);

			// Found an 'end' keyframe, or if you don't you'll use a
			// dummy keyframe with reflexive transforms @ 0.0 seconds
			if (key)
			{
				// Next bundle
				Vec3 vNext = key->mData;
				vec_t end = (vec_t)next / GetRate();

				// Actual time displacement weight
				vec_t w = (time - start) / (end - start);

				v = v + ((vNext - v) * w);
			}
		}

		return true;
	}
};


} // namespace freyja

#endif

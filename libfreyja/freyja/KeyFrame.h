/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : KeyFrame
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: KeyFrame class
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_KEYFRAME - Builds KeyFrame class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2005.07.08:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_KEYFRAME_H_
#define GUARD__FREYJA_MONGOOSE_KEYFRAME_H_

// TODO: Decide where control curves should go to drive all this

#include <hel/math.h>
#include <hel/Vector3d.h>
#include <hel/Matrix.h>
#include <hel/Quaternion.h>
#include <mstl/Vector.h>
#include "freyja.h"
#include "FreyjaFileWriter.h"
#include "FreyjaFileReader.h"


using namespace mstl;


namespace freyja {

	// Need to get scale and translation tests up by end of the week
	// TtoL  	time_t -> distance_t
	// TtoA 	time_t -> angle_t
	// TtoT 	time_t -> time_t
	// TtoU 	time_t -> double_t
	// UtoL 	unit_t -> distance_t
	// UtoA 	unit_t -> angle_t
	// UtoT 	unit_t -> time_t
	// UtoU 	unit_t -> unit_t

class KeyFrameObject
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
	// Generics are added to give you plenty of rope.  Write a
	// damn message system handler inside an kfobj if you want.
	////////////////////////////////////////////////////////////

 public:

	KeyFrameObject();

	virtual ~KeyFrameObject();

	// 'Signal' interface ( TODO: Merge with Action objects )
	virtual void PushUndoState() {}
	virtual void PushRedoState() {}
	virtual void Undo() {}
	virtual void Redo() {}
	virtual bool SerializeUndoHistory(FreyjaFileWriter &w) { return false; }
	virtual bool SerializeUndoHistory(FreyjaFileReader &r) { return false; }

	// 'Location' interface
	virtual void SetPosition(vec3_t xyz) {}
	virtual void SetPositionX(float x) {}
	virtual void SetPositionY(float y) {}
	virtual void SetPositionZ(float z) {}
	virtual void SetDeltaPosition(vec3_t xyz) {}
	virtual void SetDeltaPositionX(float x) {}
	virtual void SetDeltaPositionY(float y) {}
	virtual void SetDeltaPositionZ(float z) {}

	// 'Rotation' Quaternion and Euler angle interface 
	virtual void SetRotation(vec3_t xyz) {}
	virtual void SetRotationQuat(vec4_t wxyz) {}
	virtual void SetRotationX(float x) {}
	virtual void SetRotationY(float y) {}
	virtual void SetRotationZ(float z) {}
	virtual void SetDeltaRotation(vec3_t xyz) {}	
	virtual void SetDeltaRotationQuat(vec4_t wxyz) {}
	virtual void SetDeltaRotationX(float x) {}
	virtual void SetDeltaRotationY(float y) {}
	virtual void SetDeltaRotationZ(float z) {}

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
	virtual void SetColorRGB(vec3_t rgb) {}
	virtual void SetColorRGBA(vec4_t rgba) {}
	virtual void SetColorR(float r) {}
	virtual void SetColorB(float b) {}
	virtual void SetColorG(float g) {}
	virtual void SetColorA(float a) {}
	virtual void SetTransparentcy(float t) {}
	virtual void SetVisibility(float v) {}

	// 'Light' interface
	virtual void SetFalloff(float d) {}

	// 'Generic' interface for message passing
	virtual void SetBitFlags(unsigned int flags) {}
	virtual unsigned int GetBitFlags() { return 0; }
	virtual void SetInteger(int i) {}
	virtual int GetInteger() { return 0; }
	virtual void SetUInteger(unsigned int u) {}
	virtual unsigned int SetUInteger() { return 0; }
	virtual void SetFloat(float f) {}
	virtual float GetFloat() { return 0.0f; }
	virtual void SetCString(const char *s) {}
	virtual const char *GetCString() { return NULL; }
	virtual void SetPointer(void *ptr) {}
	virtual void *GetPointer() { return NULL; }

	virtual char *GetTypeName() { return "Object"; }

 private:

};


class KeyFrame
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	KeyFrame();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of KeyFrame
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.07.08: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~KeyFrame();
	/*------------------------------------------------------
	 * Pre  : KeyFrame object is allocated
	 * Post : Deconstructs an object of KeyFrame
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.07.08: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	index_t mIndex;                     /* Keyframe number */

	vec_t mTime;                        /* Time offset to this frame */

	Vector<KeyFrameObject *> mObjects;  /* All objects bound to this frame */



 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

};


class KeyFrameGroup
{
	// A Hybrid of keygroup and animation now

	////////////////////////////////////////////////////////////
	// Groups keyframes for indepentdent playback and blending
	////////////////////////////////////////////////////////////

 public:

	KeyFrameGroup() :
		mPool(),
		mTimeStart(0.0f),
		mTimeEnd(0.0f),
		mRate(30.0f),
		mDuration(0.0f),
		mUID(INDEX_INVALID),
		mHasRootAnimation(false)
	{
	}


	~KeyFrameGroup()
	{
	}

	void FreeKeyFrame(index_t idx)
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/
	{
		KeyFrame **array = mPool.getVectorArray();
		
		if ( idx < mPool.size() && array[idx] )
		{
			delete array[idx];
			array[idx] = NULL;
		}
	}

	index_t NewKeyframe()
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/
	{
		KeyFrame **array = mPool.getVectorArray();
		KeyFrame *obj = new KeyFrame();

		for ( uint32 i = 0, count = mPool.size(); i < count; ++i )
		{
			if (array[i] == NULL)
			{
				obj->mIndex = i;
				array[i] = obj;
				return i;
			}
		}

		mPool.pushBack(obj);
		return mPool.size() - 1;
	}


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	KeyFrame *GetKeyframe(index_t idx)
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/
	{
		KeyFrame **array = mPool.getVectorArray();
		
		if ( idx < mPool.size() )
		{
			return array[idx];
		}

		return NULL;
	}


	Vector<KeyFrame *> mPool;    /* Storage for all Keyframes */

	char mName[64];		    /* Human readable name for this group eg 'Run0' */

	vec_t mTimeStart;

	vec_t mTimeEnd;

	vec_t mRate;

	vec_t mDuration;		/* How long is this track */

	index_t mUID;			/* Unique numeric identifer for this group */

	bool mHasRootAnimation;


 private:

};


} // namespace freyja

#endif

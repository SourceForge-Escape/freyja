/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : KeyFrame
 * License : No use w/o permission (C) 2005-2006 Mongoose
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
 * 2006.11.06:
 * Mongoose - New KISS implementation is so nice
 *
 * 2005.07.08:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__FREYJA_MONGOOSE_KEYFRAME_H_
#define GUARD__FREYJA_MONGOOSE_KEYFRAME_H_

#include <math.h>
#include <hel/math.h>
#include <hel/Vector3d.h>
#include <hel/Matrix.h>
#include <hel/Quaternion.h>
#include <mstl/Vector.h>
#include <mstl/SystemIO.h>
#include <mstl/String.h>

#include "freyja.h"

using namespace mstl;


namespace freyja {

class KeyFrame
{
public:

	KeyFrame() : mFlags(0x0), mTime(0.0f) { }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of KeyFrame
	 ------------------------------------------------------*/

	virtual ~KeyFrame() { }
	/*------------------------------------------------------
	 * Pre  : KeyFrame object is allocated
	 * Post : Deconstructs an object of KeyFrame
	 ------------------------------------------------------*/

	vec_t GetTime() { return mTime; }

	virtual void SetTime(vec_t time) { mTime = time; }

	byte mFlags;       /* Used in obsolete code for interface use */

	vec_t mTime;       /* Time offset to this frame in sec */
};


class VecKeyFrame : public KeyFrame
{
 public:

	VecKeyFrame() : KeyFrame(), mData(0.0f) {}

	virtual ~VecKeyFrame() {}

	vec_t GetData() { return mData; }

	virtual void SetData(const vec_t v) { mData = v; }

	vec_t mData;                      /* Keyframe data */
};


class Vec3KeyFrame : public KeyFrame
{
 public:

	Vec3KeyFrame() : KeyFrame(), mData(0.0f, 0.0f, 0.0f) {}

	virtual ~Vec3KeyFrame() {}

	Vec3 GetData() { return mData; }

	virtual void SetData(const Vec3 &v) { mData = v; }

	Vec3 mData;                      /* Keyframe data */
};


class Vec3x3KeyFrame : public KeyFrame
{
 public:

	Vec3x3KeyFrame() : KeyFrame() {}

	virtual ~Vec3x3KeyFrame() {}

	Vec3 mData[3];                    /* Keyframe data */
};


class ScaleKeyFrame : public Vec3KeyFrame
{
 public:

	ScaleKeyFrame() : Vec3KeyFrame() { mData = Vec3(1.0f, 1.0f, 1.0f); }

	~ScaleKeyFrame() {}

	virtual void SetData(const Vec3 &v) 
	{
		for (uint32 i = 0; i < 3; ++i)
		{
			// Scale = 0.0f undefined property, so filter it out
			if (v.mVec[i] > 0.0f)
				mData.mVec[i] = v.mVec[i]; 
		}
	}

	virtual void Scale(Vec3 v) { SetData(v); }	

	virtual void Scale(vec3_t xyz) { SetData(Vec3(xyz)); }	

	virtual void Scale(vec_t s) { SetData(Vec3(s, s, s)); }

	virtual void ScaleX(vec_t x) { SetData(Vec3(x, 1.0f, 1.0f)); }

	virtual void ScaleY(vec_t y) { SetData(Vec3(1.0f, y, 1.0f)); }

	virtual void ScaleZ(vec_t z) { SetData(Vec3(1.0f, 1.0f, z)); }
};


class MatrixKeyFrame : public KeyFrame
{
 public:

	MatrixKeyFrame() : KeyFrame(), mData() {}

	virtual ~MatrixKeyFrame() {}

	const Matrix &GetData() { return mData; }

	virtual void SetData(const Matrix &m) { mData = m; }

	Matrix mData;
};


class VertexAnimKeyFrame : public KeyFrame
{
public:
	VertexAnimKeyFrame() : KeyFrame(), mVertices() { }
	
	~VertexAnimKeyFrame() {}

	void ArrayResize(uint32 sz) { mVertices.resize(sz); } 

	vec_t *GetArray() { return mVertices.getVectorArray(); }

	uint32 GetArraySize() { return mVertices.size(); }

	Vec3 GetPos(uint32 i)
	{
		Vec3 pos(0,0,0);

		if (i < mVertices.end())
		{
			vec_t *array = mVertices.getVectorArray();
			i *= 3;
			pos.mVec[0] = array[i];
			pos.mVec[1] = array[i+1];
			pos.mVec[2] = array[i+2];
		}

		return pos;
	}

	void SetPos(uint32 i, Vec3 pos)
	{
		if (i < mVertices.end())
		{
			vec_t *array = mVertices.getVectorArray();
			i *= 3;
			array[i  ] = pos.mVec[0];
			array[i+1] = pos.mVec[1];
			array[i+2] = pos.mVec[2];
		}
	}

	Vector<vec_t> mVertices;
};


} // namespace freyja

#endif

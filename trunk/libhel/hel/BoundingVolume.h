/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : libHel
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : BoundingVolume
 * License : No use w/o permission (C) 2002 Mongoose
 * Comments: Bounding volume class for world/vis 
 *           culling/clipping/collision stuff
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_BOUNDINGVOLUME - Builds BoundingVolume class unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2002.11.27:
 * Mongoose - Created
 ================================================================*/

#ifndef GUARD__LIBHEL_MONGOOSE_BOUNDINGVOLUME_H_
#define GUARD__LIBHEL_MONGOOSE_BOUNDINGVOLUME_H_

#include <hel/math.h>
#include <hel/Mat44.h>

namespace hel {

class BoundingVolume
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	BoundingVolume();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of BoundingVolume
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.11.27: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual ~BoundingVolume();
	/*------------------------------------------------------
	 * Pre  : BoundingVolume object is allocated
	 * Post : Deconstructs an object of BoundingVolume
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.11.27: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	virtual bool IsPointInside(vec3_t point) { return false; }

	virtual bool IsInsideFrustum(vec_t frustum[6][4]) { return false; }
	

	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	virtual void Reset() {}

	virtual void Translate(vec3_t xyz) {}

	virtual void Rotate(vec3_t xyz) {}

	virtual void Scale(vec3_t xyz) {}

	virtual void UpdateBounds(vec3_t point) {}


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////


	/* */
};


class BoundingSphere : public BoundingVolume
{
public:

	BoundingSphere(vec3_t center, vec_t radius) :
		mRadius(radius)
	{
		helCopyVec3(center, mCenter.mVec);
	}


	bool IsPointInside(vec3_t point)
	{
		if (helDist3v(mCenter.mVec, point) <= mRadius)
			return true;

		return false;
	}

	bool IsInsideFrustum(vec_t frustum[6][4])
	{
		unsigned int p;
		vec_t d;


		for (p = 0; p < 6; ++p)
		{
			d = (frustum[p][0] * mCenter[0] + 
				  frustum[p][1] * mCenter[1] + 
				  frustum[p][2] * mCenter[2] +
				  frustum[p][3]);

			if (d <= -mRadius)
				return false;
		}

		return true;
	}

	void Reset()
	{
		mRadius = 0.0f;
	}

	void Translate(vec3_t xyz)
	{
		helCopyVec3(xyz, mCenter.mVec);
	}


	void Scale(vec3_t xyz)
	{
		mRadius *= xyz[0];  // assume uniform scaling no way to recover hull data
	}

	void UpdateBounds(vec3_t point)
	{
		vec_t radius = helDist3v(mCenter.mVec, point);

		if (radius > mRadius)
			mRadius = radius;
	}


	hel::Vec3 mCenter;            /* Center of bounding sphere */

	vec_t mRadius;             /* Raduis of bounding sphere */	
};


class BoundingBox : public BoundingVolume
{
public:

	BoundingBox(vec3_t min, vec3_t max)
	{
		mMin[0] = min[0];	
		mMin[1] = min[1];	
		mMin[2] = min[2];

		mMax[0] = max[0];	
		mMax[1] = max[1];	
		mMax[2] = max[2];	
	}


	bool IsPointInside(vec3_t point)
	{
		if (point[0] >= mMin[0] && point[0] <= mMax[0])
		{
			if (point[1] >= mMin[1] && point[1] <= mMax[1])
			{
				if (point[0] >= mMin[2] && point[0] <= mMax[2])
				{
					return true;
				}
			}
		}

		return false;
	}

	bool IsInsideFrustum(vec_t frustum[6][4])
	{
		unsigned int p;
		vec_t *min = mMin.mVec;
		vec_t *max = mMax.mVec;

		for (p = 0; p < 6; ++p)
		{
			if (frustum[p][0] * min[0] + 
				 frustum[p][1] * min[1] + 
				 frustum[p][2] * min[2] + frustum[p][3] > 0)
				continue;

			if (frustum[p][0] * max[0] + 
				 frustum[p][1] * max[1] + 
				 frustum[p][2] * max[2] + frustum[p][3] > 0)
				continue;

			if (frustum[p][0] * min[0] + 
				 frustum[p][1] * max[1] + 
				 frustum[p][2] * max[2] + frustum[p][3] > 0)
				continue;

			if (frustum[p][0] * min[0] + 
				 frustum[p][1] * min[1] + 
				 frustum[p][2] * max[2] + frustum[p][3] > 0)
				continue;

			if (frustum[p][0] * min[0] + 
				 frustum[p][1] * max[1] + 
				 frustum[p][2] * min[2] + frustum[p][3] > 0)
				continue;

			if (frustum[p][0] * max[0] + 
				 frustum[p][1] * min[1] + 
				 frustum[p][2] * min[2] + frustum[p][3] > 0)
				continue;

			if (frustum[p][0] * max[0] + 
				 frustum[p][1] * max[1] + 
				 frustum[p][2] * min[2] + frustum[p][3] > 0)
				continue;

			if (frustum[p][0] * max[0] + 
				 frustum[p][1] * min[1] + 
				 frustum[p][2] * max[2] + frustum[p][3] > 0)
				continue;

			return false;
		}

		return true;
	}

	
	void Reset()
	{

		mMin[0] = 0.0f;	
		mMin[1] = 0.0f;	
		mMin[2] = 0.0f;

		mMax[0] = 0.0f;	
		mMax[1] = 0.0f;	
		mMax[2] = 0.0f;
	}

	void Translate(vec3_t xyz)
	{
		mMin[0] += xyz[0];	
		mMin[1] += xyz[1];	
		mMin[2] += xyz[2];

		mMax[0] += xyz[0];	
		mMax[1] += xyz[1];	
		mMax[2] += xyz[2];	
	}

	void Rotate(vec3_t xyz)
	{
		hel::Mat44 m;
		m.Rotate(xyz);
		m.Multiply3fv(mMin.mVec);
		m.Multiply3fv(mMax.mVec);
	}

	void Scale(vec3_t xyz)
	{
		mMin[0] *= xyz[0];	
		mMin[1] *= xyz[1];	
		mMin[2] *= xyz[2];

		mMax[0] *= xyz[0];	
		mMax[1] *= xyz[1];	
		mMax[2] *= xyz[2];	
	}


	void UpdateBounds(vec3_t point)
	{
		for (unsigned int i = 0; i < 3; ++i )
		{
			if (point[i] > mMax[i])
			{
				mMax[i] = point[i];
			}
			else if (point[i] < mMin[i])
			{
				mMin[i] = point[i];
			}
		}
	}

	hel::Vec3 mMin;           /* Bounding box MIN point */

	hel::Vec3 mMax;           /* Bounding box MAX point */
};


class BoundingBoxCombo : public BoundingVolume
{
public:

	BoundingBoxCombo() : BoundingVolume(),
								mSphere(Vec3().mVec, 0.0f), 
								mBox(Vec3().mVec, Vec3().mVec)					
	{
	}

	BoundingBoxCombo(BoundingSphere &s, BoundingBox &b) : BoundingVolume(), 
		mSphere(s), mBox(b)
	{
	}


	bool IsPointInside(vec3_t point)
	{
		if (mSphere.IsPointInside(point) && mBox.IsPointInside(point))
			return true;

		return false;
	}

	void Reset()
	{
		mSphere.Reset();
		mBox.Reset();
	}

	void Translate(vec3_t xyz) 
	{
		mSphere.Translate(xyz);
		mBox.Translate(xyz);
	}

	void Rotate(vec3_t xyz)
	{
		mSphere.Rotate(xyz);
		mBox.Rotate(xyz);
	}

	void Scale(vec3_t xyz)
	{
		mSphere.Scale(xyz);
		mBox.Scale(xyz);
	}

	void UpdateBounds(vec3_t point)
	{
		mSphere.UpdateBounds(point);
		mBox.UpdateBounds(point);
	}

	void GetBoundingBox(vec3_t min, vec3_t max)
	{
		helCopyVec3(mBox.mMin.mVec, min);
		helCopyVec3(mBox.mMax.mVec, max);
	}

	BoundingSphere mSphere;    /* Bounding sphere of this volume */
	
	BoundingBox mBox;          /* Bounding box of this volume */
};

} // namespace hel

#endif

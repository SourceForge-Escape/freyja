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


#ifndef FREYJA9
class BoundingSphere
{
public:
	vec3_t mCenter;            /* Center of bounding sphere */

	vec_t mRadius;             /* Raduis of bounding sphere */	
};


class BoundingBox
{
public:
	vec3_t mMin;           /* Bounding box MIN point */

	vec3_t mMax;           /* Bounding box MAX point */
};
#endif


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

	virtual bool isVertexInside(vec3_t vertex);


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////


#ifndef FREYJA9
	BoundingSphere mSphere;    /* Bounding sphere of this volume */

	BoundingBox mBox;;         /* Bounding box of this volume */
#endif

 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////


	/* */
};


#ifdef FREYJA9
class BoundingSphere : public BoundingVolume
{
public:

	BoundingSphere(vec3_t center, vec_t radius)
	{
		mCenter[0] = center[0];
		mCenter[1] = center[1];
		mCenter[2] = center[2];

		mRadius = radius;
	}


	bool isVertexInside(vec3_t vertex)
	{
		if (helDist3v(mCenter, vertex) <= mRadius)
			return true;

		return false;
	}

	vec3_t mCenter;            /* Center of bounding sphere */

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


	bool isVertexInside(vec3_t vertex)
	{
		if (vertex[0] >= mMin[0] && vertex[0] <= mMax[0])
		{
			if (vertex[1] >= mMin[1] && vertex[1] <= mMax[1])
			{
				if (vertex[0] >= mMin[2] && vertex[0] <= mMax[2])
				{
					return true;
				}
			}
		}

		return false;
	}

	vec3_t mMin;           /* Bounding box MIN point */

	vec3_t mMax;           /* Bounding box MAX point */
};


class BoundingBoxCombo : public BoundingVolume
{
public:

	BoundingBoxCombo(BoundingSphere &s, BoundingBox &b)
	{
		mSphere = s;
		mBox = b;
	}


	bool isVertexInside(vec3_t vertex)
	{
		if (mSphere.isVertexInside(vertex) &&
			 mBox.isVertexInside(vertex))
			return true;

		return false;
	}


	BoundingSphere mSphere;    /* Bounding sphere of this volume */
	
	BoundingBox mBox;          /* Bounding box of this volume */
};
#endif

#endif

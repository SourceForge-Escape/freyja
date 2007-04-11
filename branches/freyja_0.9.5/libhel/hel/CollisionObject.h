/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Hel
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : CollisionObject
 * License : No use w/o permission (C) 2003 Mongoose
 * Comments: This is the magical collision object class 
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongooseichiban@gmail.com>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_COLLISIONOBJECT - Builds CollisionObject as a unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2003.06.07:
 * Mongoose - Created
 ================================================================*/

#ifndef GUARD__HEL_COLLISIONOBJECT_H_
#define GUARD__HEL_COLLISIONOBJECT_H_

#include <mstl/Vector.h>
#include "hel/Mass.h"
#include "hel/math.h"

namespace hel {

class CollisionObject
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	CollisionObject();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of CollisionObject
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.07: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual ~CollisionObject();
	/*------------------------------------------------------
	 * Pre  : CollisionObject object is allocated
	 * Post : Deconstructs an object of CollisionObject
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.07: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	virtual bool intersectPoint(Vec3 p) = 0;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Test if this point intersects with this object
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.08:
	 * Mongoose - Created
	 ------------------------------------------------------*/



	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	Vec3 mIntersection;			   	/* Point of intersection, set if there
													was a collsion detected */

	//vec_t mRepulsionConstant;		/* Repel masses  */
	
	//vec_t mFrictionConstant;		/* Friction applied to masses  */
	
	//vec_t mAbsorptionConstant;		/* Absorption friction applied to masses  */

 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////
};


/* This object limits a entity's height */
class HeightCollisionObject: public CollisionObject
{
 public:

	HeightCollisionObject(vec_t minY, vec_t maxY) 
		: CollisionObject()
	{
		mMinY = minY;
		mMaxY = maxY;
	}


	~HeightCollisionObject()
	{
	}


	bool intersectPoint(Vec3 p)
	{
		bool outside = false;

		mIntersection = p;
		
		if (p.mY < mMinY)
		{
			mIntersection.mY = mMinY;
			outside = true;
		}
		else if (p.mY > mMaxY)
		{
			mIntersection.mY = mMaxY;
			outside = true;
		}

		//printf("Collision occurred? \t[%s]\n", outside ? "true" : "false");

		return outside;
	}


	vec_t mMaxY;
	vec_t mMinY;
};


/* This object contains the entities, it holds them inside its space */
class InternalBBoxCollisionObject: public CollisionObject
{
 public:

	InternalBBoxCollisionObject(vec_t minX, vec_t minY, vec_t minZ,
										vec_t maxX, vec_t maxY, vec_t maxZ) 
		: CollisionObject()
	{
		mMin = Vec3(minX, minY, minZ);
		mMax = Vec3(maxX, maxY, maxZ);
	}


	~InternalBBoxCollisionObject()
	{
	}


	bool intersectPoint(Vec3 p)
	{
		unsigned int i;
		bool outside = false;

		mIntersection = p;
		
		for (i = 0; i < 3; ++i)
		{
			if (p.mVec[i] < mMin.mVec[i])
			{
				mIntersection.mVec[i] = mMin.mVec[i];
				outside = true;
			}
			else if (p.mVec[i] > mMax.mVec[i])
			{
				mIntersection.mVec[i] = mMax.mVec[i];
				outside = true;
			}
		}

		//printf("Collision occurred? \t[%s]\n", outside ? "true" : "false");

		return outside;
	}


	Vec3 mMax;
	Vec3 mMin;
};

#   ifdef FIXME
class VertexAsSphereCollisionObject: public CollisionObject
{
 public:
	/* Vertex list as tiny spheres  */

	vec3_t *mVertices;

	unsigned int mVertexCount;

	vec_t radius;
};


class PlaneCollisionObject: public CollisionObject
{
 public:
	/* Plane list  */

	vec4_t *mPlanes;

	unsigned int mPlaneCount;
};


class BoundingBoxCollisionObject: public CollisionObject
{
 public:
	/* Bbox list  */

};


class BoundingSphereCollisionObject: public CollisionObject
{
 public:
	/* Sphere list  */

};
#   endif

} // namespace hel

#endif // GUARD__HEL_COLLISIONOBJECT_H_

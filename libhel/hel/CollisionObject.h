/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Hel
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : CollisionObject
 * License : No use w/o permission (C) 2003 Mongoose
 * Comments: This is the magical collision object class 
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
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


#ifndef GUARD__HEL_MONGOOSE_COLLISIONOBJECT_H_
#define GUARD__HEL_MONGOOSE_COLLISIONOBJECT_H_

#include <mstl/Vector.h>
#include <hel/Mass.h>
#include <hel/math.h>


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

	virtual bool intersectPoint(Vector3d p) = 0;
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

	Vector3d mIntersection;				/* Point of intersection, set if there
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


	bool intersectPoint(Vector3d p)
	{
		bool outside = false;

		mIntersection = p;
		
		if (p.mVec[1] < mMinY)
		{
			mIntersection.mVec[1] = mMinY;
			outside = true;
		}
		else if (p.mVec[1] > mMaxY)
		{
			mIntersection.mVec[1] = mMaxY;
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
		mMin = Vector3d(minX, minY, minZ);
		mMax = Vector3d(maxX, maxY, maxZ);
	}


	~InternalBBoxCollisionObject()
	{
	}


	bool intersectPoint(Vector3d p)
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


	Vector3d mMax;
	Vector3d mMin;
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
#endif

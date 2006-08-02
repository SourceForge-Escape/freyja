/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : hel
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : Ray
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the ray class
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2005.07.03:
 * Mongoose - Created
 ==========================================================================*/

#include <math.h>

#include "Ray.h"


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Ray::Ray() :
	mOrigin(),
	mDir()
{
}


Ray::Ray(const Vec3 &origin, const Vec3 &dir) :
	mOrigin(mOrigin),
	mDir(mDir)
{
}


Ray::Ray(const Ray &ray) :
	mOrigin(ray.mOrigin),
	mDir(ray.mDir)
{
}


Ray::~Ray()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

bool Ray::IntersectSphere(vec3_t center3, vec_t radius, vec_t &t)
{
	Vec3 center(center3);
	Vec3 l = center - mOrigin;
	Vec3 d(mDir);
	d.normalize();
	vec_t s = l * d;
	vec_t l2 = l * l;
	vec_t r2 = radius * radius;

	if ( s > 0.0f && l2 > r2)
		return false;

	vec_t m2 = l2 - (s*s);
	
	
	if (m2 > r2)
		return false;

	vec_t q = sqrt(r2 - m2);

	if (l2 > r2) 
		t = s - q;
	else
		t = s + q;

	return true;
}


////////////////////////////////////////////////////////////
// Based on MollerTrumbore97 Ray-Triangle 
// NOTE: I've reduced the persision to FLOAT, and made
//       other changes that will likely affect stability
//
//       Computed normal won't agree with defined a normal
//
//       Using the macros to keep it inlined better
//
////////////////////////////////////////////////////////////
#define EPSILON 0.000001
#define CROSS(dest,v1,v2) \
          dest[0]=v1[1]*v2[2]-v1[2]*v2[1]; \
          dest[1]=v1[2]*v2[0]-v1[0]*v2[2]; \
          dest[2]=v1[0]*v2[1]-v1[1]*v2[0];
#define DOT(v1,v2) (v1[0]*v2[0]+v1[1]*v2[1]+v1[2]*v2[2])
#define SUB(dest,v1,v2) \
          dest[0]=v1[0]-v2[0]; \
          dest[1]=v1[1]-v2[1]; \
          dest[2]=v1[2]-v2[2]; 

#define TEST_CULL 0

bool Ray::IntersectTriangle(vec3_t vert0, vec3_t vert1, vec3_t vert2, vec3_t tuv)
{
	mDir.normalize();
	vec3_t edge1, edge2, tvec, pvec, qvec;
	vec3_t &orig = mOrigin.mVec, &dir = mDir.mVec;
	vec_t det, inv_det;
	vec_t &t = tuv[0], &u = tuv[1], &v = tuv[2];

	/* find vectors for two edges sharing vert0 */
	SUB(edge1, vert1, vert0);
	SUB(edge2, vert2, vert0);

	/* begin calculating determinant - also used to calculate U parameter */
	CROSS(pvec, dir, edge2);

	/* if determinant is near zero, ray lies in plane of triangle */
	det = DOT(edge1, pvec);

#if TEST_CULL           /* define TEST_CULL if culling is desired */
	if (det < EPSILON)
		return false;

	/* calculate distance from vert0 to ray origin */
	SUB(tvec, orig, vert0);

	/* calculate U parameter and test bounds */
	u = DOT(tvec, pvec);
	if (u < 0.0 || u > det)
		return false;

	/* prepare to test V parameter */
	CROSS(qvec, tvec, edge1);

    /* calculate V parameter and test bounds */
	v = DOT(dir, qvec);
	if (v < 0.0 || u + v > det)
		return false;

	/* calculate t, scale parameters, ray intersects triangle */
	t = DOT(edge2, qvec);
	inv_det = 1.0 / det;
	tuv *= inv_det;
#else                    /* the non-culling branch */
	if (det > -EPSILON && det < EPSILON)
		return false;
	inv_det = 1.0 / det;

	/* calculate distance from vert0 to ray origin */
	SUB(tvec, orig, vert0);

	/* calculate U parameter and test bounds */
	u = DOT(tvec, pvec) * inv_det;
	if (u < 0.0 || u > 1.0)
		return false;

	/* prepare to test V parameter */
	CROSS(qvec, tvec, edge1);

	/* calculate V parameter and test bounds */
	v = DOT(dir, qvec) * inv_det;
	if (v < 0.0 || u + v > 1.0)
		return false;

	/* calculate t, ray intersects triangle */
	t = DOT(edge2, qvec) * inv_det;
#endif
	return true;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_RAY
int runRayUnitTest(int argc, char *argv[])
{
	Ray test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[Ray class test]\n");

	return runRayUnitTest(argc, argv);
}
#endif




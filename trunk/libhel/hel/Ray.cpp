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

using namespace hel;


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

bool Ray::IntersectBox(vec3_t v0, vec3_t v1, vec3_t v2, vec3_t v3, 
                       vec3_t v4, vec3_t v5, vec3_t v6, vec3_t v7, vec_t &t)
{
	mDir.Norm();
	vec_t bestDist = 99999.0f;
	bool intersect = false;
	Vec3 tuv;

	// Quick and dirty hit tests using tesselated quads for generic box.
	if ( IntersectTriangle( v1, v6, v4, tuv.mVec ) ||
	     IntersectTriangle( v4, v7, v1, tuv.mVec ) )
	{
		intersect = true;
			
		if (tuv.mX < bestDist)
		{
			t = bestDist = tuv.mX;
		}
	}

	if ( IntersectTriangle( v6, v3, v0, tuv.mVec ) ||
	     IntersectTriangle( v0, v4, v6, tuv.mVec ) )
	{
		intersect = true;
			
		if (tuv.mX < bestDist)
		{
			t = bestDist = tuv.mX;
		}
	}

	if ( IntersectTriangle( v1, v5, v2, tuv.mVec ) ||
	     IntersectTriangle( v2, v7, v1, tuv.mVec ) )
	{
		intersect = true;
			
		if (tuv.mX < bestDist)
		{
			t = bestDist = tuv.mX;
		}
	}

	if ( IntersectTriangle( v1, v6, v3, tuv.mVec ) ||
	     IntersectTriangle( v3, v5, v1, tuv.mVec ) )
	{
		intersect = true;
			
		if (tuv.mX < bestDist)
		{
			t = bestDist = tuv.mX;
		}
	}

	if ( IntersectTriangle( v4, v0, v2, tuv.mVec ) ||
	     IntersectTriangle( v2, v7, v4, tuv.mVec ) )
	{
		intersect = true;
			
		if (tuv.mX < bestDist)
		{
			t = bestDist = tuv.mX;
		}
	}

	if ( IntersectTriangle( v3, v0, v2, tuv.mVec ) ||
	     IntersectTriangle( v2, v5, v3, tuv.mVec ) )
	{
		intersect = true;
			
		if (tuv.mX < bestDist)
		{
			t = bestDist = tuv.mX;
		}
	}

	return intersect;
}


// Need to pick an optimzied algorithm and implement.  Likely will go with Woo's.
bool Ray::IntersectAABB_Old(vec3_t min, vec3_t max, vec_t &t)
{
	//mDir.Norm();

	vec_t bestDist = 99999.0f;
	const uint32 count = 6;
	Vec3 v[count];

	v[0] = Vec3(max[0], max[1], max[2]);
	v[1] = Vec3(min[0], min[1], min[2]);
	v[2] = Vec3(max[0], min[1], max[2]);
	v[3] = Vec3(min[0], max[1], max[2]);
	v[4] = Vec3(max[0], max[1], min[2]);
	v[5] = Vec3(min[0], min[1], max[2]);
	v[6] = Vec3(min[0], max[1], min[2]);
	v[7] = Vec3(max[0], min[1], min[2]);

	bool intersect = false;
	Vec3 tuv;

	// Quick and dirty hit test that assumes you can pusedo tesselate 
	// a quad here and always get as good results...
	for (uint32 i = 0, a, b, c, d; i < count; ++i)
	{
		switch (i)
		{
		case 0:    
			a = 1, b = 6, c = 4, d = 7;
			break;

		case 1:
			a = 6, b = 3, c = 0, d = 4;
			break;

		case 2:
			a = 1, b = 5, c = 2, d = 7;
			break;

		case 3:
			a = 1, b = 6, c = 3, d = 5;
			break;

		case 4:
			a = 4, b = 0, c = 2, d = 7;
			break;

		case 5:
			a = 3, b = 0, c = 2, d = 5;
			break;
		}

		if ( IntersectTriangle(v[a], v[b], v[c], tuv) ||
		     IntersectTriangle(v[c], v[d], v[a], tuv) )
		{
			intersect = true;
			
			if (tuv.mX < bestDist)
			{
				t = bestDist = tuv.mX;
			}
		}
	}

	return intersect;
}


bool Ray::IntersectAABB_SAT(Vec3 min, Vec3 max, vec_t& t)
{
	// U
    if ( fabsf( mDir.mX ) < helEpsilon() )
    {
        if ( ( mOrigin.mX < max.mX && mOrigin.mX > min.mX ) == false )
			return false;
    }

    vec_t tfirst = 0.0f;
    vec_t tlast = 1.0f;

	{
	    vec_t tmin = (min.mX - mOrigin.mX);
    	vec_t tmax = (max.mX - mOrigin.mX);

		{
			vec_t invDir = mDir.mX;
			tmin *= invDir;
			tmax *= invDir;
		}

    	if (tmin > tmax) helSwap2f(tmin, tmax);
   
    	if ( !( tmax < tfirst || tmin > tlast ) )
		{
    		if (tmin > tfirst) tfirst = tmin;
    		if (tmax < tlast)  tlast  = tmax;

			t = tfirst;
    	    return true;
		}
	}

	// V
    if ( fabsf( mDir.mY ) < helEpsilon() )
    {
        if ( ( mOrigin.mY < max.mY && mOrigin.mY > min.mY ) == false )
			return false;
    }

	{
	    vec_t tmin = (min.mY - mOrigin.mY);
    	vec_t tmax = (max.mY - mOrigin.mY);

		{
			vec_t invDir = mDir.mY;
			tmin *= invDir;
			tmax *= invDir;
		}

    	if (tmin > tmax) helSwap2f(tmin, tmax);
   
    	if ( !( tmax < tfirst || tmin > tlast ) )
    	{
	    	if (tmin > tfirst) tfirst = tmin;
    		if (tmax < tlast)  tlast  = tmax;

			t = tfirst;
			return true;
		}
	}

	// W
    if ( fabsf( mDir.mZ ) < helEpsilon() )
    {
        if ( ( mOrigin.mZ < max.mZ && mOrigin.mZ > min.mZ ) == false )
			return false;
    }

	{
	    vec_t tmin = (min.mZ - mOrigin.mZ);
    	vec_t tmax = (max.mZ - mOrigin.mZ);

		{
			vec_t invDir = mDir.mZ;
			tmin *= invDir;
			tmax *= invDir;
		}

    	if (tmin > tmax) helSwap2f(tmin, tmax);
   
    	if ( !( tmax < tfirst || tmin > tlast ) )
    	{
	    	if (tmin > tfirst) tfirst = tmin;
    		if (tmax < tlast)  tlast  = tmax;

			t = tfirst;
			return true;
		}
	}

    return false;
}


bool Ray::IntersectAABB(vec3_t min, vec3_t max)
{
	// FIXME: use plucker coords?
#if 1
	vec_t t;
	return IntersectAABB_SAT(min, max, t);
#else
	mDir.Norm();
	Vec3 v[6];

	v[0] = Vec3(max[0], max[1], max[2]);
	v[1] = Vec3(min[0], min[1], min[2]);
	v[2] = Vec3(max[0], min[1], max[2]);
	v[3] = Vec3(min[0], max[1], max[2]);
	v[4] = Vec3(max[0], max[1], min[2]);
	v[5] = Vec3(min[0], min[1], max[2]);
	v[6] = Vec3(min[0], max[1], min[2]);
	v[7] = Vec3(max[0], min[1], min[2]);

	Vec3 tuv;

	// Quick and dirty hit test that assumes you can pusedo tesselate 
	// a quad here and always get as good results...
	for (uint32 i = 0, a, b, c, d; i < 6; ++i)
	{
		switch (i)
		{
		case 0:    
			a = 1, b = 6, c = 4, d = 7;
			break;

		case 1:
			a = 6, b = 3, c = 0, d = 4;
			break;

		case 2:
			a = 1, b = 5, c = 2, d = 7;
			break;

		case 3:
			a = 1, b = 6, c = 3, d = 5;
			break;

		case 4:
			a = 4, b = 0, c = 2, d = 7;
			break;

		case 5:
			a = 3, b = 0, c = 2, d = 5;
			break;
		}

		if ( IntersectTriangle(v[a], v[b], v[c], tuv) ||
		     IntersectTriangle(v[c], v[d], v[a], tuv) )
		{
			return true;
		}
	}

	return false;
#endif
}


bool Ray::IntersectSphere(vec3_t center3, vec_t radius, vec_t &t)
{
	Vec3 center;
	helCopyVec3(center3, center.mVec);
	Vec3 l = center - mOrigin;
	vec_t s = l * mDir;
	vec_t l2 = l * l;
	vec_t r2 = radius * radius;

	if ( s < 0.0f && l2 > r2)
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
//       Computed normal won't agree with defined normal.
//
//       Using the macros to keep it inlined better.
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
	vec3_t &orig = mOrigin.mVec, &dir = mDir.mVec;
	vec_t &t = tuv[0], &u = tuv[1], &v = tuv[2];

	/* find vectors for two edges sharing vert0 */
	vec3_t edge1, edge2;
	SUB(edge1, vert1, vert0);
	SUB(edge2, vert2, vert0);

	/* begin calculating determinant - also used to calculate U parameter */
	vec3_t pvec;
	CROSS(pvec, dir, edge2);

	/* if determinant is near zero, ray lies in plane of triangle */
	vec_t det = DOT(edge1, pvec);

#if TEST_CULL           /* define TEST_CULL if culling is desired */
	if (det < EPSILON)
		return false;

	/* calculate distance from vert0 to ray origin */
	vec3_t tvec;
	SUB(tvec, orig, vert0);

	/* calculate U parameter and test bounds */
	u = DOT(tvec, pvec);
	if (u < 0.0 || u > det)
		return false;

	/* prepare to test V parameter */
	vec3_t qvec;
	CROSS(qvec, tvec, edge1);

    /* calculate V parameter and test bounds */
	v = DOT(dir, qvec);
	if (v < 0.0 || u + v > det)
		return false;

	/* calculate t, scale parameters, ray intersects triangle */
	t = DOT(edge2, qvec);
	vec_t inv_det = 1.0 / det;
	tuv *= inv_det;  // FIXME: tuv is not Vec3

#else                    /* the non-culling branch */
	if (det > -EPSILON && det < EPSILON)
		return false;

	vec_t inv_det = 1.0 / det;

	/* calculate distance from vert0 to ray origin */
	vec3_t tvec;
	SUB(tvec, orig, vert0);

	/* calculate U parameter and test bounds */
	u = DOT(tvec, pvec) * inv_det;
	if (u < 0.0 || u > 1.0)
		return false;

	/* prepare to test V parameter */
	vec3_t qvec;
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


bool Ray::IntersectTriangle(const Vec3& a, const Vec3& b, const Vec3& c, Vec3& tuv)
{
	/* Find vectors for two edges sharing vertex <a>. */
	Vec3 edge1 = b - a;
	Vec3 edge2 = c - a;
		
	/* Begin calculating determinant - also used to calculate U parameter. */
	Vec3 pvec;
	Vec3::Cross(mDir, edge2, pvec);

	/* If determinant is near zero, ray lies in plane of triangle. */
	vec_t det = Vec3::Dot(edge1, pvec);

	/* NO CULL */
	if ( det > -helEpsilon() && det < helEpsilon() )  // WTF happened here?
		return false;

	vec_t inv_det = 1.0f / det;

	/* Calculate distance from <a> to ray origin. */
	Vec3 tvec = mOrigin - a;

	/* Calculate U parameter and test bounds. */
	tuv.mY = Vec3::Dot(tvec, pvec) * inv_det;
	if (tuv.mY < 0.0f || tuv.mY > 1.0f)
		return false;

	/* Prepare to test V parameter. */
	Vec3 qvec;
	Vec3::Cross(tvec, edge1, qvec);

	/* Calculate V parameter and test bounds. */
	tuv.mZ = Vec3::Dot(mDir, qvec) * inv_det;
	if (tuv.mZ < 0.0f || (tuv.mY + tuv.mZ) > 1.0f)
		return false;

	/* calculate t, ray intersects triangle */
	tuv.mX = Vec3::Dot(edge2, qvec) * inv_det;

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




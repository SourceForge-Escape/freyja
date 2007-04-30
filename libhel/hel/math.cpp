/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Hel
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission, (C) 2002-2007 Mongoose
 * Comments: 
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongooseichiban@gmail.com>
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.04.07:
 * Mongoose - It's about time this was cleaned up a little.
 *
 * 2002.05.11:
 * Mongoose - Created
 ==========================================================================*/

#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <hel/math.h>
#include <hel/Vec3.h>
#include <hel/Mat44.h>

using namespace hel;

const char *helVersionInfo()
{
	return VERSION;
}


vec_t helIntersectionOfAbstractSpheres(vec3_t centerA, vec_t radiusA,
									   vec3_t centerB, vec_t radiusB)
{
	Vec3 a = Vec3(centerA);
	Vec3 b = Vec3(centerB);
	Vec3 d = a - b;
	vec_t dist, minDist;

	dist = Vec3::Dot(d, d);
	minDist = radiusA + radiusB;
	
	return (dist <= minDist * minDist);
}


// Returns number of intersections and intersection position(s)
// Got algorithm from http://astronomy.swin.edu.au/~pbourke/geometry/
int helIntersectionOfAbstractSphereAndLine(vec3_t center, vec_t radius,
										   vec3_t posA, vec3_t posB,
										   vec3_t intersectionA,
										   vec3_t intersectionB)
{
	// float x , y , z;
	vec_t a, b, c, mu, i ;
	

	a = (helSquare(posB[0] - posA[0]) + 
		  helSquare(posB[1] - posA[1]) + 
		  helSquare(posB[2] - posA[2]));
	b = (2 * ((posB[0] - posA[0]) * (posA[0] - center[0]) +
				 (posB[1] - posA[1]) * (posA[1] - center[1]) +
				 (posB[2] - posA[2]) * (posA[2] - center[2])));
	c = (helSquare(center[0]) + helSquare(center[1]) +
		  helSquare(center[2]) + helSquare(posA[0]) +
		  helSquare(posA[1]) + helSquare(posA[2]) -
		  2 * (center[0]*posA[0] + center[1]*posA[1] + center[2]*posA[2]) - 
		  helSquare(radius));

	i = b * b - 4 * a * c;

	
	if (i < 0.0)
	{
		// No intersection
		return 0;
	}
	else if (i == 0.0)
	{
		// One intersection
		mu = -b/(2*a) ;
		intersectionA[1] = posA[0] + mu*(posB[0]-posA[0]);
		intersectionA[2] = posA[1] + mu*(posB[1]-posA[1]);
		intersectionA[3] = posA[2] + mu*(posB[2]-posA[2]);

		return 1;
	}
	else
	{
		// Two intersections
		
		// First intersection
		mu = (-b + sqrt( helSquare(b) - 4*a*c)) / (2*a);
		intersectionA[1] = posA[0] + mu*(posB[0]-posA[0]);
		intersectionA[2] = posA[1] + mu*(posB[1]-posA[1]);
		intersectionA[3] = posA[2] + mu*(posB[2]-posA[2]);
		
		// Second intersection
		mu = (-b - sqrt( helSquare(b) - 4*a*c)) / (2*a);
		intersectionB[0] = posA[0] + mu*(posB[0]-posA[0]);
		intersectionB[1] = posA[1] + mu*(posB[1]-posA[1]);
		intersectionB[2] = posA[2] + mu*(posB[2]-posA[2]);

		return 2;
	}
}


bool helSphereIntersectLine(const Vec3 &pos, const Vec3 &lastPos,
							const Vec3 &center, vec_t radius)
{
	Vec3 seg = pos - lastPos;
	Vec3 segToCenter = center - lastPos;
	vec_t s = seg * segToCenter;

	if (s >= 1.0f || s <= 0.0f)
		return false;

	seg.Norm();
	seg = seg * s;
	seg = seg + lastPos;

	Vec3 delta = seg - center;

	return (radius >= delta * delta) ? true : false;
}


int helIntersectionLineAndPolygon(vec3_t intersect,
                                  vec3_t p1, vec3_t p2,
                                  unsigned int vertexCount, vec3_t *ploygon)
{
	// Find normal
	Vec3 a = Vec3(ploygon[1]) - Vec3(ploygon[0]);
	Vec3 b = Vec3(ploygon[2]) - Vec3(ploygon[0]);
	Vec3 normal = Vec3::Cross(a, b);
	normal.Norm();

	// Find d
	vec_t d = (normal.mVec[0] * ploygon[0][0] -
			   normal.mVec[1] * ploygon[0][1] - 
			   normal.mVec[2] * ploygon[0][2]);

	// Line segment parallel to plane?
	Vec3 pA = Vec3(p1);
	Vec3 pB = Vec3(p2);
	a = pB - pA;
	vec_t denominator = Vec3::Dot(normal, a);

	if (denominator > 0.0f)
		return 0;

	// Line segment contains intercept point?
	vec_t mu = -((d + Vec3::Dot(normal, pA)) / denominator);

	if (mu < 0.0f || mu > 1.0f)
		return 0;

	b = pA + (a * mu);
	b.Get(intersect);

	// See if the intercept is bound by polygon by winding number
#ifdef WINDING_NUMBERS_TRIANGLE
	mtkVectorSubtract(ploygon[0], intersect, a);
	mtkVectorNormalize(a, a);
	mtkVectorSubtract(ploygon[1], intersect, b);
	mtkVectorNormalize(b, b);
	mtkVectorSubtract(ploygon[2], intersect, c);
	mtkVectorNormalize(c, c);

	t0 = mtkVectorDotProduct(a, b);
	t1 = mtkVectorDotProduct(b, c);
	t2 = mtkVectorDotProduct(c, a);

	total = HEL_RAD_TO_DEG(acos(t0) + acos(t1) + acos(t2));

	if (total - 360 < 0.0f)
		return 0;
#else // assume convex polygons here for sure
	vec_t theta = Vec3::Dot(b - Vec3(ploygon[0]), normal);

	if (theta >= 90.0f) // Yeah I know
		return 0;
#endif

	return 1;
}


vec_t helDistToSphereFromPlane3v(vec3_t center,	vec_t radius, vec4_t plane)
{
	vec_t d = (plane[0] * center[0] + plane[1] * center[1] +
			   plane[2] * center[2] + plane[3]);

	if (d <= -radius)
		return 0;

	return d + radius;
}


vec_t helDistToBboxFromPlane3v(vec3_t min, vec3_t max, vec4_t plane)
{
	vec3_t center;
	helMidpoint3v(min, max, center);

	vec_t d = (plane[0] * center[0] + plane[1] * center[1] + 
			   plane[2] * center[2] + plane[3]);

	vec_t radius = helDist3v(max, center);

	if (d <= -radius)
		return 0;

	return d + radius;
}


vec_t helDist3v(vec3_t a, vec3_t b)
{
	return (sqrtf( ((b[0] - a[0]) * (b[0] - a[0])) +
				   ((b[1] - a[1]) * (b[1] - a[1])) + 
				   ((b[2] - a[2]) * (b[2] - a[2]))));
}


void helMidpoint3v(vec3_t a, vec3_t b, vec3_t mid)
{
	mid[0] = (a[0] + b[0]) * 0.5f;
	mid[1] = (a[1] + b[1]) * 0.5f;
	mid[2] = (a[2] + b[2]) * 0.5f;
}


vec_t helNorm4v(vec4_t v)
{ 
	return (sqrtf(v[0]*v[0] + v[1]*v[1] + v[2]*v[2] + v[3]*v[3]));
}


vec_t helNorm3v(vec3_t v)
{ 
	return (sqrtf(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]));
} 


vec_t helNorm2v(vec2_t v)
{ 
	return (sqrtf(v[0]*v[0] + v[1]*v[1]));
}


vec_t helRandomNum(vec_t from, vec_t to)
{
	return from + (to*rand()/(RAND_MAX+1.0));
}


/// Matrices ///////////////////////////////////////////

void helMatrixCopy(matrix_t source, matrix_t dest) 
{
	memcpy(dest, source, sizeof(matrix_t));
}


// Remember: Post multiplying column-major will give you row-major
void helMatrixMultiply(const matrix_t a, const matrix_t b, matrix_t result)
{
#if 1
	/* Column order */
	result[ 0] = a[ 0] * b[ 0] + a[ 4] * b[ 1] + a[ 8] * b[ 2] + a[12] * b[ 3];
	result[ 1] = a[ 0] * b[ 4] + a[ 4] * b[ 5] + a[ 8] * b[ 6] + a[12] * b[ 7];
	result[ 2] = a[ 0] * b[ 8] + a[ 4] * b[ 9] + a[ 8] * b[10] + a[12] * b[11];
	result[ 3] = a[ 0] * b[12] + a[ 4] * b[13] + a[ 8] * b[14] + a[12] * b[15];
	
	result[ 4] = a[ 1] * b[ 0] + a[ 5] * b[ 1] + a[ 9] * b[ 2] + a[13] * b[ 3];
	result[ 5] = a[ 1] * b[ 4] + a[ 5] * b[ 5] + a[ 9] * b[ 6] + a[13] * b[ 7];
	result[ 6] = a[ 1] * b[ 8] + a[ 5] * b[ 9] + a[ 9] * b[10] + a[13] * b[11];
	result[ 7] = a[ 1] * b[12] + a[ 5] * b[13] + a[ 9] * b[14] + a[13] * b[15];
	
	result[ 8] = a[ 2] * b[ 0] + a[ 6] * b[ 1] + a[10] * b[ 2] + a[14] * b[ 3];
	result[ 9] = a[ 2] * b[ 4] + a[ 6] * b[ 5] + a[10] * b[ 6] + a[14] * b[ 7];
	result[10] = a[ 2] * b[ 8] + a[ 6] * b[ 9] + a[10] * b[10] + a[14] * b[11];
	result[11] = a[ 2] * b[12] + a[ 6] * b[13] + a[10] * b[14] + a[14] * b[15];
	
	result[12] = a[ 3] * b[ 0] + a[ 7] * b[ 1] + a[11] * b[ 2] + a[15] * b[ 3];
	result[13] = a[ 3] * b[ 4] + a[ 7] * b[ 5] + a[11] * b[ 6] + a[15] * b[ 7];
	result[14] = a[ 3] * b[ 8] + a[ 7] * b[ 9] + a[11] * b[10] + a[15] * b[11];
	result[15] = a[ 3] * b[12] + a[ 7] * b[13] + a[11] * b[14] + a[15] * b[15];
#else
	/* Column order */
	result[ 0] = a[ 0] * b[ 0] + a[ 4] * b[ 1] + a[ 8] * b[ 2] + a[12] * b[ 3];
	result[ 4] = a[ 0] * b[ 4] + a[ 4] * b[ 5] + a[ 8] * b[ 6] + a[12] * b[ 7];
	result[ 8] = a[ 0] * b[ 8] + a[ 4] * b[ 9] + a[ 8] * b[10] + a[12] * b[11];
	result[12] = a[ 0] * b[12] + a[ 4] * b[13] + a[ 8] * b[14] + a[12] * b[15];
	
	result[ 1] = a[ 1] * b[ 0] + a[ 5] * b[ 1] + a[ 9] * b[ 2] + a[13] * b[ 3];
	result[ 5] = a[ 1] * b[ 4] + a[ 5] * b[ 5] + a[ 9] * b[ 6] + a[13] * b[ 7];
	result[ 9] = a[ 1] * b[ 8] + a[ 5] * b[ 9] + a[ 9] * b[10] + a[13] * b[11];
	result[13] = a[ 1] * b[12] + a[ 5] * b[13] + a[ 9] * b[14] + a[13] * b[15];
	
	result[ 2] = a[ 2] * b[ 0] + a[ 6] * b[ 1] + a[10] * b[ 2] + a[14] * b[ 3];
	result[ 6] = a[ 2] * b[ 4] + a[ 6] * b[ 5] + a[10] * b[ 6] + a[14] * b[ 7];
	result[10] = a[ 2] * b[ 8] + a[ 6] * b[ 9] + a[10] * b[10] + a[14] * b[11];
	result[14] = a[ 2] * b[12] + a[ 6] * b[13] + a[10] * b[14] + a[14] * b[15];
	
	result[ 3] = a[ 3] * b[ 0] + a[ 7] * b[ 1] + a[11] * b[ 2] + a[15] * b[ 3];
	result[ 7] = a[ 3] * b[ 4] + a[ 7] * b[ 5] + a[11] * b[ 6] + a[15] * b[ 7];
	result[11] = a[ 3] * b[ 8] + a[ 7] * b[ 9] + a[11] * b[10] + a[15] * b[11];
	result[15] = a[ 3] * b[12] + a[ 7] * b[13] + a[11] * b[14] + a[15] * b[15];
#endif
}


void helVectorMatrixMult3fv(matrix_t m, vec3_t v, vec3_t result)
{
	vec_t x = v[0], y = v[1], z = v[2];

	// Column order
	result[0] = m[0]*x + m[4]*y + m[ 8]*z + m[12];
	result[1] = m[1]*x + m[5]*y + m[ 9]*z + m[13];
	result[2] = m[2]*x + m[6]*y + m[10]*z + m[14];
}


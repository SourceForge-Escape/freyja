/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*================================================================
 * 
 * Project : Midgard, Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : 
 * License : No use w/o permission (C) 2002 Mongoose
 * Comments: 
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2002.05.11:
 * Mongoose - Created
 ================================================================*/

#include <stdlib.h>
#include <math.h>

#include <hel/math.h>
#include <hel/Vector3d.h>
#include <hel/Matrix.h>

#define COMPUTE

char *helVersionInfo()
{
	return VERSION;
}


void helVectorMatrixMult4dv(double v[4], matrix_t m, double result[4])
{
	/* Column major */
	result[0] = m[ 0] * v[0] + m[ 4] * v[1] + m[ 8] * v[2] + m[12] * v[3];
	result[1] = m[ 1] * v[0] + m[ 5] * v[1] + m[ 9] * v[2] + m[13] * v[3];
	result[2] = m[ 2] * v[0] + m[ 6] * v[1] + m[10] * v[2] + m[14] * v[3];
	result[3] = m[ 3] * v[0] + m[ 7] * v[1] + m[11] * v[2] + m[15] * v[3];


#ifdef USE_ROW_MAJOR_MATRIX
	/* Row major */
    result[0] = m[ 0] * v[0] + m[ 1] * v[1] + m[ 2] * v[2] + m[ 3] * v[3];
	result[1] = m[ 4] * v[0] + m[ 5] * v[1] + m[ 6] * v[2] + m[ 7] * v[3];
	result[2] = m[ 8] * v[0] + m[ 9] * v[1] + m[10] * v[2] + m[11] * v[3];
	result[3] = m[12] * v[0] + m[13] * v[1] + m[14] * v[2] + m[15] * v[3];
#endif
}


vec_t helIntersectionOfAbstractSpheres(vec3_t centerA, vec_t radiusA,
													vec3_t centerB, vec_t radiusB)
{
	Vector3d a = Vector3d(centerA);
	Vector3d b = Vector3d(centerB);
	Vector3d d = a - b;
	vec_t dist, minDist;

	dist = Vector3d::dot(d, d);
	minDist = radiusA + radiusB;
	
	return (dist <= minDist * minDist);
}


inline vec_t square(vec_t a) 
{
	return a * a;
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
	

	a = (square(posB[0] - posA[0]) + 
		  square(posB[1] - posA[1]) + 
		  square(posB[2] - posA[2]));
	b = (2 * ((posB[0] - posA[0]) * (posA[0] - center[0]) +
				 (posB[1] - posA[1]) * (posA[1] - center[1]) +
				 (posB[2] - posA[2]) * (posA[2] - center[2])));
	c = (square(center[0]) + square(center[1]) +
		  square(center[2]) + square(posA[0]) +
		  square(posA[1]) + square(posA[2]) -
		  2 * (center[0]*posA[0] + center[1]*posA[1] + center[2]*posA[2]) - 
		  square(radius));

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
		mu = (-b + sqrt( square(b) - 4*a*c)) / (2*a);
		intersectionA[1] = posA[0] + mu*(posB[0]-posA[0]);
		intersectionA[2] = posA[1] + mu*(posB[1]-posA[1]);
		intersectionA[3] = posA[2] + mu*(posB[2]-posA[2]);
		
		// Second intersection
		mu = (-b - sqrt(square(b) - 4*a*c)) / (2*a);
		intersectionB[0] = posA[0] + mu*(posB[0]-posA[0]);
		intersectionB[1] = posA[1] + mu*(posB[1]-posA[1]);
		intersectionB[2] = posA[2] + mu*(posB[2]-posA[2]);

		return 2;
	}
}


bool tmpHelSphereIntersectLine(Vector3d pos, Vector3d lastPos, 
										 Vector3d center, vec_t radius)
{
	Vector3d seg, segToCenter, delta;
	vec_t s, dSquare;


	seg = pos - lastPos;
	segToCenter = center - lastPos;

	s = seg * segToCenter;

	if (s >= 1.0f || s <= 0.0f)
		return false;

	seg.normalize();
	seg = seg * s;
	seg = seg + lastPos;

	delta = seg - center;

	dSquare = delta * delta;

	if (radius >= dSquare)
		return true;
	else
		return false;
}


int helIntersectionLineAndPolygon(vec3_t intersect,
                                  vec3_t p1, vec3_t p2,
                                  unsigned int vertexCount, vec3_t *ploygon)
{
	//	vec3_t normal, a, b;
	Vector3d a, b, normal, pA, pB;
	vec_t d, denominator, mu;
	double theta;


	pA = Vector3d(p1);
	pB = Vector3d(p2);

	// Find normal
	//mtkVectorSubtract(ploygon[1], ploygon[0], a);
	a = Vector3d(ploygon[1]) - Vector3d(ploygon[0]);
	//mtkVectorSubtract(ploygon[2], ploygon[0], b);
	b = Vector3d(ploygon[2]) - Vector3d(ploygon[0]);
	normal = Vector3d::cross(a, b);
	//mtkVectorCrossProduct(a, b, normal);
	normal.normalize();
	//mtkVectorNormalize(normal, normal);

	// find D
	//d = (normal[0] * ploygon[0][0] -
	//	  normal[1] * ploygon[0][1] - 
	//	  normal[2] * ploygon[0][2]);
	d = (normal.mVec[0] * ploygon[0][0] -
		  normal.mVec[1] * ploygon[0][1] - 
		  normal.mVec[2] * ploygon[0][2]);

	// line segment parallel to plane?
	//mtkVectorSubtract(p2, p1, a); // cache p2 - p1 => a
	a = pB - pA;

	//denominator = (normal[0] * a[0] +
	//					normal[1] * a[1] +
	//					normal[2] * a[2]);
	denominator = Vector3d::dot(normal, a);

	if (denominator > 0.0)
		return 0;

	// Line segment contains intercept point?
	//mu = - ((d + normal[0] * p1[0] + normal[1] * p1[1] + normal[2] * p1[2]) /
	//		  denominator);
	mu = -((d + Vector3d::dot(normal, pA)) / denominator);

	if (mu < 0.0 || mu > 1.0)
		return 0;

	//intersect[0] = p1[0] + mu * a[0];
	//intersect[1] = p1[1] + mu * a[1];
	//intersect[2] = p1[2] + mu * a[2];
	b = pA + (a * mu);
	intersect[0] = b.mVec[0];
	intersect[1] = b.mVec[1];
	intersect[2] = b.mVec[2];
	

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

	if (total - 360 < 0.0)
		return 0;
#else // assume convex polygons here for sure
	//mtkVectorSubtract(intersect, ploygon[0], a);
	//theta = mtkVectorDotProduct(a, normal);
	theta = Vector3d::dot(b - Vector3d(ploygon[0]), normal); // b = intersect

	if (theta >= 90.0) // Yeah I know
		return 0;
#endif

	return 1;
}


vec_t helDistToSphereFromPlane3v(vec3_t center,	vec_t radius, vec4_t plane)
{
	vec_t d;


	d = (plane[0] * center[0] + 
		  plane[1] * center[1] +
		  plane[2] * center[2] + 
		  plane[3]);

	if (d <= -radius)
		return 0;

	return d + radius;
}


vec_t helDistToBboxFromPlane3v(vec3_t min, vec3_t max, vec4_t plane)
{
	vec3_t center;
	vec_t d, radius;


	helMidpoint3v(min, max, center);

	d = (plane[0] * center[0] + 
		  plane[1] * center[1] + 
		  plane[2] * center[2] + 
		  plane[3]);

	radius = helDist3v(max, center);

	if (d <= -radius)
		return 0;

	return d + radius;
}


vec_t helDist3v(vec3_t a, vec3_t b)
{
	return (sqrt( ((b[0] - a[0]) * (b[0] - a[0])) +
					  ((b[1] - a[1]) * (b[1] - a[1])) + 
					  ((b[2] - a[2]) * (b[2] - a[2]))));
}


void helMidpoint3v(vec3_t a, vec3_t b, vec3_t mid)
{
	mid[0] = (a[0] + b[0]) / 2;
	mid[1] = (a[1] + b[1]) / 2;
	mid[2] = (a[2] + b[2]) / 2;
}


vec_t helNorm4v(vec4_t v)
{ 
	return (sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2] + v[3]*v[3]));
}


vec_t helNorm3v(vec3_t v)
{ 
	return (sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]));
} 


vec_t helNorm2v(vec2_t v)
{ 
	return (sqrt(v[0]*v[0] + v[1]*v[1]));
}


vec_t helRandomNum(vec_t from, vec_t to)
{
	return from + (to*rand()/(RAND_MAX+1.0));
}


vec_t helDegToRad(vec_t degrees)
{
#ifdef COMPUTE
	return ((degrees / 180.0) * HEL_PI);
#else
	// degrees * (180.0 / PI);
	return (degrees * HEL_180_OVER_PI);
#endif
}


vec_t helRadToDeg(vec_t rad)
{
#ifdef COMPUTE
	return ((rad / HEL_PI) * 180.0); 
#else
	// rad * (PI / 180.0);
	return (rad * HEL_PI_OVER_180);
#endif
}


#ifdef MATH_UNIT_TEST
#include <stdio.h>

void helMathTest()
{
	printf("180/PI: %f, %f, %f\n", 
			 HEL_180_OVER_PI, 
			 180.0f / HEL_PI,
			 180.0 / M_PI);
}


int main(int argc, char *argv)
{
	helMathTest();

	return 0;
}
#endif

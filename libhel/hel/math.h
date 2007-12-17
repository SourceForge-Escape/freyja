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

#ifndef GUARD__HEL_MATH_H
#define GUARD__HEL_MATH_H

// Required for sincosf
#ifndef _GNU_SOURCE
#   define _GNU_SOURCE
#endif

#include <math.h>

#ifdef sincosf
#   define helSinCosf sincosf
#else
#   define helSinCosf(a, s, c) *s = sinf(a); *c = cosf(a) 
#endif

#define HEL_PI           3.14159265358979323846  /* pi */
#define HEL_PI_OVER_2    1.57079632679489661923  /* pi/2 */
#define HEL_2_PI         6.28318530717958647692  /* pi*2 */
#define HEL_PI_OVER_4    0.78539816339744830962  /* pi/4 */
#define HEL_PI_OVER_180  0.017453292519943295    /* pi/180 */
#define HEL_180_OVER_PI  57.295779513082323      /* 180/pi */

#define helRadToDeg(a) ((a) * HEL_180_OVER_PI)

#define helDegToRad(a) ((a) * HEL_PI_OVER_180)

#define helSquare(n) ((n) * (n))

#define helEpsilon() (1.0e-7)
// 0.000001f

#define helCopyVec3(src, dest) memcpy(dest, src, sizeof(vec3_t))


extern "C" {

// FIXME: Should be using ISO types for true correct size.
typedef unsigned char byte;
typedef short int int16;
typedef unsigned short int uint16;
typedef int int32;
typedef unsigned int uint32;
typedef float vec_t;
typedef float vec2_t[2];
typedef float vec3_t[3];
typedef float vec4_t[4];
typedef vec_t matrix_t[16];  /* Used as _Column_major_ in every class now! */
typedef vec_t mat33_t[9];    /* Column major, 3x3 matrix used for rotation */

typedef	union {
	vec3_t xyz;

	struct {
		vec_t x, y, z;
	};
} uvec3_t;


inline
void helSwap2f(vec_t& a, vec_t& b) 
{ 
	vec_t c = b; 
	b = a; 
	a = c; 
}


const char *helVersionInfo();
/*------------------------------------------------------
 * Pre  : 
 * Post : Returns string with library version information.
 *
 ------------------------------------------------------*/

vec_t helIntersectionOfAbstractSpheres(vec3_t centerA, vec_t radiusA,
													vec3_t centerB, vec_t radiusB);
/*------------------------------------------------------
 * Pre  : 
 * Post : 
 *
 *-- History ------------------------------------------
 *
 * 2003.05.30:
 * Mongoose - Created, from UnRaider work
 ------------------------------------------------------*/

int helIntersectionOfAbstractSphereAndLine(vec3_t center, vec_t radius,
														 vec3_t posA, vec3_t posB,
														 vec3_t intersectionA,
														 vec3_t intersectionB);
/*------------------------------------------------------
 * Pre  : 
 * Post : 
 *
 *-- History ------------------------------------------
 *
 * 2003.05.30:
 * Mongoose - Created, from UnRaider work
 ------------------------------------------------------*/

int helIntersectionLineAndPolygon(vec3_t intersect,
											 vec3_t p1, vec3_t p2,
											 unsigned int vertexCount, vec3_t *ploygon);
/*------------------------------------------------------
 * Pre  : Given P1 and P2 of line segment and
 *        Vertex count and ploygon with count vertices
 *
 *        Only supports triangles and coplanar quads
 *
 * Post : Returns intersect point or 0 if none
 *
 *-- History ------------------------------------------
 *
 * 2003.05.30:
 * Mongoose - Created
 ------------------------------------------------------*/

vec_t helDistToSphereFromPlane3v(vec3_t center,	vec_t radius, vec4_t plane);
/*------------------------------------------------------
 * Pre  : Given center and radius of sphere and a plane
 * Post : Returns distance from sphere to plane
 *
 *-- History ------------------------------------------
 *
 * 1999.06.14:
 * Mongoose - Created, from mtk3d
 ------------------------------------------------------*/

vec_t helDistToBboxFromPlane3v(vec3_t min, vec3_t max, vec4_t plane);
/*------------------------------------------------------
 * Pre  : Given min and max points of a bounding box
 *        and a plane
 *
 * Post : Returns distance from box to plane
 *
 *-- History ------------------------------------------
 *
 * 1999.06.14:
 * Mongoose - Created, from mtk3d
 ------------------------------------------------------*/

vec_t helDist3v(vec3_t a, vec3_t b);
/*------------------------------------------------------
 * Pre  : Given point A and B 
 * Post : Returns length of line segment
 *
 *-- History ------------------------------------------
 *
 * 1999.06.14:
 * Mongoose - Created, from mtk3d
 ------------------------------------------------------*/

void helMidpoint3v(vec3_t a, vec3_t b, vec3_t mid);
/*------------------------------------------------------
 * Pre  : Given point A and B 
 * Post : Returns midpoint of line segment
 *
 *-- History ------------------------------------------
 *
 * 1999.06.14:
 * Mongoose - Created, from mtk3d
 ------------------------------------------------------*/

//vec_t helDegToRad(vec_t degrees);
/*------------------------------------------------------
 * Pre  : Given angle in degrees
 * Post : Returns angle in radians
 *
 *-- History ------------------------------------------
 *
 * 1999.06.14:
 * Mongoose - Created, from mtk3d
 ------------------------------------------------------*/

//vec_t helRadToDeg(vec_t rad);
/*------------------------------------------------------
 * Pre  : Given angle in radians
 * Post : Returns angle in degrees
 *
 *-- History ------------------------------------------
 *
 * 1999.06.14:
 * Mongoose - Created, from mtk3d
 ------------------------------------------------------*/

vec_t helRandomNum(vec_t from, vec_t to);
/*------------------------------------------------------
 * Pre  : 
 * Post : Returns random float between <from> - <to>
 *
 ------------------------------------------------------*/


/// Matrices ///////////////////////////////////////////

void helMatrixCopy(matrix_t source, matrix_t dest);
/*------------------------------------------------------
 * Pre  : 
 * Post : Copys value of source to dest
 *
 ------------------------------------------------------*/

void helMatrixPostMultiply(const matrix_t a, const matrix_t b, matrix_t result);
void helMatrixMultiply(const matrix_t a, const matrix_t b, matrix_t result);
/*------------------------------------------------------
 * Pre  : Multiplies matrices a and b ( column-major )
 *        Neither a or b is also the result
 *
 * Post : Sets resultant matrix value ( result )
 *        
 * Note : Post multiplying column-major will give you row-major
 *
 ------------------------------------------------------*/

void helVectorMatrixMult3fv(matrix_t m, vec3_t v, vec3_t result);
/*------------------------------------------------------
 * Pre  : 
 * Post : Returns <result> vector, 
 *        Also <v> and <result> can be the same vector.
 *
 ------------------------------------------------------*/

}

#endif // GUARD__HEL_MATH_H

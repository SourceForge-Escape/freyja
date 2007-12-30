/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Hel
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : Vector3d
 * License : No use w/o permission (C) 2002-2007 Mongoose
 * Comments: Math vector
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongooseichiban@gmail.com>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2007.04.01:
 * Mongoose - Major API changes, optimzations, and style updates
 *
 * 2002.12.24:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__HEL_VEC3_H_
#define GUARD__HEL_VEC3_H_

#include <string.h> // memcpy()

#include "hel/math.h"


namespace hel {

class Vec3
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Vec3() : mX(0.0f), mY(0.0f), mZ(0.0f) { }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs Vec3.
	 *
	 ------------------------------------------------------*/

	Vec3(const Vec3 &v) { memcpy(mVec, v.mVec, sizeof(vec3_t)); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs Vec3.
	 *
	 ------------------------------------------------------*/

	Vec3(const vec3_t v) { memcpy(mVec, v, sizeof(vec3_t)); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs Vec3.
	 *
	 ------------------------------------------------------*/

	Vec3(vec_t x, vec_t y, vec_t z) : mX(x), mY(y), mZ(z) { }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs Vec3.
	 *
	 ------------------------------------------------------*/

	~Vec3() { }
	/*------------------------------------------------------
	 * Pre  : Vec3 object is allocated.
	 * Post : Deconstructs an object of Vec3.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	static vec_t Dot(const Vec3 &u, const Vec3 &v)
	{ return (u.mX * v.mX + u.mY * v.mY + u.mZ * v.mZ); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns dot product of <u> and <v> vectors.
	 *
	 ------------------------------------------------------*/

	static void Cross(const Vec3& u, const Vec3& v, Vec3& result)	
	{
		result.mX = u.mY * v.mZ - u.mZ * v.mY;
		result.mY = u.mZ * v.mX - u.mX * v.mZ;
		result.mZ = u.mX * v.mY - u.mY * v.mX;
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns cross product of <u> and <v> vectors.
	 *
	 ------------------------------------------------------*/

	static Vec3 Cross(const Vec3 &u, const Vec3 &v)	
	{
		return Vec3(u.mY * v.mZ - u.mZ * v.mY,
					u.mZ * v.mX - u.mX * v.mZ,
					u.mX * v.mY - u.mY * v.mX);
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns cross product of <u> and <v> vectors.
	 *
	 ------------------------------------------------------*/

	void Get(vec3_t v) const
	{ memcpy(v, mVec, sizeof(vec3_t)); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Copies Vec3 to floating point array <v>.
	 *
	 ------------------------------------------------------*/

	vec_t Magnitude() { return sqrtf(mX*mX + mY*mY + mZ*mZ); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns magnitude this vector
	 *
	 ------------------------------------------------------*/

	Vec3 GetUnit() { Vec3 v(*this); v.Norm(); return v; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns normalized copy of this vector.
	 *
	 ------------------------------------------------------*/

	vec_t &operator [] (uint32 i) { return mVec[i]; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns reference to the <i>th value of Vec3. 
	 *
	 ------------------------------------------------------*/

	Vec3 operator +(const Vec3 &v) const
	{ return Vec3(mX+v.mX, mY+v.mY, mZ+v.mZ); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns the sum of Vec3 and <v>.
	 *
	 ------------------------------------------------------*/

	Vec3 operator -(const Vec3 &v) const 
	{ return Vec3(mX-v.mX, mY-v.mY, mZ-v.mZ); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns Vec3 subtracted by <v>.
	 *
	 ------------------------------------------------------*/

	Vec3 operator -() const { return Vec3(-mX, -mY, -mZ); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns vector of Vec3 negated.
	 *
	 ------------------------------------------------------*/

	Vec3 operator *(vec_t s) const { Vec3 v(*this); v *= s; return v; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns vector of Vec3 scaled by <s>.
	 *
	 ------------------------------------------------------*/

	Vec3 operator /(vec_t s) const { Vec3 v(*this); v /= s; return v; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns vector of Vec3 scaled by 1/<s>.
	 *
	 ------------------------------------------------------*/

	vec_t operator *(const Vec3 &v) const { return Dot(*this, v); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns dot product of <v> and Vec3.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void Norm() { const vec_t normInv = 1.0f / Magnitude(); *this *= normInv; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Vec3 is normalized.
	 *
	 ------------------------------------------------------*/

	void Set(const vec_t x, const vec_t y, const vec_t z) 
	{ mX = x; mY = y; mZ = z; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets the vector by floats.
	 *
	 ------------------------------------------------------*/

	void Set(const vec3_t v) { memcpy(mVec, v, sizeof(vec3_t)); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets the vector by floating point array.
	 *
	 ------------------------------------------------------*/

	void Zero() { mX = mY = mZ = 0.0f; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Vec3 is assigned <0, 0, 0>.
	 *
	 ------------------------------------------------------*/

	Vec3 &operator =(const Vec3 &v) 
	{ mX = v.mX; mY = v.mY; mZ = v.mZ; return *this; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Vec3 is assigned <v>.
	 *
	 ------------------------------------------------------*/

	void operator +=(const Vec3 &v) { mX += v.mX; mY += v.mY; mZ += v.mZ; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Vec3 is assigned the sum of Vec and <v>.
	 *
	 ------------------------------------------------------*/

	void operator -=(const Vec3 &v) { mX -= v.mX; mY -= v.mY; mZ -= v.mZ; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Vec3 is assigned Vec3 subtracted by <v>.
	 *
	 ------------------------------------------------------*/

	void operator /=(vec_t s) { *this *= (1.0f / s); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Vec3 is scaled by 1/<s>.
	 *
	 ------------------------------------------------------*/

	void operator *=(vec_t s) { mX *= s; mY *= s; mZ *= s; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Vec3 is scaled by <s>.
	 *
	 ------------------------------------------------------*/

	union {          /* Vector data */
		vec3_t mVec;

		struct {
			vec_t mX, mY, mZ;
		};
	};


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

};

} // namespace hel

#endif // GUARD__HEL_VEC3_H_

/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Hel
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : Vector3d
 * License : No use w/o permission (C) 2002-2007 Mongoose
 * Comments: Quaternion now in C++ class form fresh from the grove.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongooseichiban@gmail.com>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNITTEST__HEL_QUAT - Builds Quat class as unit test.
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.04.01:
 * Mongoose - Major API changes, optimzations, and style updates
 *
 * 2002.12.16:
 * Mongoose - Created, based on mtk3d ( freyja )
 ==========================================================================*/

#ifndef GUARD__HEL_QUAT_H_
#define GUARD__HEL_QUAT_H_

#include <string.h> // memcpy()
#include <math.h>

#include "hel/math.h"
#include "hel/Vec3.h"

namespace hel {

class Mat44;

class Quat
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Quat() : mW(1.0f), mX(0.0f), mY(0.0f), mZ(0.0f) { }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Quat.
	 *
	 ------------------------------------------------------*/

	Quat(vec_t w, vec_t x, vec_t y, vec_t z) :
		mW(w), mX(x), mY(y), mZ(z) { }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Quat.
	 *
	 ------------------------------------------------------*/

	Quat(const vec4_t wxyz) { memcpy(mVec, wxyz, sizeof(vec4_t)); }
	/*------------------------------------------------------
	 * Pre  : v { w, x, y, z }
	 * Post : Constructs an object of Quaternion
	 *
	 ------------------------------------------------------*/

	~Quat() { }
	/*------------------------------------------------------
	 * Pre  : Quaternion object is allocated
	 * Post : Deconstructs an object of Quaternion
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	void GetAxisAngles(vec4_t axyz);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Axis angles are returned in *radians*.
	 *
	 ------------------------------------------------------*/

	void GetEulerAngles(vec3_t phr);
	void GetEulerAngles(vec_t &pitch, vec_t &heading, vec_t &roll);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Euler angles are returned in *radians*.
	 *
	 ------------------------------------------------------*/

	void GetMatrix(matrix_t m) const;
	/*------------------------------------------------------
	 * Pre  : Matrix is valid
	 * Post : Returns col order matrix equiv of this quaternion
	 *
	 ------------------------------------------------------*/

	void GetQuat(vec4_t wxyz) { memcpy(wxyz, mVec, sizeof(vec4_t)); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	Quat &operator =(const Quat &q) 
	{ memcpy(mVec, q.mVec, sizeof(vec4_t)); return *this; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Assigns the value of <q> to this quaternion.
	 *
	 ------------------------------------------------------*/

	Quat operator *(const Quat &q) 
	{
		return Quat(mW * q.mW - mX * q.mX - mY * q.mY - mZ * q.mZ,
						mW * q.mX + mX * q.mW + mY * q.mZ - mZ * q.mY,
						mW * q.mY + mY * q.mW + mZ * q.mX - mX * q.mZ,
						mW * q.mZ + mZ * q.mW + mX * q.mY - mY * q.mX);
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns product of Quat and <q>.
	 *        Use Norm() on result for unit quaternion.
	 *
	 ------------------------------------------------------*/

	Quat operator /(const Quat &q) { return (*this * (q.GetInverse())); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns quotient of Quat by <q>.
	 *
	 ------------------------------------------------------*/

	Quat operator +(const Quat &q) 
	{ return Quat(mW+q.mW, mX+q.mX, mY+q.mY, mZ+q.mZ); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns sum of Quat by <q>.
	 *
	 ------------------------------------------------------*/

	Quat operator -(const Quat &q)
	{ return Quat(mW-q.mW, mX-q.mX, mY-q.mY, mZ-q.mZ); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Subtracts Q from this quaternion
	 *        returns resultant quaternion
	 *
	 ------------------------------------------------------*/

	bool operator ==(const Quat &q) const
	{ return (mX == q.mX && mY == q.mY && mZ == q.mZ && mW == q.mW); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Comparison of <q> to this quaternion.
	 *
	 ------------------------------------------------------*/

	Quat GetConjugate() const { return Quat(mW, -mX, -mY, -mZ); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns conjugate of this quaternion
	 *
	 ------------------------------------------------------*/

	Vec3 Rotate(const Vec3 &v) 
	{
		Quat q = (*this) * Quat(0.0f, v.mX, v.mY, v.mZ) * GetConjugate();
		return Vec3(q.mX, q.mY, q.mZ);
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Rotate
	 *
	 ------------------------------------------------------*/

	Quat Scale(vec_t s) { return Quat(mW * s, mX * s, mY * s, mZ * s); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns scaled quaternion.
	 *
	 ------------------------------------------------------*/

	Quat GetInverse() const { return GetConjugate().Scale(1.0f / Magnitude()); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns inverse of quaternion.
	 *
	 ------------------------------------------------------*/

	static vec_t Dot(const Quat &a, const Quat &b)
	{ return ((a.mW * b.mW) + (a.mX * b.mX) + (a.mY * b.mY) + (a.mZ * b.mZ)); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns dot product of <a> and <b>.
	 *
	 ------------------------------------------------------*/

	vec_t Magnitude() const { return sqrtf(Dot(*this, *this)); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns magnitude of quaternion.
	 *
	 ------------------------------------------------------*/

	static Quat Slerp(const Quat &a, const Quat &b, const vec_t time);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Interpolates between A and B rotations and
	 *        returns resultant quaternion using
	 *        spherical linear interpolation:
	 *
	 *        I = (((B . A)^ -1)^ Time) A
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void Conjugate() { mX = -mX; mY = -mY; mZ = -mZ; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns conjugate of this quaternion
	 *
	 ------------------------------------------------------*/

	void SetIdentity() {	mW = 1.0f; mX = mY = mZ = 0.0f; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets this quaternion to identity
	 *
	 ------------------------------------------------------*/

	void SetByEulerAngles(const vec3_t phr);
	void SetByEulerAngles(vec_t pitch, vec_t heading, vec_t roll);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets this quaternion by euler angles in *radians*
	 *
	 ------------------------------------------------------*/

	void SetByAxisAngles(vec_t angle, vec_t x, vec_t y, vec_t z);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets this quaternion
	 *
	 ------------------------------------------------------*/

	void Norm()
	{
		const vec_t square = mX * mX + mY * mY + mZ * mZ + mW * mW;
		const vec_t dist = (square > 0.0f) ? (1.0f / sqrtf(square)) : 1.0f;
		Scale(dist);
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Normalize this quaternion.
	 *
	 ------------------------------------------------------*/

	void SetByMatrix(matrix_t m);
	/*------------------------------------------------------
	 * Pre  : Matrix is valid column order
	 * Post : Sets matrix equiv of this quaternion.
	 *
	 ------------------------------------------------------*/

	union {                 /* Quaternion data */
		vec4_t mVec;

		struct {
			vec_t mW, mX, mY, mZ;
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


////////////////////////////////////////////////////////////
// Unit Test
////////////////////////////////////////////////////////////

#ifdef UNITTEST__HEL_QUAT
#include <mstl/UnitTest.h>

class QuatUnitTest : public UnitTest
{
public:
	QuatUnitTest() {}

	~QuatUnitTest() {}

	int Run(mstl::String &s);
};
#endif // UNIT_TEST_HEL_QUAT

} // namespace hel

#endif // GUARD__HEL_QUAT_H_

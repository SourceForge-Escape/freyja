/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : Matrix
 * License : LGPL, (C) 2002-2007 Mongoose
 * Comments: Matrix 2x2 class
 *
 * 
 *-- History ------------------------------------------------ 
 *
 * 2007.04.01:
 * Mongoose - Created, mainly a utiliy class for Mat44
 ==========================================================================*/

#ifndef GUARD__HEL_MAT22_H_
#define GUARD__HEL_MAT22_H_

#include "hel/math.h"
#include "hel/Quaternion.h"
#include "hel/Vector3d.h"

#include <mstl/String.h>


namespace hel {

class Mat22
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Mat22() { }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Matrix
	 *
	 ------------------------------------------------------*/

	Mat22(const vec2_t a, const vec2_t b) 
	{ 
		memcpy(mA, a, sizeof(vec2_t)); 
		memcpy(mB, b, sizeof(vec2_t)); 
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Matrix
	 *
	 ------------------------------------------------------*/

	~Mat22() { }
	/*------------------------------------------------------
	 * Pre  : Matrix object is allocated
	 * Post : Deconstructs an object of Matrix
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	vec_t Det()
	{ return mA[0] * mB[1] - mA[1] * mB[0]; }
	/*------------------------------------------------------
	 * Pre  : Matrix object is allocated
	 * Post : Computes Determinate of this 2x2 matrix.
	 *
	 ------------------------------------------------------*/

	static vec_t Det(const vec2_t a, const vec2_t b)
	{ return a[0] * b[1] - a[1] * b[0];	}
	/*------------------------------------------------------
	 * Pre  : Given a 2x2 matrix with columns [ <a> <b> ]
	 * Post : Computes Determinate of 2x2 matrix.
	 *
	 ------------------------------------------------------*/

	static void Invert(vec2_t a, vec2_t b)
	{ 
		const vec_t detInv = 1.0f / Det(a, b);
		a[1] *= -detInv;  b[0] *= -detInv;
		const vec_t tmp = detInv * a[0];  a[0] = detInv * b[1];  b[1] = tmp;
	}
	/*------------------------------------------------------
	 * Pre  : Given a 2x2 matrix with columns [ <a> <b> ]
	 * Post : Computes Determinate of 2x2 matrix.
	 *
	 *        m^-1 = (1/det(m)) [ m11 -m01 -m10 m00 ]
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	Mat22 &operator =(const Mat22 &m)
	{
		memcpy(mA, m.mA, sizeof(vec2_t)); 
		memcpy(mB, m.mB, sizeof(vec2_t)); 
		return *this;
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post :
	 *
	 ------------------------------------------------------*/

	Mat22 operator +(const Mat22 &m)
	{
		Mat22 n(*this);
		n.mA[0] += m.mA[0];		n.mB[0] += m.mB[0];
		n.mA[1] += m.mA[1];		n.mB[1] += m.mB[1]; 
		return n;
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post :
	 *
	 ------------------------------------------------------*/

	Mat22 operator -(const Mat22 &m)
	{
		Mat22 n(*this);
		n.mA[0] -= m.mA[0]; 	n.mB[0] -= m.mB[0];
		n.mA[1] -= m.mA[1];		n.mB[1] -= m.mB[1]; 
		return n;
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post :
	 *
	 ------------------------------------------------------*/

	Mat22 operator *(const Mat22 &m)
	{
		Mat22 n;
		n.mA[0] = mA[0] * m.mA[0] + mB[0] * m.mA[1]; 
		n.mB[0] = mA[0] * m.mB[0] + mB[0] * m.mB[1];
		n.mA[1] = mA[1] * m.mA[0] + mB[1] * m.mA[1];  
		n.mB[1] = mA[1] * m.mB[0] + mB[1] * m.mB[1];  
		return n;
	}
	/*------------------------------------------------------
	 * Pre  : Multiplies m and this matrices
	 * Post : Returns resultant matrix
	 *
	 ------------------------------------------------------*/

	Mat22 operator -()
	{
		Mat22 m(*this);
		const vec_t v = -1.0f;
		m.mA[0] *= v; m.mA[1] *= v;  m.mB[0] *= v; m.mB[1] *= v;
		return m;
	}	

	Mat22 operator *(vec_t v)
	{
		Mat22 m(*this);
		m.mA[0] *= v; m.mA[1] *= v;  m.mB[0] *= v; m.mB[1] *= v;
		return m;
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void Invert()
	{
		const vec_t detInv = 1.0f / Det();
		mA[1] *= -detInv;  mB[0] *= -detInv;
		const vec_t tmp = detInv * mA[0];  mA[0] = detInv * mB[1]; mB[1] = tmp;
	}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Computes invert of 2x2 matrix.
	 *
	 ------------------------------------------------------*/

	vec2_t mA;
	vec2_t mB;

 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

};

} // End namespace hel


#endif // GUARD__HEL_MAT22_H_

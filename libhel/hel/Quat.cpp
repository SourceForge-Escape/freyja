/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Hel
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Quaternion
 * License : No use w/o permission (C) 2002 Mongoose
 * Comments: Quaternion now in C++ class form fresh from the grove
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2002.12.16:
 * Mongoose - Created, based on mtk3d ( freyja )
 =================================================================*/

#include <math.h>

#include "Quat.h"

using namespace hel;

////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

void Quat::GetAxisAngles(vec4_t axyz)
{
	const vec_t theta = acosf(mW) * 2.0;
	const vec_t invScale = 1.0f / sinf(theta / 2.0f);
	axyz[0] = theta;
	axyz[1] = mX * invScale;
	axyz[2] = mY * invScale;
	axyz[3] = mZ * invScale;
}


void Quat::GetEulerAngles(vec3_t xyz)
{
	GetEulerAngles(xyz[0], xyz[1], xyz[2]);
}


void Quat::GetEulerAngles(vec_t &alpha, vec_t &beta, vec_t &gamma)
{
	const vec_t sqx = mX * mX;
	const vec_t sqy = mY * mY;
	const vec_t sqz = mZ * mZ;
	const vec_t sqw = mW * mW;

#if 1
	// From my old mtk3d math lib ( clamps (+-)PI )
	alpha = atan2f(2.0 * (mX*mY + mZ*mW), (sqx - sqy - sqz + sqw));
	beta = atan2f(2.0 * (mY*mZ + mX*mW), (-sqx - sqy + sqz + sqw));
	gamma = asinf(-2.0 * (mX*mZ - mY*mW));
#else
	// Much debated http://www.euclideanspace.com routine

	// If normalised, unit is 1.0f, otherwise is correction factor
	vec_t unit = sqx + sqy + sqz + sqw;
	vec_t test = mX*mY + mZ*mW;

	if (test > 0.499f * unit) { // singularity at north pole
		alpha = 0.0f;
		beta = 2.0f * atan2f(mX, mW);
		gamma = HEL_PI_OVER_2;
		return;
	}
	if (test < -0.499f * unit) { // singularity at south pole
		alpha = 0.0f;
		beta = -2.0f * atan2f(mX, mW);
		gamma = -HEL_PI_OVER_2;
		return;
	}

	alpha = atan2f(2.0f * mX * mW - 2.0f * mY * mZ, -sqx + sqy - sqz + sqw);
	beta = atan2f(2.0f * mY * mW - 2.0f * mX* mZ, sqx - sqy - sqz + sqw);
	gamma = asinf(2.0f * test / unit);
#endif
}


void Quat::GetEulerAnglesPHR(vec_t &pitch, vec_t &heading, vec_t &roll)
{
	const vec_t sqx = mX * mX;
	const vec_t sqy = mY * mY;
	const vec_t sqz = mZ * mZ;
	const vec_t sqw = mW * mW;

#if 1
	// From my old mtk3d math lib ( clamps (+-)PI )
	pitch = asinf(-2.0 * (mX*mZ - mY*mW));
	heading = atan2f(2.0 * (mX*mY + mZ*mW), (sqx - sqy - sqz + sqw));
	roll = atan2f(2.0 * (mY*mZ + mX*mW), (-sqx - sqy + sqz + sqw));
#else
	// Much debated http://www.euclideanspace.com routine

	// If normalised, unit is 1.0f, otherwise is correction factor
	vec_t unit = sqx + sqy + sqz + sqw;
	vec_t test = mX*mY + mZ*mW;

	if (test > 0.499f * unit) { // singularity at north pole
		pitch = 2.0f * atan2f(mX, mW);
		heading = HEL_PI_OVER_2;
		roll = 0.0f;
		return;
	}
	if (test < -0.499f * unit) { // singularity at south pole
		pitch = -2.0f * atan2f(mX, mW);
		heading = -HEL_PI_OVER_2;
		roll = 0.0f;
		return;
	}

	pitch = atan2f(2.0f * mY * mW - 2.0f * mX* mZ, sqx - sqy - sqz + sqw);
	heading = asinf(2.0f * test / unit);
	roll = atan2f(2.0f * mX * mW - 2.0f * mY * mZ, -sqx + sqy - sqz + sqw);
#endif
}


void Quat::GetMatrix(matrix_t m) const
{
	m[ 0] = 1.0f - 2.0f * (mY*mY + mZ*mZ);
	m[ 1] = 2.0f * (mX*mY - mW*mZ);
	m[ 2] = 2.0f * (mX*mZ + mW*mY);
	m[ 3] = 0.0f;
	
	m[ 4] = 2.0f * (mX*mY + mW*mZ);
	m[ 5] = 1.0f - 2.0f * (mX*mX + mZ*mZ);
	m[ 6] = 2.0f * (mY*mZ - mW*mX);
	m[ 7] = 0.0f;
	
	m[ 8] = 2.0f * (mX*mZ - mW*mY);
	m[ 9] = 2.0f * (mY*mZ + mW*mX);
	m[10] = 1.0 - 2.0f * (mX*mX + mY*mY);
	m[11] = 0.0f;
	
	m[12] = 0.0f;
	m[13] = 0.0f;
	m[14] = 0.0f;
	m[15] = 1.0f;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void Quat::SetByAxisAngles(vec_t angle, vec_t x, vec_t y, vec_t z)
{
	// Normalize
	const vec_t dist = (1.0f / sqrtf(x*x + y*y + z*z));
	mX = x * dist;
	mY = y * dist;
	mZ = z * dist;	
	mW = cosf(angle / 2.0f);	
}


void Quat::SetByEulerAngles(const vec3_t abg)
{
	// P H R <- R P Y
	vec_t cr, sr;
	helSinCosf(abg[1] * 0.5f, &sr, &cr); //0 
	vec_t cp, sp;
	helSinCosf(abg[2] * 0.5f, &sp, &cp); //1
	vec_t cy, sy;
	helSinCosf(abg[0] * 0.5f, &sy, &cy); //2

	const vec_t cpcy = cp * cy;
	const vec_t spsy = sp * sy;

	mW = cr * cpcy + sr * spsy;
	mX = sr * cpcy - cr *spsy;
	mY = cr * sp * cy + sr * cp * sy;
	mZ = cr * cp * sy - sr * sp * cy;

	//Norm();
}


void Quat::SetByEulerAngles(vec_t alpha, vec_t beta, vec_t gamma)
{
	vec3_t abg = { alpha, beta, gamma };
	SetByEulerAngles(abg);
}


void Quat::SetByEulerAnglesPHR(vec_t pitch, vec_t heading, vec_t roll)
{
	vec_t cp, sp;
	helSinCosf(pitch * 0.5f, &sp, &cp);
	vec_t cy, sy;
	helSinCosf(heading * 0.5f, &sy, &cy);
	vec_t cr, sr;
	helSinCosf(roll * 0.5f, &sr, &cr);

	const vec_t cpcy = cp * cy;
	const vec_t spsy = sp * sy;

	mW = cr * cpcy + sr * spsy;
	mX = sr * cpcy - cr *spsy;
	mY = cr * sp * cy + sr * cp * sy;
	mZ = cr * cp * sy - sr * sp * cy;

	//Norm();
}


Quat Quat::Slerp(const Quat &a, const Quat &b, const vec_t time)
{
	/*******************************************************************
	 * Spherical Linear Interpolation algorthim
	 *-----------------------------------------------------------------
	 *
	 * Interpolate between A and B rotations ( Find qI )
	 *
	 * qI = (((qB . qA)^ -1)^ Time) qA
	 *
	 * http://www.magic-software.com/Documentation/quat.pdf
	 *
	 * Thanks to digiben for algorithms and basis of the notes in 
	 * this func
	 *
	 *******************************************************************/

	// Don't bother if it's the same rotation, it's the same as the result
	if (a == b) 
	{
		return a;
	}

	// A . B 
	vec_t result = Dot(a, b);
  
	Quat b2(b); // b is const, so we need a variable b in one case.

	// If the dot product is less than 0, the angle is greater than 90 degrees
	if (result < 0.0f)
	{
		// Negate quaternion B and the result of the dot product
		b2 = Quat(-b.mW, -b.mX, -b.mY, -b.mZ);
		result = -result;
	}

	// Set the first and second scale for the interpolation
	vec_t scaleA = 1 - time;
	vec_t scaleB = time;

	// Next, we want to actually calculate the spherical interpolation.  Since this
	// calculation is quite computationally expensive, we want to only perform it
	// if the angle between the 2 quaternions is large enough to warrant it.  If the
	// angle is fairly small, we can actually just do a simpler linear interpolation
	// of the 2 quaternions, and skip all the complex math.  We create a "delta" value
	// of 0.1 to say that if the cosine of the angle (result of the dot product) between
	// the 2 quaternions is smaller than 0.1, then we do NOT want to perform the full on 
	// interpolation using.  This is because you won't really notice the difference.
	
	// Check if the angle between the 2 quaternions was big enough 
	// to warrant such calculations
	if (1 - result > 0.1f)
	{
		// Get the angle between the 2 quaternions, and then
		// store the sin() of that angle
		vec_t theta = acosf(result);
		vec_t sinTheta = sinf(theta);

		// Calculate the scale for qA and qB, according to
		// the angle and it's sine value
		scaleA = sinf((1 - time) * theta) / sinTheta;
		scaleB = sinf((time * theta)) / sinTheta;
	}	

	Quat a2(a); // a is const, so we need a var a in all cases by this point.

	// Calculate the x, y, z and w values for the quaternion by using a special
	// form of linear interpolation for quaternions.
	return (a2.Scale(scaleA) + b2.Scale(scaleB));
}


void Quat::SetByMatrix(matrix_t matrix)
{   
	const vec_t diagonal = matrix[0] + matrix[5] + matrix[10] + 1.0f;

	if (diagonal > 1.0e-8f) // 7? Should farm this out to mstl::System? 
	{
		// Calculate the scale of the diagonal
		vec_t scale = sqrtf(diagonal) * 2.0f;
		
		mW = 0.25f * scale;
		scale = 1.0f / scale;
		mX = (matrix[9] - matrix[6]) * scale;
		mY = (matrix[2] - matrix[8]) * scale;
		mZ = (matrix[4] - matrix[1]) * scale;
	}
	else 
	{
		// If the first element of the diagonal is the greatest value
		if (matrix[0] > matrix[5] && matrix[0] > matrix[10])
		{   
			 // Find the scale according to the first element, and double it
			vec_t scale = sqrtf(1.0f + matrix[0] - matrix[5] - matrix[10]) * 2.0f;
 
			// Calculate the quaternion
			mX = 0.25f * scale;
			scale = 1.0f / scale;
			mW = (matrix[9] - matrix[6]) * scale; 
			mY = (matrix[4] + matrix[1]) * scale;
			mZ = (matrix[2] + matrix[8]) * scale;
		} 
		// The second element of the diagonal is the greatest value
		else if (matrix[5] > matrix[10]) 
		{
			// Find the scale according to the second element, and double it
			vec_t scale = sqrtf(1.0f + matrix[5] - matrix[0] - matrix[10]) * 2.0f;
			
			// Calculate the quaternion 
			mY = 0.25f * scale;
			scale = 1.0f / scale;
			mW = (matrix[2] - matrix[8]) * scale;
			mX = (matrix[4] + matrix[1]) * scale;
			mZ = (matrix[9] + matrix[6]) * scale;
		} 
		// The third element of the diagonal is the greatest value
		else 
		{   
			// Find the scale according to the third element, and double it
			vec_t scale = sqrtf(1.0f + matrix[10] - matrix[0] - matrix[5]) * 2.0f;
			
			// Calculate the quaternion
			mZ = 0.25f * scale;
			scale = 1.0f / scale;
			mW = (matrix[4] - matrix[1]) * scale;
			mX = (matrix[2] + matrix[8]) * scale;
			mY = (matrix[9] + matrix[6]) * scale;
		}
	}
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////



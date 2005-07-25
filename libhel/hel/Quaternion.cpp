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

#include "Quaternion.h"


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Quaternion::Quaternion()
{
	setIdentity();
	//mW = 0;
	//mX = 0;
	//mY = 0;
	//mZ = 0;
}


Quaternion::Quaternion(vec_t w, vec_t x, vec_t y, vec_t z)
{
	mW = w;
	mX = x;
	mY = y;
	mZ = z;
}


Quaternion::Quaternion(vec_t pitch, vec_t heading, vec_t roll)
{
	vec_t cr, cp, cy, sr, sp, sy, cpcy, spsy;

	/* Want to use half of the values in equations from here on */
	roll *= 0.5;
	pitch *= 0.5;
	yaw *= 0.5;

	cr = cos(roll);
	cp = cos(pitch);
	cy = cos(heading);

	sr = sin(roll);
	sp = sin(pitch);
	sy = sin(heading);

	cpcy = cp * cy;
	spsy = sp * sy;

	mW = cr * cpcy + sr * spsy;
	mX = sr * cpcy - cr *spsy;
	mY = cr * sp * cy + sr * cp * sy;
	mZ = cr * cp * sy - sr * sp * cy;

	normalize();
}


Quaternion::Quaternion(vec_t roll, vec_t pitch, vec_t yaw)
{
	vec_t cr, cp, cy, sr, sp, sy, cpcy, spsy;

	/* Want to use half of the values in equations from here on */
	roll *= 0.5;
	pitch *= 0.5;
	yaw *= 0.5;

	cr = cos(roll);
	cp = cos(pitch);
	cy = cos(yaw);

	sr = sin(roll);
	sp = sin(pitch);
	sy = sin(yaw);

	cpcy = cp * cy;
	spsy = sp * sy;

	mW = cr * cpcy + sr * spsy;
	mX = sr * cpcy - cr *spsy;
	mY = cr * sp * cy + sr * cp * sy;
	mZ = cr * cp * sy - sr * sp * cy;

	normalize();
}


Quaternion::Quaternion(const vec4_t wxyz)
{
	mW = wxyz[0];
	mX = wxyz[1];
	mY = wxyz[2];
	mZ = wxyz[3];
}


Quaternion::Quaternion(vec4_t wxyz)
{
	mW = wxyz[0];
	mX = wxyz[1];
	mY = wxyz[2];
	mZ = wxyz[3];
}


Quaternion::~Quaternion()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

void Quaternion::getQuaternion4fv(vec4_t wxyz)
{
	wxyz[0] = mW;
	wxyz[1] = mX;
	wxyz[2] = mY;
	wxyz[3] = mZ;
}


void Quaternion::getAxisAngles(vec4_t axyz)
{
	vec_t theta;
	double scale;


	theta = (vec_t)acos(mW) * 2.0;
	scale = sin(theta / 2.0);

	axyz[0] = theta;
	axyz[1] = mX / scale;
	axyz[2] = mY / scale;
	axyz[3] = mZ / scale;
}


void Quaternion::getEulerAngles(vec3_t xyz)
{
	double qw2 = mW*mW;
	double qx2 = mX*mX;
	double qy2 = mY*mY;
	double qz2 = mZ*mZ;

	xyz[2] = atan(2.0 * (mY*mZ+mX*mW)/(-qx2 - qy2 + qz2 + qw2));
	xyz[0] = asin(-2.0 * (mX*mZ-mY*mW));
	xyz[1] = atan(2.0 * (mX*mY+mZ*mW)/(qx2 - qy2 + qz2 + qw2));
}


void Quaternion::getEulerAngles(vec_t *heading, vec_t *bank, vec_t *attitude)
{
	double qw2 = mW*mW;
	double qx2 = mX*mX;
	double qy2 = mY*mY;
	double qz2 = mZ*mZ;

	*heading = atan(2.0 * (mX*mY+mZ*mW)/(qx2 - qy2 + qz2 + qw2));
	*bank = atan(2.0 * (mY*mZ+mX*mW)/(-qx2 - qy2 + qz2 + qw2));
	*attitude = asin(-2.0 * (mX*mZ-mY*mW));
}


void Quaternion::getMatrix(matrix_t m)
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


//Quaternion Quaternion::operator =(const Matrix &mat)
//{
//	setByMatrix(mat.mMatrix);
//	return (*this);
//}


Quaternion Quaternion::operator =(const Quaternion &q)
{
	mW = q.mW;
	mX = q.mX;
	mY = q.mY;
	mZ = q.mZ;
  
  return (*this);
}


Quaternion Quaternion::operator *(const Quaternion &q)
{
	return multiply(*this, q);
}


Quaternion Quaternion::operator /(const Quaternion &q)
{
	return divide(*this, q);
}


Quaternion Quaternion::operator +(const Quaternion &q)
{
	return add(*this, q);
}


Quaternion Quaternion::operator -(const Quaternion &q)
{
	return subtract(*this, q);
}


bool Quaternion::operator ==(const Quaternion &q)
{
	return (mX == q.mX && mY == q.mY && mZ == q.mZ && mW == q.mW);
}


Quaternion Quaternion::conjugate()
{
	return Quaternion(mW, -mX, -mY, -mZ);
}


Vector3d Quaternion::rotate(const Vector3d &v)
{
	Quaternion vec = Quaternion(0, v.mVec[0], v.mVec[1], v.mVec[2]);
	Quaternion q = *this;
	Quaternion qinv = q.conjugate();
	Quaternion vec2 = q * vec * qinv;

	return Vector3d(vec2.mX, vec2.mY, vec2.mZ);
}


Quaternion Quaternion::scale(vec_t s)
{
   return Quaternion(mW * s, mX * s, mY * s, mZ * s);
}


Quaternion Quaternion::inverse()
{
	return conjugate().scale(1/magnitude());
}


vec_t Quaternion::dot(Quaternion a, Quaternion b)
{
	return ((a.mW * b.mW) + (a.mX * b.mX) + (a.mY * b.mY) + (a.mZ * b.mZ));
}


vec_t Quaternion::magnitude()
{
	return sqrt(dot(*this, *this));
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void Quaternion::setIdentity()
{
	mW = 1.0;
	mX = 0.0;
	mY = 0.0;
	mZ = 0.0;
}


void Quaternion::setByEulerAngles(const vec3_t xyz)
{
	vec_t pitch = xyz[0], roll = xyz[2], yaw = xyz[1];
	double cr, cp, cy, sr, sp, sy, cpcy, spsy;

	/* Want to use half of the values in equations from here on */
	roll *= 0.5;
	pitch *= 0.5;
	yaw *= 0.5;

	cr = cos(roll);
	cp = cos(pitch);
	cy = cos(yaw);

	sr = sin(roll);
	sp = sin(pitch);
	sy = sin(yaw);

	cpcy = cp * cy;
	spsy = sp * sy;

	mW = cr * cpcy + sr * spsy;
	mX = sr * cpcy - cr *spsy;
	mY = cr * sp * cy + sr * cp * sy;
	mZ = cr * cp * sy - sr * sp * cy;

	normalize();
}


void Quaternion::setByEulerAngles(vec_t x, vec_t y, vec_t z)
{
	*this = Quaternion(x, y, z);
}


void Quaternion::setByAxisAngles(vec_t angle, vec_t x, vec_t y, vec_t z)
{
	vec_t temp, dist;

	
	// Normalize
	temp = x*x + y*y + z*z;
	
	dist = (float)(1.0 / sqrt(temp));
	
	x *= dist;
	y *= dist;
	z *= dist;
	
	mX = x;
	mY = y;
	mZ = z;
	
	mW = (float)cos(angle / 2.0f);
}


void Quaternion::set(vec_t angle, vec_t x, vec_t y, vec_t z)
{
	vec_t temp, dist;

	
	// Normalize
	temp = x*x + y*y + z*z;
	
	dist = (float)(1.0 / sqrt(temp));
	
	x *= dist;
	y *= dist;
	z *= dist;
	
	mX = x;
	mY = y;
	mZ = z;
	
	mW = (float)cos(angle / 2.0f);	
}


void Quaternion::normalize()
{
	vec_t dist, square;


	square = mX * mX + mY * mY + mZ * mZ + mW * mW;
	
	if (square > 0.0)
	{
		dist = (float)(1.0 / sqrt(square));
	}
	else 
	{
		dist = 1;
	}

	mX *= dist;
	mY *= dist;
	mZ *= dist;
	mW *= dist; 
}


void Quaternion::copy(Quaternion q)
{
	mW = q.mW;
	mX = q.mX;
	mY = q.mY;
	mZ = q.mZ;
}


Quaternion Quaternion::slerp(Quaternion a, Quaternion b, vec_t time)
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

	vec_t result, scaleA, scaleB, theta, sinTheta;
	Quaternion i;


	// Don't bother if it's the same rotation, it's the same as the result
	if (a == b) 
	{
		return a;
	}

	// A . B 
	result = dot(a, b);
  
	// If the dot product is less than 0, the angle is greater than 90 degrees
	if (result < 0.0f)
	{
		// Negate quaternion B and the result of the dot product
		b = Quaternion(-b.mW, -b.mX, -b.mY, -b.mZ);
		result = -result;
	}

	// Set the first and second scale for the interpolation
	scaleA = 1 - time;
	scaleB = time;

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
		theta = (float)acos(result);
		sinTheta = (float)sin(theta);

		// Calculate the scale for qA and qB, according to
		// the angle and it's sine value
		scaleA = (float)sin((1 - time) * theta) / sinTheta;
		scaleB = (float)sin((time * theta)) / sinTheta;
	}	

	// Calculate the x, y, z and w values for the quaternion by using a special
	// form of linear interpolation for quaternions.
	return (a.scale(scaleA) + b.scale(scaleB));
}


void Quaternion::setByMatrix(matrix_t matrix)
{   
	float diagonal = matrix[0] + matrix[5] + matrix[10] + 1.0f;
	float scale = 0.0f;
	float w = 0.0f, x = 0.0f, y = 0.0f, z = 0.0f;
	
	
	if (diagonal > 0.00000001)
	{
		// Calculate the scale of the diagonal
		scale = (float)(sqrt(diagonal) * 2);
		
		w = 0.25f * scale;
		x = (matrix[9] - matrix[6]) / scale;
		y = (matrix[2] - matrix[8]) / scale;
		z = (matrix[4] - matrix[1]) / scale;
	}
	else 
	{
		 // If the first element of the diagonal is the greatest value
		if (matrix[0] > matrix[5] && matrix[0] > matrix[10])
		{   
			 // Find the scale according to the first element, and double it
			scale = (float)sqrt(1.0f + matrix[0] - matrix[5] - matrix[10])*2.0f;
			
			// Calculate the quaternion
			w = (matrix[9] - matrix[6]) / scale; 
			x = 0.25f * scale;
			y = (matrix[4] + matrix[1]) / scale;
			z = (matrix[2] + matrix[8]) / scale;
		} 
		// The second element of the diagonal is the greatest value
		else if (matrix[5] > matrix[10]) 
		{
			// Find the scale according to the second element, and double it
			scale = (float)sqrt(1.0f + matrix[5] - matrix[0] - matrix[10])*2.0f;
			
			// Calculate the quaternion 
			w = (matrix[2] - matrix[8]) / scale;
			x = (matrix[4] + matrix[1]) / scale;
			y = 0.25f * scale;
			z = (matrix[9] + matrix[6]) / scale;
		} 
		// The third element of the diagonal is the greatest value
		else 
		{   
			// Find the scale according to the third element, and double it
			scale = (float)sqrt(1.0f + matrix[10] - matrix[0] - matrix[5])*2.0f;
			
			// Calculate the quaternion
			w = (matrix[4] - matrix[1]) / scale;
			x = (matrix[2] + matrix[8]) / scale;
			y = (matrix[9] + matrix[6]) / scale;
			z = 0.25f * scale;
		}
	}

	mW = w;
	mX = x;
	mY = y;
	mZ = z;
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////

Quaternion Quaternion::multiply(Quaternion a, Quaternion b)
{
	return Quaternion(a.mW * b.mW - a.mX * b.mX - a.mY * b.mY - a.mZ * b.mZ,
							a.mW * b.mX + a.mX * b.mW + a.mY * b.mZ - a.mZ * b.mY,
							a.mW * b.mY + a.mY * b.mW + a.mZ * b.mX - a.mX * b.mZ,
							a.mW * b.mZ + a.mZ * b.mW + a.mX * b.mY - a.mY * b.mX);
#ifdef SLEEPYME
	vec_t a, b, c, d, e, f, g, h;


	a = (a.mW + a.mX) * (b.mW + b.mX);
	b = (a.mZ - a.mY) * (b.mY - b.mZ);
#endif
}


Quaternion Quaternion::divide(Quaternion a, Quaternion b)
{
	return (a * (b.inverse()));
}


Quaternion Quaternion::add(Quaternion a, Quaternion b)
{
	return Quaternion(a.mW + b.mW,
							a.mX + b.mX,
							a.mY + b.mY,
							a.mZ + b.mZ);
}


Quaternion Quaternion::subtract(Quaternion a, Quaternion b)
{
	return Quaternion(a.mW - b.mW,
							a.mX - b.mX,
							a.mY - b.mY,
							a.mZ - b.mZ);
}


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_QUATERNION
int runQuaternionUnitTest(int argc, char *argv[])
{
	return 0;
}


int main(int argc, char *argv[])
{
	printf("[Quaternion class test]\n");

	runQuaternionUnitTest(argc, argv);

	return 0;
}
#endif

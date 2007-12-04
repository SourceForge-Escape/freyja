/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Camera
 * License : No use w/o permission (C) 2001 Mongoose
 * Comments: OpenGL camera class for Freyja
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.10.22:
 * Mongoose - Taken from old Hel and moved to Freyja
 *            API clean up
 *
 * 2002.12.16:
 * Mongoose - Removed perspective setting and OpenGL dependency
 *            API changes to reflect new direction of this object:
 *              Removing outdated algorithms and code 
 *              And refactoring the class in general
 *
 * 2001.06.06:
 * Mongoose - Moving GLU code into here to setup break up
 *            into Camera base class, DynamicCamera,
 *            and GLUCamera child classes
 *
 * 2001.06.04:
 * Mongoose - Quaternion based compile option
 *
 * 2001.05.18:
 * Mongoose - Created
 ==========================================================================*/

#include <math.h>
#include "Camera.h"

#if OBSOLETE

////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

FreyjaCamera::FreyjaCamera() :
	mId(++mCounter),
	mMode(eStationary),
	mPath(),
	mQ(),
	mViewDistance(14.0f),
	mTheta(0.0f),
	mTheta2(0.0f),
	mSpeed(0),
	mUpdate(false)
{
	reset();
}


FreyjaCamera::~FreyjaCamera()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void FreyjaCamera::rotate(float angle, float x, float y, float z)
{
	hel::Quat t, n;
	hel::Mat44 mat;
	vec4_t side = { 1, 0,  0, 1 };
	vec4_t up =   { 0, 1,  0, 1 };
	vec4_t look = { 0, 0, -1, 1 };
 	unsigned int i;
	matrix_t m;
   
	t.SetByAxisAngles(angle, x, y, z);
	n = mQ * t;
	n.Norm();

	n.GetMatrix(m);
	mat.SetMatrix(m);
	mat.Multiply4fv(side, mSide);
	mat.Multiply4fv(look, mTarget);
	mat.Multiply4fv(up, mUp);

	for (i = 0; i < 3; ++i)
	{
		mSide[i] += mPos[i];
		mTarget[i] += mPos[i];
		mUp[i] += mPos[i];
	}
    
	mQ = n;
}


void FreyjaCamera::translate(float x, float y, float z)
{
	vec4_t v = {x, y, -z, 1};
	hel::Mat44 m;

	m.mMatrix[0] = mSide[0] - mPos[0];
	m.mMatrix[1] = mUp[0] - mPos[0];
	m.mMatrix[2] = mTarget[0] - mPos[0];
	m.mMatrix[3] = 0;
	m.mMatrix[4] = mSide[1] - mPos[1];
	m.mMatrix[5] = mUp[1] - mPos[1];
	m.mMatrix[6] = mTarget[1] - mPos[1];
	m.mMatrix[7] = 0;
	m.mMatrix[8] = mSide[2] - mPos[2];
	m.mMatrix[9] = mUp[2] - mPos[2];
	m.mMatrix[10] = mTarget[2] - mPos[2];
	m.mMatrix[11] = 0;
	m.mMatrix[12] = 0;
	m.mMatrix[13] = 0;
	m.mMatrix[14] = 0;
	m.mMatrix[15] = 1;

	vec4_t result;
	m.Multiply4fv(v, result);

	for (unsigned int i = 0; i < 3; ++i)
	{
		mSide[i] += result[i];
		mUp[i] += result[i];
		mTarget[i] += result[i];
		mPos[i] += result[i];
	}

	mPos[0] = x;
	mPos[1] = y;
	mPos[2] = z;	
}


void FreyjaCamera::reset()
{
	mTheta = 0.0;
	mTheta2 = 0.0;

	mPos[0] = 0.0;
	mPos[1] = 0.0;
	mPos[2] = 0.0;

	mTarget[0] = 0.0;
	mTarget[1] = 0.0;
	mTarget[2] = mViewDistance;

	mSide[0] = 1.0;
	mSide[1] = 0.0;
	mSide[2] = 0.0;

	mUp[0] = 0.0;
	mUp[1] = 1.0;
	mUp[2] = 0.0;

	mQ.SetIdentity();
	translate(0.0, 0.0, 0.0);
}


// FIXME: Mostly invalid for new QUAT_CAM (can rotate on XYZ)
bool FreyjaCamera::isBehind(int x, int z)
{
  // Set up a "virtual camera" a huge distance behind us
  vec_t bTheta = mTheta + HEL_PI;

  if (bTheta > HEL_PI)
    bTheta -= HEL_2_PI;

  // 64k is a fair distance away...
  vec_t sinT, cosT;
  helSinCosf(bTheta, &sinT, &cosT);

  vec_t bCameraX = -( (65536.0 * sinT) + mPos[0] );
  vec_t bCameraZ = -( (65536.0 * cosT) + mPos[2] );
  vec_t Distance = sqrt((bCameraX * bCameraX) + (bCameraZ * bCameraZ));

  return (Distance < 65536.0);
}




////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////

#endif // OBSOLETE

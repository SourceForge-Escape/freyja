/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : hel
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : ViewVolume
 * License : No use w/o permission (C) 2002 Mongoose
 * Comments: This is the viewing volume for culling use
 *
 *           Thanks Mark Morley for the article I used
 *           to get several algorithms.
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2002.12.15:
 * Mongoose - Created
 =================================================================*/

#include <math.h>
#include "ViewVolume.h"


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

ViewVolume::ViewVolume()
{
}


ViewVolume::~ViewVolume()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

bool ViewVolume::isBoundingVolumeInFrustum(BoundingVolume bvol)
{
	return (isBoundingSphereInFrustum(bvol.mSphere) &&
			  isBoundingBoxInFrustum(bvol.mBox));
}


bool ViewVolume::isBoundingSphereInFrustum(BoundingSphere bvol)
{
	return (isSphereInFrustum(bvol.mCenter[0],
									  bvol.mCenter[1],
									  bvol.mCenter[2],
									  bvol.mRadius));
}


bool ViewVolume::isBoundingBoxInFrustum(BoundingBox bvol)
{
	return (isBboxInFrustum(bvol.mMin, bvol.mMax));
}


bool ViewVolume::isPointInFrustum(vec_t x, vec_t y, vec_t z)
{
   unsigned int p;


   for (p = 0; p < 6; ++p)
	{
      if (mFrustum[p][0] * x + mFrustum[p][1] * y + mFrustum[p][2] * z + 
			 mFrustum[p][3] <= 0)
		{
         return false;
		}
	}

   return true;
}


bool ViewVolume::isSphereInFrustum(vec_t x, vec_t y, vec_t z, vec_t radius)
{
   unsigned int p;
   vec_t d;


   for (p = 0; p < 6; ++p)
   {
      d = mFrustum[p][0] * x + mFrustum[p][1] * y + mFrustum[p][2] * z + mFrustum[p][3];
      if (d <= -radius)
         return false;
   }

   return true;
}


bool ViewVolume::isBboxInFrustum(vec3_t min, vec3_t max)
{
   unsigned int p;


   for (p = 0; p < 6; ++p)
   {
      if (mFrustum[p][0] * min[0] + 
			 mFrustum[p][1] * min[1] + 
			 mFrustum[p][2] * min[2] + mFrustum[p][3] > 0)
         continue;

      if (mFrustum[p][0] * max[0] + 
			 mFrustum[p][1] * max[1] + 
			 mFrustum[p][2] * max[2] + mFrustum[p][3] > 0)
         continue;

      if (mFrustum[p][0] * min[0] + 
			 mFrustum[p][1] * max[1] + 
			 mFrustum[p][2] * max[2] + mFrustum[p][3] > 0)
         continue;

      if (mFrustum[p][0] * min[0] + 
			 mFrustum[p][1] * min[1] + 
			 mFrustum[p][2] * max[2] + mFrustum[p][3] > 0)
         continue;

      if (mFrustum[p][0] * min[0] + 
			 mFrustum[p][1] * max[1] + 
			 mFrustum[p][2] * min[2] + mFrustum[p][3] > 0)
         continue;

      if (mFrustum[p][0] * max[0] + 
			 mFrustum[p][1] * min[1] + 
			 mFrustum[p][2] * min[2] + mFrustum[p][3] > 0)
         continue;

      if (mFrustum[p][0] * max[0] + 
			 mFrustum[p][1] * max[1] + 
			 mFrustum[p][2] * min[2] + mFrustum[p][3] > 0)
         continue;

      if (mFrustum[p][0] * max[0] + 
			 mFrustum[p][1] * min[1] + 
			 mFrustum[p][2] * max[2] + mFrustum[p][3] > 0)
         continue;

      return false;
   }

   return true;
}


vec_t ViewVolume::getDistToSphereFromNear(vec_t x, vec_t y, vec_t z, 
														vec_t radius)
{
   unsigned int p;
   vec_t d;

   for (p = 0; p < 6; ++p)
   {
      d = mFrustum[p][0] * x + mFrustum[p][1] * y + mFrustum[p][2] * z + mFrustum[p][3];
      if (d <= -radius)
         return 0;
   }

   return d + radius;
}


vec_t ViewVolume::getDistToBboxFromNear(vec3_t min, vec3_t max)
{
	vec3_t center;
   vec_t d, radius;


	helMidpoint3v(min, max, center);

	// 5 should be near plane
	d = (mFrustum[5][0] * center[0] + 
		  mFrustum[5][1] * center[1] + 
		  mFrustum[5][2] * center[2] + 
		  mFrustum[5][3]);

	radius = helDist3v(max, center);

	if (d <= -radius)
		return 0;

   return d + radius;
}


void ViewVolume::getFrustum(vec_t frustum[6][4])
{
	unsigned int plane;


	for (plane = 0; plane < 6; ++plane)
   {
		frustum[plane][0] = mFrustum[plane][0];
		frustum[plane][1] = mFrustum[plane][1];
		frustum[plane][2] = mFrustum[plane][2];
		frustum[plane][3] = mFrustum[plane][3];
	}
}


void ViewVolume::getPlane(ViewVolumeSide p, vec4_t plane)
{
	plane[0] =  mFrustum[p][0];
	plane[1] =  mFrustum[p][1];
	plane[2] =  mFrustum[p][2];
	plane[3] =  mFrustum[p][3];
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void ViewVolume::updateFrame(matrix_t proj, matrix_t mdl)
{
	setModel(mdl);
	setProjection(proj);
	updateClip();
	updateFrustum();
}


void ViewVolume::updateFrame()
{
	updateClip();
	updateFrustum();
}


void ViewVolume::setModel(matrix_t mdl)
{
	mModel.setMatrix(mdl);
}


void ViewVolume::setProjection(matrix_t proj)
{
	/*void setProjection(viewAngle, aspectRatio, near, far)
    ****************************
	 *float far = 4000.0f;
	 *float near = 4.0f;
	 *float viewAngle = 45.0f;
	 *float aspectRatio = 800.0f/600.0f;
	 ****************************
	 *float top = near * tan(PI/180 * viewAngle/2)
    *float bottom = -top
    *float right = top * aspectRatio
    *float left = - right
	 */

	mProjection.setMatrix(proj);
}



////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////

void ViewVolume::updateClip()
{
	//mClip = mModel * mProjection;
	mClip = mProjection * mModel;
}


void ViewVolume::updateFrustum()
{
	matrix_t clip;
	vec_t t;


	mClip.getMatrix(clip);

   /* Extract the numbers for the RIGHT plane */
   mFrustum[0][0] = clip[ 3] - clip[ 0];
   mFrustum[0][1] = clip[ 7] - clip[ 4];
   mFrustum[0][2] = clip[11] - clip[ 8];
   mFrustum[0][3] = clip[15] - clip[12];

   /* Normalize the result */
   t = sqrt(mFrustum[0][0] * mFrustum[0][0] + 
				mFrustum[0][1] * mFrustum[0][1] + 
				mFrustum[0][2] * mFrustum[0][2]);
   mFrustum[0][0] /= t;
   mFrustum[0][1] /= t;
   mFrustum[0][2] /= t;
   mFrustum[0][3] /= t;

   /* Extract the numbers for the LEFT plane */
   mFrustum[1][0] = clip[ 3] + clip[ 0];
   mFrustum[1][1] = clip[ 7] + clip[ 4];
   mFrustum[1][2] = clip[11] + clip[ 8];
   mFrustum[1][3] = clip[15] + clip[12];

   /* Normalize the result */
   t = sqrt(mFrustum[1][0] * mFrustum[1][0] + 
				mFrustum[1][1] * mFrustum[1][1] + 
				mFrustum[1][2] * mFrustum[1][2]);
   mFrustum[1][0] /= t;
   mFrustum[1][1] /= t;
   mFrustum[1][2] /= t;
   mFrustum[1][3] /= t;

   /* Extract the BOTTOM plane */
   mFrustum[2][0] = clip[ 3] + clip[ 1];
   mFrustum[2][1] = clip[ 7] + clip[ 5];
   mFrustum[2][2] = clip[11] + clip[ 9];
   mFrustum[2][3] = clip[15] + clip[13];

   /* Normalize the result */
   t = sqrt(mFrustum[2][0] * mFrustum[2][0] + 
				mFrustum[2][1] * mFrustum[2][1] + 
				mFrustum[2][2] * mFrustum[2][2]);
   mFrustum[2][0] /= t;
   mFrustum[2][1] /= t;
   mFrustum[2][2] /= t;
   mFrustum[2][3] /= t;

   /* Extract the TOP plane */
   mFrustum[3][0] = clip[ 3] - clip[ 1];
   mFrustum[3][1] = clip[ 7] - clip[ 5];
   mFrustum[3][2] = clip[11] - clip[ 9];
   mFrustum[3][3] = clip[15] - clip[13];

   /* Normalize the result */
   t = sqrt(mFrustum[3][0] * mFrustum[3][0] + 
				mFrustum[3][1] * mFrustum[3][1] + 
				mFrustum[3][2] * mFrustum[3][2]);
   mFrustum[3][0] /= t;
   mFrustum[3][1] /= t;
   mFrustum[3][2] /= t;
   mFrustum[3][3] /= t;

   /* Extract the FAR plane */
   mFrustum[4][0] = clip[ 3] - clip[ 2];
   mFrustum[4][1] = clip[ 7] - clip[ 6];
   mFrustum[4][2] = clip[11] - clip[10];
   mFrustum[4][3] = clip[15] - clip[14];

   /* Normalize the result */
   t = sqrt(mFrustum[4][0] * mFrustum[4][0] + 
				mFrustum[4][1] * mFrustum[4][1] + 
				mFrustum[4][2] * mFrustum[4][2]);
   mFrustum[4][0] /= t;
   mFrustum[4][1] /= t;
   mFrustum[4][2] /= t;
   mFrustum[4][3] /= t;

   /* Extract the NEAR plane */
   mFrustum[5][0] = clip[ 3] + clip[ 2];
   mFrustum[5][1] = clip[ 7] + clip[ 6];
   mFrustum[5][2] = clip[11] + clip[10];
   mFrustum[5][3] = clip[15] + clip[14];

   /* Normalize the result */
   t = sqrt(mFrustum[5][0] * mFrustum[5][0] + 
				mFrustum[5][1] * mFrustum[5][1] + 
				mFrustum[5][2] * mFrustum[5][2]);
   mFrustum[5][0] /= t;
   mFrustum[5][1] /= t;
   mFrustum[5][2] /= t;
   mFrustum[5][3] /= t;
}


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_VIEWVOLUME
int runViewVolumeUnitTest(int argc, char *argv[])
{
	return 0;
}


int main(int argc, char *argv[])
{
	ViewVolume test;


	printf("[ViewVolume class test]\n");

	return runViewVolumeUnitTest(argc, argv);
}
#endif

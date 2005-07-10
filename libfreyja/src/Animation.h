/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : Animation
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the animation class ( holds keyframes )
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_ANIMATION - Builds Animation class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2005.07.08:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_ANIMATION_H_
#define GUARD__FREYJA_MONGOOSE_ANIMATION_H_


#include "KeyFrame.h"

class Animation
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Animation();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Animation
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.07.08: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~Animation();
	/*------------------------------------------------------
	 * Pre  : Animation object is allocated
	 * Post : Deconstructs an object of Animation
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.07.08: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	void setName(const char *name)
	{
		strncpy(mName, name, 64);
		mName[63] = 0;
	}
	
	long mId;

	char mName[64];

	vec_t mFrameRate;

	vec_t mTime;

	long mStartBone;          /* For animation blending (subsets) use */

	long mBoneCount;

	long mCurrentFrame; // render use mostly

	long mLastFrame;    // render use mostly

	vec_t mLastTime;    // render use mostly

	Vector<KeyFrame *> mKeyFrames;  // keyCount / mBoneCount = frames

	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////


	/* */
};

#endif

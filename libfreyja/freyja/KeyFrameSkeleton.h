/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : KeyFrameSkeleton
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: Skeletal keyframe
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_KEYFRAMESKELETON - Builds KeyFrameSkeleton class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2005.07.08:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_KEYFRAMESKELETON_H_
#define GUARD__FREYJA_MONGOOSE_KEYFRAMESKELETON_H_


#include "KeyFrame.h"

class KeyFrameBone : public KeyFrameObject
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	KeyFrameBone();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of KeyFrameSkeleton
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.07.08: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~KeyFrameBone();
	/*------------------------------------------------------
	 * Pre  : KeyFrameSkeleton object is allocated
	 * Post : Deconstructs an object of KeyFrameSkeleton
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.07.08: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	virtual char *GetTypeName() { return "Bone"; }


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	vec_t mTime;

	Vec3 mTranslation;

	Vec3 mScale;

	Quaternion mRotation;


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////
};

#endif

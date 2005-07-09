/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : KeyFrame
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: KeyFrame class
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_KEYFRAME - Builds KeyFrame class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2005.07.08:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_KEYFRAME_H_
#define GUARD__FREYJA_MONGOOSE_KEYFRAME_H_


class KeyFrame
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	KeyFrame();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of KeyFrame
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.07.08: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~KeyFrame();
	/*------------------------------------------------------
	 * Pre  : KeyFrame object is allocated
	 * Post : Deconstructs an object of KeyFrame
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.07.08: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	vec_t mTime;    /* Time to next frame */


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

};

#endif

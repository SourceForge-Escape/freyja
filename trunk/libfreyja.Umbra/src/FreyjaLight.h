/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : FreyjaLight
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This is the light data model class.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_FREYJALIGHT - Builds FreyjaLight class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.10.22:
 * Mongoose - Created, based on Freyja's Light class from 2002.01.27
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_FREYJALIGHT_H_
#define GUARD__FREYJA_MONGOOSE_FREYJALIGHT_H_

#include <hel/math.h>


class FreyjaLight
{
 public:

	typedef enum {
		ePoint = 1,
		eDirectional,
		eSpot

	} light_type_t;


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	FreyjaLight();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of FreyjaLight
	 *
	 *-- History ------------------------------------------
	 *

	 * 2004.10.22: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~FreyjaLight();
	/*------------------------------------------------------
	 * Pre  : FreyjaLight object is allocated
	 * Post : Deconstructs an object of FreyjaLight
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.10.22: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void setPosition(vec4_t xyz)
	{
		mPos[0] = xyz[0];
		mPos[1] = xyz[1];
		mPos[2] = xyz[2];
	}

	//FIXME temp
	vec4_t mAmbient;          /* Ambient color */

	vec4_t mDiffuse;          /* Diffuse color */

	vec4_t mSpecular;         /* Specular color */

	vec4_t mPos;              /* Light position in 3 space */


private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	unsigned int mId;         /* Unique identifier of this light */

	char mName[64];           /* Light name */

	unsigned int mOptions;    /* Option bitflags */

	light_type_t mType;       /* Type of light */


	vec4_t mDir;	          /* Direction for directional or spot light use */

	vec_t mCutOff;            /* Cut off angle for spot light use */

	vec_t mExponent;          /* Exponent for spot light use */

	vec_t mAttenuation;       /* Attenuation factor */

	static unsigned int mCounter;   /* Id generator counter */
};

#endif

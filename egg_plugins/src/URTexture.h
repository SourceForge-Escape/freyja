/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Midgard
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : URTexture
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This is the Unreal texture class
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_URTEXTURE - Builds URTexture class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.03.08:
 * Mongoose - Created
 ================================================================*/


#ifndef GUARD__MIDGARD_MONGOOSE_URTEXTURE_H_
#define GUARD__MIDGARD_MONGOOSE_URTEXTURE_H_


#include "UTPackage.h"


class URTexture
{
 public:

	enum
	{
		Indexed_8bit = 0x00,
		BGRA_24bit   = 0x04,
		BGRA_32bit   = 0x05,
		DXT1         = 0x03,
		DXT3         = 0x07,
		DXT5         = 0x08

	} TextureType;

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	URTexture();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of URTexture
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.08: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~URTexture();
	/*------------------------------------------------------
	 * Pre  : URTexture object is allocated
	 * Post : Deconstructs an object of URTexture
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.08: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	int load(UTPackage &pak, unsigned int index);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.03.08: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


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

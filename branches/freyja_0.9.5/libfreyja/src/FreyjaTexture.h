/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : FreyjaTexture
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the texture class
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_FREYJATEXTURE - Builds FreyjaTexture class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2005.05.14:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_FREYJATEXTURE_H_
#define GUARD__FREYJA_MONGOOSE_FREYJATEXTURE_H_

#include <hel/math.h>

#include "FreyjaPluginABI.h"
#include "FreyjaFileReader.h"
#include "FreyjaFileWriter.h"

class FreyjaTexture
{
 public:

	enum PixelFormat {

		Indexed8,
		RGB24,
		RGBA32
	};


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	FreyjaTexture();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of FreyjaTexture
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.14: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual ~FreyjaTexture();
	/*------------------------------------------------------
	 * Pre  : FreyjaTexture object is allocated
	 * Post : Deconstructs an object of FreyjaTexture
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.05.14: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	uint32 getSerializeSize();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	virtual bool serialize(FreyjaFileWriter &w);
	/*------------------------------------------------------
	 * Pre  : Writes this material out to disk
	 * Post : Returns true on success
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	virtual bool serialize(FreyjaFileReader &r);
	/*------------------------------------------------------
	 * Pre  : Reads the material data from disk
	 * Post : Returns true on success
	 ------------------------------------------------------*/

	void setFilename(const char *filename);
	/*------------------------------------------------------
	 * Pre  : Name is valid string
	 * Post : Sets Texture's name
	 ------------------------------------------------------*/

	void setName(const char *name);
	/*------------------------------------------------------
	 * Pre  : Name is valid string
	 * Post : Sets Texture's name
	 ------------------------------------------------------*/

	char *mName;                /* Texture name */
	
	char *mFilename;            /* Filename of image */

	byte *mImage;

	byte *mPalette;

	uint32 mBitDepth;

	PixelFormat mPixelFormat;

	uint32 mWidth;

	uint32 mHeight;

	index_t mUID;              /* Used by Gobal Pool */

 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

};

#endif

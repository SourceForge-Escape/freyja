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

#ifndef GUARD__FREYJA_TEXTURE_H_
#define GUARD__FREYJA_TEXTURE_H_

#include <hel/math.h>
#include <mstl/String.h>

#include "freyja.h"


namespace freyja {

class Texture
{
 public:

	enum PixelFormat {
		Indexed8,
		RGB24,
		RGBA32
	};

	Texture();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	virtual ~Texture();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Deconstructor.
	 *
	 ------------------------------------------------------*/

	const char* GetFilename() 
	{ return mFilename.c_str(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns filename attribute or NULL.
	 ------------------------------------------------------*/

	void SetFilename( const char* filename )
	{ mFilename = filename; }
	/*------------------------------------------------------
	 * Pre  : Name is valid string.
	 * Post : Sets Texture filename attribute.
	 *
	 ------------------------------------------------------*/

	const char* GetName() 
	{ return mName.c_str(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns name attribute or NULL.
	 ------------------------------------------------------*/

	void SetName( const char* name )
	{ mName = name; }
	/*------------------------------------------------------
	 * Pre  : Name is valid string.
	 * Post : Sets Texture name attribute.
	 *
	 ------------------------------------------------------*/

	byte* mImage;

	byte* mPalette;

	uint32 mBitDepth;

	PixelFormat mPixelFormat;

	uint32 mWidth;

	uint32 mHeight;


 protected:

	Texture(const Texture& texture);

	Texture &operator=(const Texture& texture);

	mstl::String mName;                /* Texture name */
	
	mstl::String mFilename;            /* Filename of image */
};

} // namespace freyja

#endif // GUARD__FREYJA_TEXTURE_H_

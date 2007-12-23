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
		Indexed_8bpp,
		RGB_24bpp,
		RGBA_32bpp
	};

	static freyja::Texture* Create( byte* pixmap,
									uint16 width, uint16 height, 
									PixelFormat format );
	/*------------------------------------------------------
	 * Pre  : Pixmap must be allocated and have deminsion.
	 *
	 * Post : Conditional Constructor returns Texture. 
	 *        Returns NULL on invalid precondition.
	 *
	 ------------------------------------------------------*/

	static freyja::Texture* Create( const char* filename );
	/*------------------------------------------------------
	 * Pre  : filename must be for a valid image file.
	 *
	 * Post : Conditional Constructor returns Texture.
	 *        Returns NULL on invalid precondition.
	 *
	 ------------------------------------------------------*/

	virtual ~Texture();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Deconstructor.
	 *
	 ------------------------------------------------------*/

	const char* GetName() const
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

	uint16 GetBytesPerPixel( Texture::PixelFormat format );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns bytes per pixel given format.
	 *
	 ------------------------------------------------------*/


 protected:

	Texture();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	Texture(const Texture& texture);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	Texture& operator=(const Texture& texture);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Assignment operator.
	 *
	 ------------------------------------------------------*/

	mstl::String mName;                /* Texture name */
	
	mstl::String mFilename;            /* Filename of image */

	PixelFormat mPixelFormat;

	byte* mImage;

	uint16 mWidth;

	uint16 mHeight;
};


inline
uint16 Texture::GetBytesPerPixel( Texture::PixelFormat format )
{ 
	return ( (format == Indexed_8bpp) ? 1 :
			 (format == RGB_24bpp) ? 3 :
			 (format == RGBA_32bpp) ? 4 : 
			 0 );	
}


} // namespace freyja

#endif // GUARD__FREYJA_TEXTURE_H_

/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * License : No use w/o permission (C) 2005-2007 Mongoose
 * Comments: C ABI for the PixelBuffer class.
 *
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.12.12:
 * Mongoose - All new 10.0 ABI combines Texture / Image classes.
 *
 * 2005.05.14:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__FREYJA_PIXELBUFFERABI_H_
#define GUARD__FREYJA_PIXELBUFFERABI_H_

#include "freyja.h"


extern "C" {

	///////////////////////////////////////////////////////////////////////
	// Pixelbuffer 10.0
	//
	///////////////////////////////////////////////////////////////////////

	typedef enum {
		RGBA_32,
		RGB_24,
		INDEXED_8

	} freyja_pixelformat_t;


	freyja_ptr freyjaPixelbufferCreateFilename( const char* filename );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns new PixelBuffer or NULL.
	 *
	 ------------------------------------------------------*/

	freyja_ptr freyjaPixelbufferCreateBuffer( byte *image, freyja_pixelformat_t format, 
											  uint16 width, uint16 height );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns new PixelBuffer or NULL.
	 *
	 ------------------------------------------------------*/

	void freyjaPixelbufferDelete( freyja_ptr pixelbuffer );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Deletes pixelbuffer.
	 *
	 ------------------------------------------------------*/

	freyja_ptr freyjaPixelbufferGetImage( freyja_ptr pixelbuffer );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	uint16 freyjaPixelbufferGetFormat( freyja_ptr pixelbuffer );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	uint16 freyjaPixelbufferGetHeight( freyja_ptr pixelbuffer );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	uint16 freyjaPixelbufferGetWidth( freyja_ptr pixelbuffer );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

} // extern "C"

#endif // GUARD__FREYJA_PIXELBUFFERABI_H_

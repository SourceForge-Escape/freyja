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


class FreyjaTexture
{
 public:

	enum PixelBitDepth {

		Indexed8bit  = 8,
		RGB24bit     = 24,
		RGBA32bit    = 32
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

	~FreyjaTexture();
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



	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void setName(const char *name);
	/*------------------------------------------------------
	 * Pre  : Name is valid string
	 * Post : Sets Material's name
	 ------------------------------------------------------*/

	char *name;                 /* Texture name */
	
	char *filename;             /* Filename of image */
	
	unsigned char *image;       /* RGB(A) Texture data */
	
	unsigned int imageWidth;
	
	unsigned int imageHeight;
	
	unsigned char mipmaps;

	unsigned char pixelDepth;   /* 24 - RGB24bit, 32 - RGBA32bit */
	
	unsigned int id;            /* OpenGL texture id use */


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

};

#endif

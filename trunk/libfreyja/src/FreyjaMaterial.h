/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : FreyjaMaterial
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: Material class for libfreyja, yadda yadda
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_FREYJAMATERIAL - Builds FreyjaMaterial class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2005.01.08:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_FREYJAMATERIAL_H_
#define GUARD__FREYJA_MONGOOSE_FREYJAMATERIAL_H_

#include <hel/math.h>


class FreyjaTexture
{
public:

	FreyjaTexture()
	{
		name = 0x0;
		filename = 0x0;
		image = 0x0;
	}

	~FreyjaTexture()
	{
		if (name)
			delete [] name;

		if (filename)
			delete [] filename;

		if (image)
			delete [] image;
	}

	char *name;                /* Texture name */
	
	char *filename;            /* Filename of image */
	
	unsigned char *image;      /* RGB(A) Texture data */
	
	unsigned int imageWidth;
	
	unsigned int imageHeight;
	
	unsigned char mipmaps;

	unsigned char pixelDepth; /* 3 - RGB24bit, 4 - RGBA32bit */
	
	unsigned int id;          /* OpenGL texture id use */
};


class FreyjaMaterial
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	FreyjaMaterial();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of FreyjaMaterial
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.01.08: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~FreyjaMaterial();
	/*------------------------------------------------------
	 * Pre  : FreyjaMaterial object is allocated
	 * Post : Deconstructs an object of FreyjaMaterial
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.01.08: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	long mId;                   /* Unique identifier */

	char mName[64];             /* Material name */

	long mFlags;                /* Bit flags */

	long mParent;               /* Linked material id, for shader use */

	vec4_t mAmbient;            /* Ambient color */

	vec4_t mDiffuse;            /* Diffuse color */

	vec4_t mSpecular;           /* Specular color */

	vec4_t mEmissive;           /* Emissive color */

	vec_t mShininess;           /* Specular exponent */

	vec_t mTransparency;        /* Alpha 0.0 - 1.0 */

	vec4_t mColor;              /* Solid color */

	unsigned long mBlendSrc;    /* Blend source factor */

	unsigned long mBlendDest;   /* Blend destination factor */

	long mTexture;              /* TextureData index */

	bool mHasAlphaChannel;      /* For depth sorting use */


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

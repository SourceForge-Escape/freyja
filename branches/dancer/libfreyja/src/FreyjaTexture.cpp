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
 *-- History ------------------------------------------------- 
 *
 * 2005.05.14:
 * Mongoose - Created
 ==========================================================================*/

#include "FreyjaTexture.h"


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

FreyjaTexture::FreyjaTexture()
{
	name = 0x0;
	filename = 0x0;
	image = 0x0;
}


FreyjaTexture::~FreyjaTexture()
{
	if (name)
		delete [] name;

	if (filename)
		delete [] filename;

	if (image)
		delete [] image;
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

uint32 FreyjaTexture::getSerializeSize()
{
	// FIXME
	return 0;
}


bool FreyjaTexture::serialize(FreyjaFileWriter &w)
{
	uint32 length;

	if (name)
	{
		length = strlen(name);
		w.writeInt32U(length);
		w.writeCharString(length, name);
	}
	else
	{
		w.writeInt32U(0);
	}

	
	char *filename;             /* Filename of image */
	
	unsigned char *image;       /* RGB(A) Texture data */

	unsigned int imageWidth;
	
	unsigned int imageHeight;
	
	unsigned char mipmaps;

	unsigned char pixelDepth;   /* 24 - RGB24bit, 32 - RGBA32bit */
	
	unsigned int id; 

	return false;
}



////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

bool FreyjaTexture::serialize(FreyjaFileReader &r)
{
	// FIXME
	return false;
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_FREYJATEXTURE
int runFreyjaTextureUnitTest(int argc, char *argv[])
{
	FreyjaTexture test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[FreyjaTexture class test]\n");

	return runFreyjaTextureUnitTest(argc, argv);
}
#endif

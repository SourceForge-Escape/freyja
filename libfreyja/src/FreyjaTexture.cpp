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
	mName = 0x0;
	mFilename = 0x0;
	mImage = 0x0;
	mPalette = 0x0;
	mWidth = 0;
	mHeight = 0;
	mBitDepth = 0;
	mPixelFormat = RGB24;
}


FreyjaTexture::~FreyjaTexture()
{
	if (mName)
		delete [] mName;

	if (mFilename)
		delete [] mFilename;

	if (mImage)
		delete [] mImage;

	if (mPalette)
		delete [] mPalette;
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
#ifdef FIXME
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
#endif

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

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

#include "FreyjaPluginABI.h"
#include "FreyjaImage.h"
#include "FreyjaTexture.h"

Vector<FreyjaTexture *> gFreyjaTextures;
uint32 gFreyjaTextureCount = 0;


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



///////////////////////////////////////////////////////////////////////
// Texture ( 0.9.3 ABI, Can't be used with freyjaIterators )
///////////////////////////////////////////////////////////////////////

index_t freyjaTextureCreateFilename(const char *filename)
{	
	FreyjaImage image;
	index_t uid;
	byte *pixmap;
	freyja_colormode_t colorMode;
	uint32 byteDepth;


	if (image.loadImage(filename) != 0)
		return INDEX_INVALID;

	image.getImage(&pixmap);

	switch (image.getColorMode())
	{
	case FreyjaImage::RGBA_32:
		byteDepth = 4;
		colorMode = RGBA_32;
		break;

	case FreyjaImage::RGB_24:
		byteDepth = 3;
		colorMode = RGB_24;
		break;

	case FreyjaImage::INDEXED_8:
		byteDepth = 1;
		colorMode = INDEXED_8;
		break;

	default:
		byteDepth = 0;
	}

	uid = freyjaTextureCreateBuffer(pixmap, byteDepth, 
									image.getWidth(), image.getHeight(),
									colorMode);

	/* Texture will delete the image copy here on scope exit */
	return uid;
}


index_t freyjaTextureCreateBuffer(byte *image, uint32 byteDepth,
                                  uint32 width, uint32 height,
                                  freyja_colormode_t type)
{
	FreyjaTexture *texture = new FreyjaTexture();
	index_t uid;
	uint32 i, count, size =  width * height * byteDepth;
	bool found = false;


	if (image == 0x0 || size == 0 || byteDepth == 0)
	{
		return INDEX_INVALID;
	}

	/* Setup texture */
	texture->mImage = new byte[size];
	memcpy(texture->mImage, image, size);
	texture->mWidth = width;
	texture->mHeight = height;
	texture->mBitDepth = byteDepth * 8;

	switch (type)
	{
	case RGBA_32:
		texture->mPixelFormat = FreyjaTexture::RGBA32;
		break;

	case RGB_24:
		texture->mPixelFormat = FreyjaTexture::RGB24;
		break;

	case INDEXED_8:
		texture->mPixelFormat = FreyjaTexture::Indexed8;
		break;

	default:
		byteDepth = 0;	
	}

	/* Setup UID and class container reference */
	uid = count = gFreyjaTextures.size();

	for (i = 0; i < count; ++i)
	{
		if (gFreyjaTextures[i] == 0x0)
		{
			uid = i;
			gFreyjaTextures.assign(uid, texture);

			found = true;
			break;
		}	
	}

	if (!found)
	{
		gFreyjaTextures.pushBack(texture);
	}

	++gFreyjaTextureCount;

	texture->mUID = uid;

	return uid;
}


void freyjaTextureDelete(index_t textureIndex)
{
	FreyjaTexture *texture;

	if (textureIndex < gFreyjaTextures.size())
	{
		texture = gFreyjaTextures[textureIndex];

		if (texture != 0x0)
		{
			delete texture;

			gFreyjaTextures.assign(textureIndex, 0x0);
			--gFreyjaTextureCount;
		}
	}
}


void freyjaGetTextureImage(index_t textureIndex,
                           uint32 *w, uint32 *h, uint32 *bitDepth,  
                           uint32 *type, byte **image)
{
	FreyjaTexture *texture;

	/* Init */
	*image = 0x0;
	*bitDepth = 0;
	*type = 0;
	*w = 0;
	*h = 0;

	if (textureIndex < gFreyjaTextures.size())
	{
		texture = gFreyjaTextures[textureIndex];

		if (texture != 0x0)
		{
			*image = texture->mImage;
			*bitDepth = texture->mBitDepth;
			*w = texture->mWidth;
			*h = texture->mHeight;

			switch (texture->mPixelFormat)
			{
			case FreyjaTexture::RGBA32:
				*type = RGBA_32; 
				break;

			case FreyjaTexture::RGB24:
				*type = RGB_24; 
				break;

			case FreyjaTexture::Indexed8:
				*type = INDEXED_8; 
				break;
			}
		}
	}
}


uint32 freyjaGetTexturePoolCount()
{
	return gFreyjaTextures.size();
}


uint32 freyjaGetTextureCount()
{
	return gFreyjaTextureCount;
}



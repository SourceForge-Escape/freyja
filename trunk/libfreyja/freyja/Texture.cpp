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

#include <mstl/Vector.h>
#include "FreyjaImage.h"
#include "TextureABI.h"
#include "FreyjaTexture.h"

using namespace mstl;
using namespace freyja;


Texture::Texture() :
	mName(),                /* Texture name */
	mFilename(),            /* Filename of image */
	mImage(NULL),
	mPalette(NULL),
	mPixelFormat(RGB24),
	mBitDepth(0),
	mWidth(0),
	mHeight(0)
{
}


FreyjaTexture::~FreyjaTexture()
{
	if (mImage)
		delete [] mImage;

	if (mPalette)
		delete [] mPalette;
}


freyja::Texture* Texture::Create( const char* filename )
{
	FreyjaImage image;
	if ( image.loadImage(filename) != 0 )
		return NULL;

	byte* pixmap;
	image.getImage( &pixmap );

	PixelFormat format = RGBA_32;
	uint32 byteDepth;

	switch ( image.getColorMode() )
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

	return Create( pixbuf, image.getWidth(), image.getHeight(), format );
}


freyja::Texture* 
Texture::Create( byte* pixmap,
				 uint16 width, 
				 uint16 height,
				 PixelFormat format )
{
	uint32 size = width * height * GetBytesPerPixel( format );
	freyja::Texture* texture = NULL;

	if ( pixbuf && size )
	{
		texture = new freyja::Texture();
		texture->mImage = new byte[ size ];
		memcpy( texture->mImage, image, size );
		texture->mWidth = width;
		texture->mHeight = height;
		texture->mPixelFormat = format;
	}

	return texture;
}


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_FREYJA_TEXTURE
int runTextureUnitTest(int argc, char *argv[])
{
	Texture test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[Texture class test]\n");

	return runTextureUnitTest(argc, argv);
}
#endif



///////////////////////////////////////////////////////////////////////
// Texture ( 0.9.3 ABI, Can't be used with freyjaIterators )
///////////////////////////////////////////////////////////////////////

freyja_ptr freyjaTextureCreate( const char* filename )
{
	FreyjaImage image;
	if ( image.loadImage(filename) != 0 )
		return NULL;

	byte* pixmap;
	image.getImage( &pixmap );

	freyja_colormode_t colorMode = RGBA_32;
	uint32 byteDepth;

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

	if ( pixbuf == 0x0 || size == 0 || byteDepth == 0 )
	{
		return NULL;
	}

	freyja::Texture* texture = freyjaTextureCreateBuffer(pixmap, byteDepth, 
									image.getWidth(), image.getHeight(),
									colorMode);

	/* Texture will delete the image copy here on scope exit */
	return texture;
}


freyja_ptr 
freyjaTextureCreateBuffer( byte* pixbuf, 
						   byte depth,
						   uint16 width, 
						   uint16 height,
						   freyja_colormode_t type )
{
	freyja::Texture *texture = new freyja::Texture();
	uint32 i, count, size =  width * height * byteDepth;
	bool found = false;

	if ( pixbuf == 0x0 || size == 0 || byteDepth == 0 )
	{
		return NULL;
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
		gFreyjaTextures.push_back(texture);
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
                           uint32 &w, uint32 &h, uint32 &bitDepth,  
                           uint32 &type, byte *&image)
{
	FreyjaTexture *texture;

	/* Init */
	image = 0x0;
	bitDepth = 0;
	type = 0;
	w = 0;
	h = 0;

	if (textureIndex < gFreyjaTextures.size())
	{
		texture = gFreyjaTextures[textureIndex];

		if (texture != 0x0)
		{
			image = texture->mImage;
			bitDepth = texture->mBitDepth;
			w = texture->mWidth;
			h = texture->mHeight;

			switch (texture->mPixelFormat)
			{
			case FreyjaTexture::RGBA32:
				type = RGBA_32; 
				break;

			case FreyjaTexture::RGB24:
				type = RGB_24; 
				break;

			case FreyjaTexture::Indexed8:
				type = INDEXED_8; 
				break;
			}
		}
	}
}



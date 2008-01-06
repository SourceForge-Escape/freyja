/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * License : No use w/o permission (C) 2000-2007 Mongoose
 * Object  : PixelBuffer
 * Comments: 
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.12.26:
 * Mongoose - Created, combined redundant classes with new API.
 *
 * 2005.05.14:
 * Mongoose - Created. [Texture class] 
 *
 * 2000.10.05:
 * Mongoose - Created. [Image class]
 ==========================================================================*/

#ifndef GUARD__FREYJA_PIXELBUFFER_H__
#define GUARD__FREYJA_PIXELBUFFER_H__

#include <mstl/String.h>
#include <mstl/list.h>

#include "freyja.h"


namespace freyja {

enum PixelFormat {
	Indexed_8bpp = 1,
	RGB_24bpp,
	RGBA_32bpp
};


class PixelBuffer
{
 public:

	static PixelBuffer* Create( byte* pixmap,
								uint16 width, uint16 height, 
								PixelFormat format );
	/*------------------------------------------------------
	 * Pre  : Pixmap must be allocated and have deminsion.
	 *
	 * Post : Conditional Constructor returns PixelBuffer. 
	 *        Returns NULL on invalid precondition.
	 *
	 ------------------------------------------------------*/

	static PixelBuffer* Create( const char* filename );
	/*------------------------------------------------------
	 * Pre  : filename must be for a valid image file.
	 *
	 * Post : Conditional Constructor returns PixelBuffer.
	 *        Returns NULL on invalid precondition.
	 *
	 ------------------------------------------------------*/

	virtual ~PixelBuffer();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Deconstructor.
	 *
	 ------------------------------------------------------*/

	bool ConvertPixelFormat( PixelFormat format );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Converts image to format and returns true.
	 *        Returns false on failure.
	 *
	 ------------------------------------------------------*/

	bool ConvertPixelFormat( PixelFormat format, byte* palette, uint16 size );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Converts Index_8bpp image to format and returns true.
	 *        Returns false on failure.
	 *
	 ------------------------------------------------------*/

	void Clear();	
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : The image data is deleted.
	 *
	 ------------------------------------------------------*/

	bool Import( const char* filename, const char* module_name );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool Export( const char* filename, const char* module_name ) const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	const char* GetName() const
	{ return mName.c_str(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns name attribute or NULL.
	 *
	 ------------------------------------------------------*/

	void SetName( const char* name )
	{ mName = name; }
	/*------------------------------------------------------
	 * Pre  : Name is valid string.
	 * Post : Sets PixelBuffer name attribute.
	 *
	 ------------------------------------------------------*/

	uint16 GetHeight() const
	{ return mHeight; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	uint16 GetWidth() const
	{ return mWidth; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	PixelFormat GetPixelFormat( ) const
	{ return mPixelFormat; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static uint16 GetBytesPerPixel( const PixelFormat format );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns bytes per pixel given format.
	 *
	 ------------------------------------------------------*/

	bool BrightenPalette( PixelFormat format, byte* palette, uint32 size, float weight );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 * Notes: Legacy format support.
	 *
	 ------------------------------------------------------*/

	void FlipVertical();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void MakeSquare();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool Scale( const uint32 width, const uint32 height );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	byte* CopyPixmap( );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	const int16 GetTextureId( ) const
	{ return mTextureId; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void SetTextureId( const int16 id )
	{ mTextureId = id; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	const byte* GetImage( )
	{ return mImage; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	static void AddPluginPath( const char* path )
	{ mPluginDirectories.push_back( mstl::String( path ) ); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

 protected:

	PixelBuffer();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	PixelBuffer(const PixelBuffer& PixelBuffer);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructor.
	 *
	 ------------------------------------------------------*/

	PixelBuffer& operator=(const PixelBuffer& PixelBuffer);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Assignment operator.
	 *
	 ------------------------------------------------------*/

	uint16 GetNextPowerOfTwo( const uint16 seed ) const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/


	static mstl::list<mstl::String> mPluginDirectories;

	const static uint16 mMax = 65535;

	mstl::String mName;                /* PixelBuffer name. */
	
	mstl::String mFilename;            /* Filename of image. */

	PixelFormat mPixelFormat;

	byte* mImage;

	uint16 mWidth;

	uint16 mHeight;

	int16 mTextureId;
};


inline
void PixelBuffer::Clear( )
{
	if ( mImage )
	{
		delete [] mImage;
	}

	mImage = NULL;
	mWidth = mHeight = 0;
	mName = mFilename = "";
	mTextureId = -1;
}


inline
uint16 PixelBuffer::GetBytesPerPixel( const PixelFormat format )
{ 
	return ( (format == Indexed_8bpp) ? 1 :
			 (format == RGB_24bpp) ? 3 :
			 (format == RGBA_32bpp) ? 4 : 
			 0 );	
}


} // namespace freyja

#endif // GUARD__FREYJA_PIXELBUFFER_H__

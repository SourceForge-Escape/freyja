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

#include <mstl/SystemIO.h>
#include <mstl/Vector.h>
#include "PixelBuffer.h"

using namespace mstl;
using namespace freyja;

mstl::list<mstl::String> PixelBuffer::mPluginDirectories;


PixelBuffer::PixelBuffer() :
	mName(),               
	mFilename(),           
	mPixelFormat(RGB_24bpp),
	mImage(NULL),
	mWidth(0),
	mHeight(0),
	mTextureId(-1)
{
}


PixelBuffer::~PixelBuffer()
{
	Clear();
}


PixelBuffer* 
PixelBuffer::Create( byte* pixmap,
				     uint16 width, 
				     uint16 height,
				     PixelFormat format )
{
	uint32 size = width * height * GetBytesPerPixel( format );
	freyja::PixelBuffer* texture = NULL;

	if ( pixmap && size )
	{
		texture = new freyja::PixelBuffer();
		texture->mImage = new byte[ size ];
		memcpy( texture->mImage, pixmap, size );
		texture->mWidth = width;
		texture->mHeight = height;
		texture->mPixelFormat = format;
	}

	return texture;
}


bool PixelBuffer::ConvertPixelFormat( PixelFormat mode )
{
	if ( !mImage || mWidth < 1 || mHeight < 1 )
		return false;

	bool converted = false;

	/* Note:
	 * If this is called 'Indexed_8' is considered merely greyscale.
	 */

	switch ( mode )
	{
	case Indexed_8bpp:
		switch ( mPixelFormat )
		{
		case Indexed_8bpp:
			converted = true;
			break;

		case RGB_24bpp: 
			{
				byte* pixmap = new byte[ mWidth * mHeight ];
		
				for ( uint32 i = 0, n = mWidth * mHeight; i < n; ++i )
				{
					const unsigned int idx = i * 3;
					pixmap[i] = ( ( mImage[idx  ] >> 2 ) +  // ~30%
								  ( mImage[idx+1] >> 1 ) +  // ~59%
								  ( mImage[idx+2] >> 3 ) ); // ~11%
				}

				delete [] mImage;
				mImage = pixmap;
				mPixelFormat = Indexed_8bpp;
				converted = true;
			}
			break;

		case RGBA_32bpp: 
			{
				byte* pixmap = new byte[ mWidth * mHeight ];
		
				for ( uint32 i = 0, n = mWidth * mHeight; i < n; ++i )
				{
					const unsigned int idx = i * 4;
					pixmap[i] = ( ( mImage[idx  ] >> 2 ) +  // ~30%
								  ( mImage[idx+1] >> 1 ) +  // ~59%
								  ( mImage[idx+2] >> 3 ) ); // ~11%
				}

				delete [] mImage;
				mImage = pixmap;
				mPixelFormat = Indexed_8bpp;
				converted = true;
			}
			break;

		default:
			;
		}
		break;

	case RGB_24bpp:

		switch ( mPixelFormat )
		{
		case Indexed_8bpp:
			{
				byte* pixmap = new byte[ mWidth * mHeight * 3 ];
		
				for ( uint32 i = 0, n = mWidth * mHeight; i < n; ++i )
				{
					const unsigned int idx = i * 3;
					pixmap[idx] = pixmap[idx+1] = pixmap[idx+2] = mImage[i];
				}

				delete [] mImage;
				mImage = pixmap;
				mPixelFormat = RGB_24bpp;
				converted = true;
			}
			break;

		case RGB_24bpp:
			converted = true;
			break;

		case RGBA_32bpp:
			{
				byte* image = new byte[mWidth * mHeight * 3];
			
				for (uint32 i = 0, size = mWidth * mHeight; i < size; ++i)
				{
					const unsigned int idx = i*3;
					const unsigned int idx2 = i*4;
					image[idx  ] = mImage[idx2  ];
					image[idx+1] = mImage[idx2+1];
					image[idx+2] = mImage[idx2+2];
					// Alpha component not used in RGB_24.
				}

				delete [] mImage;
				mImage = image;
				mPixelFormat = RGB_24bpp;
				converted = true;
			}
			break;
		
		default:
			;

		}


	case RGBA_32bpp:

		switch ( mPixelFormat )
		{
		case Indexed_8bpp:
			{
				byte* pixmap = new byte[ mWidth * mHeight * 4 ];
		
				for ( uint32 i = 0, n = mWidth * mHeight; i < n; ++i )
				{
					const unsigned int idx = i * 4;
					pixmap[idx] = pixmap[idx+1] = pixmap[idx+2] = mImage[i];
					pixmap[idx+3] = 255;
				}

				delete [] mImage;
				mImage = pixmap;
				mPixelFormat = RGBA_32bpp;
				converted = true;
			}
			break;

		case RGB_24bpp:
			{
				byte* image = new byte[mWidth * mHeight * 4 ];
			
				for (uint32 i = 0, size = mWidth * mHeight; i < size; ++i)
				{
					const unsigned int idx = i*4;
					const unsigned int idx2 = i*3;
					image[idx  ] = mImage[idx2  ];
					image[idx+1] = mImage[idx2+1];
					image[idx+2] = mImage[idx2+2];
					image[idx+3] = 255;
				}

				delete [] mImage;
				mImage = image;
				mPixelFormat = RGBA_32bpp;
				converted = true;
			}
			break;

		case RGBA_32bpp:
			converted = true;
			break;
		
		default:
			;

		}
	}

	return converted;
}


bool PixelBuffer::ConvertPixelFormat( PixelFormat format, byte* palette, uint16 size )
{
	if ( mPixelFormat != Indexed_8bpp || !palette || !size )
		return false;

	bool converted = false;

	switch ( format )
	{
	case Indexed_8bpp:
		converted = true;
		break;

	case RGB_24bpp: 
		{
			byte* pixmap = new byte[ mWidth * mHeight ];
		
			for ( uint32 i = 0, n = mWidth * mHeight; i < n; ++i )
			{
				const uint16 pidx = mImage[i];

				if ( pidx < size )
				{
					const unsigned int idx = i * 3;
					const uint32 pidx3 = pidx * 3;
					pixmap[idx  ] = palette[pidx3  ];
					pixmap[idx+1] = palette[pidx3+1];
					pixmap[idx+2] = palette[pidx3+2];
				}
			}

			delete [] mImage;
			mImage = pixmap;
			mPixelFormat = RGB_24bpp;
			converted = true;
		}
		break;

	case RGBA_32bpp: 
		{
			byte* pixmap = new byte[ mWidth * mHeight ];
		
			for ( uint32 i = 0, n = mWidth * mHeight; i < n; ++i )
			{
				const uint16 pidx = mImage[i];

				if ( pidx < size )
				{
					const unsigned int idx = i * 4;
					const uint32 pidx3 = pidx * 3;
					pixmap[idx  ] = palette[pidx3  ];
					pixmap[idx+1] = palette[pidx3+1];
					pixmap[idx+2] = palette[pidx3+2];
				}
			}

			delete [] mImage;
			mImage = pixmap;
			mPixelFormat = RGBA_32bpp;
			converted = true;
		}
		break;

	default:
		;
	}

	return converted;
}


bool PixelBuffer::BrightenPalette( PixelFormat format, byte* palette, uint32 size, float weight )
{
	if ( !palette || !size || weight <= 0.0f || weight > 0.9999f )
	{
		//printf("Image::BrightenPalette> %f isn't a postive percentage\n", p);
		return false;
	}

	bool ret = false;

	switch ( format )
	{
	case RGB_24bpp:
		for ( uint32 i = 0; i < size; ++i )
		{
			const uint32 idx = i * 3;
			palette[idx  ] += (unsigned char)(palette[idx] * weight);
			palette[idx+1] += (unsigned char)(palette[idx+1] * weight);
			palette[idx+2] += (unsigned char)(palette[idx+2] * weight);
		}    
		ret = true;
		break;

	case RGBA_32bpp:
		for ( uint32 i = 0; i < size; ++i )
		{
			const uint32 idx = i * 4;
			palette[idx  ] += (unsigned char)(palette[idx] * weight);
			palette[idx+1] += (unsigned char)(palette[idx+1] * weight);
			palette[idx+2] += (unsigned char)(palette[idx+2] * weight);
		}    
		ret = true;
		break;

	default:
		;
	}

	return ret;
}


void PixelBuffer::FlipVertical()
{
	if ( !mImage )
		return;

	uint16 bpp = GetBytesPerPixel( mPixelFormat );

	if ( bpp )
	{
		const uint32 stride = mWidth*bpp;
		byte* swap_row = new byte[ stride ];

		for ( uint32 i = 0, j = mHeight-1, n = mHeight/2; i < n; ++i, --j )
		{
			memcpy( swap_row, &mImage[i*stride], stride );
			memcpy( &mImage[i*stride], &mImage[j*stride], stride);
			memcpy( &mImage[j*stride], swap_row, stride);
		}

		delete [] swap_row;		
	}
}


PixelBuffer* PixelBuffer::Create( const char* filename )
{
#ifdef IMAGE_PLUGINS
	mstl::SystemIO::FileReader reader;

	if ( !reader.DoesFileExist( filename ) )
	{
		//print("File '%s' couldn't be accessed.", filename);
		return NULL;
	}

	for ( mstl::list<mstl::String>::iterator it = mPluginDirectories.begin(); it != it.end(); it++ )
	{
		if ( !reader.OpenDir( (*it).c_str() ) )
		{
			freyjaPrintMessage( "Couldn't access image plugin directory" );
			continue;
		}

		bool done = false;

		while ( !done )
		{
			const char* module = reader.GetNextDirectoryListing();

			if ( !module )
				break;

			if ( reader.IsDirectory(module) )
				continue;

			void* handle = freyjaModuleLoad( module );

			if ( !handle )
			{
				 /* Try the next plugin, even after a bad module load. */
				continue;
			}
			else
			{
				int (*importf)(char* filename, byte** image,
					unsigned int* width, unsigned int* height, char* type);

				importf = (int (*)(char *filename, unsigned char **image,
									 unsigned int *width, unsigned int *height, 
									 char *type))freyjaModuleImportFunction(handle, "import_image");

				if ( importf == NULL )  
				{
					freyjaModuleUnload(handle);
					continue;
				}

				unsigned char *image = 0x0;
				unsigned int width = 0, height = 0;
				char type = 0;

				if ( !(*importf)((char*)filename, &image, &width, &height, &type) )
				{
					const uint16 bpp = type * 8;

					if ( image && width > 0 && height > 0 && 
					     bpp == 32 || bpp == 24 || bpp == 8 )
					{
						PixelBuffer* buf = new PixelBuffer();
						buf->mFilename = filename;
						buf->mName = mstl::SystemIO::BasenameString( filename );
						buf->mImage = image;
						buf->mWidth = width;
						buf->mHeight = height;
						buf->mPixelFormat = 
							( bpp == 32 ) ? RGBA_32bpp :
							( bpp == 24 ) ? RGB_24bpp : Indexed_8bpp;

						freyjaModuleUnload( handle );

						return buf;
					}
					else if ( image )
					{
						delete [] image;
					}
				}

				freyjaModuleUnload(handle);
			}
		}
	
		reader.CloseDir();
	}
#else
	freyjaPrintMessage("PixelBuffer: This build was compiled w/o plugin support.");
#endif

	return NULL;
}


bool PixelBuffer::Export( const char* filename, const char* module_name ) const
{
#ifdef IMAGE_PLUGINS
	SystemIO::FileReader reader;
	char symbol[256];
	int (*export_img)(char *filename, unsigned char *image,
					  unsigned int width, unsigned int height, 
					  char type);
	bool done = false;
	const char *module_filename;
	void *handle;


	freyjaPrintMessage("[Image plugin system invoked]");


	for ( mstl::list<mstl::String>::iterator it = mPluginDirectories.begin(); it != it.end(); it++ )
	{
		if (!reader.OpenDir( (*it).c_str() ) )
		{
			//printError("Couldn't access image plugin directory '%s'",
			//		   gImagePluginDirectories[i].c_str());
			continue;
		}

	while (!done && (module_filename = reader.GetNextDirectoryListing()))
	{
		if (reader.IsDirectory(module_filename))
			continue;

		if (!(handle = freyjaModuleLoad(module_filename)))
		{
			continue; /* Try the next plugin, even after a bad module load */
		}
		else
		{
			freyjaPrintMessage("Module '%s' opened.", module_filename);

			snprintf(symbol, 256, "freyja_image_export__%s", module_name);

			export_img = (int (*)(char *filename, unsigned char *image,
								  unsigned int width, unsigned int height, 
								  char type))freyjaModuleImportFunction(handle, symbol);

			if (export_img == NULL)  
			{
				freyjaModuleUnload(handle);
				continue;
			}
			
			done = 
			!(*export_img)((char*)filename, mImage, mWidth, mHeight, GetBytesPerPixel( mPixelFormat ) );

			freyjaModuleUnload(handle);
		}
	}

	reader.CloseDir();

	}

	freyjaPrintMessage("[FreyjaPlugin module loader sleeps now]\n");

	if (done)
		return true;
#else
	freyjaPrintMessage("Image: This build was compiled w/o plugin support");
#endif

	return false;
}


void PixelBuffer::MakeSquare()
{
	uint16 side = GetNextPowerOfTwo( (mWidth > mHeight) ? mWidth : mHeight );
	Scale( side, side );
}


bool PixelBuffer::Scale( const uint32 maxW, const uint32 maxH)
{
   if ( !mImage || !mWidth || !mHeight )
		return false;

   uint32 height = GetNextPowerOfTwo(mHeight);
   uint32 width = GetNextPowerOfTwo(mWidth);
	
   if ( height > maxH ) 
		height = maxH;

   if ( width > maxW ) 
		width = maxW;

   /* Check to see if scaling was needed. */
   if (height == mHeight && width == mWidth) 
		return true;

   uint16 components = GetBytesPerPixel( mPixelFormat ); 	
   byte* image = new unsigned char[ height * width * components ];
   float* tempin = new float[ mWidth * mHeight * components ];
   float* tempout = new float[ width * height * components ];
	
   if ( !tempout || !tempin )
   {
		if (tempout)
			delete [] tempout;
		
		if (tempin)
			delete [] tempin;
		
		//printf("Out of memory!\n");
		return false;
   }

   /* Copy user data to float format. */
   for (uint32 i = 0, n = mWidth*mHeight*components; i < n; ++i) 
   {
	   tempin[i] = (float)mImage[i];
   }
	
   /* Determine which filter to use by checking ratios. */
   float sx, sy;

   if (width > 1) 
   {
		sx = (float)(mWidth - 1) / (float)(width - 1);
   } 
   else 
   {
		sx = (float)(mWidth - 1);
   }
	
   if (height > 1)
   {
		sy = (float)(mHeight - 1) / (float)(height - 1);
   } 
   else 
   {
		sy = (float)(mHeight - 1);
   }

   /* This filter code based off of gluScaleImage(). */
   if (sx < 1.0 && sy < 1.0) 
   {
		/* Magnify both width and height:  use weighted sample of 4 pixels. */
		for(uint32 i = 0; i < height; ++i) 
		{
			int i0 = (int)(i * sy);
			int i1 = i0 + 1;
			
			if (i1 >= mHeight) 
			{
				i1 = mHeight - 1;
			}
			
			float alpha = i * sy - i0;
			
			for (uint32 j = 0; j < width; ++j) 
			{
				int j0 = (int) (j * sx);
				int j1 = j0 + 1;
				
				if (j1 >= mWidth) 
				{
					j1 = mWidth - 1;
				}
				
				float beta = j * sx - j0;
				
				/* Compute weighted average of pixels in rect (i0,j0)-(i1,j1) */
				float* src00 = tempin + (i0 * mWidth + j0) * components;
				float* src01 = tempin + (i0 * mWidth + j1) * components;
				float* src10 = tempin + (i1 * mWidth + j0) * components;
				float* src11 = tempin + (i1 * mWidth + j1) * components;

				float* dst = tempout + (i * width + j) * components;
				
				for (uint32 k = 0; k < components; ++k) 
				{
					float s1 = *src00++ * (1.0 - beta) + *src01++ * beta;
					float s2 = *src10++ * (1.0 - beta) + *src11++ * beta;
					*dst++ = s1 * (1.0 - alpha) + s2 * alpha;
				} 
			}
		}     
   }
   else 
   {
		/* Shrink width and/or height:  use an unweighted box filter. */
		for (uint32 i = 0; i < height; ++i) 
		{
			int i0 = (int) (i * sy);
			int i1 = i0 + 1;
			
			if (i1 >= mHeight) 
			{
				i1 = mHeight - 1;
			}
			
			for (uint32 j = 0; j < width; ++j) 
			{
				int j0 = (int) (j * sx);
				int j1 = j0 + 1;
				
				if (j1 >= mWidth) 
				{
					j1 = mWidth - 1;
				}
				
				float* dst = tempout + (i * width + j) * components;
				
				/* Compute average of pixels in the rectangle (i0,j0)-(i1,j1). */
				for (uint32 k = 0; k < components; ++k) 
				{
					float sum = 0.0;
					
					for (int ii = i0; ii <= i1; ++ii) 
					{
						for (int jj = j0; jj <= j1; ++jj) 
						{
							sum += *(tempin + (ii * mWidth + jj) * components + k);
						}
					}
					
					sum /= ( j1 - j0 + 1 ) * ( i1 - i0 + 1 );
					*dst++ = sum;
				}
			} 
		}
   }
	
   /* Copy to the resulting scaled pixmap. */
   for( uint32 i = 0, n = height*width*components; i < n; ++i ) 
   {
		image[i] = (byte)tempout[i];
   }
	
   /* Delete our temp buffers and old pixmap. */
   delete[] tempin;
   delete[] tempout;
   delete[] mImage;
	
   mImage = image;
   mWidth = width;
   mHeight = height;
   return true;
}


byte* PixelBuffer::CopyPixmap( )
{
	if ( mImage == NULL )
		return NULL;

	uint32 size = mWidth * mHeight * GetBytesPerPixel( mPixelFormat );
	byte* pixmap = new byte[size];
	memcpy( pixmap, mImage, size );
	
	return pixmap; 
}


uint16 PixelBuffer::GetNextPowerOfTwo( const uint16 seed ) const
{
	if ( seed >= mMax ) 
		return mMax;

	uint16 i;
	for( i = 1; i < seed; i *= 2 ) 
		;

	return i;
}


#if DISABLED
byte* PixelBuffer::GenerateCheckerBoard(byte bg[4], byte fg[4],
										uint32 width, uint32 height,
										uint32 runlen )
{
	byte* image = new byte[height*width*4];
	bool swap = true;

	for (uint32 i = 0, l = 0; i < width; ++i, ++l)
	{
		for (uint32 j = 0; j < height; ) // 2px black border
		{
			byte *&rgba = (swap) ? bg : fg;			

			for (uint32 k = 0; k < runlen; ++k, ++j)
			{
				uint32 idx = ( height * i + j ) * 4;

				/* RBGA */
				image[idx]   = rgba[0];
				image[idx+1] = rgba[1];
				image[idx+2] = rgba[2];
				image[idx+3] = rgba[3];
			}

			swap = !swap;
		}

		if ( l >= runlen )
		{
			l = 0;
			swap = !swap;	
		}
	}

	return image;
}
#endif


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_FREYJA_PIXELBUFFER
int PixelBufferUnitTest(int argc, char *argv[])
{
	PixelBuffer test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[Texture class test]\n");

	return PixelBufferUnitTest(argc, argv);
}
#endif // UNIT_TEST_FREYJA_PIXELBUFFER


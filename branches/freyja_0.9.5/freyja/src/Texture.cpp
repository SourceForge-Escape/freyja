/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : Midgard, MTK, Freyja, OpenRaider
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440
 * Email   : stu7440@westga.edu
 * Object  : Texture
 * Comments: This is the Texture class.
 *
 *           See file COPYING for license details.
 *
 * 
 *-- History ---------------------------------------------------------- 
 *
 * 2003.06.30, 
 * Mongoose - API update, SDL_TTF support moved here, misc features
 *            SDL_TTF support based on Sam Lantinga's public domain
 *            SDL_TTF demo functions and algorithms 
 *
 * 2001.05.29:
 * Mongoose - Removed legacy code and done clean up
 *
 * 2001.02.19:
 * Mongoose - Moved from GooseEgg to mtk
 *
 * 2000.04.29:
 * Mongoose - Created from old PPM/PCX codebases I have done before
 ==========================================================================*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef HAVE_FREYJA_IMAGE
#   include <freyja/FreyjaImage.h>
#endif

#include "FreyjaOpenGL.h"
#include "Texture.h"


Texture *Texture::mSingleton = NULL;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Texture::Texture()
{
	mTextureIds = NULL;

	mFlags = 0;
	mTextureId = -1;
	mTextureId2 = -1;
	mTextureCount = 2; // FIXME: Use new OpenGL ext wrapper class
	mTextureLimit = 2;

	mSingleton = this;
}


Texture::~Texture()
{
	reset();
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

unsigned char *Texture::generateColorTexture(unsigned char rgba[4],
											 unsigned int width, 
											 unsigned int height)
{
	unsigned char *image;
	unsigned int i, size;


	image = new unsigned char[height*width*4];

	for (i = 0, size = width*height; i < size; ++i)
	{
		/* RBGA */
		image[i*4]   = rgba[0];
		image[i*4+1] = rgba[1];
		image[i*4+2] = rgba[2];
		image[i*4+3] = rgba[3];
	}

	return image;
}



////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

int Texture::loadColorTexture(unsigned char rgba[4],
							  unsigned int width, unsigned int height)
{
	unsigned char *image;
	int id = -1;

	image = generateColorTexture(rgba, width, height);
	id = loadBuffer(image, width, height, RGBA, 32);
	delete [] image;

	return id;
}


void Texture::reset()
{
	glEnable(GL_TEXTURE_2D);

	if (mTextureIds)
	{
		glDeleteTextures(mTextureLimit, mTextureIds);
		delete [] mTextureIds;
	}

	mTextureIds = 0x0;
	mTextureCount = 0;
	mTextureLimit = 0;
}


void Texture::disableMultiTexture()
{
	mFlags ^= fUseMultiTexture;
	mTextureId = -1;
	mTextureId2 = -1;

	glDisable(GL_TEXTURE_2D);

#ifdef USING_OPENGL_EXT
	if (h_glActiveTextureARB)
	{
		h_glActiveTextureARB(GL_TEXTURE0_ARB);
	}
#endif
}


void Texture::useMultiTexture(float aU, float aV, float bU, float bV)
{
	if (!(mFlags & fUseMultiTexture))
		return;

#ifdef USING_OPENGL_EXT
	if (h_glMultiTexCoord2fARB)
	{
		h_glMultiTexCoord2fARB(GL_TEXTURE0_ARB, aU, aV);
		h_glMultiTexCoord2fARB(GL_TEXTURE1_ARB, bU, bV);
	}
#endif
}


void Texture::useMultiTexture(float u, float v)
{
	if (!(mFlags & fUseMultiTexture))
		return;

#ifdef USING_OPENGL_EXT
	if (h_glMultiTexCoord2fARB)
	{
		h_glMultiTexCoord2fARB(GL_TEXTURE0_ARB, u, v);
		h_glMultiTexCoord2fARB(GL_TEXTURE1_ARB, u, v);
	}
#endif
}


void freyja_renders_old_multi(int texture, int texture2)
{
#ifdef USING_OPENGL_EXT
	if (h_glActiveTextureARB)
	{
		h_glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture);
		// bump
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
		//glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_REPLACE);
	
		h_glActiveTextureARB(GL_TEXTURE1_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture2);
		// bump
		//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
		//glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_ADD);

		// Combine, gamma correct
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
		glTexEnvi(GL_TEXTURE_ENV, GL_RGB_SCALE_ARB, 2);
	}
#endif
}


void Texture::bindMultiTexture(int texture0, int texture1)
{
	if (//(int)a == mTextureId && (int)b == mTextureId2 ||
		 !mTextureIds || 
		 texture0 < 0 || texture0 > (int)mTextureCount ||
		 texture1 < 0 || texture1 > (int)mTextureCount)
   {
		return;
	}

	mFlags |= fUseMultiTexture;
	mTextureId  = texture0;
	mTextureId2 = texture1;

#ifdef USING_OPENGL_EXT
	if (h_glActiveTextureARB)
	{
		h_glActiveTextureARB(GL_TEXTURE0_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, mTextureIds[texture0]);

		h_glActiveTextureARB(GL_TEXTURE1_ARB);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, mTextureIds[texture1]);
	}
#endif
}


void Texture::setMaxTextureCount(unsigned int n)
{
	mTextureLimit = n;

	mTextureIds = new unsigned int[n];

	glGenTextures(n, mTextureIds);
}


int Texture::getTextureCount()
{
	return (mTextureCount-1);
}



int Texture::loadBuffer(unsigned char *image, 
								unsigned int width, unsigned int height,
								ColorMode mode, unsigned int bpp)
{
	int id;


	id = loadBufferSlot(image, width, height, mode, bpp, mTextureCount++);

	if (id < 0)
	{
		return id;
	}

	return ++id;
}


void convertARGB32bppToRGBA32bpp(unsigned char *image, 
								 unsigned int w, unsigned int h)
{
	unsigned int i, size = w*h;
	unsigned char swap;


	for (i = 0; i < size; ++i)
	{
		/* 32-bit ARGB to RGBA */
		swap = image[i*4+3];
		image[i*4]   = image[i*4+1];
		image[i*4+1] = image[i*4+2];
		image[i*4+2] = image[i*4+3];
		image[i*4+3] = swap;
	}
}


int Texture::loadBufferSlot(unsigned char *image, 
							unsigned int width, unsigned int height,
							ColorMode mode, unsigned int bpp,
							unsigned int slot)
{
	unsigned char bytes;
	unsigned int glcMode;


	if (!mTextureIds || slot >= mTextureLimit)
	{
		printf("Texture::Load> ERROR Not initalizied or out of free slots\n");
		return -1000;
	}

	if (!width || !height || !image)
	{
		printf("Texture::Load> ERROR Assertion 'image is valid' failed\n");
		return -1;
	}

	switch (mode)
	{
	case GREYSCALE:
		if (bpp != 8)
		{
			printf("Texture::Load> ERROR Unsupported GREYSCALE, %i bpp\n", bpp);
			return -2;
		}
		
		bytes = 1;
		glcMode = GL_LUMINANCE;
		break;
	case RGB:
		if (bpp != 24)
		{
			printf("Texture::Load> ERROR Unsupported RGBA, %i bpp\n", bpp);
			return -2;
		}
		
		bytes = 3;
		glcMode = GL_RGB;
		break;
	case ARGB:
		if (bpp == 32)
		{
			convertARGB32bppToRGBA32bpp(image, width, height);
		}
	case RGBA:
		if (bpp != 32)
		{
			printf("Texture::Load> ERROR Unsupported RGBA/ARGB, %i bpp\n", bpp);
			return -2;
		}
		
		bytes = 4;
		glcMode = GL_RGBA;
		break;
	default:
		printf("Texture::Load> ERROR Unknown color mode %i\n", mode);
		return -2;
	}

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
  
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glBindTexture(GL_TEXTURE_2D, mTextureIds[slot]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (mFlags & fUseMipmaps)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
							 GL_NEAREST_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
							 GL_LINEAR_MIPMAP_LINEAR);
		
		gluBuild2DMipmaps(GL_TEXTURE_2D, bytes, width, height,
								glcMode, GL_UNSIGNED_BYTE, image);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		
		glTexImage2D(GL_TEXTURE_2D, 0, glcMode, width, height, 0,
						 glcMode, GL_UNSIGNED_BYTE, image);
	}

	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	return slot;
}


void Texture::bindTextureId(unsigned int n)
{
	if ((int)n == mTextureId || !mTextureIds || n > mTextureCount)
	{
		return;
	}

	mTextureId = n;

	glEnable(GL_TEXTURE_2D);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBindTexture(GL_TEXTURE_2D, mTextureIds[n]);
}


void Texture::glScreenShot(const char *base, 
						   unsigned int width, unsigned int height)
{
  FILE *f;
  int sz = width*height;
  unsigned char *image = new unsigned char[sz*3];
  unsigned char *swap_row = new unsigned char[width*3];
  char filename[1024];
  static int count = 0;
  bool done = false;
  int i, j, size;
  unsigned char comment_lenght;
  unsigned char colormap_type; 
  unsigned char image_type;    
  unsigned short colormap_index;      
  unsigned short colormap_lenght;     
  unsigned char colormap_bbp;         
  unsigned short origin_x;
  unsigned short origin_y;
  unsigned short swidth;   
  unsigned short sheight;  
  char comment[32] = "Mongoose TGA 0.0.1\0";
  unsigned char tmp, bpp, desc_flags;


  if (!image || !width || !height)
  {
    if (image)
    {
      delete [] image;
    }

    printf("glScreenShot> ERROR: Couldn't allocate image!\n");
    return;
  }

  // Don't overwrite files
  while (!done)
  {
    snprintf(filename, 1024, "%s-%04i.tga", base, count++);
    
    f = fopen(filename, "rb");

    if (f)
      fclose(f);
    else
      done = true;
  }

  f = fopen(filename, "wb");

  if (!f)
  {
    printf("glScreenShot> ERROR: Couldn't write screenshot.\n");
    perror("glScreenShot> ERROR: ");
    return;
  }

  // Capture frame buffer
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);

  // Flip vertical
  for (i = 0, j = (int)height-1; i < (int)height/2; ++i, --j)
  {
    memcpy(swap_row, &image[i*width*3], width*3);
    memcpy(&image[i*width*3], &image[j*width*3], width*3);
    memcpy(&image[j*width*3], swap_row, width*3);
  }

  delete [] swap_row;

  comment_lenght = strlen(comment);
  colormap_type = 0;
  image_type = 2;
  colormap_index = 0;
  colormap_lenght = 0;
  colormap_bbp = 0;
  origin_x = origin_y = 0;
  swidth = width;
  sheight = height;
  bpp = 24;
  desc_flags = 32;

  // Write TGA header
  fwrite(&comment_lenght, 1, 1, f);
  fwrite(&colormap_type, 1, 1, f); 
  fwrite(&image_type, 1, 1, f);
  fwrite(&colormap_index, 2, 1, f);
  fwrite(&colormap_lenght, 2, 1, f);
  fwrite(&colormap_bbp, 1, 1, f);
  fwrite(&origin_x, 2, 1, f);
  fwrite(&origin_y, 2, 1, f);
  fwrite(&swidth, 2, 1, f);
  fwrite(&sheight, 2, 1, f);
  fwrite(&bpp, 1, 1, f);
  fwrite(&desc_flags, 1, 1, f);

  // Write comment
  fwrite(&comment, 1, comment_lenght, f);

  size = width * height * 3;
 
  for (i = 0; i < size; i += 3)
  {
    tmp = image[i];
    image[i] = image[i + 2];
    image[i + 2] = tmp;
  }

  // Write image data
  if (fwrite(image, size, 1, f) < 1)
  {
    perror("glScreenShot> Disk write failed.\n");
	 fclose(f);
    return;
  }

  fclose(f);

  delete [] image;

  printf("Took screenshot '%s'.\n", filename);
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////

int Texture::nextPower(int seed)
{
  int i;

  for(i = 1; i < seed; i *= 2) 
    ;

  return i;
}


/* This code based off on gluScaleImage()  */
unsigned char *Texture::scaleBuffer(unsigned char *image, 
												int width,  int height,	
												int components)
{
   int i, j, k;
   float* tempin;
   float* tempout;
   float sx, sy;
	//   int components = 3;
   unsigned char *timage;
	int original_height = height;
	int original_width = width;


   if (!image || !width || !height)
	{
		return NULL;
	}

   height = nextPower(height);
   width = nextPower(width);

   if (height > 256) 
     height = 256;
    
   if (width > 256) 
     width = 256;

   // Check to see if scaling is needed
   if (height == original_height && width == original_width) 
		return NULL;

	//printf("%i\n", components);

   timage = new unsigned char[height * width * components];
   tempin = new float[original_width * original_height * components * sizeof(float)];
   tempout = new float[width * height * components * sizeof(float)];

   if (!tempout || !tempin)
   {
     if (tempout)
       delete [] tempout;

     if (tempin)
       delete [] tempin;

     printf("Oh shit out of memory!\n");
     return NULL;
   }

   // Copy user data to float format.
   for (i = 0; i < original_height * original_width * components; ++i) 
   {
     tempin[i] = (float)image[i];
   }

   // Determine which filter to use by checking ratios.
   if (width > 1) 
   {
     sx = (float)(original_width - 1) / (float)(width - 1);
   } 
   else 
   {
     sx = (float)(original_width - 1);
   }

   if (height > 1)
   {
     sy = (float)(original_height - 1) / (float) (height - 1);
   } 
   else 
   {
     sy = (float)(original_height - 1);
   }

   if (sx < 1.0 && sy < 1.0) 
   {
     /* Magnify both width and height:  use weighted sample of 4 pixels */
     int i0, i1, j0, j1;
     float alpha, beta;
     float* src00;
     float* src01;
     float* src10;
     float* src11;
     float s1, s2;
     float* dst;

     for(i = 0; i < height; ++i) 
     {
       i0 = (int)(i * sy);
       i1 = i0 + 1;

       if (i1 >= original_height) 
       {
			 i1 = original_height - 1;
       }

       alpha = i * sy - i0;

       for (j = 0; j < width; ++j) 
       {
			 j0 = (int) (j * sx);
			 j1 = j0 + 1;

			 if (j1 >= original_width) 
			 {
				 j1 = original_width - 1;
			 }

			 beta = j * sx - j0;

			 /* Compute weighted average of pixels in rect (i0,j0)-(i1,j1) */
			 src00 = tempin + (i0 * original_width + j0) * components;
			 src01 = tempin + (i0 * original_width + j1) * components;
			 src10 = tempin + (i1 * original_width + j0) * components;
			 src11 = tempin + (i1 * original_width + j1) * components;

			 dst = tempout + (i * width + j) * components;

			 for (k = 0; k < components; ++k) 
			 {
				 s1 = *src00++ * (1.0 - beta) + *src01++ * beta;
				 s2 = *src10++ * (1.0 - beta) + *src11++ * beta;
				 *dst++ = s1 * (1.0 - alpha) + s2 * alpha;
			 } 
       }
     }     
   }
   else 
   {
     /* Shrink width and/or height:  use an unweighted box filter */
     int i0, i1;
     int j0, j1;
     int ii, jj;
     float sum;
     float* dst;

     for (i = 0; i < height; ++i) 
     {
       i0 = (int) (i * sy);
       i1 = i0 + 1;

       if (i1 >= original_height) 
       {
			 i1 = original_height - 1;
       }

       for (j = 0; j < width; ++j) 
       {
			 j0 = (int) (j * sx);
			 j1 = j0 + 1;

			 if (j1 >= original_width) 
			 {
				 j1 = original_width - 1;
			 }

			 dst = tempout + (i * width + j) * components;

			 /* Compute average of pixels in the rectangle (i0,j0)-(i1,j1) */
			 for (k = 0; k < components; ++k) 
			 {
				 sum = 0.0;

				 for (ii = i0; ii <= i1; ++ii) 
				 {
					 for (jj = j0; jj <= j1; ++jj) 
					 {
						 sum += *(tempin + (ii * original_width + jj) 
									 * components + k);
					 }
				 }

				 sum /= ( j1 - j0 + 1 ) * ( i1 - i0 + 1 );
				 *dst++ = sum;
			 }
       } 
     }
   }

   // Copy to our results.
   for( i = 0; i < height * width * components; ++i) 
   {
     timage[i] = (unsigned char)tempout[i];
   }

   // Delete our temp buffers.
   delete[] tempin;
   delete[] tempout;
   delete[] image;

   return timage;
}


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : MtkImage
 * License : GPL See file COPYING, also (C) 2000 Mongoose
 * Comments: This is the new mtkImage class for mtk
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2000-10-05:
 * Mongoose - Created
 =================================================================*/

#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#ifdef EGGIMAGE_PLUGINS
#   include <dlfcn.h>
#endif

#include "EggFileReader.h"
#include "EggFileWriter.h"

#include "EggImage.h"



EggImage::EggImage()
{
	_image = 0x0;
	_palette = 0x0;
	_width = 0;             
	_height = 0;            
	_original_width = 0;    
	_original_height = 0;   
	_color_mode = RGB_24;
	_image_bpp = 0;
	_palette_bpp = 0;
}


EggImage::~EggImage()
{
	erase();
}


void EggImage::erase()
{
   if (_image)
      delete [] _image;
	
   if (_palette)
      delete [] _palette;
	
   _width = 0;
   _height = 0;  
   _original_width = 0;
   _original_height = 0;
   _palette_bpp = 0;
}


int EggImage::getWidth()
{
	return _width;
}


int EggImage::getHeight()
{
	return _height;
}


int EggImage::getOriginalWidth()
{
  return _original_width;
}


int EggImage::getOriginalHeight()
{
	return _original_height;
}


void EggImage::setColorMode(colormode_t mode)
{
	fprintf(stderr, "EggImage::Mode> ERROR not implemented, %s:%i\n",
			  __FILE__, __LINE__);
}


//FIXME: Add support for 32bit!
int EggImage::setPalette(unsigned char *buffer, int bpp)
{
  if (buffer && buffer != _palette && 
      (bpp == 8 || bpp == 16 || bpp == 24))
  {
    if (_palette)
      delete [] _palette;

    _palette_bpp = bpp;

    _palette = new unsigned char[768];
    memcpy(_palette, buffer, 768);
      
    return 0;
  }

  printf("EggImage::LoadPaletteBuffer()> Assertion Error bpp not {8,16,24}\n");
  return -1;
}


int EggImage::loadIndexedPixmap(unsigned char *image, int width, int height)
{
  int i, size, index;


  if (!image || !_palette || !width || !height || image == _image)
  {
    printf("EggImage::LoadIndexedBuffer> Assertion failed\n");
    return -1;
  }

  if (_image)
    delete [] _image;

  size = width * height;

  _width = _original_width = width;
  _height = _original_height = height;
  _image = new unsigned char[size * 3];

  _color_mode = RGB_24;

  for (i = 0; i < size; ++i)
  {
    index = image[i];

    if (index * 3 + 2 > 768)
      printf("Index outside of palette!\n");
    else
    {
      _image[i*3] = _palette[index*3];
      _image[i*3+1] = _palette[index*3+1];
      _image[i*3+2] = _palette[index*3+2];
    }
  }

  return 0;
}


void EggImage::brightenPalette(float p)
{
  int i;


  if (p <= 0.0 || p > 0.999)
  {
    printf("EggImage::BrightenPalette> %f isn't a postive percentage\n", p);
    return;
  }

  if (_palette)
  {
    for (i = 0; i < 255; ++i)
    {
      _palette[i*3] += (unsigned char)(_palette[i*3] * p);
      _palette[i*3+1] += (unsigned char)(_palette[i*3+1] * p);
      _palette[i*3+2] += (unsigned char)(_palette[i*3+2] * p);
    }    
  }
}


void EggImage::getImage(unsigned char **buffer)
{
  *buffer = NULL;

  if (!_image)
  {
    printf("Image::ImageBuffer> No image allocated\n");
    return;
  }

  switch (_color_mode)
  {
  case RGB_24:
    *buffer = new unsigned char[_width*_height*3];
    memcpy(*buffer, _image, _width*_height*3);
    break;
  case RGBA_32:
    *buffer = new unsigned char[_width*_height*4];
    memcpy(*buffer, _image, _width*_height*4);
    break;
  case INDEXED_8:
    break;
  }
}


void EggImage::getPalette(unsigned char **buffer)
{
  *buffer = NULL;

  if (!_palette)
  {
    printf("Image::PaletteBuffer> No palette allocated\n");
    return;
  }

  *buffer = new unsigned char[768];
  memcpy(*buffer, _palette, 768);
}


void EggImage::getIndexedImage(unsigned char **buffer)
{
  int size = _width * _height;
  float rgb[3];
  int i;


  *buffer = NULL;

  if (!_image || !_palette)
  {
    printf("EggImage::IndexedImage> No image or palette allocated\n");
    return;
  }

  *buffer = new unsigned char[size];

  for (i = 0; i < size; ++i)
  {
    rgb[0] = _image[i*3]/255.0;
    rgb[1] = _image[i*3+1]/255.0;
    rgb[2] = _image[i*3+2]/255.0;

    (*buffer)[i] = matchPaletteColor(rgb);
  }
}

void EggImage::flipVertical()
{
  int i, j;
  unsigned char *swap_row = NULL;


  if (!_image)
    return;

  switch(_color_mode)
  {
  case INDEXED_8:
    // We don't store images as indexed, they're computed
    break;
  case RGB_24:
    swap_row = new unsigned char[_width*3];

    for (i = 0, j = _height-1; i < _height/2; ++i, --j)
    {
      memcpy(swap_row, &_image[i*_width*3], _width*3);
      memcpy(&_image[i*_width*3], &_image[j*_width*3], _width*3);
      memcpy(&_image[j*_width*3], swap_row, _width*3);
    }

    delete [] swap_row;
    break;
  case RGBA_32:
    swap_row = new unsigned char[_width*4];

    for (i = 0, j = _height-1; i < _height/2; ++i, --j)
    {
      memcpy(swap_row, &_image[i*_width*4], _width*4);
      memcpy(&_image[i*_width*4], &_image[j*_width*4], _width*4);
      memcpy(&_image[j*_width*4], swap_row, _width*4);
    }

    delete [] swap_row;
    break;
  }
}


int EggImage::loadPixmap(unsigned char *image, 
								 unsigned int w, unsigned int h,
								 colormode_t mode)
{
  switch (mode)
  {
  case INDEXED_8:
    return loadIndexedPixmap(image, w, h);
    break;
  case RGB_24:
    if (!image || !w || !h || image == _image)
      return -1;  

    _width = _original_width = w;
    _height = _original_height = h;

    if (_image)
      delete [] _image;

    _color_mode = RGB_24;

    _image = new unsigned char[w * h * 3];
    memcpy(_image, image, w * h * 3);

    return 0;
    break;
  case RGBA_32:
    if (!image || !w || !h || image == _image)
      return -1;  

    _width = _original_width = w;
    _height = _original_height = h;

    if (_image)
      delete [] _image;

    _color_mode = RGBA_32;

    _image = new unsigned char[w * h * 4];
    memcpy(_image, image, w * h * 4);

    return 0;
    break;
  }

  return -1;
}


// Palette ////////////////////////////////////////////

// Search palette for matching ( or closest ) rgb color
// Colors *must be 0.0 to 1.0 form!
//
// Note if you have a small palette this may 'wash' to black
// quickly after running through a few times ( 0.0 is default )
unsigned int EggImage::matchPaletteColor(color3_t rgb)
{
  color3_t color;
  float best_weight, current_weight;
  int i, best;


  if (!_palette)
  {
    printf("EggImage::MatchColor> ERROR no palette loaded\n");
    return 0;
  }

  best_weight = current_weight = 0.0;
  
  for (best = 0, i = 0; i < 256; i++)
  {
    // Convert from 0-255 to 0.0-1.0 color scale
    getPaletteColor(i, color);
    //color[0] = _palette[i*3]/255.0;
    //color[1] = _palette[i*3+1]/255.0;
    //color[2] = _palette[i*3+2]/255.0;

    // FIXME: Should be 0.0 (far off) to 1.0 (exact match)
    current_weight = 1.0 - ((fabs(rgb[0] - color[0]) + 
			     fabs(rgb[1] - color[1]) + 
			     fabs(rgb[2] - color[2])) / 3.0);

    if (current_weight > best_weight)
    {
      best_weight = current_weight;
      best = i;

      if (best_weight == 1.0)
	i = 1024; // break;
    }
  }

  return best;
}

void EggImage::getPaletteColor(unsigned int i, color3_t rgb)
{
  if (i > 0 && i < 256 && rgb && _palette)
  {
    rgb[0] = (float)_palette[i*3]/255.0;
    rgb[1] = (float)_palette[i*3+1]/255.0;
    rgb[2] = (float)_palette[i*3+2]/255.0;
  }
}	


int EggImage::loadImage(const char *filename)
{
#ifdef EGGIMAGE_PLUGINS
	EggFileReader reader;
	int (*import)(const char *filename, unsigned char *image,
					  unsigned int *width, unsigned int *height, 
					  char *type);
	bool loaded = false;
	char *module_filename;
	void *handle;
	char *error;
	unsigned char *image;
	unsigned int width, height;
	char type;


	if (!reader.doesFileExist(filename))
	{
		print("File '%s' couldn't be accessed.", filename);
		return -1;
	}

	print("[EggImage plugin system invoked]");

	if (!reader.openDirectory("/usr/local/lib/freyja8/modules/image/"))
	{
		printError("Couldn't access image plugin directory");
		return -2;
	}

	while ((module_filename = reader.getNextDirectoryListing()))
	{
		if (!(handle = dlopen(module_filename, RTLD_NOW))) //RTLD_LAZY)))
		{
			printError("In module '%s'.", module_filename);

			if ((error = dlerror()) != NULL)  
			{
				printError("%s", error);
			}

			continue; /* Try the next plugin, even after a bad module load */
		}
		else
		{
			print("\tModule '%s' opened.", module_filename);
    
			import = (int (*)(const char *filename, unsigned char *image,
									unsigned int *width, unsigned int *height, 
									char *type))dlsym(handle, "import");

			if ((error = dlerror()) != NULL)  
			{
				printError("%s", error);
				dlclose(handle);
				continue;
			}

	      loaded = (loaded || (!(*import)(filename, image, 
													  &width, &height, &type)));

			if ((error = dlerror()) != NULL) 
			{
				dlclose(handle);
				continue;
			}

			dlclose(handle);
		}
	}

	reader.closeDirectory();

	print("[EggPlugin module loader sleeps now]\n");

	if (loaded)
	{
		loadPixmap(image, width, height, (EggImage::colormode_t)type);

		return 0; /* Success! */
	}
#endif

	print("EggImage: This build was compiled w/o plugin support");

	return -1;
}


int EggImage::loadPaletteLMP(const char *filename)
{
  FILE *f;
	

  f = fopen(filename, "rb");

  if (f)
  {
    if (!_palette)
      _palette = new unsigned char[768];

    fread(_palette, 1, 768, f);
    fclose(f);
    return 0;
  }

  return -1;
}

int EggImage::loadPaletteMTK(const char *filename)
{
  FILE *f;
  int i;
  unsigned int rgb[3];


  if (!filename)
    return -1;

  f = fopen(filename, "r");

  if (f)
  {
    if (_palette)
      delete [] _palette;

    _palette = new unsigned char[768];

    for (i = 0; i < 768; i+=3)
    {
      fscanf(f, "%u %u %u\n", 
	     &rgb[0],
	     &rgb[1],
	     &rgb[2]);

      _palette[i]   = rgb[0];
      _palette[i+1] = rgb[1];
      _palette[i+2] = rgb[2];
    }
    
    fclose(f);
    return 0;
  }

  return 1;
}

int EggImage::savePaletteMTK(const char *filename)
{
  FILE *f;
  int i;


  if (!_palette || !filename)
    return -1;

  f = fopen(filename, "w");

  if (f)
  {
    for (i = 0; i < 768; i+=3)
    {
      fprintf(f, "%i %i %i\n", 
	      (int)_palette[i], 
	      (int)_palette[i+1], 
	      (int)_palette[i+2]);
    }
    
    fclose(f);
    return 0;
  }

  return 1;
}

EggImage::colormode_t EggImage::getColorMode()
{
  return _color_mode;
}


/* This code based off on gluScaleImage()  */
void EggImage::scaleImage()
{
   int i, j, k;
   float* tempin;
   float* tempout;
   float sx, sy;
   int components = 3;
   unsigned char *image;
	
	
   if (!_image || !_width || !_height)
		return;
	
   _height = getNextPower(_height);
   _width = getNextPower(_width);
	
   if (_height > 256) 
		_height = 256;
	
   if (_width > 256) 
		_width = 256;
	
	
   // Check to see if scaling is needed
   if (_height == _original_height && _width == _original_width) 
		return;
	
   image = new unsigned char[_height * _width * 3];
	
   tempin = new float[_original_width * _original_height * 
							 components * sizeof(float)];
   tempout = new float[_width * _height * components * sizeof(float)];
	
   if (!tempout || !tempin)
   {
		if (tempout)
			delete [] tempout;
		
		if (tempin)
			delete [] tempin;
		
		printf("Oh shit out of memory!\n");
		return;
   }

   // Copy user data to float format.
   for (i = 0; i < _original_height * _original_width * components; i++) 
   {
		tempin[i] = (float)_image[i];
   }
	
   // Determine which filter to use by checking ratios.
   if (_width > 1) 
   {
		sx = (float)(_original_width - 1) / (float)(_width - 1);
   } 
   else 
   {
		sx = (float)(_original_width - 1);
   }
	
   if (_height > 1)
   {
		sy = (float)(_original_height - 1) / (float) (_height - 1);
   } 
   else 
   {
		sy = (float)(_original_height - 1);
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
		
		for(i = 0; i < _height; i++) 
		{
			i0 = (int)(i * sy);
			i1 = i0 + 1;
			
			if (i1 >= _original_height) 
			{
				i1 = _original_height - 1;
			}
			
			alpha = i * sy - i0;
			
			for (j = 0; j < _width; j++) 
			{
				j0 = (int) (j * sx);
				j1 = j0 + 1;
				
				if (j1 >= _original_width) 
				{
					j1 = _original_width - 1;
				}
				
				beta = j * sx - j0;
				
				/* Compute weighted average of pixels in rect (i0,j0)-(i1,j1) */
				src00 = tempin + (i0 * _original_width + j0) * components;
				src01 = tempin + (i0 * _original_width + j1) * components;
				src10 = tempin + (i1 * _original_width + j0) * components;
				src11 = tempin + (i1 * _original_width + j1) * components;

				dst = tempout + (i * _width + j) * components;
				
				for (k = 0; k < components; k++) 
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
		
		for (i = 0; i < _height; i++) 
		{
			i0 = (int) (i * sy);
			i1 = i0 + 1;
			
			if (i1 >= _original_height) 
			{
				i1 = _original_height - 1;
			}
			
			for (j = 0; j < _width; j++) 
			{
				j0 = (int) (j * sx);
				j1 = j0 + 1;
				
				if (j1 >= _original_width) 
				{
					j1 = _original_width - 1;
				}
				
				dst = tempout + (i * _width + j) * components;
				
				/* Compute average of pixels in the rectangle (i0,j0)-(i1,j1) */
				for (k = 0; k < components; k++) 
				{
					sum = 0.0;
					
					for (ii = i0; ii <= i1; ii++) 
					{
						for (jj = j0; jj <= j1; jj++) 
						{
							sum += *(tempin + (ii * _original_width + jj) 
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
   for( i = 0; i < _height * _width * components; i++ ) 
   {
		image[i] = (unsigned char) tempout[i];
   }
	
   // Delete our temp buffers.
   delete[] tempin;
   delete[] tempout;
   delete[] _image;
	
   _image = image;
}

int EggImage::getNextPower(int seed)
{
  int i;

  for(i = 1; i < seed; i *= 2) 
    ;

  return i;
}


void EggImage::print(char *s, ...)
{
	va_list args;

	va_start(args, s);
	fprintf(stdout, "EggImage::");
	vfprintf(stdout, s, args);
	fprintf(stdout, "\n");
	va_end(args);
}


void EggImage::printError(char *s, ...)
{
	va_list args;
	
	va_start(args, s);
	fprintf(stderr, "EggImage::");
	vfprintf(stderr, s, args);
	fprintf(stderr, "\n");
	va_end(args);
}

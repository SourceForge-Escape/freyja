/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : 
 * License : GPL See file COPYING, also (C) 2000 Mongoose
 * Comments: Png plug-in
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2000-10-15:
 * Mongoose - Created
 ================================================================*/

#include <stdio.h>

#ifdef LIB_PNG
extern "C" {
#include <png.h>
}
#endif

#define MTK_RGB            3
#define MTK_RGBA           4
#define PNG_BYTES_TO_CHECK 4

extern "C" {

	int check(FILE *f);

	int import(const char *filename, unsigned char **image, 
			  unsigned int *w, unsigned int *h, 
			  char *type);
}

int check(FILE *f)
{
#ifdef LIB_PNG
	char buffer[PNG_BYTES_TO_CHECK];


	if (!f)
	{
		perror("png_check> ERROR opening file\n");
		return -1;
	}

	fseek(f, 0, SEEK_SET);

	if (fread(buffer, 1, PNG_BYTES_TO_CHECK, f) != PNG_BYTES_TO_CHECK)
	{
		fprintf(stderr,"png_check> ERROR can't read %i bytes\n", 
				  PNG_BYTES_TO_CHECK);
		return -2;
	}

	if (!(png_sig_cmp((png_byte *)buffer,(png_size_t)0, PNG_BYTES_TO_CHECK)))
	{
      return 0;
	}
	else
	{
		fprintf(stderr, "png_load> Unknown image format.\n");
	}

	return -2;
#else
	return -100;
#endif
}


int import(const char *filename, unsigned char **image, 
			  unsigned int *w, unsigned int *h, 
			  char *type)
{
#ifdef LIB_PNG
	FILE *f = fopen(filename, "rbx");
   int interlace_type, color_type, depth;
   png_uint_32 width, height;
   png_structp png_ptr;
   png_infop info_ptr;
   png_byte **png_image;
   unsigned int i = 0, k = 0, j = 0;
   //unsigned int sig_read = 0;


   if (!f)
   {
		perror("mtk_image__png_load> ERROR: Opening file\n");
		return -1;
   }

	if (check(f) < 0)
	{
		if (f)
			fclose(f);

		return -100;
	}

   fseek(f, 0, SEEK_SET);

   png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

   if (!png_ptr)
   {
		fclose(f);
		fprintf(stderr, "mtk_image__png_load> ERROR: Creating png read struct\n");
		return -2;
   }

   info_ptr = png_create_info_struct(png_ptr);

   if (!info_ptr)
   {
      fclose(f);
      png_destroy_read_struct(&png_ptr, (png_infopp)NULL, (png_infopp)NULL);
      fprintf(stderr, "mtk_image__png_load> ERROR: Creating png info struct\n");
      return -3;
   }


   if (setjmp(png_jmpbuf(png_ptr)))
   {
      png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);
      fclose(f);
      fprintf(stderr, "mtk_image__png_load> ERROR: Can't set jmp\n");
      return -4;
   }

   png_init_io(png_ptr, f);

   //png_set_sig_bytes(png_ptr, sig_read);

   png_read_info(png_ptr, info_ptr);
   png_get_IHDR(png_ptr, info_ptr, &width, &height, &depth, &color_type,
					 &interlace_type, NULL, NULL);

   *w = width;
   *h = height;
   *type = MTK_RGB;
   *image = NULL;

   if (color_type == PNG_COLOR_TYPE_RGB_ALPHA)
	{
		*type = MTK_RGBA;
	}

   /////////////////////////////////////

		//if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
		//    png_set_expand(png_ptr);

		if (!*w || !*h)
		{
			fprintf(stderr, "mtk_image__png_load> ERROR: Invalid dimensions\n");

			fclose(f);
			return -5;
		}

		*image = new unsigned char[*w * *h * *type];

		png_image = new png_byte *[*h];

		for (i = 0; i < *h; ++i)
		{
			png_image[i] = new png_byte[*w * 4];
		}

		png_read_image(png_ptr, png_image);

		for (k = 0, i = 0; i < *h; ++i)
		{
			for (j = 0; j < *w; ++j, ++k)
			{
				if (*type == 4)
				{
					(*image)[k*4  ] = png_image[i][j*4];
					(*image)[k*4+1] = png_image[i][j*4+1];
					(*image)[k*4+2] = png_image[i][j*4+2];
					(*image)[k*4+3] = png_image[i][j*4+3];
				}
				else
				{
					(*image)[k*3  ] = png_image[i][j*3];
					(*image)[k*3+1] = png_image[i][j*3+1];
					(*image)[k*3+2] = png_image[i][j*3+2];
				}
			}
		}

		for (i = 0; i < *h; ++i)
		{
			if (png_image[i])
				delete [] png_image[i];
		}

		if (png_image)
		{
			delete [] png_image;
		}

		//////////////////////////////////////

		png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)NULL);

		fclose(f);

		fprintf(stderr, "mtk_image__png_load> DEBUG %s %ix%i@%ibpp\n", 
				  (*type == MTK_RGB) ? "RBG" : "RBGA", *w, *h, depth);

		return 0;
#else
		return -100;
#endif
}


#ifdef TEST_PNG
int save_ppm(unsigned char *image, unsigned int w, unsigned int h, 
				 unsigned int type)
{
	unsigned int sz, i;


	printf("P3\n");  // ASCII header
	printf("# CREATOR Mongoose's image utils\n");
	printf("%i %i\n", w, h);
	printf("255\n");
  
	sz = w * h;
  
	for (i = 0; i < sz; ++i)
	{
		printf("%i\n", image[i]);
	}
  
	return 0;
}


int main(int argc, char *argv[])
{
	unsigned char *image;
	unsigned int w, h, bpp, type;


	if (argc <= 0)
	{
		printf("%s filename.png > filename.ppm\n", argv[0]);
	}
	else
	{
		if (mtk_image__png_load(argv[1], &image, &w, &h, &bpp, &type) || !image)
		{
			printf("Error loading image\n");
			return -1;
		}

		save_ppm(image, w, h, type);
		delete [] image;
	}
  
	return 0;
}
#endif

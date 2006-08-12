/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : 
 * License : GPL See file COPYING, also (C) 2000 Mongoose
 * Comments: jpeg plug-in
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2000-10-15:
 * Mongoose - Created
 ==========================================================================*/


#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

#ifdef HAVE_LIBJPEG
#   include <jpeglib.h>
#endif

extern "C" {
	int import_image(char *filename, unsigned char **image, 
						  unsigned int *w, unsigned int *h, char *bpp);
}


#ifdef HAVE_LIBJPEG
int mtk_image__jpeg_check(FILE *f)
{
	unsigned char buffer[16];


	if (!f)
	{
		perror("jpeg.so: ERROR opening file\n");
		return -1;
	}

	fseek(f, 0, SEEK_SET);
	fread(buffer, 1, 6, f);
	fseek(f, 0, SEEK_SET);

	if (buffer[0] == 0xff && buffer[1] == 0xd8)
		return 0;

	printf("jpeg.so: Inavlid or unknown JPEG format.\n");

	return -2;
}


typedef struct stupid_jpeg_error_mgr 
{
  struct jpeg_error_mgr dickheads;
  jmp_buf setjmp_buffer;
  int padding[8];

} stupid_jpeg_error_mgr_t;


void stupid_jpeg_error_exit(j_common_ptr image)
{
   stupid_jpeg_error_mgr_t *err = (stupid_jpeg_error_mgr_t *)image->err;

	(*image->err->output_message)(image);

	longjmp(err->setjmp_buffer, 1);
}
#endif


int import_image(char *filename, unsigned char **imageRET, 
					  unsigned int *w, unsigned int *h, char *bpp)
{
#ifdef HAVE_LIBJPEG
	struct jpeg_decompress_struct image;
	struct stupid_jpeg_error_mgr jerr;
	//JSAMPROW rows[1], bptr;
	JSAMPARRAY buffer;
	unsigned char *imageb = 0x0;
	FILE *f;
	unsigned int i, j, width, height, depth;
	
	
	f = fopen(filename, "rb");

	if (!f)
	{
		perror("jpeg.so: ERROR\n");
		return -1;
	}

	if (mtk_image__jpeg_check(f))
	{
		fclose(f);
		return -1;
	}

	fseek(f, 0, SEEK_SET);
	image.err = jpeg_std_error(&jerr.dickheads);
	jerr.dickheads.error_exit = stupid_jpeg_error_exit;
	
	if (setjmp(jerr.setjmp_buffer)) 
	{
		fprintf(stderr, "jpeg.so: stupid_jpeg jmp called\n");
		jpeg_destroy_decompress(&image);
		fclose(f);
		return -1;
	}
	
	jpeg_create_decompress(&image);
	jpeg_stdio_src(&image, f);
	jpeg_read_header(&image, TRUE);
  
	if (image.image_width < 1 || image.image_height < 1)
	{    
		fprintf(stderr, "jpeg.so: ERROR invalid image.\n");
		jpeg_destroy_decompress(&image);
		fclose(f);
		return -2;
	}

   switch (image.jpeg_color_space)
	{
	case JCS_RGB:
		break;
	case JCS_GRAYSCALE:
		image.out_color_space = JCS_RGB;
		break;
	default:
		fprintf(stderr, "jpeg.so: Unknown colorspace.\n");
		//image.out_color_space = JCS_RGB;
		//return -1;
	}

	image.quantize_colors = FALSE;
	image.do_fancy_upsampling = FALSE;
	image.do_block_smoothing = FALSE;
	jpeg_calc_output_dimensions(&image);
	jpeg_start_decompress(&image);

	width = image.output_width;
	height = image.output_height;
	depth = image.output_components;

	if (width < 1 || height < 1 || depth < 3)
	{
		fprintf(stderr, "jpeg.so: ERROR libjpeg refuses to output color image.\n");
		jpeg_destroy_decompress(&image);
		fclose(f);
		return -2;
	}

	buffer = ((*image.mem->alloc_sarray)
				 ((j_common_ptr)&image, JPOOL_IMAGE, width * depth, 1));
	//bptr = rows[0] = (JSAMPROW)malloc(width * depth);

	if (!buffer)
	//if (!rows[0])
	{
		fprintf(stderr, "jpeg.so: ERROR libjpeg didn't allocate scanline memory.\n");
		jpeg_destroy_decompress(&image);
		fclose(f);
		return -1;
	}

	imageb = new unsigned char[width * height * 3];
		
	j = 0;
	
	while (image.output_scanline < height) 
	{
		jpeg_read_scanlines(&image, buffer/*rows*/, 1); //(JDIMENSION)1);

		//	bptr = rows[0];

		for (i = 0; i < width && j < width * height; ++i) 
		{
			//imageb[j*3] = (unsigned char)*bptr++;
			//imageb[j*3+1] = (unsigned char)*bptr++;
			//imageb[j*3+2] = (unsigned char)*bptr++;

			imageb[j*3] = (unsigned char)buffer[0][i*3];
			imageb[j*3+1] = (unsigned char)buffer[0][i*3+1];
			imageb[j*3+2] = (unsigned char)buffer[0][i*3+2];
			j++;
		}
	}

	/* Clean up */
	jpeg_finish_decompress(&image);
	jpeg_destroy_decompress(&image);


	//free(rows[0]);

	fclose(f);


	/* Return new RBG pixmap */
	*w = width;
	*h = height;
	*imageRET = imageb;
	*bpp = 3;

	imageb = 0x0;

	return 0;
	
#else
#   ifdef JPEG_BY_PIPES
	/* TODO: Replace libjpeg entirely */

	FILE *f = popen("/tmp/jpeg.test", "rb");
	int err = 0;


	if (!f)
	{
		return -1;
	}

	err = ppm_import_image(f, imageRET, w, h, bpp);	

	if (pclose(f) < 0)
		;      /* This could have an error as well closing the fork and pipe. */

	return err;
#   else
	fprintf(stderr, "JPEG support not in this build\n");
	return -100;
#   endif
#endif
}


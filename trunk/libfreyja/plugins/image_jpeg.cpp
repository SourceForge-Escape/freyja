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

#ifdef LIB_JPEG
extern "C" 
{
  #include <ctype.h>
  #include <string.h>
  #include <stdio.h>
  #include <stdlib.h>
  #include <unistd.h>
  #include <jpeglib.h>
}
#endif


extern "C" {
	int import_image(char *filename, unsigned char **image, 
						  unsigned int *w, unsigned int *h, char *bpp);
}


int mtk_image__jpeg_check(FILE *f)
{
#ifdef LIB_JPEG
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
#endif

	return -2;
}


int import_image(char *filename, unsigned char **imageRET, 
					  unsigned int *w, unsigned int *h, char *bpp)
{
#ifdef LIB_JPEG
	struct jpeg_decompress_struct image;
	struct jpeg_error_mgr error;
	JSAMPROW buffer[1], bptr;
	unsigned char *imageb = NULL;
	FILE *f;
	unsigned int i, j, width, height;
	
	
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

	image.err = jpeg_std_error(&error);
	jpeg_create_decompress(&image);
	jpeg_stdio_src(&image, f);
	jpeg_read_header(&image, TRUE);
  
	if (image.image_width < 1 || image.image_height < 1)
	{
		fclose(f);
		return -2;
	}

   switch (image.jpeg_color_space)
	{
	case JCS_GRAYSCALE:
		//image.out_color_space = JCS_GRAYSCALE;
		//break;
	default:		 
		image.out_color_space = JCS_RGB;
	}

	image.quantize_colors = FALSE;
	image.do_fancy_upsampling = FALSE;
	image.do_block_smoothing = FALSE;
	jpeg_calc_output_dimensions(&image);
  
	bptr = buffer[0] = (JSAMPROW)malloc(image.image_width*image.num_components);

	if (!buffer[0])
	{
		fclose(f);
		return -1;
	}

	jpeg_start_decompress(&image);

	width = image.image_width;
	height = image.image_height;
	imageb = new unsigned char[image.image_width * image.image_height * 3];
		
	j = 0;
	
	while (image.output_scanline < image.output_height) 
	{
		jpeg_read_scanlines(&image, buffer, (JDIMENSION)1);

		bptr = buffer[0];

		for (i = 0; i < image.image_width; ++i) 
		{
			imageb[j*3] = (unsigned char)bptr[i*3];
			imageb[j*3+1] = (unsigned char)bptr[i*3+1];
			imageb[j*3+2] = (unsigned char)bptr[i*3+2];
			j++;
		}
	}

	/* Clean up */
	jpeg_finish_decompress(&image);
	jpeg_destroy_decompress(&image);
	fclose(f);

	if (buffer[0])
		free(buffer[0]); 


	/* Return new RBG pixmap */
	*w = width;
	*h = height;
	*imageRET = imageb;
	*bpp = 3;

	return 0;
	
#else
	printf("JPEG support not in this build\n");
	return -100;
#endif
}


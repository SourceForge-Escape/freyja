/*================================================================
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
 ================================================================*/


#include "image_jpeg.h"

#ifdef LIB_JPEG
extern "C" 
{
  #include <ctype.h>
  #include <string.h>
  #include <unistd.h>
  #include <jpeglib.h>
}
#endif


int mtk_image__jpeg_check(FILE *f)
{
#ifdef LIB_JPEG
  unsigned char buffer[16];


  if (!f)
  {
    perror("jpeg_check> ERROR opening file\n");
    return -1;
  }

  fseek(f, 0, SEEK_SET);
  fread(buffer, 1, 6, f);

  if (buffer[0] == 0xff && buffer[1] == 0xd8)
    return 0;
  else
    fprintf(stderr, "jpeg_load> Unknown image format.\n");

  return -2;
#else
  return -100;
#endif
}


int mtk_image__jpeg_load(FILE *f, unsigned char **imageRET, 
			 unsigned int *w, unsigned int *h, char *type)
{
#ifdef LIB_JPEG
  unsigned char *buffer = NULL;
  unsigned char *imageb = NULL;
  unsigned char *palette = NULL;
  struct jpeg_decompress_struct image;
  struct jpeg_error_mgr error;
  unsigned int i, j;
  int r, b, g, bpp;


  if (!f)
  {
    perror("jpeg_load> ERROR:\n");
    return -1;
  }

  fseek(f, 0, SEEK_SET);

  *type = 1;
  bpp = 16; // hack hack, fix for 011. loss of color in release build

  image.err = jpeg_std_error(&error);
  jpeg_create_decompress(&image);
  jpeg_stdio_src(&image, f);
  jpeg_read_header(&image, true);
  
  *w = image.image_width;
  *h = image.image_height;

  if (!*w || !*h)
    return -2;

  if (bpp <= 8)
  {
    image.quantize_colors = true;
    image.dither_mode = JDITHER_FS;
    image.desired_number_of_colors = 255;
  }
  
  jpeg_calc_output_dimensions(&image);
  
  /*
  while ((_width > 255) || (_height > 255)) 
  {
    image.scale_denom++;
    jpeg_calc_output_dimensions(&image);
  }
  */

  buffer = new unsigned char[*w * image.output_components];

  jpeg_start_decompress(&image);


  switch (bpp)
  {
  case 8:
    //printf("8bit JPEG.\n");
    //_color_mode = GL_RGB;
    //_palette_bpp = 24;
    
    j = image.actual_number_of_colors;
    palette = new unsigned char[j*3];


    for (i = 0; i < j; i++) 
    {
      palette[i*3]   = image.colormap[0][i];// << 8;
      palette[i*3+1] = image.colormap[1][i];// << 8;
      palette[i*3+2] = image.colormap[2][i];// << 8;
    }

    imageb = new unsigned char[*w * *h * 3];
          
    i = 0;
      
    // FIXME: This should put colored pixels in, not indexed
    while (image.output_scanline < (unsigned)*h)
    {
      jpeg_read_scanlines(&image, &buffer, 1);
      
      memcpy(&imageb[i * *w], buffer, *w);
      i++;
    }
  default:
    //printf("16+bit JPEG.\n");
    //   _color_mode = GL_RGB;
    //   _palette_bpp = 24;

    imageb = new unsigned char[*w * *h * 3];

    j = 0;

    while (image.output_scanline < (unsigned)*h) 
    {
      jpeg_read_scanlines(&image, &buffer, 1);
                        
      for (i = 0; i < *w; i++) 
      {
	r = buffer[i * image.output_components];// << 8;

	if (image.output_components ^ 1) 
	{
	  g = buffer[(i * image.output_components) + 1];//  << 8;
	  b = buffer[(i * image.output_components) + 2];//  << 8;
        } 
	else 
	  g = b = r;

	imageb[j*3] = (unsigned char)r;
	imageb[j*3+1] = (unsigned char)g;
	imageb[j*3+2] = (unsigned char)b;
	j++;
      }
    }
  }

  if (buffer)  
    delete [] buffer; 

  if (palette)  
    delete [] palette; 

  jpeg_finish_decompress(&image);
  jpeg_destroy_decompress(&image);

  *imageRET = imageb;
  return 0;
#else
  printf("JPEG support not in this build\n");
  return -100;
#endif
}

#ifdef TEST_JPEG
int main(int argc, char *argv[])
{
  unsigned char *image;
  int w, h, bpp, i;


  if (argc > 1)
  {
    image = read_jpeg(argv[1], &w, &h, &bpp);
   
    printf("P3\n");  // ASCII PPM header
    printf("# CREATOR Mongoose, from %s\n", argv[1]);
    printf("%i %i\n", w, h);
    printf("255\n");

    if (image)
    {
      for (i = 0; i < w*h; i++)
	printf("%i\n%i\n%i\n",  image[i*3], image[i*3+1], image[i*3+2]);

      delete [] image;
    }
  }
  else
    printf("%s filename.jpeg > filename.ppm\n", argv[0]);

  return 0;
}
#endif

/*================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : 
 * License : GPL See file COPYING, also (C) 2000 Mongoose
 * Comments: TGA plug-in
 *
 *           TODO: type should pass more info
 *                 2 bits for RGBA | RGB | GREY
 *                 val for depth
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2001-10-25:
 * Mongoose - support for screen origin bit
 *
 * 2000-10-15:
 * Mongoose - Created
 ================================================================*/

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "tga.h"

extern "C" {

  int check(FILE *f);
  
  int import_image(char *filename, unsigned char **image, 
		   unsigned int *w, unsigned int *h, 
		   char *type);

  int freyja_image_export__tga(char *filename, unsigned char *image, 
			       unsigned int w, unsigned int h, 
			       char type);
}

int check(FILE *f)
{
  char buffer[10];


  if (!f)
  {
    perror("tga.so: ERROR Passed invalid file.\n");
    return -1;
  }

  /* Read the header */
  fseek(f, 0, SEEK_SET);
  fread(buffer, 8, 1, f);

  // buffer[1] = 0 - Means not color mapped ( 1 would mean mapped )
  if (!(buffer[1] == 0 && (buffer[2] == TGA_TYPE__COLOR ||
			   //buffer[2] == TGA_TYPE__GREYSCALE ||
			   buffer[2] == TGA_TYPE__COLOR_RLE)))
  {
    printf("tga.so: Inavlid or unknown TGA format.\n");
    return -2;
  }

  return 0;
}


int import_image(char *filename, unsigned char **image, 
		 unsigned int *width, unsigned int *height, char *type)
{
  FILE *f = fopen(filename, "rb");
  mtk_image_tga_t header;
  char comment[256];
  unsigned char pixel[4];
  unsigned char *swap_row = NULL;
  unsigned char tmp, packet;
  bool must_flip = 0;
  unsigned int size;
  unsigned int i, j;


  if (!f)
  {
    fprintf(stderr, "mtk.so: Invalid file.\n");
    return -1;
  }

  if (check(f))
  {
    fclose(f);
    return -1;
  }

  fseek(f, 0, SEEK_SET);

  // Read TGA header
  fread(&header.comment_lenght, 1, 1, f);
  fread(&header.colormap_type, 1, 1, f); 
  fread(&header.image_type, 1, 1, f);
  fread(&header.colormap_index, 2, 1, f);
  fread(&header.colormap_lenght, 2, 1, f);
  fread(&header.colormap_bbp, 1, 1, f);

  fread(&tmp, 1, 1, f);
  header.origin_x = tmp;
  fread(&tmp, 1, 1, f);
  header.origin_x += tmp * 256;

  fread(&tmp, 1, 1, f);
  header.origin_y = tmp;
  fread(&tmp, 1, 1, f);
  header.origin_y += tmp * 256;

  fread(&tmp, 1, 1, f);
  header.width = tmp;
  fread(&tmp, 1, 1, f);
  header.width += tmp * 256;

  fread(&tmp, 1, 1, f);
  header.height = tmp;
  fread(&tmp, 1, 1, f);
  header.height += tmp * 256;

  fread(&header.bpp, 1, 1, f);
  fread(&header.desc_flags, 1, 1, f);

  *width = header.width;
  *height = header.height;

  switch (header.bpp)
  {
  case 32:
    *type = 4;//32;
    break;
  case 24:
    *type = 3;//24;
    break;
  case 8:
    *type = 1;//8;
    break;
  default:
    *type = 0;
  }

  printf("TGA [%ix%i@%ibpp, %it, %ix, %iy, %uf]\n", 
	 header.width, header.height, header.bpp, header.image_type,  
	 header.origin_x, header.origin_y,
	 header.desc_flags);

  // Comments can be 0 - 255
  if (header.comment_lenght) 
  {
    fread(&comment, 1, header.comment_lenght, f);
    comment[255] = 0;
    printf("Comment: %s\n", comment);
  }

  size = header.width * header.height;

  if (!size || (!(header.colormap_type == 0 && 
		  (header.image_type == 2 || header.image_type == 10))))
  {
    fprintf(stderr, "mtk_image__tga_load> Unknown image format.\n");
    return -2;
  }

  *image = NULL;

  // Mongoose: Added 'screen origin bit' support back here
  if (!(header.desc_flags & 32))
  {
    must_flip = true;
  }

  switch (header.bpp)
  {
  case 32:
    size *= 4;
    *image = new unsigned char [size];

    switch (header.image_type)
    {
    case TGA_TYPE__COLOR_RLE:
      for (i = 0; i < size;)
      {
	fread(&packet, 1, 1, f);
	
	if (packet & 0x80)  // Run Lenght
	{
	  packet = (packet &0x7F) + 1;

	  fread(&pixel, 4, 1, f);

	  for (j = 0; j < packet; j++)
	  {
	    (*image)[i++] = pixel[2];
	    (*image)[i++] = pixel[1];
	    (*image)[i++] = pixel[0];
	    (*image)[i++] = pixel[3];
	  }
	}
	else // RAW
	{
	  packet = (packet &0x7F) + 1;
	  
	  for (j = 0; j < packet; j++)
	  {
	    fread(&pixel, 4, 1, f);

	    (*image)[i++] = pixel[2];
	    (*image)[i++] = pixel[1];
	    (*image)[i++] = pixel[0];
	    (*image)[i++] = pixel[3];
	  }
	}
      }
      break;
    case TGA_TYPE__COLOR:
      if (fread((*image), size, 1, f) < 1)
      {
	fprintf(stderr, "mtk_image__tga_load> Image fread failed.\n");
	delete [] *image;
	return -4;
      }

      for (i = 0; i < size; i += 4)
      { 
	tmp = (*image)[i];
	(*image)[i] = (*image)[i + 2];
	(*image)[i + 2] = tmp;
      }
      break;
    default:
      ;
    }

    if (must_flip)
    {
      swap_row = new unsigned char [header.width * 4];
      
      for (i = 0, j = ((header.height < 2) ? 0 : header.height - 1); 
			(int)i < header.height/2; i++, j--)
      {
	memcpy(swap_row, &(*image)[i*header.width*4], header.width*4);
	memcpy(&(*image)[i*header.width*4], &(*image)[j*header.width*4], 
	       header.width*4);
	memcpy(&(*image)[j*header.width*4], swap_row, header.width*4);
      }
			
      delete [] swap_row;
    }
    break;
  case 24:
    size *= 3;
    *image = new unsigned char [size];

    switch (header.image_type)
    {
    case TGA_TYPE__COLOR_RLE:
      for (i = 0; i < size;)
      {
	fread(&packet, 1, 1, f);

	if (packet & 0x80)  // Run Lenght
	{
	  packet = (packet &0x7F) + 1;

	  fread(&pixel, 3, 1, f);

	  for (j = 0; j < packet; j++)
	  {
	    (*image)[i++] = pixel[2];
	    (*image)[i++] = pixel[1];
	    (*image)[i++] = pixel[0];
	  }
	}
	else // RAW
	{
	  packet = (packet &0x7F) + 1;

	  for (j = 0; j < packet; j++)
	  {
	    fread(&pixel, 3, 1, f);

	    (*image)[i++] = pixel[2];
	    (*image)[i++] = pixel[1];
	    (*image)[i++] = pixel[0];
	  }
	}
      }
      break;
    case TGA_TYPE__COLOR:
      if (fread((*image), size, 1, f) < 1)
      {
	fprintf(stderr, "mtk_image__tga_load> Image fread failed.\n");
	delete [] *image;
	return -4;
      }

      for (i = 0; i < size; i += 3)
      {
	tmp = (*image)[i];
	(*image)[i] = (*image)[i + 2];
	(*image)[i + 2] = tmp;
      }
      break;
    default:
      ;
    }

    if (must_flip)
    {
      swap_row = new unsigned char [header.width * 3];
      
      for (i = 0, j = header.height - 1; (int)i < header.height / 2; i++, j--)
      {
	memcpy(swap_row, &(*image)[i*header.width*3], header.width*3);
	memcpy(&(*image)[i*header.width*3], &(*image)[j*header.width*3], 
	       header.width*3);
	memcpy(&(*image)[j*header.width*3], swap_row, header.width*3);
      }

      delete [] swap_row;
    }

    break;
  case 8: 
    printf("tga_load> 8bpp Not implemented\n");
    break;
  default:
    ;
  }
    
#ifdef DEBUG_TGA
  char c;

  printf("Comment:\n");

  while (fread(&c, 1, 1, f) == 1)
  {
    printf("%c", c);
  }

  printf("\n");
#endif

  fclose(f);

  return 0; 
}


int mtk_image__tga_save(FILE *f, unsigned char *image,
			unsigned int width, unsigned int height, char type);

int freyja_image_export__tga(char *filename, unsigned char *image, 
			     unsigned int w, unsigned int h, char bbp)
{
  long ret;
  FILE *f;

  f = fopen(filename, "wb");

  if (!f)
  {
    perror(filename);
    return -1;
  }

  ret = mtk_image__tga_save(f, image, w, h, bbp);

  fclose(f);

  return ret;
}


int mtk_image__tga_save(FILE *f, unsigned char *image,
			unsigned int width, unsigned int height, char type)
{
  mtk_image_tga_t header;
  unsigned int i, size;
  unsigned char tmp;
  char comment[64];


  if (!f || !image || !width || !height)
  {
    fprintf(stderr, "mtk_image__tga_save> Invalid parameters.\n");
    return -1;
  }

  strcpy(comment, "Mongoose TGA module 0.0.1");
  header.comment_lenght = strlen(comment);
  header.colormap_type = 0;
  header.image_type = 2;
  
  // No colormaps
  header.colormap_index = 0;
  header.colormap_lenght = 0;
  header.colormap_bbp = 0;

  header.origin_x = header.origin_y = 0;
  header.width = width;
  header.height = height;

  switch (type)
  {
  case 4:
    header.bpp = 32;
    break;

  case 3:
    header.bpp = 24;
    break;

  default:
    header.bpp = 24; // so wrong
  }

  header.desc_flags = 32; // flip me vertically on load he says

  // Write TGA header
  fwrite(&header.comment_lenght, 1, 1, f);
  fwrite(&header.colormap_type, 1, 1, f); 
  fwrite(&header.image_type, 1, 1, f);
  fwrite(&header.colormap_index, 2, 1, f);
  fwrite(&header.colormap_lenght, 2, 1, f);
  fwrite(&header.colormap_bbp, 1, 1, f);
  fwrite(&header.origin_x, 2, 1, f);
  fwrite(&header.origin_y, 2, 1, f);
  fwrite(&header.width, 2, 1, f);
  fwrite(&header.height, 2, 1, f);
  fwrite(&header.bpp, 1, 1, f);
  fwrite(&header.desc_flags, 1, 1, f);

  // Write comment
  fwrite(&comment, 1, header.comment_lenght, f);

  size = header.width * header.height;

  switch (header.bpp)
  {
  case 32:
    size = header.width * header.height * 4;
 
    for (i = 0; i < size; i += 4)
    {
      tmp = image[i];
      image[i] = image[i + 2];
      image[i + 2] = tmp;
    }
    break;


  case 24:
    size = header.width * header.height * 3;
 
    for (i = 0; i < size; i += 3)
    {
      tmp = image[i];
      image[i] = image[i + 2];
      image[i + 2] = tmp;
    }
    break;
  }

  // Write image data
  if (fwrite(image, size, 1, f) < 1)
  {
    perror("mtk_image__tga_save> Disk write failed.\n");
    return -2;
  }

  return 0;
}

#ifdef __TEST_
int main(int argc, char *argv[])
{
  FILE *f;
  unsigned char *image;
  unsigned int width;
  unsigned int height; 
  char type;

  if (argc > 1)
  {
    f = fopen(argv[1], "r");

    if (!f)
    {
      perror("Failed to open file> ");
      return -1;
    }

    if (!mtk_image__tga_check(f))
    {
      if (!mtk_image__tga_load(f, &image, &width, &height, &type))
	delete [] image;
    }

    fclose(f);
  }

  return 0;
}
#endif

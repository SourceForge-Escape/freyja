/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http:/icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : GPL See file COPYING, also (C) 2000 Mongoose
 * Comments: TGA plug-in
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

#ifndef GUARD_FREYJA__MONGOOSE_MTK_TGA_H
#define GUARD_FREYJA__MONGOOSE_MTK_TGA_H

#include <stdio.h>


typedef enum 
{
  TGA_TYPE__NO_DATA    = 0,
  TGA_TYPE__MAPPED     = 1,
  TGA_TYPE__COLOR      = 2,
  TGA_TYPE__GREYSCALE  = 3,
  TGA_TYPE__MAPPED_RLE = 9,
  TGA_TYPE__COLOR_RLE  = 10
  //  TGA_TYPE__GREYSCALE_COMPRESSED  = 11,
  //  TGA_TYPE__COLOR_HUFFMAN_DELTA_RLE  = 32,
  //  TGA_TYPE__COLOR_HUFFMAN_DELTA_RLE_4PASS  = 33

} tga_type_t;


typedef struct mtk_image_tga_s 
{
  unsigned char comment_lenght;       /* Number of bytes in comment */
  unsigned char colormap_type;        /* 0 No colormap; 1 Has colormap  */
  unsigned char image_type;           /* 1 Colormapped, 2 Unmapped;
					 9 Colormapped RLE; 10 Unmapped RLE */
  
  unsigned short colormap_index;      /* Index of first color map entry */
  unsigned short colormap_lenght;     /* Number of color map entries */
  unsigned char colormap_bbp;         /* 16, 24, or 32 bits per pixel format */
  
  unsigned short origin_x;            /* X coor of lower-left corner */
  unsigned short origin_y;            /* Y coor of lower-left corner */
  unsigned short width;               /* Width in pixels */
  unsigned short height;              /* Height in pixels */
  unsigned char bpp;                  /* Number of bits in a pixel index */
  unsigned char desc_flags;           /* Various magic bits */
  
} mtk_image_tga_t;


  int mtk_image__tga_check(FILE *f);
  /*------------------------------------------------------
   * Pre  : Filename is to be checked
   * Post : Returns 0 if valid, else error condition
   *
   *-- History ------------------------------------------
   *
   * 2001.04.01:
   * Mongoose - Created
   ------------------------------------------------------*/

  int mtk_image__tga_load(FILE *f, unsigned char **image, 
			 unsigned int *width, unsigned int *height, char *type);
  /*------------------------------------------------------
   * Pre  : Filename is a valid image file
   * Post : Image is either a valid pixmap or NULL
   *
   *-- History ------------------------------------------
   *
   * 2001.04.01:
   * Mongoose - Created
   ------------------------------------------------------*/

  int mtk_image__tga_save(FILE *f, unsigned char *image, 
			  unsigned int width, unsigned int height, char type);
  /*------------------------------------------------------
   * Pre  : Filename is a valid image file
   * Post : Image is either a valid pixmap or NULL
   *
   *-- History ------------------------------------------
   *
   * 2001.04.01:
   * Mongoose - Created
   ------------------------------------------------------*/
#endif

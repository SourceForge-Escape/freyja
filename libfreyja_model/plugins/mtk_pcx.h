/*================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : 
 * License : GPL See file COPYING, also (C) 2000 Mongoose
 * Comments: Pcx plug-in
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

#ifndef __MONGOOSE_PCX_H
#define __MONGOOSE_PCX_H

#include <stdio.h>

typedef struct pcx_header_s
{
    char signature;
    char version;
    char encoding;
    char bytes_per_pixel;
    unsigned short int xmin;
    unsigned short int ymin;
    unsigned short int xmax;
    unsigned short int ymax;
    unsigned short int vres;
    unsigned short int hres;
    char palette[48];
    char reserved;
    char color_layers;
    unsigned short int bytes_per_line;
    unsigned short int palette_type;
    char unused[58];

} pcx_header_t;


   int mtk_image__pcx_check(FILE *f);
  /*------------------------------------------------------
   * Pre  : Filename is to be checked for pcx header
   * Post : Returns 0 if pcx, else error condition
   *
   *-- History ------------------------------------------
   *
   * 2001.04.01:
   * Mongoose - Created
   ------------------------------------------------------*/

   int mtk_image__pcx_load(FILE *f, unsigned char **image, 
			 unsigned int *width, unsigned int *height, char *type);
  /*------------------------------------------------------
   * Pre  : Filename is a valid pcx file
   * Post : Image is either a valid pixmap or NULL
   *
   *-- History ------------------------------------------
   *
   * 2001.04.01:
   * Mongoose - Created
   ------------------------------------------------------*/
#endif

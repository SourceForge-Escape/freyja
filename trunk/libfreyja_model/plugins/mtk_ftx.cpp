/*================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : 
 * License : GPL See file COPYING, also (C) 2000 Mongoose
 * Comments: Ftx plug-in
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
//#include "mtk_ftx.h"


int mtk_image__ftx_check(char *filename)
{
  FILE *f;

  if (!filename)
  {
    fprintf(stderr, "ftx_check> ERROR Invalid filename\n");
    return -1;
  }

  f = fopen(filename, "rb");

  if (!f)
  {
    fprintf(stderr, "ftx_check> ERROR Can't read file\n");
    return -1;
  }

  if (mtkExtCheck(filename, ".ftx") != 0)
    return -2;

  return 0;
}


int mtk_image__ftx_load(char *filename, unsigned char **image, 
			int *w, int *h, int *bpp)
{
  FILE *f;
  unsigned char *image;
  unsigned int w, h, sz;
  

  if (!filename)
    return - 1;

  if (mtkExtCheck(filename, ".ftx") != 0)
    return -2;

  f = fopen(filename, "rb");

  if (!f)
    return -3;

  fread(&w, 4, 1, f);
  fread(&h, 4, 1, f);

  sz = w*h*4;

  image = new unsigned char[sz];

  if (1 != fread(image, sz, 1, f))
  {
    perror("ftx_load> Read Error ");
    
    if (_image)
      delete [] _image;

    _original_width = _width = 0;
    _original_height = _height = 0;    

    return -4;
  }

  _color_mode = COLORMODE_RGBA;
  _original_width = _width = w;
  _original_height = _height = h;      
  _image = image;

  return 0;
}

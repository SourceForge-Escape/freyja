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

#include "mtk_pcx.h"


int mtk_image__pcx_check(FILE *f)
{
  unsigned char signature, version;


  if (!f)
  {
    fprintf(stderr, "pcx_check> ERROR Can't read file\n");
    return -1;
  }

  /* Read the header */
  fseek(f, 0, SEEK_SET);
  fread(&signature, sizeof(unsigned char), 1, f);
  fread(&version, sizeof(unsigned char), 1, f);

  if ((signature != 0x0a) || (version != 5))
  {
    fprintf(stderr, "pcx_load> Unknown image format.\n");
    return -2;
  }

  return 0;
}


int mtk_image__pcx_load(FILE *f, unsigned char **image, 
			unsigned int *w, unsigned int *h, char *type)
{
   unsigned char *indexed_image = NULL;
   unsigned char *palette = NULL;
   pcx_header_t header;
   int size, i, index;
   unsigned char buf;
   unsigned int counter;


   if (!f) 
   {
      perror("pcx_load> ERROR Opening file.\n");
      return -2;
   }

   *image = NULL;

   fseek(f, 0, SEEK_SET);
   fread(&header, sizeof(pcx_header_t), 1, f);    /* Read the header */

   if ((header.signature != 0x0a) || (header.version != 5))
   {
     printf("pcx_load> ERROR Can't read this PCX format.\n");
     return -3;
   }
   else
   {
     palette = NULL;
     palette = new unsigned char[768];
  
     /* Return height and length */
     *w = header.xmax + 1 - header.xmin;
     *h = header.ymax + 1 - header.ymin;

     size = *w * *h;

     indexed_image = new unsigned char[size];

     if (!indexed_image)
     {
       printf("mtk_image__pcx_load> ERROR Malloc failed for indexed_image.\n");
       return -4;
     }

     i = 0;

     while (i <= size)  /* Read the indexed image from file */
     {
       /* Get one byte */
       fread(&buf, 1, 1, f);

       /* Check the 2 most significant bits */
       if ((buf & 192) == 192)
       {
	 /* We have 11xxxxxx */
	 counter = (buf & 63); /* Number of times to repeat next byte */

	 fread(&buf, 1, 1, f);     /* Get next byte */

	 for(; counter > 0; counter--) /* and copy it counter times */
	 {
	   indexed_image[i] = buf;
	   i++;   /* increase the number of bytes written */
	 }
       }
       else
       {
	 /* Just copy the byte */
	 indexed_image[i] = buf;

	 i++;   /* Increase the number of bytes written */
       }
     }

     /* Read the palette index */
     if (palette)
     {
        fseek(f, -768, SEEK_END);
        fread(palette, 1, 768, f);
     }
     else
     {
       if (indexed_image)   
	 delete [] indexed_image;

       printf("pcx_load> ERROR Malloc failed for palette.\n");
       return -5;
     }

     *image = new unsigned char[size * 3];

     if (!*image)
     {
       printf("pcx_load> ERROR Malloc failed for image.\n");

       if (indexed_image)
	 delete [] indexed_image;

       if (palette)
	 delete [] palette;

       return -5;
     }

     /* Convert to RGB */
     for (i = 0; i < size; i++)
     {
       index = indexed_image[i];
	
       if (index * 3 + 2 > 768)
	 printf("pcx_load> Oops, not using expected pal!\n");
       else
       {
	 (*image)[i*3  ] = palette[index*3];
	 (*image)[i*3+1] = palette[index*3+1];
	 (*image)[i*3+2] = palette[index*3+2];
       }
     }

     if (indexed_image)
       delete [] indexed_image;

     fclose(f);

     *type = 1;
     return 0;
   }

   return -6;
}

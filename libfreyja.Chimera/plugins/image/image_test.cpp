/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */

#include <stdio.h>

extern "C" {
	int import_image(char *filename, unsigned char **image, 
						  unsigned int *w, unsigned int *h, char *bpp);
}


int main(int argc, char *argv[])
{
	unsigned char *image;
	unsigned int w, h, i;
	char bpp;


	if (argc > 1)
	{
		/* Import RBG[A] pixmap */
		if (!import_image(argv[1], &image, &w, &h, &bpp) && 
			 (image && w > 0 && h > 0 && bpp > 0))
		{
			/* Dump ASCII PPM header */
			printf("P3\n");
			printf("# CREATOR Mongoose, from %s\n", argv[1]);
			printf("%i %i\n", w, h);
			printf("255\n");
			
			/* Dump ASCII PPM image */
			for (i = 0; i < w*h; i++)
			{
				printf("%i\n%i\n%i\n",  
						 image[i*bpp], image[i*bpp+1], image[i*bpp+2]);
			}

			delete [] image;
		}
		else
		{
			printf("Error loading image\n");
		}
	}
	else
	{
		printf("Usage:\n\t%s image_filename > filename.ppm\n", argv[0]);
	}

	return 0;
}

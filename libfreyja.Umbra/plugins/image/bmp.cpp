/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : 
 * License : GPL, Also (c) Mongoose 2004
 * Comments: BMP plug-in
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History -------------------------------------------------------------- 
 *
 * 2004.10.22:
 * Mongoose - Created, header spec from docs
 ==========================================================================*/

#include <freyja/FreyjaFileReader.h>


extern "C" {

	int import_image(char *filename, unsigned char **image, 
						  unsigned int *w, unsigned int *h, 
						  char *type);
}


typedef struct {
	unsigned short magic;
	unsigned int file_size;
	unsigned int reserved;
	unsigned int data_offset;

} bmp_header_t;


typedef struct {
	unsigned int header_size;
	int width;
	int height;
	unsigned short planes;
	unsigned short bpp;
	unsigned int compression;
	unsigned int data_size;
	int x_ppm;
	int y_ppm;
	unsigned int n_colors;
	unsigned int n_important_colors;

} bmp_desc_header_t;


int import_image(char *filename, unsigned char **image, 
					  unsigned int *w, unsigned int *h, 
					  char *type)
{
	FreyjaFileReader r;
	bmp_header_t header;
	bmp_desc_header_t desc;
	unsigned int byte_size;


	if (!r.openFile(filename))
	{
		printf("File '%s' couldn't be accessed.", filename);
		return -1;
	}

	/* Read file header */
	header.magic = r.readInt16U();
	header.file_size = r.readInt32U();
	header.reserved = r.readInt32U();
	header.data_offset = r.readInt32U();

	if (header.magic != 0x4D42)
	{
		return -2;
	}

	/* Read bitmap desc */
	desc.header_size = r.readInt32U();
	desc.width = r.readInt32();
   desc.height = r.readInt32();
   desc.planes = r.readInt16U();
   desc.bpp = r.readInt16U();
   desc.compression = r.readInt32U();
   desc.data_size = r.readInt32U();
   desc.x_ppm = r.readInt32();
   desc.y_ppm = r.readInt32();
   desc.n_colors = r.readInt32U();
   desc.n_important_colors = r.readInt32U();
	
	if (desc.data_size == 0)
		return -3;

	if (desc.compression != 0)
		return -4;

	byte_size = desc.data_size; //*type * desc.width * desc.height;

	*w =	desc.width;
	*h = desc.height;
	*type = desc.bpp/8;
	*image = new unsigned char[byte_size];
	r.readBufferUnsignedChar(byte_size, *image);

	return 0;
}



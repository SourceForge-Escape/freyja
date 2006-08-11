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

#include <mstl/SystemIO.h>

using namespace mstl;

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
	SystemIO::FileReader r;
	bmp_header_t header;
	bmp_desc_header_t desc;
	unsigned int i, j, byte_size;
	unsigned char tmp;
	bool flip = true;


	if (!r.Open(filename))
	{
		SystemIO::Print("File '%s' couldn't be accessed.", filename);
		return -1;
	}

	/* Read file header */
	header.magic = r.ReadInt16U();
	header.file_size = r.ReadInt32U();
	header.reserved = r.ReadInt32U();
	header.data_offset = r.ReadInt32U();

	if (header.magic != 0x4D42)
	{
		return -2;
	}

	/* Read bitmap desc */
	desc.header_size = r.ReadInt32U();
	desc.width = r.ReadInt32();
	desc.height = r.ReadInt32();
	desc.planes = r.ReadInt16U();
	desc.bpp = r.ReadInt16U();
	desc.compression = r.ReadInt32U();
	desc.data_size = r.ReadInt32U();
	desc.x_ppm = r.ReadInt32();
	desc.y_ppm = r.ReadInt32();
	desc.n_colors = r.ReadInt32U();
	desc.n_important_colors = r.ReadInt32U();
	
	if (desc.data_size == 0)
		return -3;

	if (desc.compression != 0)
		return -4;

	byte_size = desc.data_size; //*type * desc.width * desc.height;

	*w = desc.width;
	*h = desc.height;
	*type = desc.bpp/8;
	*image = new unsigned char[byte_size];
	r.ReadBuffer(byte_size, *image);

	/* Swap color */
	for (i = 0; i < byte_size; i += 3)
	{
		tmp = (*image)[i];
		(*image)[i] = (*image)[i + 2];
		(*image)[i + 2] = tmp;
	}

	/* Flip vertical */
	if (flip)
	{
		unsigned int byteSize = (desc.bpp / 8);
		unsigned int s = (desc.width * byteSize);
		unsigned char *swap_row = new unsigned char[s];
      
		for (i = 0, j = desc.height - 1; (int)i < desc.height / 2; ++i, --j)
		{
			memcpy(swap_row, &(*image)[i*s], s);
			memcpy(&(*image)[i*s], &(*image)[j*s], s);
			memcpy(&(*image)[j*s], swap_row, s);
		}
			
		delete [] swap_row;
	}

	return 0;
}



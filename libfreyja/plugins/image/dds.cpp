/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : 
 * License : GPL See file COPYING, also (C) 2004 Mongoose
 * Comments: DDS plug-in ( w/o mipmap support for this plugin system )
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History -------------------------------------------------------------- 
 *
 * 2004.08.24:
 * Mongoose -  Decompression algorithms patterned after those in DevIL --
 *             couldn't find specs on msdn as it was 404
 *
 * 2004.08.23:
 * Mongoose - Created, used mdsn spec as a guide 
 *            ( and some comments from spec for bitflags )
 ==========================================================================*/

#include <stdio.h>
#include <freyja/FreyjaFileReader.h>


extern "C" {

	int import_image(char *filename, unsigned char **image, 
						  unsigned int *w, unsigned int *h, 
						  char *type);
}


//The dwFlags member of the original DDSURFACEDESC2 structure can be set to one or more of the following values:
#define DDSD_CAPS	         0x00000001
#define DDSD_HEIGHT	      0x00000002
#define DDSD_WIDTH	      0x00000004
#define DDSD_PITCH	      0x00000008
#define DDSD_PIXELFORMAT	0x00001000
#define DDSD_MIPMAPCOUNT	0x00020000
#define DDSD_LINEARSIZE   	0x00080000
#define DDSD_DEPTH      	0x00800000

//The ddpfPixelFormat member of the DDSURFACEDESC2 structure can be set to one or more of the following values:
#define DDPF_ALPHAPIXELS	0x00000001
#define DDPF_FOURCC	      0x00000004
#define DDPF_RGB	         0x00000040

//The dwCaps1 member of the DDSCAPS2 structure can be set to one or more of the following values:
#define DDSCAPS_COMPLEX	   0x00000008
#define DDSCAPS_TEXTURE	   0x00001000
#define DDSCAPS_MIPMAP	   0x00400000

//The dwCaps2 member of the DDSCAPS2 structure can be set to one or more of the following values:
#define DDSCAPS2_CUBEMAP	         0x00000200
#define DDSCAPS2_CUBEMAP_POSITIVEX	0x00000400
#define DDSCAPS2_CUBEMAP_NEGATIVEX	0x00000800
#define DDSCAPS2_CUBEMAP_POSITIVEY	0x00001000
#define DDSCAPS2_CUBEMAP_NEGATIVEY	0x00002000
#define DDSCAPS2_CUBEMAP_POSITIVEZ	0x00004000
#define DDSCAPS2_CUBEMAP_NEGATIVEZ	0x00008000
#define DDSCAPS2_VOLUME	            0x00200000



typedef struct {
	unsigned int size;
	unsigned int flags;
	unsigned int encoding; /* eg "DXT1", "DXT2", "DXT3", "DXT4", or "DXT5" */
	unsigned int bits_per_pixel;
	unsigned int red_bit_mask;
	unsigned int green_bit_mask;
	unsigned int blue_bit_mask;
	unsigned int alpha_bit_mask;

} dds_pixelformat_t;


typedef struct {
	unsigned int capabilities;
	unsigned int capabilities2;
	unsigned int reserved[2];	

} dds_capabilities_t;


typedef struct {
	unsigned int size;
	unsigned int flags;
	unsigned int height;
	unsigned int width;
	unsigned int pitch_or_linear_size;
	unsigned int depth;
	unsigned int mipmap_count;
	unsigned int reserved[11];
	dds_pixelformat_t pixelformat;
	dds_capabilities_t capabilities;
	unsigned int reserved2;	

} dds_header_t;


typedef struct {
	unsigned int magic;   /* 'DDS ' 0x20534444 */
	dds_header_t header;
	unsigned char *surface_data;

} dds_image_t;


typedef struct {
	unsigned char r, g, b, a;
} dds_rgba_pixel_t;


typedef struct {
	unsigned r : 5;
	unsigned g : 6;
	unsigned b : 5;
} dds_rgb_pixel_t;


typedef struct {
	short row[4];
} dds_dxt_alpha_block_t;


typedef struct {
	char alpha0;
	char alpha1;
	char stuff[6];
} dds_dxt_alpha_block_3bit_t;


//////////////////////////////////////////////////////////////////////////


/* Decomrpession algorithms from DevIL -- couldn't find on msdn it was 404 */
void dds_correct_color(unsigned char *image, unsigned int size)
{
	unsigned int i;


	for (i = 0; i < size; i += 4) 
	{
		if (image[i+3]) 
		{
			image[i]   = (unsigned char)(((unsigned int)image[i]  <<8)/image[i+3]);
			image[i+1] = (unsigned char)(((unsigned int)image[i+1]<<8)/image[i+3]);
			image[i+2] = (unsigned char)(((unsigned int)image[i+2]<<8)/image[i+3]);
		}
	}

	return;
}


void dds_decompress_DXT1(dds_image_t &dds, 
								 unsigned char *image, unsigned int mipmap)
{
	unsigned char *temp = dds.surface_data;
	unsigned int x, y, z, i, j, k, select, bitmask, offset;
	dds_rgb_pixel_t *color_0, *color_1;  // 565?
	dds_rgba_pixel_t colors[4]; 
	dds_rgba_pixel_t *col;
	unsigned int bpp, bps, sz;


	//if (mipmap > dds.header.depth)
	//{
	//	return;
	//}

	z = mipmap;

	if (dds.header.pixelformat.bits_per_pixel == 0)
		dds.header.pixelformat.bits_per_pixel = 32;

	bpp = 4;
	bps = (dds.header.width * 
			 dds.header.pixelformat.bits_per_pixel/8);
	sz = dds.header.width * dds.header.height;


	for (y = 0; y < dds.header.height; y += 4) 
	{
		for (x = 0; x < dds.header.width; x += 4) 
		{
			color_0 = ((dds_rgb_pixel_t*)temp);
			color_1 = ((dds_rgb_pixel_t*)(temp+2));
			bitmask = ((unsigned int*)temp)[1];
			temp += 8;

			colors[0].r = color_0->r << 3;
			colors[0].g = color_0->g << 2;
			colors[0].b = color_0->b << 3;
			colors[0].a = 0xFF;

			colors[1].r = color_1->r << 3;
			colors[1].g = color_1->g << 2;
			colors[1].b = color_1->b << 3;
			colors[1].a = 0xFF;

			if (*((unsigned short*)color_0) > *((unsigned short*)color_1))
			{
				// Four-color block: derive the other two colors.
				// 00 = color_0, 01 = color_1, 10 = color_2, 11 = color_3
				// These 2-bit codes correspond to the 2-bit fields 
				// stored in the 64-bit block.
				colors[2].b = (2 * colors[0].b + colors[1].b + 1) / 3;
				colors[2].g = (2 * colors[0].g + colors[1].g + 1) / 3;
				colors[2].r = (2 * colors[0].r + colors[1].r + 1) / 3;
				colors[2].a = 0xFF;
				
				colors[3].b = (colors[0].b + 2 * colors[1].b + 1) / 3;
				colors[3].g = (colors[0].g + 2 * colors[1].g + 1) / 3;
				colors[3].r = (colors[0].r + 2 * colors[1].r + 1) / 3;
				colors[3].a = 0xFF;
			}
			else
			{ 
				// Three-color block: derive the other color.
				// 00 = color_0,  01 = color_1,  10 = color_2,
				// 11 = transparent.
				// These 2-bit codes correspond to the 2-bit fields 
				// stored in the 64-bit block. 
				colors[2].b = (colors[0].b + colors[1].b) / 2;
				colors[2].g = (colors[0].g + colors[1].g) / 2;
				colors[2].r = (colors[0].r + colors[1].r) / 2;
				colors[2].a = 0xFF;
				
				colors[3].b = (colors[0].b + 2 * colors[1].b + 1) / 3;
				colors[3].g = (colors[0].g + 2 * colors[1].g + 1) / 3;
				colors[3].r = (colors[0].r + 2 * colors[1].r + 1) / 3;
				colors[3].a = 0x00;
			}

			for (j = 0, k = 0; j < 4; ++j)
			{
				for (i = 0; i < 4; ++i, ++k)
				{
					select = (bitmask & (0x03 << k*2)) >> k*2;
					col = &colors[select];

					if (((x + i) < dds.header.width) && 
						 ((y + j) < dds.header.height))
					{
						offset = (z * sz) + ((y + j) * bps) + ((x + i) * bpp);

						image[offset + 0] = col->b;
						image[offset + 1] = col->g;
						image[offset + 2] = col->r;
						image[offset + 3] = col->a;
					}
				}
			}
		}
	}
}


//FIXME
void dds_decompress_DXT3(dds_image_t &dds, 
								 unsigned char *image, unsigned int mipmap)
{
	unsigned char *temp = dds.surface_data;
	unsigned int x, y, z, i, j, k, select, bitmask, offset;
	dds_rgb_pixel_t *color_0, *color_1;  // 565?
	dds_rgba_pixel_t colors[4]; 
	dds_rgba_pixel_t *col;
	unsigned short word;
	dds_dxt_alpha_block_t *alpha;
	unsigned int bpp, bps, sz;


	z = 0;//mipmap;

	if (dds.header.pixelformat.bits_per_pixel == 0)
		dds.header.pixelformat.bits_per_pixel = 32;

	bpp = 4;
	bps = (dds.header.width * dds.header.pixelformat.bits_per_pixel/8);
	sz = dds.header.width * dds.header.height;

	for (y = 0; y < dds.header.height; y += 4) 
	{
		for (x = 0; x < dds.header.width; x += 4) 
		{
			alpha = (dds_dxt_alpha_block_t*)temp;
			temp += 8;
			color_0 = ((dds_rgb_pixel_t*)temp);
			color_1 = ((dds_rgb_pixel_t*)(temp+2));
			bitmask = ((unsigned int*)temp)[1];
			temp += 8;

			colors[0].r = color_0->r << 3;
			colors[0].g = color_0->g << 2;
			colors[0].b = color_0->b << 3;
			colors[0].a = 0xFF;
			
			colors[1].r = color_1->r << 3;
			colors[1].g = color_1->g << 2;
			colors[1].b = color_1->b << 3;
			colors[1].a = 0xFF;
			
			// Four-color block: derive the other two colors.    
			// 00 = color_0, 01 = color_1, 10 = color_2, 11 = color_3
			// These 2-bit codes correspond to the 2-bit fields 
			// stored in the 64-bit block.
			colors[2].b = (2 * colors[0].b + colors[1].b + 1) / 3;
			colors[2].g = (2 * colors[0].g + colors[1].g + 1) / 3;
			colors[2].r = (2 * colors[0].r + colors[1].r + 1) / 3;
			colors[2].a = 0xFF;

			colors[3].b = (colors[0].b + 2 * colors[1].b + 1) / 3;
			colors[3].g = (colors[0].g + 2 * colors[1].g + 1) / 3;
			colors[3].r = (colors[0].r + 2 * colors[1].r + 1) / 3;
			colors[3].a = 0xFF;
			
			for (j = 0, k = 0; j < 4; ++j)
			{
				for (i = 0; i < 4; ++i, ++k)
				{
					select = (bitmask & (0x03 << k*2)) >> k*2;
					col = &colors[select];
					
					if (((x + i) < dds.header.width) &&
						 ((y + j) < dds.header.height))
					{
						offset = z * sz + (y + j) * bps + (x + i) * bpp;
						image[offset + 0] = col->b;
						image[offset + 1] = col->g;
						image[offset + 2] = col->r;
					}
				}
			}
			
			for (j = 0; j < 4; ++j)
			{
				word = alpha->row[j];
				
				for (i = 0; i < 4; ++i)
				{
					if (((x + i) < dds.header.width)
						 && ((y + j) < dds.header.height))
					{
						offset = z * sz + (y + j) * bps + (x + i) * bpp + 3;
						image[offset] = word & 0x0F;
						image[offset] = image[offset] | (image[offset] << 4);
					}

					word >>= 4;
				}
			}
		}
	}	
}


void dds_decompress_DXT2(dds_image_t &dds, 
								 unsigned char *image, unsigned int mipmap)
{
	dds_decompress_DXT3(dds, image, mipmap);
	dds_correct_color(image, dds.header.width * dds.header.height);
}


void dds_decompress_DXT5(dds_image_t &dds, 
								 unsigned char *image, unsigned int mipmap)
{	unsigned char *temp = dds.surface_data;
	unsigned int x, y, z, i, j, k, select, bitmask, offset;
	dds_rgb_pixel_t *color_0, *color_1;  // 565?
	dds_rgba_pixel_t colors[4]; 
	dds_rgba_pixel_t *col;
	unsigned char alphas[8];
	unsigned char *alphamask;
	unsigned int bits;
	unsigned int bpp, bps, sz;


	z = mipmap;

	if (dds.header.pixelformat.bits_per_pixel == 0)
		dds.header.pixelformat.bits_per_pixel = 32;

	bpp = 4;
	bps = (dds.header.width * dds.header.pixelformat.bits_per_pixel/8);
	sz = dds.header.width * dds.header.height;

	for (y = 0; y < dds.header.height; y += 4) 
	{
		for (x = 0; x < dds.header.width; x += 4) 
		{
			if (y >= dds.header.height || x >= dds.header.width)
				break;

			alphas[0] = temp[0];
			alphas[1] = temp[1];
			alphamask = temp + 2;
			temp += 8;
			color_0 = ((dds_rgb_pixel_t*)temp);
			color_1 = ((dds_rgb_pixel_t*)(temp+2));
			bitmask = ((unsigned int*)temp)[1];
			temp += 8;
			
			colors[0].r = color_0->r << 3;
			colors[0].g = color_0->g << 2;
			colors[0].b = color_0->b << 3;
			colors[0].a = 0xFF;

			colors[1].r = color_1->r << 3;
			colors[1].g = color_1->g << 2;
			colors[1].b = color_1->b << 3;
			colors[1].a = 0xFF;

			// Four-color block: derive the other two colors.    
			// 00 = color_0, 01 = color_1, 10 = color_2, 11 = color_3
			// These 2-bit codes correspond to the 2-bit fields 
			// stored in the 64-bit block.
			colors[2].b = (2 * colors[0].b + colors[1].b + 1) / 3;
			colors[2].g = (2 * colors[0].g + colors[1].g + 1) / 3;
			colors[2].r = (2 * colors[0].r + colors[1].r + 1) / 3;
			colors[2].a = 0xFF;
			
			colors[3].b = (colors[0].b + 2 * colors[1].b + 1) / 3;
			colors[3].g = (colors[0].g + 2 * colors[1].g + 1) / 3;
			colors[3].r = (colors[0].r + 2 * colors[1].r + 1) / 3;
			colors[3].a = 0xFF;

			for (j = 0, k = 0; j < 4; ++j) 
			{
				for (i = 0; i < 4; ++i, ++k)
				{
					select = (bitmask & (0x03 << k*2)) >> k*2;
					col = &colors[select];
					
					// only put pixels out < width or height
					if (((x + i) < dds.header.width) &&
						 ((y + j) < dds.header.height)) 
					{
						offset = z * sz + (y + j) * bps + (x + i) * bpp;
						image[offset + 0] = col->r;
						image[offset + 1] = col->g;
						image[offset + 2] = col->b;
					}
				}
			}
			
			// 8-alpha or 6-alpha block?    
			if (alphas[0] > alphas[1])
			{    
				// 8-alpha block:  derive the other six alphas.    
				// Bit code 000 = alpha_0, 001 = alpha_1, others are interpolated.
				alphas[2] = (6 * alphas[0] + 1 * alphas[1] + 3) / 7; // bit code 010
				alphas[3] = (5 * alphas[0] + 2 * alphas[1] + 3) / 7; // bit code 011
				alphas[4] = (4 * alphas[0] + 3 * alphas[1] + 3) / 7; // bit code 100
				alphas[5] = (3 * alphas[0] + 4 * alphas[1] + 3) / 7; // bit code 101
				alphas[6] = (2 * alphas[0] + 5 * alphas[1] + 3) / 7; // bit code 110
				alphas[7] = (1 * alphas[0] + 6 * alphas[1] + 3) / 7; // bit code 111
			}
			else
			{
				// 6-alpha block.
				// Bit code 000 = alpha_0, 001 = alpha_1, others are interpolated.
				alphas[2] = (4 * alphas[0] + 1 * alphas[1] + 2) / 5; // Bit code 010
				alphas[3] = (3 * alphas[0] + 2 * alphas[1] + 2) / 5; // Bit code 011
				alphas[4] = (2 * alphas[0] + 3 * alphas[1] + 2) / 5; // Bit code 100
				alphas[5] = (1 * alphas[0] + 4 * alphas[1] + 2) / 5; // Bit code 101
				alphas[6] = 0x00;										// Bit code 110
				alphas[7] = 0xFF;										// Bit code 111
			}

			// Note: Have to separate the next two loops,
			//	it operates on a 6-byte system.

			// First three bytes
			bits = *((int*)alphamask);

			for (j = 0; j < 2; ++j)
			{
				for (i = 0; i < 4; ++i)
				{
					// only put pixels out < width or height
					if (((x + i) < dds.header.width) && 
						 ((y + j) < dds.header.height))
					{
						offset = z * sz + (y + j) * bps + (x + i) * bpp + 3;
						image[offset] = alphas[bits & 0x07];
					}
					bits >>= 3;
				}
			}

			// Last three bytes
			bits = *((int*)&alphamask[3]);

			for (j = 2; j < 4; ++j)
			{
				for (i = 0; i < 4; ++i)
				{
					// only put pixels out < width or height
					if (((x + i) < dds.header.width) && 
						 ((y + j) < dds.header.height))
					{
						offset = z * sz + (y + j) * bps + (x + i) * bpp + 3;
						image[offset] = alphas[bits & 0x07];
					}
					bits >>= 3;
				}
			}
		}
	}
}


void dds_decompress_DXT4(dds_image_t &dds, 
								 unsigned char *image, unsigned int mipmap)
{
	dds_decompress_DXT5(dds, image, mipmap);
	dds_correct_color(image, dds.header.width * dds.header.height);
}


int import_image(char *filename, unsigned char **image, 
					  unsigned int *w, unsigned int *h, char *bpp)
{
	FreyjaFileReader r;
	dds_image_t dds;
	unsigned int i, blocksize;


	if (!r.openFile(filename))
	{
		printf("File '%s' couldn't be accessed.", filename);
		return -1;
	}

	/* File magic */
	dds.magic = r.readInt32U();

	if (dds.magic != 0x20534444)
	{
		printf("dds.so: Inavlid or unknown DDS format.\n");
		r.closeFile();
		return -2;
	}
	
	/* Surface header */
	dds.header.size = r.readInt32U();

	if (dds.header.size != 124)
	{
		printf("dds.so: Inavlid or unknown DDS format.\n");
		r.closeFile();
		return -2;
	}

	dds.header.flags = r.readInt32U();
	dds.header.height = r.readInt32U();
	dds.header.width = r.readInt32U();
	dds.header.pitch_or_linear_size = r.readInt32U();
	dds.header.depth = r.readInt32U();
	dds.header.mipmap_count = r.readInt32U();

	for (i = 0; i < 11; ++i)
		dds.header.reserved[i] = r.readInt32U();

	dds.header.pixelformat.size = r.readInt32U();

	if (dds.header.pixelformat.size != 32)
	{
		printf("dds.so: Inavlid or unknown DDS format.\n");
		r.closeFile();
		return -2;
	}

	dds.header.pixelformat.flags = r.readInt32U();
	dds.header.pixelformat.encoding = r.readInt32U();
	dds.header.pixelformat.bits_per_pixel = r.readInt32U();
	dds.header.pixelformat.red_bit_mask = r.readInt32U();
	dds.header.pixelformat.green_bit_mask = r.readInt32U();
	dds.header.pixelformat.blue_bit_mask = r.readInt32U();
	dds.header.pixelformat.alpha_bit_mask = r.readInt32U();

	dds.header.capabilities.capabilities = r.readInt32U();
	dds.header.capabilities.capabilities2 = r.readInt32U();
	dds.header.capabilities.reserved[0] = r.readInt32U();
	dds.header.capabilities.reserved[1] = r.readInt32U();

	dds.header.reserved2 = r.readInt32U();

	/* Surface data */
	switch (dds.header.pixelformat.encoding)
	{
	case 0x31545844: /* 'DTX1' */
		blocksize = (dds.header.width/4) * (dds.header.height/4) * 8;
		break;

	case 0x32545844: /* 'DTX2' */
	case 0x33545844: /* 'DTX3' */
	case 0x34545844: /* 'DTX4' */
	case 0x35545844: /* 'DTX5' */
		blocksize = (dds.header.width/4) * (dds.header.height/4) * 16;
		break;

	default:
		blocksize = (dds.header.width/4) * (dds.header.height/4) * 8;
	}	

	dds.surface_data = new unsigned char[blocksize];
	r.readBufferUnsignedChar(blocksize, dds.surface_data);

	r.closeFile();

	/* Dump debug info about loaded surface */
	printf("dds.so: %c%c%c%c %ux%u %ibpp, %ubytes, %i mipmaps, %s\n", 
			 ((char*)(&dds.header.pixelformat.encoding))[0],
			 ((char*)(&dds.header.pixelformat.encoding))[1],
			 ((char*)(&dds.header.pixelformat.encoding))[2],
			 ((char*)(&dds.header.pixelformat.encoding))[3],
			 dds.header.width, dds.header.height, 
			 dds.header.pixelformat.bits_per_pixel, blocksize, dds.header.depth,
			 (dds.header.flags & DDSD_LINEARSIZE) ? "RLE" : "uncompressed");


	/* Remember to convert to RBG24 or RBGA32 for single surface plugin use */
	switch (dds.header.pixelformat.encoding)
	{
	case 0x31545844: /* 'DTX1' */
		*w = dds.header.width;
		*h = dds.header.height;
		*bpp = 4;
		*image = new unsigned char[*w * *h * *bpp];

		/* Just get top mipmap for now */
		dds_decompress_DXT1(dds, *image, 0);
		break;

	case 0x32545844: /* 'DTX2' */
		*w = dds.header.width;
		*h = dds.header.height;
		*bpp = 4;
		*image = new unsigned char[*w * *h * *bpp];

		/* Just get top mipmap for now */
		dds_decompress_DXT2(dds, *image, 0);
		break;

	case 0x33545844: /* 'DTX3' */
		*w = dds.header.width;
		*h = dds.header.height;
		*bpp = 4;
		*image = new unsigned char[*w * *h * *bpp];

		/* Just get top mipmap for now */
		dds_decompress_DXT3(dds, *image, 0);
		break;

	case 0x34545844: /* 'DTX2' */
		*w = dds.header.width;
		*h = dds.header.height;
		*bpp = 4;
		*image = new unsigned char[*w * *h * *bpp];

		/* Just get top mipmap for now */
		dds_decompress_DXT4(dds, *image, 0);
		break;

	case 0x35545844: /* 'DTX2' */
		*w = dds.header.width;
		*h = dds.header.height;
		*bpp = 4;
		*image = new unsigned char[*w * *h * *bpp];

		/* Just get top mipmap for now */
		dds_decompress_DXT5(dds, *image, 0);
		break;

	default:
		printf("dds.so: Unsupported DDS format %c%c%c%c.\n", 
			 ((char*)(&dds.header.pixelformat.encoding))[0],
			 ((char*)(&dds.header.pixelformat.encoding))[1],
			 ((char*)(&dds.header.pixelformat.encoding))[2],
			 ((char*)(&dds.header.pixelformat.encoding))[3]);

		if (dds.surface_data)
			delete [] dds.surface_data;

		return -1;
	}


	if (dds.surface_data)
		delete [] dds.surface_data;

	return 0;
}

/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Midgard
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : URTexture
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: This is the Unreal texture class
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.03.08:
 * Mongoose - Created
 =================================================================*/

#include <math.h>
#include <string.h>
#include <stdio.h>

#ifdef HAVE_FREYJA
#   include <freyja/FreyjaImage.h>
#endif

#include "URTexture.h"


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

URTexture::URTexture()
{
}


URTexture::~URTexture()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

void convertDX5Texture(const unsigned char *data)
{
	unsigned int x, y;
	//CPicturePixel   texel[16];
	unsigned int tw = 0;//itsPixmap.W() / 4;
	unsigned int th = 0;//itsPixmap.H() / 4;
	
	for (y = 0; y < th; ++y) 
	{
		for (x = 0; x < tw; ++x) 
		{
			//getTexelDXT5( texel, aData );
			//putTexel( 4 * x, 4 * y, texel );
			data = data + 16;
		}
	}
}


int URTexture::load(UTPackage &pak, unsigned int index)
{
	int idx, vSize = -1, uSize = -1;
	unsigned char u8, format, wBit, hBit;
	unsigned char *palette = 0x0, *image = 0x0, *imageRGBA = 0x0;
	// unsigned short u16;
	unsigned int u32, i, j, count, size, wOffset, w, h, byteperpixel = 0;
	unsigned int paletteId = 0, paletteSize, dataOffset;
	unsigned char *data;
	UTPackageProperty prop;


	printf("Exporting texture...\n");

	/* Property list */
	while ((idx = pak.loadProperty(prop)) == 0)
	{
		printf("____________________________________________________\n");
		printf("Property '%s', nameId = %i, idx = %i found\n",
				 prop.name, prop.nameIndex, idx);

		if (strcmp("None", prop.name) == 0)
		{
			printf("End of properties\n");
			break;
		}
		else if (strcmp("USize", prop.name) == 0)
		{
			uSize =  *(unsigned int *)(prop.data);
			printf("uSize = %i\n", uSize);
			break;
		}
		else if (strcmp("VSize", prop.name) == 0)
		{
			vSize =  *(unsigned int *)(prop.data);
			printf("vSize = %i\n", vSize);
			break;
		}
		else if (strcmp("Palette", prop.name) == 0)
		{
			paletteId = *(unsigned int *)(prop.data);
			printf("Palette export = %u\n", paletteId);
		}
		else if (strcmp("Format", prop.name) == 0)
		{
			format = prop.data[0];

			switch (format)
			{
			case 0x00: /* Indexed 8bit, uncompressed mipmaps */
				printf("Indexed 8bit, uncompressed mipmaps\n");
				byteperpixel = 1;
				break;
			case 0x04: /* BGRA 32bit, uncompressed mipmaps */
				printf("BGR 24bit, uncompressed mipmaps\n");
				byteperpixel = 3;
				break;
			case 0x05: /* BGRA 32bit, uncompressed mipmaps */
				printf("BGRA 32bit, uncompressed mipmaps\n");
				byteperpixel = 4;
				break;
			case 0x03: /* DXT1 */
				printf("\tDXT1 texture\n");
				byteperpixel = 0;
				break;
			case 0x07: /* DXT3 */
				printf("\tDXT3 texture\n");
				byteperpixel = 0;
				break;
			case 0x08: /* DXT5 */
				printf("\tDX5 texture\n");
				byteperpixel = 0;
				break;
			default:
				printf("Unknown mipmap format 0x%x\n", format);
				byteperpixel = 0;
			}
		}
		else
		{
			printf("Property '%s' (name[%i|%i]) not handled\n",
					 prop.name, prop.nameIndex, idx);
		}

		//prop.clear();
	}

	if (idx == -2)
	{
		printf("FATAL ERROR: Invalid property returned for class!\n");
		return -1;
	}


	/* Texture class, mipmaps start */
	printf("==============================================\n");
	pak.dRead(&u8, 1, 1);
	count = u8;

	printf("MipMap count: %i\n", count);

	switch (format)
	{
	case 0x00: /* Indexed 8bit, uncompressed mipmaps */
		printf("Format: Indexed 8bit, uncompressed mipmaps\n");
		break;
	case 0x04: /* BGRA 32bit, uncompressed mipmaps */
		printf("Format: BGR 24bit, uncompressed mipmaps\n");
		break;
	case 0x05: /* BGRA 32bit, uncompressed mipmaps */
		printf("Format: BGRA 32bit, uncompressed mipmaps\n");
		break;
	case 0x03: /* DXT1 */
		printf("Format: DXT1 texture\n");
		break;
	case 0x07: /* DXT3 */
		printf("Format: DXT3 texture\n");
		break;
	case 0x08: /* DXT5 */
		printf("Format: DX5 texture\n");
		break;
	default:
		printf("Format: Unknown mipmap format 0x%x\n", format);
		byteperpixel = 0;
	}

	for (i = 0, wOffset = 0; i < count; ++i)
	{
		printf("MipMap[%i] v%u\n", i, pak.mHeader.version);

		if (pak.mHeader.version < 63)
		{
		}
		else if (pak.mHeader.version < 100)
		{
			pak.dRead(&wOffset, 4, 1);
			size = pak.dReadIndexValue();
		}
		else if (pak.mHeader.version < 120)
		{
			pak.dRead(&wOffset, 4, 1);
			pak.dRead(&u32, 4, 1);
			size = pak.dReadIndexValue();
		}
		else /* ut2003 mostly */
		{
			pak.dRead(&wOffset, 4, 1);
			size = pak.dReadIndexValue();
		}

		printf("\tWidthOffset : %u (0x%x)\n", wOffset, wOffset);
		printf("\tPixelCount  : %u\n", size);				
		printf("\tBytesize    : %u (%.2fK)\n", 
				 size*byteperpixel, (size*byteperpixel/1024.0));			

		if (size == 0)
		{
			printf("Skipping, No pixel count!\n");
			return -2;
		}

		switch (format)
		{
		case 0x00: /* Indexed 8bit, uncompressed mipmaps */
			printf("\tIndexed 8bit, uncompressed mipmaps\n");
			byteperpixel = 1;
			palette = 0x0;

			dataOffset = pak.dTell();
			printf("\tdataOffset = %u ( 0x%x )\n", 
					 dataOffset, dataOffset);

			/* You have to grab palette via export table, duh */
			if (0)
			{
				unsigned char found = 0;
				unsigned int i, sz, off, index, type;
				int nameIndex;

				for (i = 0; i < pak.mHeader.exportCount; ++i)
				{
					index = pak.useIndex(pak.mHeader.exportTable[i].objClass, &type);
					nameIndex = ((type == UTPackage::UT_NULL) ? -1 :
									 (type == UTPackage::UT_IMPORT) ? 
									 pak.mHeader.importTable[index].objName :
									 pak.mHeader.exportTable[index].objName);

					printf("* Export Class '%s'\n", 
							 ((nameIndex < 0) ? "Null" :
							  pak.mHeader.nameTable[nameIndex].objName));

					index = pak.useIndex(pak.mHeader.exportTable[i].objName, &type);
					
					printf("         Name '%s'\n", 
							 ((type == UTPackage::UT_NULL) ? "Null" :
							  pak.mHeader.nameTable[index].objName));

					sz = pak.useIndex(pak.mHeader.exportTable[i].serialSize, &type);
					printf("         Size %i bytes\n", sz);
					
					off = pak.useIndex(pak.mHeader.exportTable[i].serialOffset, &type);
					printf("         Offset 0x%x (%u bytes)\n", off, off);

					if (strcmp(NULL, "") == 0)
					{
						found = 1;
						break;
					}
				}

				if (found)
				{
					// Seek to palette class and read it in
				}

				/*** Palette class **********************************/ 

				/* Property list */
				while ((idx = pak.loadProperty(prop)) == 0)
				{
					if (strcmp("None", prop.name) == 0)
					{
						printf("End of properties\n");
						break;
					}	
				}

				paletteSize = pak.dReadIndexValue();

				printf("\t%u colors in palette\n", paletteSize);
						
				if (paletteSize == 0)
				{
					printf("\tERROR: Invalid texture 0 colors\n");	
					return -2;					
				}
				else if (paletteSize > 256)
				{
					printf("\tERROR: Invalid texture > 256 colors\n");	
					return -2;
				}

				// RGBA colors
				palette = new unsigned char[paletteSize*4];
				pak.dRead(palette, paletteSize*4, 1);
			}
			else
			{
				printf("\t*Palette load disabled, using a greyscale palette\n");
				paletteSize = 256;
				palette = new unsigned char[paletteSize*4];
				// memset(palette, 0, paletteSize*4);

				/* Replace all black with quick greyscale gradient
				 * Black -> White */
				for (j = 0; j < paletteSize; ++j)
				{
					palette[j*4]   = j;
					palette[j*4+1] = j;
					palette[j*4+2] = j;
					palette[j*4+3] = 0;
				}
			}

			if (paletteSize == 0 || paletteSize > 256)
			{
				printf("\tERROR: Invalid texture > 256 colors\n");	

				if (palette)
					delete [] palette;

				return -2;
			}

			/* Image dims */
			pak.dSeek(dataOffset+size, SEEK_SET);
			//pak.dSeek(wOffset+size, SEEK_SET);
			pak.dRead(&w, 4, 1);
			pak.dRead(&h, 4, 1);

			if (vSize > 0 && uSize > 0)
			{
				w = uSize;
				h = vSize;
				size = w*h;
			}

			printf("\t%u x %u, %u bytes, %u colors\n", 
					 w, h, size, paletteSize);

			if (w*h > size)
			{
				printf("ERROR: Texture size, pixel count mismatch\n");
				return -3;
			}

			/* INDEXed image */
			pak.dSeek(dataOffset, SEEK_SET);
			image = new unsigned char[size];
			pak.dRead(image, size, 1);

			imageRGBA = new unsigned char[size*4];

			for (j = 0; j < size; ++j)
			{
				imageRGBA[j*4] = palette[image[j]*4];
				imageRGBA[j*4+1] = palette[image[j]*4+1];
				imageRGBA[j*4+2] = palette[image[j]*4+2];
				imageRGBA[j*4+3] = palette[image[j]*4+3];
			}

#ifdef HAVE_FREYJA
			printf("\tWriting /tmp/utpak/Texture/%s-%i.tga...\n", 
					 pak.mHeader.nameTable[index].objName, i);

			if (1)
			{
				FreyjaImage img;
				char filenameTGA[512];
				snprintf(filenameTGA, 512, "/tmp/utpak/Texture/%s-%i.tga", 
							pak.mHeader.nameTable[index].objName, i);
				img.loadPixmap(imageRGBA, w, h, FreyjaImage::RGBA_32);
				img.saveImage(filenameTGA, "tga");
			}
#else
#   warning HAVE_FREYJA undefined - No image export support
#endif

			if (imageRGBA)
				delete [] imageRGBA;

			if (image)
				delete [] image;

			if (palette)
				delete [] palette;
			break;
		case 0x04: /* BGRA 24bit, uncompressed mipmaps */
		case 0x05: /* BGRA 32bit, uncompressed mipmaps */

			/* Read in uncompressed image data */
			data = new unsigned char[size*byteperpixel];
			pak.dRead(data, 1, size*byteperpixel);
					
			/* Skip to width */
			if (pak.mHeader.version >= 63)
			{	
				printf("\t*Seeking to byte %u\n", wOffset);
				pak.dSeek(wOffset, SEEK_SET);
			}

			/* Read in dim of image */					
			pak.dRead(&w, 4, 1);
			pak.dRead(&h, 4, 1);
 			pak.dRead(&wBit, 1, 1);
			pak.dRead(&hBit, 1, 1);

			printf("\t%u x %u = ( 2^%u x 2^%u )?\n", w, h, wBit, hBit);
			w = (w == 0) ? (unsigned int)pow(2, wBit) : w;
			h = (h == 0) ? (unsigned int)pow(2, hBit) : h;

			printf("\tWriting /tmp/utpak/Texture/%s-%i.tga...\n", 
					 pak.mHeader.nameTable[index].objName, i);
#ifdef HAVE_FREYJA
			if (1)
			{
				FreyjaImage img;
				char filenameTGA[512];
				snprintf(filenameTGA, 512, "/tmp/utpak/Texture/%s-%i.tga", 
							pak.mHeader.nameTable[index].objName, i);
				img.loadPixmap(imageRGBA, w, h, 
								  (byteperpixel == 3) ? FreyjaImage::RGB_24 : FreyjaImage::RGBA_32);
				img.saveImage(filenameTGA, "tga");
			}
#else
#   warning HAVE_FREYJA undefined - No image export support
#endif						
			delete [] data;
					
			printf("\n");
			printf("\n");					
			break;
		case 0x03: /* DXT1 */
			printf("\tDXT1 texture loading not implemented\n");
			break;
		case 0x07: /* DXT3 */
			if (vSize > 0 && uSize > 0)
			{
				w = uSize;
				h = vSize;
				size = w*h;
			}

			printf("\t%u x %u, %u bytes\n", 
					 w, h, size);

			printf("\tDXT3 texture loading not implemented\n");
			break;
		case 0x08: /* DXT5 */
			if (vSize > 0 && uSize > 0)
			{
				w = uSize;
				h = vSize;
				size = w*h;
			}

			printf("\t%u x %u, %u bytes\n", 
					 w, h, size);

			printf("\tDX5 texture loading not implemented\n");
			break;
		default:
			printf("\tUnknown mipmap type, skipping\n");
			byteperpixel = 0;
		}
	}

	return 0;
}

////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////

#ifdef UTCMS_CRAP_CODE_THAT_KIND_OF_HELPS_SHOW_DXT5_FORMAT
// --------------------------------------------------------------------
// private: Read in four bytes as a dword
// --------------------------------------------------------------------
word_t  CUnTexture::GetTexelW   (   const byte_t *  aData ) const {
    return  (word_t)((word_t)aData[0] + ((word_t)aData[1] << 8));
}

// --------------------------------------------------------------------
// private: Read in four bytes as a dword
// --------------------------------------------------------------------
dword_t CUnTexture::GetTexelDW  (   const byte_t *  aData ) const {
    return     (dword_t)aData[0]
            + ((dword_t)aData[1] << 8)
            + ((dword_t)aData[2] << 16)
            + ((dword_t)aData[3] << 24);
}

// --------------------------------------------------------------------
// private: Read in alpha channel (DXT5)
// --------------------------------------------------------------------
void    CUnTexture::GetTexelA5  (   CPicturePixel * aTexel,
                                    const byte_t *  aData ) const {
    byte_t  aa  [8];
    word_t  alpha;
    word_t  bitnow  = 0;
    word_t  roffs   = 2;

    aa[0] = aData[0];
    aa[1] = aData[1];

    if  ( aa[0] > aa[1] ) {
        aa[2] = (byte_t)((6 * (word_t)aa[0] + 1 * (word_t)aa[1]) / 7);
        aa[3] = (byte_t)((5 * (word_t)aa[0] + 2 * (word_t)aa[1]) / 7);
        aa[4] = (byte_t)((4 * (word_t)aa[0] + 3 * (word_t)aa[1]) / 7);
        aa[5] = (byte_t)((3 * (word_t)aa[0] + 4 * (word_t)aa[1]) / 7);
        aa[6] = (byte_t)((2 * (word_t)aa[0] + 5 * (word_t)aa[1]) / 7);
        aa[7] = (byte_t)((1 * (word_t)aa[0] + 6 * (word_t)aa[1]) / 7);
    }
    else {
        aa[2] = (byte_t)((4 * (word_t)aa[0] + 1 * (word_t)aa[1]) / 5);
        aa[3] = (byte_t)((3 * (word_t)aa[0] + 2 * (word_t)aa[1]) / 5);
        aa[4] = (byte_t)((2 * (word_t)aa[0] + 3 * (word_t)aa[1]) / 5);
        aa[5] = (byte_t)((1 * (word_t)aa[0] + 4 * (word_t)aa[1]) / 5);
        aa[6] = 0;
        aa[7] = 0xff;
    }

    alpha =  (word_t)aData[roffs++];

    for ( int i = 0; i < 16; i++ ) {
        aTexel[i].A( aa[ alpha & 0x07 ] );

        if      ( ( bitnow % 8 ) == 0 ) {
            alpha |= (word_t)(((word_t)aData[roffs++] << 8));
            alpha >>= 3;
        }
        else if ( ( bitnow % 8 ) > 5 ) {
            alpha >>= (8 - ( bitnow % 8 ));
            alpha |= (word_t)(((word_t)aData[roffs++] << 8));
            alpha >>= (3 - 8 + ( bitnow % 8 ));
        }
        else {
            alpha >>= 3;
        }
        bitnow = (word_t)(bitnow + 3);
    }

}


// --------------------------------------------------------------------
// private: Read in the four color palette
// --------------------------------------------------------------------
void    CUnTexture::GetTexelTP  (   CPicturePixel * aTp,
                                    const byte_t *  aData,
                                    bool            aAlpha ) const {
    word_t  color0  = GetTexelW( aData );
    word_t  color1  = GetTexelW( aData + 2 );
    word_t  r, g, b;

    aTp[0] = color0;
    aTp[1] = color1;

    if  ( ( aAlpha ) && ( color0 <= color1 ) ) {
        r = (word_t)(((word_t)aTp[0].R() + (word_t)aTp[1].R()) / 2);
        g = (word_t)(((word_t)aTp[0].G() + (word_t)aTp[1].G()) / 2);
        b = (word_t)(((word_t)aTp[0].B() + (word_t)aTp[1].B()) / 2);

        aTp[2].RGB( (byte_t)r, (byte_t)g, (byte_t)b );

        aTp[3].RGB( 0, 0, 0 );
    }
    else {
        r = (word_t)(((word_t)aTp[0].R() + (word_t)aTp[0].R() + (word_t)aTp[1].R()) / 3);
        g = (word_t)(((word_t)aTp[0].G() + (word_t)aTp[0].G() + (word_t)aTp[1].G()) / 3);
        b = (word_t)(((word_t)aTp[0].B() + (word_t)aTp[0].B() + (word_t)aTp[1].B()) / 3);

        aTp[2].RGB( (byte_t)r, (byte_t)g, (byte_t)b );

        r = (word_t)(((word_t)aTp[0].R() + (word_t)aTp[1].R() + (word_t)aTp[1].R()) / 3);
        g = (word_t)(((word_t)aTp[0].G() + (word_t)aTp[1].G() + (word_t)aTp[1].G()) / 3);
        b = (word_t)(((word_t)aTp[0].B() + (word_t)aTp[1].B() + (word_t)aTp[1].B()) / 3);

        aTp[3].RGB( (byte_t)r, (byte_t)g, (byte_t)b );
    }

}

// --------------------------------------------------------------------
// private: Read in the texel image
// --------------------------------------------------------------------
void    CUnTexture::GetTexelTX  (   byte_t *        aTx,
                                    const byte_t *  aData ) const {
    dword_t texim = CUnTexture::GetTexelDW( aData );
    for ( int i = 0; i < 16; i++, texim >>= 2 ) aTx[i] = (byte_t)( texim & 0x03 );
}

// --------------------------------------------------------------------
// private: Read in RGB info (DXT3 and DXT5)
// --------------------------------------------------------------------
void    CUnTexture::GetTexelC3  (   CPicturePixel * aTexel,
                                    const byte_t *  aData ) const {
    CPicturePixel   tp[4];
    byte_t          tx[16];
    CUnTexture::GetTexelTP( tp, aData, false );
    CUnTexture::GetTexelTX( tx, aData + 4 );
    for ( int i = 0; i < 16; i++ )  aTexel[i].RGB( tp[tx[i]] );
}

// --------------------------------------------------------------------
// private: Read in one 4 x 4 texel (DXT5)
// --------------------------------------------------------------------
void    CUnTexture::GetTexelDXT5 (  CPicturePixel * aTexel,
                                    const byte_t *  aData ) const {
	CUnTexture::GetTexelA5( aTexel, aData );
	CUnTexture::GetTexelC3( aTexel, aData + 8 );
}
#endif



////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_URTEXTURE
int runURTextureUnitTest(int argc, char *argv[])
{
	return 0;
}


int main(int argc, char *argv[])
{
	URTexture test;


	printf("[URTexture class test]\n");

	return runURTextureUnitTest(argc, argv);
}
#endif

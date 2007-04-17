/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : OpenGLPrinter
 * License : No use w/o permission (C) 2006 Mongoose
 * Comments: 
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2007.01.31:
 * Mongoose - Removed SDL_ttf implementation and replaced with FreeType
 *            based implementation.
 *
 * 2006.07.30:
 * Mongoose - Created
 ==========================================================================*/

//#include "FreyjaOpenGL.h"

#include "OpenGLPrinter.h"

using namespace freyja3d;

#ifdef HAVE_FREETYPE2
FT_Library OpenGLPrinter::mLibrary = NULL;
#endif // HAVE_FREETYPE2


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

OpenGLPrinter::OpenGLPrinter()
{
#ifdef HAVE_FREETYPE2
	FT_Error error;

	if (mLibrary == NULL)
	{
		error = FT_Init_FreeType(&mLibrary); 
		
		if (error) 
		{
			printf("FreeType library initialization failed...\n");
		}
	}
#endif // HAVE_FREETYPE2
}


OpenGLPrinter::~OpenGLPrinter()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

bool OpenGLPrinter::GenerateTexture(const char *filename,
									const unsigned int pt, 
									const unsigned int dpi,
									const char *text,
									unsigned char *image, 
									const unsigned int image_width)
{
#ifdef HAVE_FREETYPE2
	FT_Face face;
	FT_Error error;

	if (mLibrary)
	{
		error = FT_New_Face(mLibrary, filename, 0, &face); 

		if (error) 
		{
			printf("FreeType fontface creation failed for '%s'...\n", filename);
			return false;
		}		
		else
		{
			// 50pt, 100dpi
			//error = FT_Set_Char_Size( face, 50 * 64, 0, 100, 0 );
			error = FT_Set_Char_Size( face, pt * 64, 0, dpi, 0 );

			if (error)
			{
				printf("FreeType font size set failed for '%s'...\n", filename);
				return false;
			}
			else
			{

			}
		}
	}

	FT_GlyphSlot slot = face->glyph;
	//FT_Bitmap bitmap;

	const unsigned int x_padding = 8;//pt/4;
	const unsigned int y_padding = pt;
	const unsigned int n = strlen(text);

	// Start pen at padding offset from edges on the 0th row...
	unsigned int pen_y = y_padding;
	unsigned int pen_x = x_padding;
	unsigned int row = 1;

	for (unsigned int i = 0; i < n; ++i) 
	{ 
		/* load glyph image into the slot (erase previous one) */ 
		error = FT_Load_Char(face, text[i], FT_LOAD_RENDER); 
			
		if ( error ) 
			continue; /* ignore errors */ 

		if (pen_x + slot->bitmap.width > image_width)
		{
			++row;
			pen_y = (row * y_padding);
			pen_x = x_padding;
		}
			
		if (pen_y + slot->bitmap.rows > image_width) // error
		{
			printf("* %s:%i, ERROR: Row %i cut off @ %ipts, %i dpi.\n",
				   __FILE__, __LINE__, row, pt, dpi);
			return false;
		}

		AddGlyphToTexture32(image, image_width,
							slot->bitmap.buffer,
							slot->bitmap.width, slot->bitmap.rows,
							pen_x, pen_y - slot->bitmap_top);

		pen_x += slot->bitmap.width + x_padding;
	}

	return true;
#else
	return false;
#endif // HAVE_FREETYPE2
}


bool OpenGLPrinter::SavePPM(const char *filename,
							unsigned char *image, 
							unsigned int w, unsigned int h)
{
	FILE *f = fopen(filename, "w");

	if (f)
	{
		// ASCII PPM header
		fprintf(f, "P3\n");
		fprintf(f, "# CREATOR OpenGLPrinter::SavePPM\n");
		fprintf(f, "%i %i\n", w, h);
		fprintf(f, "255\n");
				
		for (unsigned int i = 0, sz = w * h; i < sz; ++i)
		{
			fprintf(f, "%i\n", image[i]);
		}

		printf("Wrote '%s'...\n", filename);

		return true;
	}

	return false;
}


bool OpenGLPrinter::SaveTGA(const char *filename, 
							unsigned char *image,
							unsigned int width, unsigned int height)
{
	if (!image || !width || !height)
	{
		printf("OpenGLPrinter::SaveTGA() ERROR: Invalid image!\n");
		return false;
	}

	FILE *f = fopen(filename, "wb");

	if (!f)
	{
		printf("OpenGLPrinter::SaveTGA() ERROR: Couldn't write screenshot.\n");
		perror("OpenGLPrinter::SaveTGA() ERROR: ");
		return false;
	}

	// Build a TGA header
	char comment[32] = "OpenGLPrinter::SaveTGA\0";
	unsigned char comment_lenght = strlen(comment);
	unsigned char colormap_type = 0;
	unsigned char image_type = 2;
	unsigned short colormap_index = 0;
	unsigned short colormap_lenght = 0;
	unsigned char colormap_bbp = 0;
	unsigned short origin_x = 0, origin_y = 0;
	unsigned short swidth = width;
	unsigned short sheight = height;
	unsigned char bpp = 32;
	unsigned char desc_flags = 32; // vertical flipped

	// Write TGA header
	fwrite(&comment_lenght, 1, 1, f);
	fwrite(&colormap_type, 1, 1, f); 
	fwrite(&image_type, 1, 1, f);
	fwrite(&colormap_index, 2, 1, f);
	fwrite(&colormap_lenght, 2, 1, f);
	fwrite(&colormap_bbp, 1, 1, f);
	fwrite(&origin_x, 2, 1, f);
	fwrite(&origin_y, 2, 1, f);
	fwrite(&swidth, 2, 1, f);
	fwrite(&sheight, 2, 1, f);
	fwrite(&bpp, 1, 1, f);
	fwrite(&desc_flags, 1, 1, f);

	// Write comment
	fwrite(&comment, 1, comment_lenght, f);

	unsigned int size = width * height * 4;
 
	for (unsigned int i = 0; i < size; i += 4)
	{
		unsigned char tmp = image[i];
		image[i] = image[i + 2];
		image[i + 2] = tmp;
	}

	// Write image data
	if (fwrite(image, size, 1, f) < 1)
	{
		printf("OpenGLPrinter::SaveTGA()  Disk write failed.\n");
		perror("OpenGLPrinter::SaveTGA() \n");
		fclose(f);
		return false;
	}

	fclose(f);

	printf("Wrote '%s'...\n", filename);

	return true;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////

void OpenGLPrinter::AddGlyphToTexture32(unsigned char *image, 
										unsigned int img_width,
										unsigned char *pixmap,
										unsigned int width, unsigned int height,
										unsigned int pen_x, unsigned int pen_y)
{
	for (unsigned int i = 0; i < width; ++i)
	{
		for (unsigned int j = 0; j < height; ++j)
		{
			const unsigned int off = ((i + pen_x) + (j + pen_y) * img_width)*4;
			const unsigned char g = pixmap[j * width + i];
			image[off  ] |= g;
			image[off+1] |= g;
			image[off+2] |= g;
			image[off+3] |= g;
		}
	}
}


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_OPENGLPRINTER
int runOpenGLPrinterUnitTest(int argc, char *argv[])
{
	OpenGLPrinter test;

	const char text[] = 
	"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz1234567890~!@#$%^&*()-=+;:_'\",./?[]|\\ `<>";
	
	const unsigned int image_width = 256;
	unsigned char image[image_width*image_width*4];
	
	// sazanami-gothic
	memset(image, 0, sizeof(image)); // clear buffer
	test.GenerateTexture("/home/mongoose/.fonts/sazanami-gothic.ttf", 24, 100,
						 text, image, image_width);
	test.SaveTGA("/tmp/sazanami-gothic.tga", image, image_width, image_width);

	// tahoma
	memset(image, 0, sizeof(image)); // clear buffer
	test.GenerateTexture("/home/mongoose/.fonts/tahoma.ttf", 24, 100,
						 text, image, image_width);
	test.SaveTGA("/tmp/tahoma.tga", image, image_width, image_width);

	// zrnic___
	memset(image, 0, sizeof(image)); // clear buffer
	test.GenerateTexture("/home/mongoose/.fonts/zrnic___.ttf", 28, 100,
						 text, image, image_width);
	test.SaveTGA("/tmp/zrnic___.tga", image, image_width, image_width);

	return 0;
}


int main(int argc, char *argv[])
{
	printf("\n[OpenGLPrinter class test]\n");

	return runOpenGLPrinterUnitTest(argc, argv);
}
#endif


////////////////////////////////////////////////////////////
// Non-class junk code, floating around
////////////////////////////////////////////////////////////

#if 0
void mglPrint2d(float x, float y, float scale, char *string)
{
	gl_font_t *font = gFontTest;

	if (!font)
	{
		static int errors = 0;

		if (errors < 10)
			printf("ERROR: glPrintf2d failed, %i\n", ++errors);
		return;
	}

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, font->textureId);
	glTranslatef(x, y, 0);
	glScalef(scale, scale, 1);
	
	/* FIXME: 
	 * Add utf-8 dencoding of char* string
	 *
	 *	Also this string must be preprocessed to have glyph offsets
	 * instead of ASCII text in it and support counts over 256 */
	glListBase(font->drawListBase - font->utf8Offset);
	glCallLists(strlen(string), GL_BYTE, string);
	glPopMatrix();
}


void mglPrint3d(float x, float y, float z, 
					float pitch, float yaw, float roll, 
					float scale,
					char *string)
{
	gl_font_t *font = gFontTest;

	if (!font)
	{
		static int errors = 0;

		if (errors < 10)
			printf("ERROR: glPrintf3d failed, %i\n", ++errors);
		return;
	}

	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, font->textureId);
	glTranslatef(x, y, z);
	glRotatef(roll,  1, 0, 0);
	glRotatef(yaw,   0, 1, 0);
	glRotatef(pitch, 0, 0, 1);
	glScalef(scale, scale, scale);
	
	/* FIXME: 
	 * Add utf-8 dencoding of char* string
	 *
	 *	Also this string must be preprocessed to have glyph offsets
	 * instead of ASCII text in it and support counts over 256 */
	glListBase(font->drawListBase - font->utf8Offset);
	glCallLists(strlen(string), GL_BYTE, string);
	glPopMatrix();
}

#endif




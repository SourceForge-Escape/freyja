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


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////


OpenGLPrinter::OpenGLPrinter(const char *filenameTTF) : 
	mFace(NULL)
{
	FT_Error error;

	if (mLibrary == NULL)
	{
		error = FT_Init_FreeType(&mLibrary); 
		
		if (error) 
		{
			printf("FreeType library initialization failed...\n");
		}
	}

	if (mLibrary)
	{
		error = FT_New_Face(mLibrary, filenameTTF, 0, &mFace); 

		if (error) 
		{
			printf("FreeType fontface creation failed '%s'...\n", filenameTTF);
		}		
		else
		{
			// 50pt, 100dpi
			//error = FT_Set_Char_Size( mFace, 50 * 64, 0, 100, 0 );

			error = FT_Set_Char_Size( mFace, 24 * 64, 0, 100, 0 );

			if (error)
			{
				printf("FreeType font size set failed...\n");
			}
			else
			{
				const char *text[] = {
#if 0
					"ABCDEFGHIJKLMNOPQRSTUVWXYZ",
					"abcdefghijklmnopqrstuvwxyz",
					"1234567890~!@#$%^&*()-=+;:",
					"'\",./?[]|\\ <>`" 
#else
					"ABCDEFGHIJKLM", // 'square up' texture map, less memory
					"NOPQRSTUVWXYZ",
					"abcdefghijklm",
					"nopqrstuvwxyz",
					"1234567890~!@",
					"#$%^&*()-=+;:",
					"'\",./?[]|\\ <>`"
#endif
				};

				const unsigned int lines = 6;
				const unsigned int image_width = 256;
				unsigned char image[image_width*image_width*4];
				GenerateTexture(lines, text, image, image_width);
			}
		}
	}
}


void OpenGLPrinter::GenerateTexture(const unsigned int count, const char **text,
									unsigned char *image, 
									unsigned int image_width)
{
	FT_Error error;
	FT_GlyphSlot slot = mFace->glyph;
	//FT_Bitmap bitmap;

	for (unsigned int line = 0; line < count; ++line)
	{ 
		const unsigned int n = strlen(text[line]);
		const unsigned int x_padding = 6;//8;
		const unsigned int y_padding = 24;

		// Start pen at 8, 32...
		unsigned int pen_x = 8;
		unsigned int pen_y = 24 + line * y_padding;

		for (unsigned int i = 0; i < n; ++i) 
		{ 
			/* load glyph image into the slot (erase previous one) */ 
			error = FT_Load_Char(mFace, text[line][i], FT_LOAD_RENDER); 
			
			if ( error ) 
				continue; /* ignore errors */ 

			AddGlyphToTexture32(image, image_width,
								slot->bitmap.buffer,
								slot->bitmap.width, slot->bitmap.rows,
								pen_x, pen_y - slot->bitmap_top);

			pen_x += slot->bitmap.width + x_padding;
		}
	}

	SaveTGA("/tmp/test.tga", image, image_width, image_width);
}


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

#else

OpenGLPrinter::OpenGLPrinter(const char *filenameTTF)
{
}

#endif // HAVE_FREETYPE2


OpenGLPrinter::~OpenGLPrinter()
{
#if 0
  if (gFontTest)
  {
    glDeleteLists(gFontTest->drawListBase, gFontTest->count);
    delete gFontTest;
  }
#endif
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////

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
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_OPENGLPRINTER
int runOpenGLPrinterUnitTest(int argc, char *argv[])
{
	OpenGLPrinter test("/home/mongoose/.fonts/zrnic___.ttf");

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[OpenGLPrinter class test]\n");

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




void mglEnterMode2d(unsigned int width, unsigned int height)
{
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	/* This allows alpha blending of 2D textures with the scene */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0.0, (GLdouble)width, (GLdouble)height, 0.0, 0.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}


void mglExitMode2d()
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glPopAttrib();

	glMatrixMode(GL_MODELVIEW);
}
#endif




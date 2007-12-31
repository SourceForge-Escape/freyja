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
 * 2007.04.16:
 * Mongoose - Cleaned up for public release.  Removed tons of old code.
 *
 * 2007.01.31:
 * Mongoose - Removed SDL_ttf implementation and replaced with FreeType
 *            based implementation.
 *
 * 2006.07.30:
 * Mongoose - Created, based on old 2001 midgard codebase.
 ==========================================================================*/

#include <mstl/SystemIO.h>

#ifdef HAVE_OPENGL
#   include "FreyjaOpenGL.h"
#   include "Texture.h"
#endif // HAVE_OPENGL

#include "OpenGLPrinter.h"

using namespace freyja3d;

#ifdef HAVE_FREETYPE2
FT_Library OpenGLPrinter::mLibrary = NULL;
#endif // HAVE_FREETYPE2


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

OpenGLPrinter::OpenGLPrinter() :
	mInit( false )
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
#ifdef HAVE_OPENGL
	glDeleteLists(mFont.mListBase, mFont.mCount);
#endif // HAVE_OPENGL
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

const char *OpenGLPrinter::GetASCIIString()
{
	return " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}";
}


bool OpenGLPrinter::GenerateFont(Font &font,
								 const char *text, const glyph_t *glyphs,
								 const unsigned int textureId,
								 const unsigned char *image, 
								 const unsigned int image_width)
{
#ifdef HAVE_OPENGL
	if (!text || !text[0] || !glyphs || !image_width || ! image)
		return false;

	printf("@ Generating gl font from texture...\n");

	font.mCount = strlen(text);
	font.mTextureId = textureId;
	font.mOffset = (int)text[0];
	font.mListBase = glGenLists(font.mCount);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBindTexture(GL_TEXTURE_2D, textureId);

	const float spacing = 4.0f;
	const float invWidth = 1.0f / (float)image_width;

	for (unsigned int i = 0; i < font.mCount; i++)
	{
		/* Generate texture coordinates for this glyph */
		float u = invWidth * (float)glyphs[i].x;
		float v = invWidth * (float)glyphs[i].y;
		float u2 = invWidth * (float)(glyphs[i].x + glyphs[i].w);
		float v2 = invWidth * (float)(glyphs[i].y + glyphs[i].h);
		const int h = glyphs[i].yOffset;

		/* Make a list for this TTF glyph, one nonuniform Quad per glyph */
		glNewList(font.mListBase + i, GL_COMPILE);

		glBegin(GL_QUADS);

		 glTexCoord2f(u2, v);	/* Top, right */
		 glVertex3i(glyphs[i].w, h + glyphs[i].h, 0);
		
		 glTexCoord2f(u, v);	/* Top, left */
		 glVertex3i(0, h + glyphs[i].h, 0);

		 glTexCoord2f(u, v2);	/* Bottom, left */
		 glVertex3i(0, h, 0);
		
		 glTexCoord2f(u2, v2);	/* Bottom, right */
		 glVertex3i(glyphs[i].w, h, 0);

		glEnd();
		
		/* Move To The Left Of The Character */
		glTranslated(glyphs[i].w + spacing, 0, 0);
		glEndList();
	}

	return true;
#else
	return false;
#endif
}


bool OpenGLPrinter::GenerateTexture(const char *filename,
									const unsigned int pt, 
									const unsigned int dpi,
									const char *text, glyph_t *glyphs,
									unsigned char *image, 
									const unsigned int image_width)
{
	if ( !filename || !filename[0] || !mstl::SystemIO::File::DoesFileExist( filename ) )
		return false;

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


	const unsigned int x_padding = 8;
	// Idealy you'd iterate all glyphs for max_y instead of using pt
	const unsigned int y_padding = pt;
	const unsigned int n = strlen(text);

	// Start pen at padding offset from edges on the 0th row...
	unsigned int pen_y = y_padding;
	unsigned int pen_x = x_padding;
	unsigned int row = 1;

	FT_GlyphSlot slot = face->glyph;

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

		if (glyphs)
		{
			glyphs[i].x = pen_x;
			glyphs[i].y = pen_y - slot->bitmap_top;
			glyphs[i].w = slot->bitmap.width;
			glyphs[i].h = slot->bitmap.rows;
			glyphs[i].yOffset = ( slot->metrics.height - slot->metrics.horiBearingY ) >> 6;

			if ( glyphs[i].yOffset )
			{
				//printf("\t %ix%i, %i\n", glyphs[i].w, glyphs[i].h, glyphs[i].yOffset);				
				glyphs[i].yOffset = -glyphs[i].yOffset;
			}
		}

		pen_x += slot->bitmap.width + x_padding;
	}

	return true;
#else
	return false;
#endif // HAVE_FREETYPE2
}


void OpenGLPrinter::Print2d(float x, float y, float scale, const char *text)
{
	if (!text || !text[0])
		return;

#ifdef HAVE_OPENGL
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, mFont.mTextureId);
	glTranslatef(x, y, 0);
	glScalef(scale, scale, 1);
	
	RenderString(text);

	glPopMatrix();
#endif // HAVE_OPENGL
}


void OpenGLPrinter::Print3d(float x, float y, float z, 
							float pitch, float yaw, float roll, 
							float scale, const char *text)
{
	if (!text || !text[0])
		return;

#ifdef HAVE_OPENGL
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, mFont.mTextureId);
	glTranslatef(x, y, z);
	glRotatef(roll,  1, 0, 0);
	glRotatef(yaw,   0, 1, 0);
	glRotatef(pitch, 0, 0, 1);
	glScalef(scale, scale, scale);
	
	RenderString(text);

	glPopMatrix();
#endif // HAVE_OPENGL
}


bool OpenGLPrinter::SaveMetadata(const char *filename, 
								 const char *text, glyph_t *glyphs)
{
	if (!glyphs || !text || !text[0])
		return false;

	unsigned int n = strlen(text);

	FILE *f = fopen(filename, "w");

	if (f)
	{
		for (unsigned int i = 0; i < n; ++i)
		{
			fprintf(f, "'%c' @ %i, %i ( %i x %i )\n", 
					text[i], 
					glyphs[i].x, glyphs[i].y, glyphs[i].w, glyphs[i].h);
		}

		fclose(f);

		return true;
	}
	else
	{
		perror("OpenGLPrinter::SaveMetadata() ERROR: ");
	}

	return false;
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

		fclose(f);

		return true;
	}
	else
	{
		printf("OpenGLPrinter::SavePPM() ERROR: Couldn't write PPM.\n");
		perror("OpenGLPrinter::SavePPM() ERROR: ");
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
		printf("OpenGLPrinter::SaveTGA() ERROR: Couldn't write TGA.\n");
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

bool OpenGLPrinter::Init(const char* font, 
						 const unsigned int pt, const unsigned int dpi)
{
	mInit = false;

	if ( font && font[0] && mstl::SystemIO::File::DoesFileExist( font ) )
	{
		const unsigned int width = 256;
		const char *text = GetASCIIString();
		unsigned char image[width*width*4]; // If texture doesn't copy this is a bad idea.
		glyph_t glyphs[strlen(text)];	

		/* Clear pixmap buffer, generate a texture. */
		memset(image, 0, sizeof(image));

		if ( GenerateTexture(font, pt, dpi, text, glyphs, image, width) )
		{
#ifdef HAVE_OPENGL
			/* Bind texture in OpenGL. */
			int id = Texture::mSingleton->loadBuffer(image, width, width, Texture::RGBA, 32);
#endif // HAVE_OPENGL
		
			mInit = GenerateFont(mFont, text, glyphs, id, image, width);
		}
	}

	return mInit;
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////

void OpenGLPrinter::AddGlyphToTexture32(unsigned char *image, 
										unsigned int img_width,
										unsigned char *pixmap,
										unsigned int width, unsigned int height,
										unsigned int pen_x, int pen_y)
{
	if ( pen_y < 0 )
		pen_y = 0;

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


void OpenGLPrinter::RenderString( const char* text )
{
#ifdef HAVE_OPENGL

	// FIXME: I now support partial text blocks, so this doesn't work anymore,
	//        however it's a good excuse to add extra support since a tranlate
	//        method needs to go here.
	//
	//        The easiest way would be to make a false block range, and hope
	//        no one requests the missing glyphs in that false block.
	//
	//        This will work if using 'ASCII block' or kana strings.

	/* FIXME: 
	 * Add utf-8 dencoding of char* string
	 *
	 *	Also this string must be preprocessed to have glyph offsets
	 * instead of ASCII text in it and support counts over 256 */
	if ( mInit && text && text[0] )
	{
		glListBase( mFont.mListBase - mFont.mOffset );
		glCallLists( strlen(text), GL_BYTE, text );
	}
#endif // HAVE_OPENGL
}


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_OPENGLPRINTER
int RunOpenGLPrinterUnitTest(int argc, char *argv[])
{
	OpenGLPrinter test;

	const char *text = test.GetASCIIString();
	glyph_t glyphs[strlen(text)];	

	const unsigned int image_width = 256;
	unsigned char image[image_width*image_width*4];
	
	// sazanami-gothic
	memset(image, 0, sizeof(image)); // clear buffer
	test.GenerateTexture("/home/mongoose/.fonts/sazanami-gothic.ttf", 24, 100,
						 text, glyphs, image, image_width);
	test.SaveTGA("/tmp/sazanami-gothic.tga", image, image_width, image_width);
	test.SaveMetadata("/tmp/sazanami-gothic.txt", text, glyphs);

	// tahoma
	memset(image, 0, sizeof(image)); // clear buffer
	test.GenerateTexture("/home/mongoose/.fonts/tahoma.ttf", 24, 100,
						 text, glyphs, image, image_width);
	test.SaveTGA("/tmp/tahoma.tga", image, image_width, image_width);
	test.SaveMetadata("/tmp/tahoma.txt", text, glyphs);

	// zrnic___
	memset(image, 0, sizeof(image)); // clear buffer
	test.GenerateTexture("/home/mongoose/.fonts/zrnic___.ttf", 28, 100,
						 text, glyphs, image, image_width);
	test.SaveTGA("/tmp/zrnic___.tga", image, image_width, image_width);
	test.SaveMetadata("/tmp/zrnic___.txt", text, glyphs);

	return 0;
}


int main(int argc, char *argv[])
{
	printf("\n[OpenGLPrinter class test]\n");
	return RunOpenGLPrinterUnitTest(argc, argv);
}
#endif





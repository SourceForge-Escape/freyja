/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Midgard
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : OpenGLPrinter
 * License : No use w/o permission (C) 2004 Mongoose
 * Comments: Text output to OpenGL
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.11.06:
 * Mongoose - Created
 ==========================================================================*/


#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef HAVE_OPENGL
#   include <GL/gl.h>
#   include <GL/glu.h>
#else
#   error Texture class requires HAVE_OPENGL
#endif

#ifdef HAVE_SDL_TTF
#   include <SDL/SDL_ttf.h>
#endif


#include "OpenGLPrinter.h"


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

OpenGLPrinter::OpenGLPrinter()
{
	initFonts();
}


OpenGLPrinter::~OpenGLPrinter()
{
	unsigned int i;
	gl_font_t *f;

	for (i = mFonts.begin(); i < mFonts.end(); ++i)
	{
		f = mFonts[i];

		if (f)
		{
			glDeleteLists(f->drawListBase, f->count);
			delete f;
		}
	}
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////


void bufferedPrintf(char *string, unsigned int len, char *s, ...)
{
	va_list args;


	if (s && s[0])
	{
		va_start(args, s);
		vsnprintf(string, len-1, s, args);	
		string[len-1] = 0;
		va_end(args);
	}
}


void glPrint2d(float x, float y, float scale, char *string)
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


void glPrint3d(float x, float y, float z, 
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


int Texture::loadFontTTF(char *filename, 
						 unsigned int utf8Offset, unsigned int count)
{
	ttf_texture_t *texture;
	unsigned char rgb[3] = {0xff, 0xff, 0xff};


	if (!filename || !filename[0])
	{
		printf("fontTest> Passed bad filename\n");
		return -1;
	}

	texture = generateFontTexture(filename, 24, 256, rgb,
											//0x303f, 0x3093-0x303f, // hiragana
											//32, 126 - 32,  // english
											utf8Offset, count,
											false);

	if (texture)
	{
#ifdef DUMP_TTF_TGA
		FILE *f = fopen("/tmp/font.tga", "wb");
		if (f)
		{
			mtk_image__tga_save(f, texture->texture, 256, 256, 4);
			fclose(f);
		}
		else
		{
			perror("/tmp/ttf_font.tga");
		}
#endif

		gFontTest = generateFont(texture);

		/* FIXME: Until UTF8 decoder is working, we map from 
			ASCII when rendering */
		gFontTest->utf8Offset = 32; // hack to use ASCII strings to test unicode

		delete [] texture->texture;
		delete [] texture->glyphs;
		delete texture;
	}
	else
	{
		return -2;
	}

	return 0;
}


gl_font_t *Texture::generateFont(ttf_texture_t *texture)
{
#ifdef HAVE_SDL_TTF
	const float spacing = 4.0;
	unsigned int i;
	float u, v, u2, v2;
	int h;
	gl_font_t *font;


	if (!texture)
		return NULL;

	printf("@ Generating gl font from texture...\n");

	font = new gl_font_t;
	font->utf8Offset = texture->utf8Offset;
	font->count = texture->count;
	font->textureId = loadBuffer(texture->texture,
										  texture->width, texture->width, RGBA, 32);

	glColor3f(1.0, 1.0, 1.0);
	font->drawListBase = glGenLists(texture->count);
	glBindTexture(GL_TEXTURE_2D, font->textureId);

	for (i = 0; i < texture->count; i++)
	{
		/* Generate texture coordinates for this TTF glyph */
		u = (float)texture->glyphs[i].x / (float)texture->width;
		v = (float)texture->glyphs[i].y / (float)texture->width;
		u2 = (((float)texture->glyphs[i].x + (float)texture->glyphs[i].w) /
				(float)texture->width);
		v2 = (((float)texture->glyphs[i].y + (float)texture->glyphs[i].h) /
				(float)texture->width);

#ifdef DEBUG_TTF_OFFSET
		if (i+texture->utf8Offset == 'y' || i+texture->utf8Offset == 'x')
		{
			printf("%c: %i %i %i\n", 
					 i+texture->utf8Offset,
					 texture->fontDescent,
					 texture->glyphs[i].miny, texture->glyphs[i].maxy);
		}
#endif

		if (texture->glyphs[i].h < texture->fontHeight)
		{
			h = texture->fontHeight - texture->glyphs[i].h;
		}
		else
		{
			h = 0;
		}

		h += -texture->fontHeight/2-(texture->fontDescent + texture->glyphs[i].miny);

		/* Make a list for this TTF glyph, one nonuniform Quad per glyph */
		glNewList(font->drawListBase + i, GL_COMPILE);

		glBegin(GL_QUADS);
		 glTexCoord2f(u2, v);	/* Top, right */
		 glVertex3i(texture->glyphs[i].w, h, 0);
		
		 glTexCoord2f(u, v);	/* Top, left */
		 glVertex3i(0, h, 0);

		 glTexCoord2f(u, v2);	/* Bottom, left */
		 glVertex3i(0, h+texture->glyphs[i].h, 0);
		
		 glTexCoord2f(u2, v2);	/* Bottom, right */
		 glVertex3i(texture->glyphs[i].w, h+texture->glyphs[i].h, 0);
		glEnd();
		
		/* Move To The Left Of The Character */
		glTranslated(texture->glyphs[i].w + spacing, 0, 0);
		glEndList();
	}

	return font;
#else
	return NULL;
#endif
}


ttf_texture_t *Texture::generateFontTexture(char *filename, int pointSize, 
														  unsigned int textureWidth,
														  unsigned char color[3],
														  unsigned int utf8Offset,
														  unsigned int count,
														  char verbose)
{
#ifdef HAVE_SDL_TTF
	unsigned int i;
	int xx = 0, yy = 0, hh = 0, k, h, w, offset;
	unsigned char b;
	unsigned char *image;
	SDL_Surface* glyph;
	SDL_Color sdlColor;
	TTF_Font *font;
	ttf_texture_t *texture;
	FILE *f;


	sdlColor.r = color[0];
	sdlColor.g = color[1];
	sdlColor.b = color[2];

	if (!mFlags & fUseSDL_TTF)
	{
		printf("SDL_TTF couldn't be used... exiting\n");
		return NULL;
	}

	if (pointSize < 8)
	{
		pointSize = 8;
	}

	/* Work around for TTF_OpenFont for file not found segfault */
	f = fopen(filename, "rb");
	if (!f)
	{
		printf("generateFontTexture> Couldn't load '%s'\n", filename);
		perror(filename);
		return NULL;
	}
	fclose(f);

	// Open the font file at the requested point size
	font = TTF_OpenFont(filename, pointSize);

	if (font == NULL)
	{
		fprintf(stderr, "generateFontTexture> Couldn't load %d pt font from %s: %s\n",
				  pointSize, filename, SDL_GetError());
		return NULL;
	}

	// TTF_STYLE_NORMAL, TTF_STYLE_BOLD, TTF_STYLE_ITALIC, TTF_STYLE_UNDERLINE
	int renderStyle = TTF_STYLE_NORMAL;
	TTF_SetFontStyle(font, renderStyle);

	/* Allocate a new TTF font texture */
	printf("@ Creating font texture from '%s'...\n", filename);
	texture = new ttf_texture_t;
	texture->width = textureWidth;
	texture->utf8Offset = utf8Offset;
	texture->count = count;
	texture->glyphs = new ttf_glyph_t[count];
	texture->texture = new unsigned char[textureWidth*textureWidth*4];
	memset(texture->texture, 0, textureWidth*textureWidth*4);
		
	texture->fontHeight = TTF_FontHeight(font);
	texture->fontAscent = TTF_FontAscent(font);
	texture->fontDescent = TTF_FontDescent(font);
	texture->fontSpacing = TTF_FontLineSkip(font);

	for (i = 0; i < count; ++i) 
	{
		glyph = TTF_RenderGlyph_Blended(font, i + utf8Offset, sdlColor);

		if (glyph)
		{
	      image = (unsigned char*)glyph->pixels;

			TTF_GlyphMetrics(font, i + utf8Offset,
								  &texture->glyphs[i].minx, &texture->glyphs[i].maxx, 
								  &texture->glyphs[i].miny, &texture->glyphs[i].maxy,
								  &texture->glyphs[i].advance);

	      texture->glyphs[i].w = glyph->w;
	      texture->glyphs[i].h = glyph->h;
			
	      if (xx + texture->glyphs[i].w > ((int)textureWidth - 1))
	      {
				yy += hh;
				hh = 2;
				xx = 2;
				texture->glyphs[i].x = 0;
				texture->glyphs[i].y = yy;
	      }
	      else
	      {
				(texture->glyphs[i].h > hh) ? hh = texture->glyphs[i].h: 0;
				texture->glyphs[i].x = xx;
				texture->glyphs[i].y = yy;
	      }
			
	      xx += glyph->w;
			
			if (verbose)
			{
				printf("0x%x : %ix%i @ %i, %i\n", i + utf8Offset,
						 texture->glyphs[i].w, texture->glyphs[i].h, 
						 texture->glyphs[i].x, texture->glyphs[i].y);
			}

	      /* Blit @ xx, yy - in pixels */
	      for (k = 0; k < glyph->w*glyph->h; ++k)
	      {
				w = texture->glyphs[i].x + k%glyph->w;
				h = texture->glyphs[i].y + k/glyph->w;
				offset = (w + h*textureWidth);
				
				if (verbose)
				{
					printf("Offset: %i; Pixel: %i,%i; Data: 0x%08x\n", 
							 offset, w, h, *((unsigned int *)&image[k*4]));
				}
				
				/* 32-bit ARGB to RGBA */
				b = image[k*4+3];
				texture->texture[offset*4]   = image[k*4]   = image[k*4+1];
				texture->texture[offset*4+1] = image[k*4+1] = image[k*4+2];
				texture->texture[offset*4+2] = image[k*4+2] = image[k*4+3];
				texture->texture[offset*4+3] = image[k*4+3] = b;
	      }
		}
	}

	TTF_CloseFont(font);

	return texture;
#else
	printf("SDL_TTF not enabled in this build.\n");
	return NULL;
#endif
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////
void Texture::initFonts()
{
#ifdef HAVE_SDL_TTF
	// Initialize the TTF library
	if (TTF_Init() < 0)
	{
		fprintf(stderr, "initSDL_TTF> TTF_Init() failed!\n");
		fprintf(stderr, "initSDL_TTF> Error is [%s].\n", SDL_GetError());
	}
	else
	{	
		mFlags |= fUseSDL_TTF;

		printf("@ Started SDL_TTF subsystem...\n");
		atexit(TTF_Quit);
	}
#else
#   error "Not implemented"
#endif
}

////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


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

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[OpenGLPrinter class test]\n");

	return runOpenGLPrinterUnitTest(argc, argv);
}
#endif

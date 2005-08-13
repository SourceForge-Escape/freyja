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
 *-- Test Defines -----------------------------------------------
 *           
 * UNIT_TEST_OPENGLPRINTER - Builds OpenGLPrinter class as a console unit test 
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.11.06:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__MIDGARD_MONGOOSE_OPENGLPRINTER_H_
#define GUARD__MIDGARD_MONGOOSE_OPENGLPRINTER_H_

#include <mstl/Vector.h>

typedef struct
{
	int x, y, w, h;
	int minx; int maxx; int miny; int maxy; int advance;

} ttf_glyph_t; 

typedef struct
{
	unsigned int utf8Offset;
	unsigned int count;   

/* [utf8Offset -> utf8Offset+count], 
	 matches indexing into glyphs[0 -> count] for texcoords, etc
	----------------------------------------
	41     -> 126     ASCII English w/ special chars,
	0x303f -> 0x3093  Japanese hiragana kana,
	0x301a -> 0x30f6  Japanese katakana kana */

	unsigned int width;     /* Width and height of RGBA texture */
	unsigned char *texture; /* RGBA texture data */

	ttf_glyph_t *glyphs;    /* For typesetting and rendering use */
	int fontHeight;
	int fontAscent;
	int fontDescent;
	int fontSpacing;

} ttf_texture_t; 

typedef struct
{
	unsigned int utf8Offset;
	unsigned int count;
	int textureId;
	int drawListBase;

} gl_font_t;


class OpenGLPrinter
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	OpenGLPrinter();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of OpenGLPrinter
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~OpenGLPrinter();
	/*------------------------------------------------------
	 * Pre  : OpenGLPrinter object is allocated
	 * Post : Deconstructs an object of OpenGLPrinter
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////



	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////


	Vector<gl_font_t *> mFonts;     /* Fun with fonts */
};

#endif

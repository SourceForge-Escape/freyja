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

#ifndef GUARD__FREYJA3D_OPENGLPRINTER_H_
#define GUARD__FREYJA3D_OPENGLPRINTER_H_

#ifdef HAVE_FREETYPE2
#   include <ft2build.h>
#   include FT_FREETYPE_H
#endif

namespace freyja3d {

typedef struct {
	unsigned int x, y, w, h;
	unsigned int yOffset;
} glyph_t;


class Font
{
public:
	Font() : mCount(0), mOffset(0), mListBase(0), mTextureId(0) {}

	Font operator =(const Font &font)
	{
		Font f;
		f.mCount = font.mCount;
		f.mOffset = font.mOffset;
		f.mListBase = font.mListBase;
		f.mTextureId = font.mTextureId;
		return f;
	}

	unsigned int mCount;
	unsigned int mOffset;
	unsigned int mListBase;
	unsigned int mTextureId;
};


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
	 ------------------------------------------------------*/

	~OpenGLPrinter();
	/*------------------------------------------------------
	 * Pre  : OpenGLPrinter object is allocated
	 * Post : Deconstructs an object of OpenGLPrinter
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	const char *GetASCIIString();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns 'block order' visible ASCII characters.
	 *
	 ------------------------------------------------------*/

	//void Print3dBillboard(float x, float y, float z,
	//					  float scale, const char *text);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Not implemented in this release.
	 *
	 ------------------------------------------------------*/

	void Print3d(float x, float y, float z, 
				 float pitch, float yaw, float roll, 
				 float scale, const char *text);
	/*------------------------------------------------------
	 * Pre  : <X>, <Y>, and <Z> are valid world coordinates
	 *        <String> is a valid string
	 *
	 *        System::bufferString(..) can cache printf()
	 *        style calls for use with this method
	 *
	 * Post : Renders string in OpenGL ( 3d projection )
	 *
	 ------------------------------------------------------*/

	void Print2d(float x, float y, float scale, const char *text);
	/*------------------------------------------------------
	 * Pre  : <X>, and <Y> are valid world coordinates
	 *        <String> is a valid string
	 *     
	 *        Requires glEnterMode2d() call before entry
	 *
	 *        System::bufferString(..) can cache printf()
	 *        style calls for use with this method
	 *
	 * Post : Renders string in OpenGL ( 2d projection )
	 *
	 *        Call glExitMode2d() after finishing calls
	 *        to this method and other 2d rendering
	 *
	 ------------------------------------------------------*/

	bool GenerateFont(Font &font,
					  const char *text, const glyph_t *glyphs,
					  const unsigned int textureId,
					  const unsigned char *image, 
					  const unsigned int image_width);
	/*------------------------------------------------------
	 * Pre  : 
	 *
	 * Post : Returns true if successful.
	 *        
	 ------------------------------------------------------*/

	bool GenerateTexture(const char *filename,
						 const unsigned int pt, const unsigned int dpi,
						 const char *text, glyph_t *glyphs,
						 unsigned char *image, const unsigned int image_width);
	/*------------------------------------------------------
	 * Pre  : Takes a single string and generates 'best fit' texture.
	 *
	 *        <filename> of the TTF font used to generate texture.
	 *        <pt> point size and <dpi> dots per inch are required.
	 *        <text> of the glyphs used to generate texture.
	 *        <glyphs> stores pos/dim of glyphs that correspond to <text>.
	 *        <image> is allocated RGBA, 32bpp, <image_width>^2. 
	 *
	 * Post : Returns true if successful.
	 *        Doesn't partial blit glyphs that don't 'fit'.
	 *
	 *        If this returns false then try calling it with a smaller 
	 *        pt size or a larger texture size.
	 *
	 *        If <glyphs> is NULL you just don't get the information back.
	 *
	 ------------------------------------------------------*/

	bool SavePPM(const char *filename,
				 unsigned char *image, unsigned int w, unsigned int h);
	/*------------------------------------------------------
	 * Pre  : Allocated RGB, 24bbp - image[w*h*3]
	 * Post : Returns true if PPM was wrote to disk.
	 *
	 ------------------------------------------------------*/

	bool SaveTGA(const char *filename,
				 unsigned char *image, unsigned int w, unsigned int h);
	/*------------------------------------------------------
	 * Pre  : Allocated RGBA, 32bbp - image[w*h*4]
	 * Post : Returns true if TGA was wrote to disk.
	 *
	 ------------------------------------------------------*/

	bool SaveMetadata(const char *filename, 
					  const char *text, glyph_t *glyphs);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	bool Init(const char *font, const unsigned int pt, const unsigned int dpi);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void SetFont(const Font &font) { mFont = font; }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////

	void AddGlyphToTexture32(unsigned char *image, unsigned int img_width,
							 unsigned char *pixmap,
							 unsigned int width, unsigned int height,
							 unsigned int pen_x, unsigned int pen_y);
	/*------------------------------------------------------
	 * Pre  : My... what a large stack you have.
	 *        <image> is allocated and w = h = <img_width> @ 32bpp ARGB 
	 *        <pixmap> is allocated and agrees with <width> and <height>.
	 *        The pen coordinates agree with both values. 
	 *
	 * Post : Generic 32bpp texture glyph 'blit'ter operation.
	 *        The <pixmap> is blitted to <image> ( texture ).
	 *        Currently ARGB/RBGA are the same weights.
	 *
	 ------------------------------------------------------*/

	void RenderString(const char *text);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Converts text to callist(s) and renders.
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

#ifdef HAVE_FREETYPE2
	static FT_Library mLibrary;
#endif // HAVE_FREETYPE2

	Font mFont;
};

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
	(20)41     -> 126     ASCII English w/ special chars,
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

} // namespace freyja3d

#endif // GUARD__FREYJA3D_OPENGLPRINTER_H_

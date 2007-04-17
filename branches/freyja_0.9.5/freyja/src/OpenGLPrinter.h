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

#ifndef GUARD__FREYJA3D_OPENGLPRINTER_H_
#define GUARD__FREYJA3D_OPENGLPRINTER_H_

#ifdef HAVE_FREETYPE2
#   include <ft2build.h>
#   include FT_FREETYPE_H
#endif

namespace freyja3d {

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

	OpenGLPrinter(const char *face);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of OpenGLPrinter
	 *
	 *-- History ------------------------------------------
	 *
	 * 2006.07.30: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~OpenGLPrinter();
	/*------------------------------------------------------
	 * Pre  : OpenGLPrinter object is allocated
	 * Post : Deconstructs an object of OpenGLPrinter
	 *
	 *-- History ------------------------------------------
	 *
	 * 2006.07.30: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	void Print3dBillboard(float x, float y, float z,
						  float scale, char *string);

	void Print3d(float x, float y, float z, 
				 float pitch, float yaw, float roll, 
				 float scale, char *string);
	/*------------------------------------------------------
	 * Pre  : <X>, <Y>, and <Z> are valid world coordinates
	 *        <String> is a valid string
	 *
	 *        System::bufferString(..) can cache printf()
	 *        style calls for use with this method
	 *
	 * Post : Renders string in OpenGL ( 3d projection )
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.03:
	 * Mongoose - Ported to SDL_TTF
	 *
	 * 2001.12.31: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void Print2d(float x, float y, float scale, char *string);
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
	 *-- History ------------------------------------------
	 *
	 * 2003.06.03:
	 * Mongoose - Ported to SDL_TTF
	 *
	 * 2001.12.31: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void Enter2dMode(unsigned int width, unsigned int height);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : OpenGL ortho projection
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.03:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void Exit2dMode();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : OpenGL model matrix projection
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.03:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	gl_font_t *GenerateFont(ttf_texture_t *texture);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.30:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	ttf_texture_t *GenerateFontTexture(char *filename, int pointSize, 
									   unsigned int textureWidth,
									   unsigned char color[3],
									   unsigned int utf8Offset,
									   unsigned int count,
									   char verbose);
	/*------------------------------------------------------
	 * Pre  : <Filename> of TTF font
	 *        <PointSize> to generate
	 *        <TextureWidth> is width of texture, height will match it
	 *        <Color> is RGB 24bit color
	 *        <Utf8Offset> is offset into font's encoding chart
	 *        <Count> is number of glyphs to read from offset start
	 *        <Verbose> dumps debug info to stdout 
	 *
	 * Post : Generates a font texture with typeset info from TTF
	 *
	 *        DOES NOT load the texture itself, call loadFont()
	 *        on returned ttf_texture_t
	 *
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.03:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int LoadTTF(const char *filename, unsigned int offset, unsigned int count);
	/*------------------------------------------------------
	 * Pre  : <Filename> of TTF font
	 *        <Offset> is offset into UTF-8 UNICODE table
	 *        <Count> is number of glyphs to load
	 *
	 * Post : Loads a TTF, 
	 *        Generates: texture image, glyph list, and drawlist
	 *
	 *        Returns font id if sucessful, or < 0 if error
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.07.05:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void Init();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Loads SDL_TTF if avalible
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.03:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void GenerateTexture(const unsigned int count, const char **text,
						 unsigned char *image, unsigned int image_width);
	/*------------------------------------------------------
	 * Pre  : Multiple lines of text to generate texture.
	 *        <image> is allocated RGBA, 32bpp, <image_width>^2. 
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
	 *        <image> is allocated and agrees with <img_width> 
	 *        <pixmap> is allocated and agrees with <width> and <height>.
	 *        The pen coordinates agree with both values. 
	 *
	 * Post : Generic 32bpp texture glyph 'blit'ter operation.
	 *        The <pixmap> is blitted to <image> ( texture ).
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


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

#ifdef HAVE_FREETYPE2
	static FT_Library mLibrary;

	FT_Face mFace;
#endif // HAVE_FREETYPE2
};

} // namespace freyja3d

#endif // GUARD__FREYJA3D_OPENGLPRINTER_H_

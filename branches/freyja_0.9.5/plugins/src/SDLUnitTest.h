/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*================================================================
 * 
 * Project : Midgard
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : SDLUnitTest
 * License : No use w/o permission (C) 2003 Mongoose
 * Comments: 
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2003.12.29:
 * Mongoose - Created, based on my SDLUnitTest C functions and
 *            SDLSystem Class
 ================================================================*/


#ifndef GUARD__MIDGARD_MONGOOSE_SDLUNITTEST_H_
#define GUARD__MIDGARD_MONGOOSE_SDLUNITTEST_H_

#ifdef HAVE_SDL
#   include <SDL/SDL.h>
#else
#   warning "SDLUnitTest.o requires -DHAVE_SDL"
#endif

#ifdef HAVE_OPENGL
#   include <GL/gl.h>
#   include <GL/glu.h>
/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : Midgard, MTK, Freyja, OpenRaider
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440
 * Email   : stu7440@westga.edu
 * Object  : Texture
 * Comments: This is the Texture registery
 *
 *           See file COPYING for license details.
 *
 * 
 *-- History ---------------------------------------------------------- 
 *
 * 2003.06.30, 
 * Mongoose - API update, SDL_TTF support moved here, misc features
 *            SDL_TTF support based on Sam Lantinga's public domain
 *            SDL_TTF demo functions and algorithms 
 *
 * 2001.05.29:
 * Mongoose - Removed legacy code and done clean up
 *
 * 2001.02.19:
 * Mongoose - Moved from GooseEgg to mtk
 *
 * 2000.04.29:
 * Mongoose - Created from old PPM/PCX codebases I have done before
 ==========================================================================*/


#ifndef GUARD_MIDGARD__MONGOOSE_TEXTURE_H
#define GUARD_MIDGARD__MONGOOSE_TEXTURE_H

#include <stdio.h>

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



class Texture
{
 public:

	enum ColorMode
	{
		GREYSCALE = 1,
		RGB,
		RGBA,
		ARGB
	};

	enum TextureFlag
	{
		fUseMipmaps			= 1,
		fUseMultiTexture	= 2,
		fUseSDL_TTF			= 4
	};


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

   Texture();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Texture
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.05.29:
	 * Mongoose - Big code clean up, documentation
	 *
	 * 2000.10.05: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

   ~Texture();
	/*------------------------------------------------------
	 * Pre  : This object exists
	 * Post : Deconstructs an object of Texture
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.05.29:
	 * Mongoose - Big code clean up, documentation
	 *
	 * 2000.10.05: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	static unsigned char *generateColorTexture(unsigned char rgba[4],
															 unsigned int width, 
															 unsigned int height);
	/*------------------------------------------------------
	 * Pre  : <Rgba> is 32bpp RGBA color
	 *        <Width> and <Height> are powers of two, pref
	 *        the same number
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.30:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	gl_font_t *generateFont(ttf_texture_t *texture);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.30:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	ttf_texture_t *generateFontTexture(char *filename, int pointSize, 
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

   int getTextureCount();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns number of textures in use, or -1 for
	 *        error ( Not initalized )
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.05.29:
	 * Mongoose - Big code clean up, documentation
	 *
	 * 2000.10.05: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void glScreenShot(char *base, unsigned int width, unsigned int height);
	/*------------------------------------------------------
	 * Pre  : <Base> is base filename, 
	 *        <Width> and <Height> are viewport dim
	 *
	 * Post : Dumps a screenshot to disk, 
	 *        avoids overwriting files with same base name
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.06.16:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void bindMultiTexture(int texture0, int texture1);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets up multitexture rendering with passed texture ids
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.12.24:
	 * Mongoose - Created
	 ------------------------------------------------------*/

   void bindTextureId(unsigned int n);
	/*------------------------------------------------------
	 * Pre  : n is valid texture index
	 * Post : Binds the texture for use in GL
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.05.29:
	 * Mongoose - Big code clean up, documentation
	 *
	 * 2000.10.05: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void clearFlag(TextureFlag flag);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : CLears a option flag
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.01.05:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void disableMultiTexture();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.12.24:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void initSDL_TTF();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Loads SDL_TTF if avalible
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.03:
	 * Mongoose - Created
	 ------------------------------------------------------*/

   int loadBuffer(unsigned char *image, 
						unsigned int width, unsigned int height, 
						ColorMode mode, unsigned int bpp);
	/*------------------------------------------------------
	 * Pre  : image must be a valid pixmap that agrees
	 *        with mode, width, and height
	 *
	 * Post : Returns texture id or < 0 error flag
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.05.29:
	 * Mongoose - Big code clean up, documentation
	 *
	 * 2000.10.05: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

   int loadBufferSlot(unsigned char *image, 
							 unsigned int width, unsigned int height, 
							 ColorMode mode, unsigned int bpp,
							 unsigned int slot);
	/*------------------------------------------------------
	 * Pre  : image must be a valid pixmap that agrees
	 *        with mode, width, and height, slot ( ID )
	 *
	 * Post : Returns texture id or < 0 error flag
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.09.05: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int loadColorTexture(unsigned char rgba[4],
								unsigned int width, unsigned int height);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Generates and loads a solid color texture,
	 *        returns texture Id or -1 if failed
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.30:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int loadFontTTF(char *filename, 
						 unsigned int utf8Offset, unsigned int count);
	/*------------------------------------------------------
	 * Pre  : <Filename> of TTF font
	 *        <Utf8Offset> is offset into UNICODE table
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

	int loadPNG(char *filename);
	/*------------------------------------------------------
	 * Pre  : Texture is init and filename/file is valid
	 * Post : Loads PNG as texture and returns ID or -1 error
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.06.16:
	 * Mongoose - Created, from Freyja
	 ------------------------------------------------------*/

	int loadTGA(char *filename);
	/*------------------------------------------------------
	 * Pre  : Texture is init and filename/file is valid
	 * Post : Loads TGA as texture and returns ID or -1 error
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.06.16:
	 * Mongoose - Created
	 ------------------------------------------------------*/

   void reset();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Resets all texture data
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.05.29:
	 * Mongoose - Big code clean up, documentation
	 *
	 * 2000.10.05: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setFlag(TextureFlag flag);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets a option flag
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.01.05:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setMaxTextureCount(unsigned int n);
	/*------------------------------------------------------
	 * Pre  : n is max number of textures you wish to allow
	 * Post : Sets up GL texturing, and must be called 
	 *        as the first setup step
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.05.29:
	 * Mongoose - Big code clean up, documentation
	 *
	 * 2000.10.05: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void useMultiTexture(float u, float v);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.12.24:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void useMultiTexture(float aU, float aV, float bU, float bV);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.12.24:
	 * Mongoose - Created
	 ------------------------------------------------------*/


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////

	int nextPower(int seed);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.06.16:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	unsigned char *scaleBuffer(unsigned char *image, int width, int height,	
							   int components);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.06.16:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	unsigned int *mTextureIds;		/* GL texture list */

	unsigned int mTextureCount;	    /* Texture counter */
	
	unsigned int mTextureLimit;	    /* The texture limit */
	
	unsigned int mFlags;			/* Class options */
	
	int mTextureId;					/* Currently bound texture id */
	
	int mTextureId2;				/* Multitexture Texture Id */
};

	////////////////////////////////////////////////////////////
	// Gobal functions
	////////////////////////////////////////////////////////////

	void bufferedPrintf(char *string, unsigned int len, char *s, ...);
	/*------------------------------------------------------
	 * Pre  : <String> is allocated to <Len> characters
	 *        the rest is just like a printf() call
	 *
	 * Post : Generates the string and buffers it in <String>
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.12.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void glPrint3d(float x, float y, float z, 
				   float pitch, float yaw, float roll, 
				   float scale,
				   char *string);
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

	void glPrint2d(float x, float y, float scale, char *string);
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

	void glEnterMode2d(unsigned int width, unsigned int height);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : OpenGL ortho projection
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.03:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void glExitMode2d();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : OpenGL model matrix projection
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.03:
	 * Mongoose - Created
	 ------------------------------------------------------*/
#endif

#else
#   warning "SDLUnitTest.o requires -DHAVE_OPENGL"
#endif

#include <mstl/Vector.h>
//#include <Resource.h>

/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*================================================================
 * 
 * Project : Midgard
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : Resource
 * License : No use w/o permission (C) 2001 Mongoose
 * Comments: Resource (metadata) handler class
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2004.01.26:
 * Mongoose - API change for function signature
 *
 * 2004.01.04:
 * Mongoose - Major API changes, fun updates  =)
 *
 * 2001.11.02:
 * Mongoose - Created
 ================================================================*/


#ifndef GUARD__MIDGARD_MONGOOSE_RESOURCE_H_
#define GUARD__MIDGARD_MONGOOSE_RESOURCE_H_

#include <stdio.h>

typedef enum
{
	BOOLEAN   = 1,
	LIST      = 2,
	INT       = 3,
	FLOAT     = 4,
	CSTRING   = 5,
	FUNC      = 6,
	BEGIN     = 7,
	END       = 8,
	RESERVED3 = 9,
	RESERVED4 = 10

} mObjectType;


typedef enum
{
	mObjDisableGarbageCollection = 1

} mObjectFlags;


typedef struct mObject_s
{
	unsigned int type;
	unsigned char flags;
	char *symbol;
	void *data;

} mObject;


typedef struct mObjectList_s
{
	struct mObjectList_s *next;
	mObject *data;

} mObjectList;


#define getNumber(obj) (float)((obj->type == INT) ? (*((int*)(obj->data))) : \
							 (obj->type == FLOAT) ? (*((float*)(obj->data))):0)
#define getString(obj) (obj->type == CSTRING) ? (char *)(obj->data) : 0
#define getInt(obj) (obj->type == INT) ? (*((int *)(obj->data))) : 0
#define objTypeP(obj, objtype) obj ? (obj->type == objtype) : 0 
#define objNumberP(obj) obj ? (obj->type == INT || obj->type == FLOAT) : 0 


void deleteObj(mObject **object);
mObject *newADTObj(unsigned int type, void *data);

mObjectList *objAppend(mObject *object, mObjectList *list);
void objPush(mObjectList **list, mObject *object);
mObject *objPop(mObjectList **list);
mObject *objPeek(mObjectList *list);
mObject *newListObj(mObjectList *list);


class Resource
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	Resource();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of Resource
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.02: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~Resource();
	/*------------------------------------------------------
	 * Pre  : Resource object is allocated
	 * Post : Deconstructs an object of Resource
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.02: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	void dumpSymbols();
	/*------------------------------------------------------
	 * Pre  : 
	 *
	 * Post : Prints out all symbols in resource system
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int getSymbolData(char *symbol, unsigned int type, void **data);
	/*------------------------------------------------------
	 * Pre  : CAUTION
	 *        This is low level, know what you're doing
	 * 
	 *        Set TYPE to 0 for the whole mObject
	 *
	 * Post : Gets the address of the symbol's data, 
	 *        func ptr, etc -- so you can do some real damage
	 *
	 *        Returns 0 if there are no errors
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.01.04: 
	 * Mongoose - Created, replaces Lookup API
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void setDebugLevel(int level);
	/*------------------------------------------------------
	 * Pre  : Larger the number, more dumping
	 * Post : Sets debug level
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int evalBuffer(const char *buffer);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Parses and evaluates buffer
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int evalFile(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 *
	 * Post : Loads a resource file
	 *        Inits resource parsing system
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	mObject *parseNextSymbol();
	/*------------------------------------------------------
	 * Pre  : 
	 *
	 * Post : Pull the next symbol out of text buffer
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.21:
	 * Mongoose - Handle bound symbols int/float/string/adt
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int registerLispFunction(char *symbol, 
							 mObject *(*func)(mObjectList *));
	/*------------------------------------------------------
	 * Pre  : Symbol is valid string
	 *        Func is a valid and working function
	 *
	 * Post : Func is bound to symbol, can now be used by
	 *        mlisp
	 *        Returns 0 on sucess
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int registerSymbol(const char *symbol, unsigned int type, void *data);
	/*------------------------------------------------------
	 * Pre  : <Symbol> is valid string symbol
	 *        <Type> is valid for data
	 *        <Data> is valid for type and allocated
	 *
	 * Post : Symbol is created and bound to symbol table.
	 *
	 *        The data is set directly in the object, and
	 *        it is expected that deallocation for this data
	 *        occur after Resource is deallocated.
	 *
	 *        Returns 0 on sucess
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.01.04: 
	 * Mongoose - Shares data by reference with C varible
	 *
	 * 2002.01.21:
	 * Mongoose - Full type implementation?
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int registerSymbolValue(const char *symbol, float d);
	int registerSymbolValue(const char *symbol, int i);
	int registerSymbolValue(const char *symbol, char *string);
	/*------------------------------------------------------
	 * Pre  : Symbol is valid string
	 *
	 * Post : Symbol is created and bound to symbol table
	 *        Returns 0 on sucess, the data value is copied
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.21:
	 * Mongoose - Full type implementation?
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	
private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////

	static bool isAtoZ(char c);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	static bool isatoz(char c);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	static bool isDigit(char c);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	static bool isMisc(char c);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	static bool isNumeric(char *symbol, float *n);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void printError(char *format, ...);
	/*------------------------------------------------------
	 * Pre  : Works like printf, but cooler than that
	 *        also limits text to 256 bytes
	 *
	 * Post : Prints error and the line number in 
	 *        which in occurs in mlisp
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	int appendSymbolTable(mObject *object);
	/*------------------------------------------------------
	 * Pre  : <Object> is valid in every field
	 *
	 * Post : Puts the <Object> in the 'symbol table'
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void bind(const char *symbol, mObject *data);
	/*------------------------------------------------------
	 * Pre  : Symbol is a CSTRING type and has string data
	 *
	 * Post : Symbol's string data becomes bound to Data's
	 *        data resource
	 *
	 *        All passed args are freed
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int bufferFile(const char *filename, char **buffer, unsigned int *bytes);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Buffers loaded script file
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void clear();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Flushs resource symbol table and other
	 *        allocated memory
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.02.02:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void eval();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Evaluates whatever is in the mExecStack
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void expected(char c);
	/*------------------------------------------------------
	 * Pre  : C is char that was expected from the text
	 *        buffer
	 *
	 * Post : Records error
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	bool is(char c);
	/*------------------------------------------------------
	 * Pre  : C is char you're looking for next from text
	 *        buffer
	 *
	 * Post : Compares C to actual next/top char in text
	 *        buffer
	 *
	 *        Returns true if they do match
	 *        Returns false if they don't match
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	mObject *getNextSymbol();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets the next symbol from eval buffer
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.02: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void lex();
	/*------------------------------------------------------
	 * Pre  : 
	 *
	 * Post : Move text buffer cursor forward
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	bool match(char c);
	/*------------------------------------------------------
	 * Pre  : C is char you're looking for next from text
	 *        buffer
	 *
	 * Post : Compares C to actual next/top char in text
	 *        buffer, then moves the text buffer cusor to
	 *        next char
	 *
	 *        Returns true if they do match
	 *        Returns false if they don't match
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	int parseEvalBuffer(const char *buffer);
	/*------------------------------------------------------
	 * Pre  : 
	 *
	 * Post : Parses eval buffer and loads the execution 
	 *        stack for eval()
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void parseComment();
	/*------------------------------------------------------
	 * Pre  : 
	 *
	 * Post : Strips comments from text buffer line
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	mObject *parseFunction();
	/*------------------------------------------------------
	 * Pre  : Arg is valid and allocated mObject
	 *
	 * Post : Arg is bound to symbol
	 *        Returns arg list generated by function
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void parseSeperator();
	/*------------------------------------------------------
	 * Pre  : 
	 *
	 * Post : Whitespace stripping from buffer line and
	 *        between symbols and seperators
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void parseString(char *string, int len);
	/*------------------------------------------------------
	 * Pre  : String is allocated to buffer the string
	 *        Len is the number of allocated bytes of
	 *        String 
	 *
	 * Post : Handles quote offset string data in text
	 *        buffer  eg "a string"
	 *
	 *-- History ------------------------------------------
	 *
	 * 2001.11.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	mObject *setq(mObjectList *list);
	/*------------------------------------------------------
	 * Pre  : 
	 *
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.04.13: 
	 * Mongoose - Created, moved into class
	 ------------------------------------------------------*/

	int registerSymbolObject(const char *symbol, mObject *object);
	/*------------------------------------------------------
	 * Pre  : Be really careful making valid objects
	 * Post : Registers a premade object to symbol table
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.01.13:
	 * Mongoose - Created
	 ------------------------------------------------------*/


	mObjectList *mSymbolTable;    /* Holds all the symbols, data, func */

	mObjectList *mDataStack;      /* Stack of records for internal use */

	mObjectList *mExecStack;      /* Execution stack for internal use */

	mObject *mScopeBegin;         /* Used to define scope boundries */

	mObject *mScopeEnd;           /* Used to define scope boundries */


	/* RDP goodies */

	char *mSymbol;  	          /* Symbol buffer */

	char *mBuffer;                /* Eval buffer */

	unsigned int mBufferSize;     /* Symbol buffer byte size */

	unsigned int mSymbolSize;     /* Eval buffer byte size */

	unsigned int mTop;

	char mLook;

	FILE *mTape;

	int mLine;

	int mString;

	int mErrors;                  /* Parse error counter */

	int mDebug;                   /* Level of debugging for parser */

	bool mGarbageCollection;
};

#endif


class TextBox
{
public:
	TextBox()
	{
		x = y = 0;
		scale = 1.0;
		string = 0x0;
	}
	
	~TextBox()
	{
		if (string)
			delete [] string;
	}

	void setStringPointer(char *ptr)
	{
		string = ptr;
	}

	int x;
	int y;
	float scale;
	char *string;
};


/* Implement these functions in your Unit Test */
void displayFrame();
void gameFrame();
void handleKeyPressEvent(unsigned int button, unsigned int mod);
void handleKeyReleaseEvent(unsigned int button, unsigned int mod);
void handleEventCode(unsigned int event);
void initScene(int argc, char *argv[]);

class SDLUnitTest
{
 public:

	typedef enum 
	{
		fAlphaBlending = 1,
		fLighting      = 2,
		fWireframe     = 4,
		fAutoRotate    = 8,
		fRenderTexture = 16,
		fRenderPoints  = 32,
		fRenderBones   = 64,
		fRenderFaces   = 128
		
	} SDLUnitTestFlags_t;

 
	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////
	
	SDLUnitTest();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of SDLUnitTest
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.12.29: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~SDLUnitTest();
	/*------------------------------------------------------
	 * Pre  : SDLUnitTest object is allocated
	 * Post : Deconstructs an object of SDLUnitTest
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.12.29: 
	 * Mongoose - Created
	 ------------------------------------------------------*/
	
	
	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////
	
	unsigned int getHeight();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.12.29:
	 * Mongoose - Created
	 ------------------------------------------------------*/
	
	unsigned int getTicks();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns number of milliseconds since start of
	 *        program
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.06.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/
	
	unsigned int getWidth();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.12.29:
	 * Mongoose - Created
	 ------------------------------------------------------*/
	
	
	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void drawWidgets();

	void initGL();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.08.09?: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void bindKey(int key, unsigned int mod, int event);
	/*------------------------------------------------------
	 * Pre  : <Key> is a valid keyboard code
	 *        <Mod> is a valid keyboard modifier flag
	 *        <Event> is a valid game event Id
	 *
	 * Post : Sets <Event> binding to <Key> + <Mod> press 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.06.03:
	 * Mongoose - Created
	 ------------------------------------------------------*/
	
	void initVideo(unsigned int width, unsigned int height, 
				   bool fullscreen);
	/*------------------------------------------------------
	 * Pre  : Video mode request <Width> x <Height>
	 *        <Fullscreen> enables fullscreen rendering
	 *
	 * Post : Start up video subsystem
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.06.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	bool loadResourceFile(char *filename);
	/*------------------------------------------------------
	 * Pre  :
	 * Post : Returns 0 for no error or error code 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.01.03:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void resize(unsigned int width, unsigned int height);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Resizes game window and GL context
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.06.06: 
	 * Mongoose - Created, from old SDLUnitTest code
	 ------------------------------------------------------*/

	void runGame();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Start game loop
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.06.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setDriverGL(const char *driver);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.08.09?: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setFastCardPerformance(bool isFast);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.08.09?: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setGrabMouse(bool on);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.01.05:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setWindowTitle(char *title);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.01.05:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setShowCursor(bool on);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2003.01.05:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void shutdown(int i);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Shutsdown the game subsystems, exits game loop
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.06.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void swapBuffersGL();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Swaps OpenGL buffers ( call at end of frame )
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.06.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void toggleFullscreen();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Toggle fullscreen windowing mode
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.06.06: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	Texture mTexture;       /* Texture subsystem */

	Resource mResource;     /* Resource subsystem */

	/* Other misc attributes used mostly outside class */

	unsigned int mFlags;

	char mWindowTitle[128];

	char mMessage[256];

	bool mYawOn;

	float mYaw;

	float mPitch;

	float mScale;

	float mLightPos[4];

	Vector<TextBox*> mTextBoxes;

 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	bool handleWidgetMouseClick(int button, int state, int x, int y);
	/*------------------------------------------------------
	 * Pre  : x,y cursor position
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.01.03:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	SDL_Surface *mWindow;	/* This is the pointer to the SDL surface */

	unsigned int mWidth;    /* Width of the viewport */

	unsigned int mHeight;   /* Height of the viewport */

	bool mFastCard;         /* Assume expensive calls are fine if true */

	char *mDriverGL;        /* String for dynamic use of GL library */

	float mClipNear;        /* Clip near distance */

	float mClipFar;         /* Clip far distance */

	float mFovY;            /* Field of vision */


	/* Not used yet or misc prototyping */

	unsigned int mDepressedKey;      /* FIXME: Key flags or event toggles */
	unsigned int mDepressedModKeys;

	int mBtnWidth, mBtnHeight;
	float mBtnTextScale;
};


////////////////////////////////////////////////////////////
// Gobal Functions
////////////////////////////////////////////////////////////

unsigned int getTicks();
/*------------------------------------------------------
 * Pre  : 
 * Post : Returns number of ticks ( game time counter )
 *
 *-- History ------------------------------------------
 *
 * 2003.12.29:
 * Mongoose - Made visible in header file
 ------------------------------------------------------*/

void glDrawAxis();
/*------------------------------------------------------
 * Pre  : 
 * Post : Makes colored axis arrows, mostly for use in demos
 *
 *-- History ------------------------------------------
 *
 * 2003.12.29:
 * Mongoose - Made visible in header file
 ------------------------------------------------------*/

void glDrawAxis3f(float length, float arrowLenght, float arrowTip);
/*------------------------------------------------------
 * Pre  : 30.0, 26.25, 3.75 are used in most demos
 * Post : Makes colored axis arrows, mostly for use in demos
 *
 *-- History ------------------------------------------
 *
 * 2003.12.29:
 * Mongoose - Made visible in header file
 ------------------------------------------------------*/

void glDrawGrid(float size, float step);
/*------------------------------------------------------
 * Pre  : 500, 50 are used in most demos
 * Post : Makes a grid of lines, mostly for use in demos
 *
 *-- History ------------------------------------------
 *
 * 2003.12.29:
 * Mongoose - Made visible in header file
 ------------------------------------------------------*/

void glDrawLight(float size);
/*------------------------------------------------------
 * Pre  : 5.0 is used in most demos
 * Post : Makes a point to represent a light
 *
 *-- History ------------------------------------------
 *
 * 2003.12.29:
 * Mongoose - Made visible in header file
 ------------------------------------------------------*/

#endif

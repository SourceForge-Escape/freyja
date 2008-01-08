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

#include "FreyjaOpenGL.h"

class Texture
{
 public:

	enum ColorMode
	{
		GREYSCALE = 1,
		INDEXED,
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

	static Texture* GetInstance()
	{ return (mInstance) ? mInstance : mInstance = new Texture(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
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

	void glScreenShot(const char *base, 
					  unsigned int width, unsigned int height);
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

	void clearFlag(TextureFlag flag) { mFlags &= ~flag; }
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

	void setFlag(TextureFlag flag) { mFlags |= flag; }
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

	void Bind( GLenum texture_unit, uint16 id ) const;
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/


protected:

	////////////////////////////////////////////////////////////
	// Protected Accessors
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
	// Protected Mutators
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

	GLuint *mTextureIds;		/* GL texture list */

	unsigned int mTextureCount;	    /* Texture counter */
	
	unsigned int mTextureLimit;	    /* The texture limit */
	
	unsigned int mFlags;			/* Class options */
	
	int mTextureId;					/* Currently bound texture id */
	
	int mTextureId2;				/* Multitexture Texture Id */

 private:

	static Texture* mInstance;
};


#endif

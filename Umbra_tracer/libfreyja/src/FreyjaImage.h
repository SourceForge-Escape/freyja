/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : MtkImage
 * License : GPL See file COPYING, also (C) 2000 Mongoose
 * Comments: This is the mtkImage agent/class for mtk
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2004.08.21:
 * Mongoose - MtkImage repackaged as FreyjaImage with mostly minor changes
 *
 * 2000.10.05:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_FREYJAIMAGE_H_
#define GUARD__FREYJA_MONGOOSE_FREYJAIMAGE_H_


class FreyjaImage
{
 public:

	typedef enum {
		INDEXED_8 = 1, 
		RGB_24, 
		RGBA_32
	} colormode_t;

	typedef float color3_t[3];


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	FreyjaImage();
	/*--------------------------------------------
	 * Created  : 2000-10-05 by Mongoose
	 * Modified : 
	 *
	 * Pre  : None
	 * Post : MtkImage object is constructed
	 --------------------------------------------*/

	virtual ~FreyjaImage();
	/*--------------------------------------------
	 * Created  : 2000-10-05 by Mongoose
	 * Modified : 
	 *
	 * Pre  : MtkImage object is allocated
	 * Post : MtkImage object is deconstructed
	 --------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////
	
   colormode_t getColorMode();
	/*--------------------------------------------
	 * Created  : 2000-10-05 by Mongoose
	 * Modified : 
	 *
	 * Pre  : None
	 * Post : Returns image color mode
	 --------------------------------------------*/

   int getWidth();
	/*--------------------------------------------
	 * Created  : 2000-10-05 by Mongoose
	 * Modified : 
	 *
	 * Pre  : None
	 * Post : Returns image width
	 --------------------------------------------*/

   int getHeight();
	/*--------------------------------------------
	 * Created  : 2000-10-05 by Mongoose
	 * Modified : 
	 *
	 * Pre  : None
	 * Post : Returns image height
	 --------------------------------------------*/

   int getOriginalWidth();
	/*--------------------------------------------
	 * Created  : 2000-10-05 by Mongoose
	 * Modified : 
	 *
	 * Pre  : None
	 * Post : Returns image width before transforms
	 --------------------------------------------*/

   int getOriginalHeight();
	/*--------------------------------------------
	 * Created  : 2000-10-05 by Mongoose
	 * Modified : 
	 *
	 * Pre  : None
	 * Post : Returns image height before transforms
	 --------------------------------------------*/

   void getPalette(unsigned char **buffer);
	/*--------------------------------------------
	 * Created  : 2000-10-05 by Mongoose
	 * Modified : 
	 *
	 * Pre  : buffer must be an unallocated pointer
	 * Post : Returns a copy of the palette buffer
	 --------------------------------------------*/

   void getImage(unsigned char **image);
	/*--------------------------------------------
	 * Created  : 2000-10-05 by Mongoose
	 * Modified : 
	 *
	 * Pre  : image must be an unallocated pointer
	 *
	 * Post : Returns a copy of the image buffer
	 *        in mode, or NULL if unable
	 --------------------------------------------*/

   void getIndexedImage(unsigned char **image);
	/*--------------------------------------------
	 * Created  : 2000-10-05 by Mongoose
	 * Modified : 
	 *
	 * Pre  : image must be an unallocated pointer
	 *        A palette must have been loaded
	 *
	 * Post : Returns a copy of a computed
	 *        indexed image
	 --------------------------------------------*/

   void getPaletteColor(unsigned int i, color3_t rgb);
	/*--------------------------------------------
	 * Created  : 2000-10-05 by Mongoose
	 * Modified : 
	 *
	 * Pre  :
	 * Post : Gets color at palette[i] 
	 --------------------------------------------*/

   unsigned int matchPaletteColor(color3_t rgb);
	/*--------------------------------------------
	 * Created  : 2000-10-05 by Mongoose
	 * Modified : 
	 *
	 * Pre  : rgb is a color as defined as an
	 *        array of Red Green and Blue with
	 *        colors ranged 0.0 to 1.0
	 *
	 * Post : Returns closest matched color index
	 *        it can find in a loaded palette
	 *
	 *        NOTE: tends to lean to darker shades
	 --------------------------------------------*/

   int saveImage(const char *filename, const char *module_name);
	/*--------------------------------------------
	 * Created  : 2000-10-05 by Mongoose
	 * Modified : 
	 *
	 * Pre  :
	 * Post : Saves image to file ( brought it back )
	 --------------------------------------------*/

   int savePaletteMTK(const char *filename);
	/*--------------------------------------------
	 * Created  : 2000-10-05 by Mongoose
	 * Modified : 
	 *
	 * Pre  :
	 * Post : Saves MTK palette to file
	 --------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

   void brightenPalette(float brightness);
	/*--------------------------------------------
	 * Created  : 2000-10-05 by Mongoose
	 * Modified : 
	 *
	 * Pre  : 0.0 < brightness < 1.0
	 * Post : 
	 --------------------------------------------*/

   void erase();
	/*--------------------------------------------
	 * Created  : 2000-10-05 by Mongoose
	 * Modified : 
	 *
	 * Pre  : None
	 * Post : MtkImage agent's data is reset to 
	 *        initial state
	 --------------------------------------------*/

   void flipVertical();
	/*--------------------------------------------
	 * Created  : 2000-10-05 by Mongoose
	 * Modified : 
	 *
	 * Pre  :
	 * Post : flips the image vertically 
	 --------------------------------------------*/

   int loadPixmap(unsigned char *image,
						unsigned int width, unsigned int height, 
						colormode_t mode);
	/*--------------------------------------------
	 * Created  : 2000-10-05 by Mongoose
	 * Modified : 
	 *
	 * Pre  : image must be valid
	 *        width and height must be valid
	 *        mode must be valid
	 *
	 *        1. Palettes must be loaded to make
	 *           a computed INDEXED mode image
	 *
	 * Post : Loads image of type mode
	 --------------------------------------------*/

   int loadImage(const char *filename);
	/*--------------------------------------------
	 * Created  : 2000-10-05 by Mongoose
	 * Modified : 
	 *
	 * Pre  :
	 * Post : Loads MTK palette from file
	 --------------------------------------------*/

   int loadPaletteLMP(const char *filename);
	/*--------------------------------------------
	 * Created  : 2000-10-05 by Mongoose
	 * Modified : 
	 *
	 * Pre  :
	 * Post : Loads LMP palette from file
	 --------------------------------------------*/

   int loadPaletteMTK(const char *filename);
	/*--------------------------------------------
	 * Created  : 2000-10-05 by Mongoose
	 * Modified : 
	 *
	 * Pre  :
	 * Post : Loads MTK palette from file
	 --------------------------------------------*/

   void scaleImage();
	/*--------------------------------------------
	 * Created  : ???? by Mongoose, based on gluScaleImage
	 * Modified : 
	 *
	 * Pre  :
	 * Post : Scales image to next power of 2
	 --------------------------------------------*/

   void setColorMode(colormode_t mode);
	/*--------------------------------------------
	 * Created  : 2000-10-05 by Mongoose
	 * Modified : 
	 *
	 * Pre  : mode is an mtk_colormode
	 * 
	 *        NOTE: You can't set to an indexed mode
	 *        since they are computed per palette
	 * 
	 * Post : Converts image into a new color mode
	 *        if not in that mode already
	 --------------------------------------------*/

   int setPalette(unsigned char *buffer, int bpp);
	/*--------------------------------------------
	 * Created  : 2000-10-05 by Mongoose
	 * Modified : 
	 *
	 * Pre  : buffer must be allocated with given bitdepth
	 * Post : Sets new image palette
	 --------------------------------------------*/


 private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

   int getNextPower(int seed);
  /*--------------------------------------------
   * Created  : 2000-10-05 by Mongoose
   * Modified : 
   *
   * Pre  : seed is valid ( last power )
   * Post : Returns next power of 2
   --------------------------------------------*/

   int loadIndexedPixmap(unsigned char *image, int width, int height);
  /*--------------------------------------------
   * Created  : 2000-10-05 by Mongoose
   * Modified : 
   *
   * Pre  : image must be valid indexed image
   *        width must be valid
   *        height must be valid
   *        pallette must have been previously
   *        loaded
   *
   * Post : Loads indexed image using previously
   *        loaded pallette
   --------------------------------------------*/

	virtual void printError(char *format, va_list *args);
	virtual void printMessage(char *format, va_list *args);

	virtual void print(char *s, ...);
	/*------------------------------------------------------
	 * Pre  : Format string and args are valid
	 * Post : Report messages to stdout
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	virtual void printError(char *s, ...);
	/*------------------------------------------------------
	 * Pre  : String and args are valid
	 * Post : Report an error to stderr
	 *
	 *-- History ------------------------------------------
	 *
	 * 1999.07.31:
	 * Mongoose - Created
	 ------------------------------------------------------*/


   unsigned char *_image;                 /* Image buffer */

   unsigned char *_palette;               /* Image palette */

   int _width;                            /* Width of image */

   int _height;                           /* Height of image */

   int _original_width;                   /* Width before transforms */

   int _original_height;                  /* Height before transforms */

   colormode_t _color_mode;               /* Color mode of image */

   int _image_bpp;                        /* Color depth of image */ 

   int _palette_bpp;                      /* Color depth of palette */
};

#endif

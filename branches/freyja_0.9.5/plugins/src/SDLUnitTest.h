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
#else
#   warning "SDLUnitTest.o requires -DHAVE_OPENGL"
#endif

#include <mstl/Vector.h>
#include <mstl/String.h>

#include "Texture.h"
#include "Resource.h"

using namespace mstl;

/* Implement these functions in your Unit Test */
void displayFrame();
void gameFrame();
void handleKeyPressEvent(unsigned int button, unsigned int mod);
void handleKeyReleaseEvent(unsigned int button, unsigned int mod);
void handleEventCode(unsigned int event);
void initScene(int argc, char *argv[]);


class TextBox
{
public:
	TextBox() : x(0), y(0), scale(1.0f), string() { }
	
	~TextBox() { }

	void SetStringPointer(char *s) { string = s; }

	int x;
	int y;
	float scale;
	String string;
};


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

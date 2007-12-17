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
 * Mongoose - Created, based on my SDLUnitTest C functions 
 ================================================================*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <freyja/FreyjaImage.h>
#include <mstl/Vector.h>
#include <mstl/String.h>

#include "SDLUnitTest.h"

using namespace mstl;


/* FIXME: Use key down flags instead of key repeats!! */
/* Arg list types for Resource use */
#define ARG_GL_MENU          64
#define ARG_GL_SUBMENU       65
#define ARG_GL_MENU_ITEM     66

 /* Widget system menu test */

typedef struct {
	char label[33];
	//char state;
	int event;
} menu_item_t; 


typedef struct {
	char label[33]; 
	char active;
	Vector<menu_item_t *> menuitems; 
} submenu_t;

typedef struct {
	int x, y;
	Vector<submenu_t *> submenus; 
} menu_t;

Vector<menu_t*> gMenus;

mObject *menu(mObjectList *parms);
mObject *submenu(mObjectList *parms);
mObject *menu_item(mObjectList *parms);


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

SDLUnitTest::SDLUnitTest()
{
	mWindow = 0x0;
	mWidth = 640;
	mHeight = 460;
	mFastCard = true;
	mClipFar  = 4600.0f;
	mClipNear = 8.0f;
	mFovY     = 45.0f;
	mDriverGL = 0x0;

#ifdef WIN32
	setDriverGL("libGL32.dll");
#else
	setDriverGL("/usr/lib/libGL.so.1");
#endif

	mYawOn = true;
	mPitch = -20.0f;
	mScale = 1.0f;
	mLightPos[0] = 0.0;
	mLightPos[1] = 512.0;
	mLightPos[2] = 1024.0;
	mLightPos[3] = 0.0;
	
	mBtnWidth = 128;
	mBtnHeight = 32;
	mBtnTextScale = 0.90;

	mResource.registerLispFunction("menu",      menu);
	mResource.registerLispFunction("submenu",   submenu);
	mResource.registerLispFunction("menu_item", menu_item);

	//mResource.registerSymbol("cSDL_Width",  UINT, (void *)&mWidth);
	//mResource.registerSymbol("cSDL_Height", UINT, (void *)&mHeight);

	/*
	  mResource.registerSymbolValue("EVENT_SCREENSHOT",   SDLK_F1);
	  mResource.registerSymbolValue("EVENT_FULLSCREEN",   SDLK_F2);
	  mResource.registerSymbolValue("EVENT_AUTOROTATE",   SDLK_F8);
	  mResource.registerSymbolValue("EVENT_RES640x480",   SDLK_F9);
	  mResource.registerSymbolValue("EVENT_RES800x600",   SDLK_F10);
	  mResource.registerSymbolValue("EVENT_RES1024x768",  SDLK_F11);
	  mResource.registerSymbolValue("EVENT_RES1280x1024", SDLK_F12);
	  mResource.registerSymbolValue("EVENT_QUIT",         SDLK_ESCAPE);
	*/
}

SDLUnitTest::~SDLUnitTest()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

unsigned int SDLUnitTest::getHeight()
{
	return mHeight;
}


unsigned int SDLUnitTest::getTicks()
{
	return SDL_GetTicks();
}


unsigned int SDLUnitTest::getWidth()
{
	return mWidth;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void SDLUnitTest::drawWidgets()
{
	unsigned int i, j, k;

	/* Resource menus hack/test */
	for (i = mTextBoxes.begin(); i < mTextBoxes.end(); ++i)
	{
		TextBox *textbox = mTextBoxes[i];

		glColor4f(1.0, 1.0, 1.0, 1.0);
		mglPrint2d(textbox->x, textbox->y, textbox->scale, (char*)textbox->string.c_str());
	}

	for (i = gMenus.begin(); i < gMenus.end(); ++i)
	{
		float mx, my;
		menu_t *menu = gMenus[i];
		
		mx = menu->x;
		my = menu->y;
		
		for (j = menu->submenus.begin(); j < menu->submenus.end(); ++j,
			 mx += 129)
		{
			submenu_t *submenu = menu->submenus[j];

			my = menu->y;
			
			/* Draw a 'button' quad with label */
			glPushAttrib(GL_ENABLE_BIT);
			glDisable(GL_TEXTURE_2D);
			
			glColor4f(0.1, 0.1, 0.1, 0.75);				
			glBegin(GL_QUADS);
			glVertex2d(mx, my);
			glVertex2d(mx+128, my);
			glVertex2d(mx+128, my+32);
			glVertex2d(mx, my+32);
			glEnd();
			
			glPopAttrib();
				
			glColor4f(1.0, 1.0, 1.0, 1.0);
			mglPrint2d(mx+2, my+2, 1.0, submenu->label);

			if (!submenu->active)
				continue;

			my += 33;

			for (k = submenu->menuitems.begin(); k < submenu->menuitems.end(); 
				 ++k, my += 33)
			{
				
				menu_item_t *menuitem = submenu->menuitems[k];
				
				/* Draw a 'button' quad with label */
				glPushAttrib(GL_ENABLE_BIT);
				glDisable(GL_TEXTURE_2D);

				glColor4f(0.2, 0.2, 0.2, 0.75);				
				glBegin(GL_QUADS);
				glVertex2d(mx, my);
				glVertex2d(mx+128, my);
				glVertex2d(mx+128, my+32);
				glVertex2d(mx, my+32);
				glEnd();

				glPopAttrib();

				glColor4f(1.0, 1.0, 1.0, 1.0);
				mglPrint2d(mx+2, my+2, 1.0, menuitem->label);
			}
		}
	}

	glColor3f(1.0, 1.0, 1.0);
	mglPrint2d(4, mHeight - 32, 1.0, mMessage);
}


void SDLUnitTest::setDriverGL(const char *driver)
{
	unsigned int len;


	if (mDriverGL)
	{
		delete [] mDriverGL;
	}

	if (driver && driver[0])
	{
		len = strlen(driver);
		mDriverGL = new char[len+1];
		strncpy(mDriverGL, driver, len);
		mDriverGL[len] = 0;
	}
}


void SDLUnitTest::setFastCardPerformance(bool is_fast)
{
	mFastCard = is_fast;
}


void SDLUnitTest::initGL()
{
	char *s;


	// Print driver support information
	printf("\n\n\t## GL Driver Info ##\n");
	printf("\tVendor     : %s\n", glGetString(GL_VENDOR));
	printf("\tVersion    : %s\n", glGetString(GL_VERSION));
	printf("\tRenderer   : %s\n", glGetString(GL_RENDERER));
	printf("\tExtensions : %s\n\n\n", (char*)glGetString(GL_EXTENSIONS));

	// Testing for goodies
	// Mongoose 2001.12.31, Fixed string use to check for bad strings
	s = (char*)glGetString(GL_EXTENSIONS);

	if (s && s[0])
	{
		printf("\tGL_ARB_multitexture       \t\t");

		if (strstr(s, "GL_ARB_multitexture"))
		{
			printf("YES\n");
		}
		else
		{
			printf("NO\n");
		}

		//glActiveTextureARB
		//glMultiTexCoord2fARB
		//glFogCoordfEXT

		printf("\tGL_EXT_texture_env_combine\t\t");

		if (strstr(s, "GL_EXT_texture_env_combine"))
		{
			printf("YES\n");
		}
		else
		{
			printf("NO\n");
		}
	}

	/* Set background to black */
	glClearColor(0.0, 0.0, 0.0, 1.0);

#ifdef DISABLED_OLD
	// Setup shading
	glShadeModel(GL_SMOOTH);

	// Disable lighting 
	glDisable(GL_LIGHTING);

	glPointSize(5.0);

	// Set up Z buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Set up culling
	glEnable(GL_CULL_FACE);
	//glFrontFace(GL_CW);
	glFrontFace(GL_CCW);
	//glCullFace(GL_FRONT);

	// Set up alpha blending
	if (mFastCard) 
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

		//glEnable(GL_ALPHA_TEST); // Disable per pixel alpha blending
		glAlphaFunc(GL_GREATER, 0);
	}
	else
	{
		glDisable(GL_BLEND);
		glDisable(GL_ALPHA_TEST);
	}


	if (mFastCard) 
	{
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glHint(GL_FOG_HINT, GL_NICEST);
		glDisable(GL_COLOR_MATERIAL);
		glEnable(GL_DITHER);
		
		// AA polygon edges
		//glEnable(GL_POLYGON_SMOOTH);
		//glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

		glEnable(GL_POINT_SMOOTH);
	}
	else
	{
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
		glHint(GL_FOG_HINT, GL_FASTEST);
		glDisable(GL_COLOR_MATERIAL);
		glDisable(GL_DITHER);
		glDisable(GL_POLYGON_SMOOTH);
		glDisable(GL_POINT_SMOOTH);
		glDisable(GL_FOG);
	}

	glDisable(GL_LINE_SMOOTH);
	glDisable(GL_AUTO_NORMAL);
	glDisable(GL_LOGIC_OP);
	glDisable(GL_TEXTURE_1D);
	glDisable(GL_STENCIL_TEST);

	glDisable(GL_NORMALIZE);

	glEnableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_EDGE_FLAG_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);

	glPolygonMode(GL_FRONT, GL_FILL);

	glMatrixMode(GL_MODELVIEW);
#endif
}


void SDLUnitTest::setGrabMouse(bool on)
{
	on ? SDL_WM_GrabInput(SDL_GRAB_ON) : SDL_WM_GrabInput(SDL_GRAB_OFF);
}


void SDLUnitTest::setShowCursor(bool on)
{
	on ? SDL_WM_GrabInput(SDL_GRAB_OFF) : SDL_ShowCursor(SDL_DISABLE);
}


void SDLUnitTest::setWindowTitle(char *title)
{
	memcpy(mWindowTitle, title, 64);
	mWindowTitle[63] = 0;
	SDL_WM_SetCaption(mWindowTitle, mWindowTitle);
}


void SDLUnitTest::initVideo(unsigned int width, unsigned int height, 
							bool fullscreen)
{
	int flags; //, x, y;


	// Create GL context
	SDL_Init(SDL_INIT_VIDEO);
	printf("@Created OpenGL Context...\n");
	atexit(SDL_Quit);

	mWidth = width;
	mHeight = height;

	if (!mDriverGL || !mDriverGL[0] || SDL_GL_LoadLibrary(mDriverGL) < 0)
	{
		SDL_ClearError();
		
		// Fallback 1
		if (SDL_GL_LoadLibrary("libGL.so") < 0)
		{
			SDL_ClearError();
    
			// Fallback 2
			if (SDL_GL_LoadLibrary("libGL.so.1") < 0)
			{
				fprintf(stderr, "initVideo> SDL_GL_LoadLibrary failed!\n");
				fprintf(stderr, "initVideo> Error is [%s].\n", SDL_GetError());
				shutdown(1);
			}
		}
	}

	flags = 0;

	flags |= SDL_OPENGL;

	if (fullscreen)
	{
		flags |= SDL_FULLSCREEN;
	}

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	mWindow = SDL_SetVideoMode(width, height, 16, flags);
	SDL_WM_SetCaption(mWindowTitle, mWindowTitle);
	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);

	// Start game renderer
	initGL();
	
	// Resize context
	resize(width, height);
}


void SDLUnitTest::resize(unsigned int width, unsigned int height)
{
	GLfloat aspect;


	if (width == 0 || height == 0)
	{
		printf("resizeGL> ASSERTION FAILED: (width > 0 && height > 0)\n");
		return;
	}

	mWidth = width;
	mHeight = height;

	aspect = (GLfloat)width / (GLfloat)height;
	
	/* Resize GL Context */
	glViewport(0, 0, width, height); 

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(mFovY, aspect, mClipNear, mClipFar);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	/* Resize SDL Window */
	mWindow = SDL_SetVideoMode(width, height, 16, SDL_OPENGL);
}


void SDLUnitTest::runGame()
{
	SDL_Event event;
	unsigned int mkeys, mod, key;
	int btn, x, y;


	printf("\n@Starting event loop...\n");

	for (;;)
	{
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				shutdown(0);
				break;
			case SDL_MOUSEMOTION:
				/* Wrap motion */
				//handleMouseMotionEvent(event.motion.xrel * 2, 
				//					   event.motion.yrel * 2);
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				//handleMouseEvent(event.button.button, event.button.state,
				//			          event.button.x, event.button.y);

				SDL_GetMouseState(&x, &y);

				if (handleWidgetMouseClick(event.button.button, 
										   (SDL_MOUSEBUTTONDOWN && 
											(event.button.state == SDL_PRESSED)),										   x, y))
				{
					printf("Widget ate mouse event @ %i, %i\n", x, y);
				}
				else if (event.button.state == SDL_PRESSED)
				{
					handleKeyPressEvent(btn, mod);
				}
				else
				{
					handleKeyReleaseEvent(btn, mod);
				}
				break;
			case SDL_KEYUP:	
			case SDL_KEYDOWN:
				mkeys = (unsigned int)SDL_GetModState();
				mod = 0;
#ifdef OBSOLETE
				if (mkeys & KMOD_LSHIFT)
					mod |= SYS_MOD_KEY_LSHIFT;

				if (mkeys & KMOD_RSHIFT)
					mod |= SYS_MOD_KEY_RSHIFT;

				if (mkeys & KMOD_LCTRL)
					mod |= SYS_MOD_KEY_LCTRL;

				if (mkeys & KMOD_RCTRL)
					mod |= SYS_MOD_KEY_RCTRL;

				if (mkeys & KMOD_LALT)
					mod |= SYS_MOD_KEY_LALT;

				if (mkeys & KMOD_RALT)
					mod |= SYS_MOD_KEY_RALT;
#endif
				key = event.key.keysym.sym;

				/* Update class watch */
				mDepressedModKeys = mod;
				mDepressedKey = key;

				switch (event.type)
				{
				case SDL_KEYDOWN:
					handleKeyPressEvent(key, mod);
					break;
				default:
					handleKeyReleaseEvent(key, mod);
				}
				break;
			case SDL_VIDEORESIZE:			  
				//resize(event.resize.w, event.resize.h);
				break;
			}
		}

		/* Game frame */
		gameFrame();
		displayFrame();
	}
}


void SDLUnitTest::shutdown(int i)
{
	/* Moved to atexit() call */
	//SDL_QuitSubSystem(SDL_OPENGL);
	//SDL_Quit();

#ifdef DEBUG_MEMORY_PROFILE
	printf("[Mongoose MEMORY_DEBUG_PROFILE]\nUnfreed memory table:\n");
	dump_memory_report();
#endif

	exit(0);
}


void SDLUnitTest::toggleFullscreen()
{
	if (mWindow)
	{		
		SDL_WM_ToggleFullScreen(mWindow);
	}
}


void SDLUnitTest::swapBuffersGL()
{
	SDL_GL_SwapBuffers();
}


bool SDLUnitTest::handleWidgetMouseClick(int button, int state, int x, int y)
{
	unsigned int i, j, k;


	if (!state)
		return false;

	/* Resource based menus testing */
	for (i = gMenus.begin(); i < gMenus.end(); ++i)
	{
		float mx, my;
		menu_t *menu = gMenus[i];
		
		mx = menu->x;
		my = menu->y;
		
		for (j = menu->submenus.begin(); 
			 j < menu->submenus.end(); 
			 ++j, mx += mBtnWidth+1)
		{
			submenu_t *submenu = menu->submenus[j];
			
			my = menu->y;
			
			if (x > mx && x < mx + mBtnWidth)
			{
				if (y > my && y < my + mBtnHeight)
				{
					submenu->active = !submenu->active;
					return true;
				}
			}
			
			if (!submenu->active)
				continue;
			
			my += mBtnHeight+1;
			
			for (k = submenu->menuitems.begin(); 
				 k < submenu->menuitems.end(); 
				 ++k, my += mBtnHeight+1)
			{
				menu_item_t *menuitem = submenu->menuitems[k];
				
				if (x > mx && x < mx + mBtnWidth)
				{
					if (y > my && y < my + mBtnHeight)
					{
						printf("Button emits event [%i]\n", menuitem->event);
						handleEventCode(menuitem->event);
						return true;
					}
				}
			}
		}
	}

	return false;
}


////////////////////////////////////////////////////////////
// Gobal Functions
////////////////////////////////////////////////////////////

unsigned int getTicks()
{
	return SDL_GetTicks();
}


void glDrawAxis()//float length, float arrowLenght)
{
	/* Draw XYZ axis */
	glBegin(GL_LINES);

	/* X axis */
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-8.0f, 0.0f, 0.0f);
	glVertex3f(8.0f, 0.0f, 0.0f);

	/* X direction */
	glVertex3f(8.0f, 0.0f, 0.0f);
	glVertex3f(7.0f, 1.0f, 0.0f);
	glVertex3f(8.0f, 0.0f, 0.0f);
	glVertex3f(7.0f, -1.0f, 0.0f);

	/* Y axis */
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -8.0f, 0.0f);
	glVertex3f(0.0f, 8.0f, 0.0f);

	/* Y direction */
	glVertex3f(0.0f, 8.0f, 0.0f);
	glVertex3f(0.0f, 7.0f, 1.0f);
	glVertex3f(0.0f, 8.0f, 0.0f);
	glVertex3f(0.0f, 7.0f, -1.0f);

	/* Z axis */
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -8.0f);
	glVertex3f(0.0f, 0.0f, 8.0f);

	/* Z direction */
	glVertex3f(0.0f, 0.0f, 8.0f);
	glVertex3f(0.0f, 1.0f, 7.0f);
	glVertex3f(0.0f, 0.0f, 8.0f);
	glVertex3f(0.0f, -1.0f, 7.0f);

	glEnd();
}


void glDrawAxis3f(float length, float arrowLenght, float arrowTip)
{
	glBegin(GL_LINES);

	/* X arrow shaft */
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(-arrowLenght, 0.0f, 0.0f);
	glVertex3f(arrowLenght, 0.0f, 0.0f);

	/* X arrowhead */
	glVertex3f(arrowLenght, 0.0f, 0.0f);
	glVertex3f(arrowLenght, arrowTip, 0.0f);
	glVertex3f(arrowLenght, 0.0f, 0.0f);
	glVertex3f(arrowLenght, -arrowTip, 0.0f);

	/* Y arrow shaft */
	glColor3f(0.0f, 1.0f, 0.0f);
	glVertex3f(0.0f, -arrowLenght, 0.0f);
	glVertex3f(0.0f, arrowLenght, 0.0f);

	/* Y arrowhead */
	glVertex3f(0.0f, arrowLenght, 0.0f);
	glVertex3f(0.0f, arrowLenght, arrowTip);
	glVertex3f(0.0f, arrowLenght, 0.0f);
	glVertex3f(0.0f, arrowLenght, -arrowTip);

	/* Z arrow shaft */
	glColor3f(0.0f, 0.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, -arrowLenght);
	glVertex3f(0.0f, 0.0f, arrowLenght);

	/* Z arrowhead */
	glVertex3f(0.0f, 0.0f, arrowLenght);
	glVertex3f(0.0f, arrowTip, arrowLenght);
	glVertex3f(0.0f, 0.0f, arrowLenght);
	glVertex3f(0.0f, -arrowTip, arrowLenght);
	glEnd();
}


void glDrawGrid(float size, float step)
{
	float x, y;


	// Draw grid
	glPushMatrix();
	glScalef(2.0f, 2.0f, 2.0f);
	glColor3f(0.4f, 0.4f, 0.6f);

	for (x = -size; x < size; x += step)
	{
		glBegin(GL_LINE_LOOP);
		for (y = -size; y < size; y += step)
		{
			glVertex3f(x + step, 0.0f, y);	
			glVertex3f(x, 0.0f, y);	
			glVertex3f(x, 0.0f, y + step);
			glVertex3f(x + step, 0.0f, y + step);
		}
		glEnd();
	}

	glPopMatrix();
}


void glDrawLight(float size)
{
	glPushAttrib(GL_POINT_BIT);

	glPointSize(size);

	glBegin(GL_POINTS);
	glColor3f(1.0f, 1.0f, 1.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glEnd();

	glPopAttrib();
}


mObject *menu(mObjectList *parms)
{
	mObject *xPos, *yPos, *obj, *ret;

	xPos = objPop(&parms);
	yPos = objPop(&parms);
	ret = NULL;
	
	if (objNumberP(xPos) && objNumberP(yPos))
	{
		menu_t *menu = new menu_t;
		menu->x = (int)getNumber(xPos);
		menu->y = (int)getNumber(yPos);

		while ((obj = objPeek(parms)) && objTypeP(obj, ARG_GL_SUBMENU))
		{
			// printf("MENU adds SUBMENU\n");
			menu->submenus.pushBack((submenu_t *)obj->data);
			objPop(&parms);
		}

		// printf("MENU\n");
		gMenus.pushBack(menu); // Hhhmm... cheap hack for now
		ret = newADTObj(ARG_GL_MENU, (void *)menu);
	}
	else
	{
		printf("menu> Failed to extract strict typed data from script\n");
	}

	return ret;
}


mObject *submenu(mObjectList *parms)
{
	mObject *label, *obj, *ret;

	label = objPop(&parms);
	ret = NULL;
	
	if (objTypeP(label, CSTRING))
	{
		submenu_t *submenu = new submenu_t;
		strncpy(submenu->label, getString(label), 32);
		submenu->label[32] = 0;
		submenu->active = 0;
		
		while ((obj = objPeek(parms)) && objTypeP(obj, ARG_GL_MENU_ITEM))
		{
			// printf("SUBMENU adds MENU_ITEM\n");
			submenu->menuitems.pushBack((menu_item_t *)obj->data);
			objPop(&parms);
		}

		// printf("SUBMENU\n");
		ret = newADTObj(ARG_GL_SUBMENU, (void *)submenu);
	}
	else
	{
		printf("submenu> Failed to extract strict typed data from script\n");
	}

	return ret;
}


mObject *menu_item(mObjectList *parms)
{
	mObject *label, *event, *ret;

	label = objPop(&parms);
	event = objPop(&parms);
	ret = NULL;
	
	if (objTypeP(label, CSTRING) && objTypeP(event, INT))
	{
		menu_item_t *menuitem = new menu_item_t;
		strncpy(menuitem->label, getString(label), 32);
		menuitem->label[32] = 0;
		menuitem->event = getInt(event);

		// printf("MENU_ITEM\n");

		ret = newADTObj(ARG_GL_MENU_ITEM, (void *)menuitem);
	}
	else
	{
		printf("menu_item> Failed to extract strict typed data from script\n");
	}

	return ret;
}


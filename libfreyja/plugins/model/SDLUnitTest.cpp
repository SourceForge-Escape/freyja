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

#include "SDLUnitTest.h"

#undef USING_MTK_TGA // FIXME

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
		glPrint2d(textbox->x, textbox->y, textbox->scale, textbox->string);
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
			glPrint2d(mx+2, my+2, 1.0, submenu->label);

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
				glPrint2d(mx+2, my+2, 1.0, menuitem->label);
			}
		}
	}

	glColor3f(1.0, 1.0, 1.0);
	glPrint2d(4, mHeight - 32, 1.0, mMessage);
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
 * 2004.01.04:
 * Mongoose - Major API changes, fun updates  =)
 *
 * 2001.11.02:
 * Mongoose - Created
 ================================================================*/

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

//#include "Resource.h"

#define RESOURCE_DEBUG_ENABLED

/* TODO:
 *
 * Add quote support and execution stack use
 * Handle scoping?
 * True function parm matching
 * Return Objects instead of ObjectLists
 */


////////////////////////////////////////////////////////////
/// Lisp Object functions //////////////////////////////////
////////////////////////////////////////////////////////////

mObject *newFloatObj(float n)
{
	mObject *obj;
	float *f;


	f = new float;
	*f = n;

	obj = new mObject;
	obj->symbol = NULL;
	obj->type = FLOAT;
	obj->data = (void *)f;	

	return obj;
}


mObject *newIntObj(int n)
{
	mObject *obj;
	int *i;

	i = new int;
	*i = n;

	obj = new mObject;
	obj->symbol = NULL;
	obj->type = INT;
	obj->data = (void *)i;

	return obj;
}


mObject *newStringObj(const char *s)
{
	mObject *obj = NULL;
	unsigned int lenght;


	obj = new mObject;
	obj->symbol = NULL;
	obj->type = CSTRING;
	obj->data = NULL;

	if (!s || !s[0])
	{
		printf("Empty constant string allocated\n");
		return obj;
	}

	lenght = strlen(s);

	obj->data = (void *)new char[lenght+1];
	strncpy((char *)obj->data, s, lenght);
	((char *)obj->data)[lenght] = 0;

	return obj;
}


/* (define FUNC_PTR, PARM_TYPE_LIST, NAME) */
mObject *newFuncObj(mObject *(*f)(mObjectList *), char *name)
{
	mObject *obj = NULL;
	unsigned int lenght;


	if (!name || !name[0])
		return obj;

	lenght = strlen(name);

	obj = new mObject;
	obj->type = FUNC;
	obj->data = (void *)f;
	obj->symbol = new char[lenght+1];
	strncpy(obj->symbol, name, lenght);
	obj->symbol[lenght] = 0;

	return obj;
}


mObject *newListObj(mObjectList *list)
{
	mObject *obj;

	obj = new mObject;
	obj->symbol = NULL;
	obj->type = LIST;
	obj->data = (void *)list;	

	return obj;
}


mObject *newADTObj(unsigned int type, void *data)
{
	mObject *obj = new mObject;

	obj->symbol = NULL;
	obj->type = type;
	obj->data = data;

	if (type < 33)
	{
		printf("ERROR: Likely type index conflict\n");
	}

	return obj;
}


void deleteObj(mObject **object)
{
	if (!(*object) || ((*object)->flags & mObjDisableGarbageCollection))
		return;

	if ((*object)->symbol)
		delete (char *)(*object)->symbol;

	switch ((*object)->type)
	{
	case FUNC:
		break;
	case CSTRING:
		if ((*object)->data)
			delete (char *)(*object)->data;		
		break;
	case INT:
		if ((*object)->data)
			delete (int*)(*object)->data;
		break;
	case FLOAT:
		if ((*object)->data)
			delete (float*)(*object)->data;
		break;
	case LIST:
		while ((mObjectList *)(*object)->data)
		{
			objPop((mObjectList **)&((*object)->data));
		}
		break;
	default:
		;
	}
		
	delete *object;
	*object = NULL;
}


void objPush(mObjectList **list, mObject *object)
{
	mObjectList *newList;


	if (!(*list))
	{
		*list = new mObjectList;
		(*list)->next = NULL;
	}
	else
	{
		newList = new mObjectList;
		newList->next = *list;
		*list = newList;
	}

	(*list)->data = object;
}


mObject *objPop(mObjectList **list)
{
	mObjectList *oldHead;
	mObject *object;


	if (!(*list))
	{
		return NULL;
	}

	object = (*list)->data;

	oldHead = *list;
	*list = (*list)->next;
	delete oldHead;

	return object;
}


mObject *objPeek(mObjectList *list)
{
	if (!list)
	{
		return NULL;
	}

	return list->data;
}


/* Heh, this should be two lists in, one back out */
mObjectList *objAppend(mObject *object, mObjectList *list)
{
	mObjectList *current;


	if (!list)
	{
		list = new mObjectList;
		list->next = NULL;
		list->data = object;
	}
	else
	{
		current = list;

		while (current->next)
			current = current->next;

		current->next = new mObjectList;
		current->next->data = object;
	}

	return list;
}


void deleteObjectList(mObjectList **list)
{
	while (*list)
	{
		objPop(list);
	}
}


void printObj(mObject *obj)
{
	if (!obj)
	{
		printf("NULL Object\n");
		return;
	}

	switch (obj->type)
	{
	case INT:
		printf("int <%s> : %i\n", obj->symbol, *(int *)obj->data);
		break;
	case FLOAT:
		printf("float <%s> : %f\n", obj->symbol, *(float *)obj->data);
		break;
	case CSTRING:
		printf("string <%s> : \"%s\"\n", obj->symbol, (char *)obj->data);
		break;
	case FUNC:
		printf("func <%s>(...)\n", obj->symbol);
		break;
	case LIST:
		printf("list <%s>\n", obj->symbol);
		break;
	case BEGIN:
		printf("SCOPE BEGIN\n");
		break;
	case END:
		printf("SCOPE END\n");
		break;
	default:
		printf("[Unknown type %i] <%s>\n",
			   obj->type, obj->symbol);
	}
}


void printObjList(mObjectList *list)
{
	mObjectList *current = list;
	mObject *obj;

	if (!list)
	{
		printf("ObjectList is NULL.\n");
		return;
	}

	while (current)
	{
		obj = objPeek(current);
		current = current->next;

		printObj(obj);
	}
}


////////////////////////////////////////////////////////////
/// Internal built-in functions ////////////////////////////
////////////////////////////////////////////////////////////

/* (append ELEMENT LIST )
 * Returns list which is LIST with 
 * ELEMENT appended to the end. */


mObject *numberp(mObjectList *parms)
{
	//if (object->type == INT || object->type == FLOAT)
	//	return (mObject *)1;  /* FIXME: ew bad form old bean */

	return NULL;
}


mObject *nil(mObjectList *parms)
{
	return NULL; // (NULL == parms->data || something)
}


mObject *setq(mObjectList *parms)
{
	return NULL;
}


/* (rest LIST)
 * Returns the list of remaining elements 
 * after the first element of LIST. */
mObject *rest(mObjectList *list)
{
	if (list)
		return newListObj(list->next);

	return NULL;
}

 
/* (endp x)
 * Determines whether x is the empty list, 
 * returning the symbolic atom T if x=(), 
 * or the symbolic atom F if x is nonempty. */

/* (first LIST) 
 * Returns the first element of LIST. */
mObject *first(mObjectList *list)
{
	//if (list)
	//	return list->data;

	return NULL;
}

/* (cons ELEMENT LIST)
 * Returns a new list consisting of the ELEMENT, 
 * followed by the elements of LIST. */
mObject *cons(mObjectList *list)
{
	/* FIXME */

	return NULL;
}


////////////////////////////////////////////////////////////
/// Internal built-in math functions ///////////////////////
////////////////////////////////////////////////////////////

/* Assign value */
mObject *assign(mObjectList *list)
{
	mObjectList *current = list;
	mObject *objA = NULL, *objB = NULL;
	unsigned int lenght;


	objA = objPop(&current);
	objB = objPop(&current);

	if (objA && objB && objA->type == objB->type)
	{
		switch (objA->type)
		{
		case INT:
			objA->data = objB->data;
			break;
		case FLOAT:
			objA->data = objB->data;
			break;
		case CSTRING:

			if ((char *)objA->data && ((char *)objA->data)[0])
			{
				/* Mongoose 2004.01.13, 
				 * Shallow copies only -- could fuck up CVARS otherwise */
				lenght = strlen((char *)objA->data);
				strncpy((char *)objA->data, (char *)objB->data, lenght);
				((char *)objA->data)[lenght] = 0;
			}
			break;
		default:
			objA->data = objB->data;
		}
	}

	return objA;
}


mObject *add(mObjectList *list)
{
	mObjectList *current = list;
	mObject *obj;
	float n = 0.0;

	
	while (current)
	{
		obj = objPeek(current);
		current = current->next;

		switch (obj->type)
		{
		case FLOAT:
			n += (*(float *)obj->data);
			break;
		case INT:
			n += (*(int *)obj->data);
			break;
		default:
			;
		}
	}

	return newFloatObj(n); 
}


mObject *subtract(mObjectList *list)
{
	mObjectList *current = list;
	mObject *obj;
	float n = 0.0, s;
	char first = 1;

	
	while (current)
	{
		obj = objPeek(current);
		current = current->next;

		switch (obj->type)
		{
		case FLOAT:
			s = (*(float *)obj->data);
			break;
		case INT:
			s = (*(int *)obj->data);
			break;
		default:
			;
		}

		if (first)
		{
			n = s;
			first = 0;
		}
		else
		{
			n -= s;
		}
	}

	return newFloatObj(n); 
}


mObject *multiply(mObjectList *list)
{
	mObjectList *current = list;
	mObject *obj;
	float n = 0.0, m;
	char first = 1;

	
	while (current)
	{
		obj = objPeek(current);
		current = current->next;

		switch (obj->type)
		{
		case FLOAT:
			m = (*(float *)obj->data);
			break;
		case INT:
			m = (*(int *)obj->data);
			break;
		default:
			;
		}

		if (first)
		{
			n = m;
			first = 0;
		}
		else
		{
			n *= m;
		}
	}

	return newFloatObj(n); 
}


mObject *divide(mObjectList *list)
{
	mObjectList *current = list;
	mObject *obj;
	float n = 0.0, d;
	char first = 1;

	
	while (current)
	{
		obj = objPeek(current);
		current = current->next;

		switch (obj->type)
		{
		case FLOAT:
			d = (*(float *)obj->data);
			break;
		case INT:
			d = (*(int *)obj->data);
			break;
		default:
			;
		}

		if (first)
		{
			first = 0;
			n = d;
		}
		else
		{
			if (d == 0.0)
			{
				printf("Error: Divide by zero avoided in lisp math\n");
			}
			else
			{
				n /= d;
			}
		}
	}

	return newFloatObj(n); 
}


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Resource::Resource()
{
	mSymbolTable = NULL;
	mExecStack = NULL;
	mDataStack = NULL;
	mScopeBegin = newADTObj(BEGIN, NULL);
	mScopeEnd = newADTObj(END, NULL);
	mBuffer = NULL;
	mSymbol = NULL;
	mBufferSize = 0;
	mSymbolSize = 65;
	mTop = 0;
	mLook = 0;
	mString = 0;
	mErrors = 0;
	mLine = 1;

	mDebug = 0;
	mGarbageCollection = false;

	/* The most basic procedures:
	 *
	 * (car LIST)     - Given a LIST, return the first element
	 * (cdr LIST)     - Given a LIST, return the second element
	 * (cons OBJ OBJ) - Construct a new LIST given first and second elements
	 * (quote EXPR)   - Denote an expression as literal, do not to interpret
	 * (eq OBJ OBJ)   - Compare two objects for equality, return true or false
	 *
	 * (if <test> <then form> <else form>) - Branch operation
	 *
	 * (define <name> (<parms>) <form>)    - Define functions
	 *
	 * mObject     - Element
	 * mObjectList - List
	 */
	
	/* Append lisp built-in functions to symbol table */
	registerLispFunction("nil", nil);
	//registerLispFunction("setq", &Resource::setq);
	registerLispFunction("first", first);
	registerLispFunction("rest", rest);

	registerLispFunction("+", add);
	registerLispFunction("-", subtract);
	registerLispFunction("*", multiply);
	registerLispFunction("/", divide);

	/* Append lisp built-in variables */
	registerSymbolValue("T", 0);
	registerSymbol("cDebugLevel", INT, (void *)(&mDebug));
}


Resource::~Resource()
{
	clear();
}


mObject *Resource::setq(mObjectList *list)
{
	mObject *symbol, *obj;

	symbol = objPop(&list);
	obj = objPop(&list);

	if (symbol && obj && symbol->type == CSTRING)
	{
		registerSymbol((char *)symbol->data, obj->type, obj->data);
	}

	return obj;
}

////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

void Resource::dumpSymbols()
{
	printf("\n\nDumping symbols:\n");
	printf("------------------------------------------------------------\n");
	printObjList(mSymbolTable);
	printf("------------------------------------------------------------\n");
}


int Resource::getSymbolData(char *symbol, unsigned int type, void **data)
{
	mObjectList *current;
	mObject *obj;


	if (!symbol || !symbol[0])
		return -1;

	current = mSymbolTable;

	while (current)
	{
		obj = objPeek(current);
		current = current->next;

		/* If type == 0 then return the whole object */
		if (type == 0) 
		{
			// Mongoose 2002.01.12, FIXME, string
			if (strcmp(symbol, obj->symbol) == 0)
			{
				*data = obj;
				return 0;
			}			
		}
		else if (obj->type == type)
		{
			// Mongoose 2002.01.12, FIXME, string
			if (strcmp(symbol, obj->symbol) == 0)
			{
				*data = obj->data;
				return 0;
			}
		}
	}

	return -2;
}


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////

void Resource::setDebugLevel(int level)
{
	mDebug = level;
}


int Resource::evalBuffer(const char *buffer)
{
	if (!parseEvalBuffer(buffer))
		eval();

	return 0;
}


int Resource::evalFile(const char *filename)
{
	if (mBuffer)
	{
		delete [] mBuffer;
	}

	if (mSymbol)
	{
		delete [] mSymbol;
	}

	if (mSymbolSize < 65)
	{
		mSymbolSize = 65;
	}

	mSymbol = new char[mSymbolSize];

	if (!bufferFile(filename, &mBuffer, &mBufferSize))
	{
		if (!evalBuffer(mBuffer))
		{
			return 0;
		}
	}

	return -1;
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////

bool Resource::isAtoZ(char c)
{
	return ((unsigned char)c > 64 && (unsigned char)c < 91);
}


bool Resource::isatoz(char c)
{
	return ((unsigned char)c > 96 && (unsigned char)c < 123);
}


bool Resource::isDigit(char c)
{
	return ((unsigned char)c > 47 && (unsigned char)c < 58);
}


bool Resource::isMisc(char c)
{
	if ((unsigned char)c == 95)
		return true;

	return false;
}


bool Resource::isNumeric(char *symbol, float *n)
{
	int len, i;
	bool real = false;
	
	
	if (!symbol || !symbol[0])
		return false;
	
	len = strlen(symbol);
	
	for (i = 0; i < len; i++)
	{
		// Mongoose: 2001-10-31 Handle '[0-9]+[.][0-9]+'
		if (i && symbol[i] == '.')
		{
			real = true;
			i++;
		}

		// Mongoose 2002.01.23, Handle '-[0-9]+'
		if (i == 0 && symbol[i] == '-')
		{
			i++;
		}

		if (!isDigit(symbol[i]))
		{
			return false;
		}
	}

	if (real)
	{
		*n = atof(symbol);
	}
	else
	{
		*n = atoi(symbol);
	}

	return true;
}


void Resource::printError(char *format, ...)
{
	char buffer[256];
	va_list args;
	unsigned int l;

	
	va_start(args, format);
	vsnprintf(buffer, 128, format, args);

	l = strlen(buffer);
    
	if (!l || !buffer[0])
	{
		return;
	}

	/* Strip message of an trailing carrage return */
	if (buffer[l-1] == '\n')
	{
		buffer[l-1] = 0;
	}
    
	fprintf(stderr, "lisp> ERROR #%i: Line %i, %s\n",
			mErrors, mLine, buffer);
    
	va_end(args);
}


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////

int Resource::appendSymbolTable(mObject *object)
{
	mSymbolTable = objAppend(object, mSymbolTable);

	return !(mSymbolTable == NULL);
}


void Resource::bind(const char *symbol, mObject *data)
{
	int len;


	if (!symbol || !symbol[0] || !data)
	{
		++mErrors;
		printError("BIND receieved invalid symbol for binding");
		deleteObj(&data);
		return;
	}

	/* Make a copy of the symbol to store in Object */
	len = strlen(symbol);
	data->symbol = new char[len + 1];
	strncpy(data->symbol, symbol, len);
	data->symbol[len] = 0;

	/* Append to the lisp object/symbol table */
	appendSymbolTable(data);
}


int Resource::bufferFile(const char *filename, 
						 char **buffer, unsigned int *bytes)
{
	FILE *f;


	*buffer = NULL;

	if (!filename || !filename[0])
	{
		return -1;
	}

	f = fopen(filename, "r");

	if (!f)
	{
		perror("fopen> ERROR: ");
		return -2;
	}

	fseek(f, 0, SEEK_END);
	*bytes = ftell(f);
	fseek(f, 0, SEEK_SET);

	/* FIXME: Might want to go ahead and strip comments here,
	 *        and go ahead and do preprocessing instead of
	 *        a straight file buffer.
	 */

	*buffer = new char[*bytes];
	fread(*buffer, *bytes, 1, f);
	fclose(f);

	return 0;
}


void Resource::clear()
{
	while (mDataStack)
	{
		objPop(&mDataStack);
	}

	while (mExecStack)
	{
		objPop(&mExecStack);
	}

	while (mSymbolTable)
	{
		mObject *obj = objPeek(mSymbolTable);
		deleteObj(&obj);
		objPop(&mSymbolTable);
	}

	if (mScopeBegin)
	{
		deleteObj(&mScopeBegin);
	}

	if (mScopeEnd)
	{
		deleteObj(&mScopeEnd);
	}

	if (mBuffer)
	{
		delete [] mBuffer;
	}

	if (mSymbol)
	{
		delete [] mSymbol;
	}
}


void Resource::eval()
{
	mObject *(*callFunction)(mObjectList *);
	mObjectList *parms = NULL, *currentStack = NULL, *curStack, *cur;
	mObject *obj, *result;
	unsigned int frame = 0;


#ifdef RESOURCE_DEBUG_ENABLED
	if (mDebug > 0)
	{
		printf("\n--------------------------------\n");
		printf("Evaluating lisp\n");
	}
#endif

	while (mExecStack)
	{
		obj = objPop(&mExecStack);

#ifdef RESOURCE_DEBUG_ENABLED
		if (mDebug > 4)
		{
			/* Not an AR or stack frame, just a stack use counter */
			printf("-frame %u----------------\n", ++frame);
		}
#endif

		if (obj)
		{
			switch (obj->type)
			{
			case FUNC:
				callFunction = (mObject * (*)(mObjectList *))obj->data;

				/* 1. Pop a Scope BEGIN obj */
				obj = objPop(&currentStack);

				if (!objTypeP(obj, BEGIN))
				{
					printf("FATAL ERROR: Lisp data stack corupt... BEGIN !=");
					printObj(obj);
				}

				/* 2. Copy currentStack items to parms stack in same order */
				parms = NULL;
				curStack = currentStack;

				//printf("=\n");
				while (curStack)
				{
					obj = objPeek(curStack);

					if (obj->type == END)
						break;

					if (!parms)
					{
						parms = new mObjectList;
						cur = parms;
					}
					else
					{
						cur->next = new mObjectList;
						cur = cur->next;
					}

					//printObj(obj);						
					cur->data = obj;
					cur->next = NULL;

					curStack = curStack->next;
				}
				//printf("=\n");

				/* 3. Call C function implementation */
				result = (*callFunction)(parms);

				/* 4. Pop off this entire scope now */
				while ((obj = objPop(&currentStack)))
				{
#ifdef RESOURCE_DEBUG_ENABLED
					if (mDebug > 4)
					{
						printf("POP ");
						printObj(obj);
					}
#endif
					if (obj->type == END)
						break;

					if (mGarbageCollection)
					{
						/* Weak garbage collect for now */
						deleteObj(&obj);
					}
				}

				/* 5. Push result on stack */
#ifdef RESOURCE_DEBUG_ENABLED
				if (mDebug > 4)
				{
					printf("RESULT_PUSH ");
					printObj(result);
				}
#endif
				objPush(&currentStack, result);
				break;
			case END:
				/* END objects mark the end of a function parm list */
#ifdef RESOURCE_DEBUG_ENABLED
				if (mDebug > 4)
				{
					printf("END_PUSH\n");
				}
#endif
				objPush(&currentStack, obj);

				//objPush(&mDataStack, newListObj(currentStack));
				//obj = objPeek(mDataStack);
				//currentStack = (mObjectList *)obj->data;
				break;
			case BEGIN:
				/* BEGIN objects mark the start of a function parm list */
#ifdef RESOURCE_DEBUG_ENABLED
				if (mDebug > 4)
				{
					printf("BEGIN_PUSH\n");
				}
#endif
				objPush(&currentStack, obj);

				//obj = objPop(&mDataStack);
				//currentStack = (mObjectList *)obj->data;
				//printObjectList(currentStack);
				break;
			default:
				/* Everything else push into the scope list */
#ifdef RESOURCE_DEBUG_ENABLED
				if (mDebug > 4)
				{
					printf("DATA_PUSH ");
					printObj(obj);
				}
#endif
				objPush(&currentStack, obj);
			}
		}
		else
		{
			printError("FATAL ERROR: eval() feed NULL data");
			return;
		}
	}

#ifdef RESOURCE_DEBUG_ENABLED
	  if (mDebug > 0)
	  {
		  printf("--------------------------------\n");
	  }
#endif

	  //dumpListSymbols(currentStack);
	  //objPush(&mStack, newListObj(currentStack));
}


void Resource::expected(char c)
{
	char s[4];


	switch (mLook)
	{
	case '\n':
		strncpy(s, "EOL", 4);
		break;
	case '\v':
		strncpy(s, "\\v", 4);
		break;
	case '\t':
		strncpy(s, "TAB", 4);
		break;
	default:
		s[0] = mLook;
		s[1] = 0;
	}

	++mErrors;
	printError("Expected '%c', not '%s' (%u)\nLast symbol before error: '%s'", 
			   c, s, mLook, mSymbol);
}


mObject *Resource::getNextSymbol()
{
	mObject *obj;

	parseSeperator();

	if (mLook == ';' && !mString)
	{
		parseComment();
		parseSeperator();
	}

	obj = parseNextSymbol();
	
	return obj;
}


bool Resource::is(char c)
{
  return (c == mLook);
}


void Resource::lex()
{
  if (mTop < mBufferSize)
  {
	  ++mTop;
	  mLook = mBuffer[mTop];

#ifdef RESOURCE_DEBUG_ENABLED
	  if (mDebug > 9)
	  {
		  printf("%c", mLook);
		  fflush(stdout);
	  }
#endif

	  if (mLook == ';' && !mString)
	  {
		  parseComment();
	  }
  }
  else
  {
	  mLook = EOF;
  }
}


bool Resource::match(char c)
{
  bool r = is(c);


  if (!r)
  {
    expected(c);
  }

  lex();

  return r;
}


void Resource::parseComment()
{
	match(';');

	while (!is('\n') && !is(EOF))
	{
		lex();
	}

	if (is(';'))
	{
		parseComment();
	}
}


int Resource::parseEvalBuffer(const char *buffer)
{
	mObject *object;


	if (!buffer || !buffer[0])
	{
		return -1;
	}
	
#ifdef RESOURCE_DEBUG_ENABLED
	if (mDebug > 0)
	{
		printf("> Parsing eval buffer...\n");
	}

	if (mDebug > 14)
	{
		printf("> %s\n", buffer);
	}
#endif
	
	/* Start counting lines at 1, errors at 0 */
	mLine = 1; 
	mErrors = 0;

	/* Reset parsing counters */
	mTop = 0;
	mLook = 0;
	mString = 0;

	lex();
	parseSeperator();

	while (is('('))
	{
		object = parseFunction();

#ifdef RESOURCE_DEBUG_ENABLED
		if (mDebug > 4)
		{
			printf("Pushing, ");
			printObj(object);
		}
#endif

		//objPush(&mExecStack, object);
		parseSeperator();
	}

	if (mErrors)
	{
		printf("\n\nlisp> Encountered %i parse errors\n\n", mErrors);
	}

	return 0;
}


mObject *Resource::parseFunction()
{
	mObject *func = NULL, *object = NULL;
	void *data;


	match('(');

	func = getNextSymbol();

	if (!getSymbolData(mSymbol, FUNC, &data) && data)
	{
		objPush(&mExecStack, func);
	}
	else
	{
		++mErrors;
		printError("Invalid function '%s'\n", mSymbol);
		return NULL;
	}

#ifdef RESOURCE_DEBUG_ENABLED
	if (mDebug > 4)
	{
		printf("> BEGIN %s\n", func->symbol);
	}
#endif

	objPush(&mExecStack, mScopeBegin);

	/* Handle function parms, must be either FUNC objects or 
	 * the elements must be composed into a object list 
	 * pushed on the stack */
	while (!is(')'))
	{
		object = getNextSymbol();

#ifdef RESOURCE_DEBUG_ENABLED
		if (mDebug > 4)
		{
			printf("   Pushing parm, ");
			printObj(object);
		}
#endif
		
		if (object && object->type != FUNC)
		{
			objPush(&mExecStack, object);
		}
	}

#ifdef RESOURCE_DEBUG_ENABLED
	if (mDebug > 4)
	{
		printf("> END %s\n", func->symbol);
	}
#endif

	objPush(&mExecStack, mScopeEnd);

	match(')');
	parseSeperator();

	return func;
}


mObject *Resource::parseNextSymbol()
{
	int i = 0, j;
	mObject *object = NULL;
	bool string = false;
	bool fp = false;
	float f;


	mSymbol[0] = 0;
	
	if (is('"'))
	{
		lex();
		string = true;
	}
	
	/* Mongoose 2001.11.09: 
	 * Best handle for? grouped (func (func */
	if (is('('))
	{
		return parseFunction();
	}
	
	while (string || (i == 0 && mLook == '-') ||
		   isatoz(mLook) || isAtoZ(mLook) || isDigit(mLook) || isMisc(mLook))
	{
		if (i < (int)mSymbolSize)
		{
			mSymbol[i++] = mLook;
			mSymbol[i] = 0;
		}
		
		if (string && is('"'))
		{
			--i;
			mSymbol[i] = 0;
			lex();
			break;
		}
		
		lex();
		
		if (string)
		{
			continue;
		}
		
		if (is('.'))
		{
			fp = true;
			
			for (j = 0; j < i; ++j)
			{
				if (!j && mSymbol[j] == '-')
					continue;
				
				if (!isDigit(mSymbol[j]))
					break;
			}

			if (i == j)
			{
			  mSymbol[i++] = mLook;
			  mSymbol[i] = 0;
			  lex();
			}
		}
		
		
		// Mongoose 2002.01.23, Hhhmm... fixes -100.00 and -100, 
		//   but is it 'good idea'?
		if (is('-') && i == 0)
		{
			mSymbol[i++] = mLook;
			mSymbol[i] = 0;
			lex();
		}
	}
	
#ifdef RESOURCE_DEBUG_ENABLED
	if (mDebug > 7)
	{
		printf("\n> New symbol <%s>\n", mSymbol);
	}
#endif
	
	if (isNumeric(mSymbol, &f))
	{
		if (fp)
		{
			object = newFloatObj(f);
		}
		else
		{
			object = newIntObj((int)f);
		}
	}
	else
	{
		/* Handle symbol references here */
		if ((getSymbolData(mSymbol, 0, (void **)&object)) == 0)
		{
		}
		else
		{
			if (!string)
			{
				++mErrors;
				printError("Making string out of non-string <%s>?\n", mSymbol);
			}

			object = newStringObj(mSymbol);
		}
	}
	
	return object;
}


void Resource::parseSeperator()
{
	// Real whitespace stripping
	while (is(' ') || is('\t') || is('\n'))
	{
		if (is('\n'))
		{
			mLine++;
		}
		
		lex();
	}
}


void Resource::parseString(char *string, int len)
{
	int i = 0;
	
	
	match('"');
	
	mString = 1;
	
	len -= 2;
	
	while (!is('"') && !is(EOF))
	{
		if (i < len)
		{
			string[i++] = mLook;
			string[i] = 0;
		}
		
		lex();
	}

	match('"');

	mString = 0;
}


int Resource::registerLispFunction(char *symbol, 
								   mObject *(*func)(mObjectList *))
{
	if (!func)
	{
		return -1;
	}

	return appendSymbolTable(newFuncObj(func, symbol));
}


int Resource::registerSymbol(const char *symbol, unsigned int type, void *data)
{
	mObject *object;


	if (!symbol || !symbol[0])
	{
		return -1;
	}

	object = new mObject;
	object->type = type;
	object->data = data;
	object->flags = mObjDisableGarbageCollection;

	bind(symbol, object);

	return 0;
}


int Resource::registerSymbolValue(const char *symbol, float d)
{
	mObject* object = newFloatObj(d);


	if (registerSymbolObject(symbol, object) < 0)
	{
		deleteObj(&object);
		return -1;
	}

	object->flags = mObjDisableGarbageCollection;

	return 0;
}


int Resource::registerSymbolValue(const char *symbol, int i)
{
	mObject* object = newIntObj(i);


	if (registerSymbolObject(symbol, object) < 0)
	{
		deleteObj(&object);
		return -1;
	}

	return 0;
}


int Resource::registerSymbolValue(const char *symbol, char *string)
{
	mObject* object = newStringObj(string);


	if (registerSymbolObject(symbol, object) < 0)
	{
		deleteObj(&object);
		return -1;
	}

	return 0;
}


int Resource::registerSymbolObject(const char *symbol, mObject *object)
{
	if (!symbol || !symbol[0] || !object)
	{
		printf("Error: Couldn't bind invalid NULL object symbol\n");
		return -1;
	}

	bind(symbol, object);

	return 0;
}


////////////////////////////////////////////////////////////
// Unit Test
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_RESOURCE
mObject *menu(mObjectList *parms)
{
	mObject *xPos, *yPos, *ret, *obj;


	xPos = objPop(&parms);
	yPos = objPop(&parms);
	ret = NULL;
	
	if (objNumberP(xPos) && objNumberP(yPos))
	{
		/* Call event func here - simulated with printf for test */
		printf("extern \"C\" { menu(%f, %f); }\n",
			   getNumber(xPos), getNumber(yPos));

		while ((obj = objPeek(parms)))
		{
			printf("             submenu?, ");
			printObj(obj);
			
			objPop(&parms);
		}

		ret = newStringObj("Menu");
	}
	else
	{
		printf("menu> Failed to extract strict typed data from script\n");
	}
	
	/* Would return menu struct here if it wasn't a simulation */
	return ret; 
}


mObject *submenu(mObjectList *parms)
{
	mObject *label, *obj, *ret = NULL;


	label = objPop(&parms);

	if (objTypeP(label, CSTRING))
	{
		/* Call event func here - simulated with printf in tests */
		printf("extern \"C\" { submenu(\"%s\"); }\n",
				 getString(label));

		while ((obj = objPeek(parms)))
		{
			printf("             menuitem?, ");
			printObj(obj);
			
			objPop(&parms);
		}

		ret = newStringObj(getString(label));
	}
	else
	{
		printf("submenu> Failed to extract strict typed data from script\n");
	}

	/* Would return submenu struct here if it wasn't a simulation */
	return ret;
}


mObject *menu_item(mObjectList *parms)
{
	mObject *label, *event, *ret;


	label = objPop(&parms);
	event = objPop(&parms);
	ret = NULL;

	if (objTypeP(label, CSTRING) && objTypeP(label, INT))
	{
		/* Call event func here - simulated with printf in tests */
		printf("extern \"C\" { menu_item(\"%s\", %i); }\n",
				 getString(label), getInt(event));

		ret = newStringObj(getString(label));
	}
	else
	{
		printObj(label);
		printObj(event);
		printf("menu_item> Failed to extract strict typed data from script\n");
	}

	/* Would return menu_item struct here if it wasn't a simulation */
	return ret;
}


int runUnitTest(int argc, char *argv[])
{
	Resource rc;
	int i = 0;


	rc.setDebugLevel(5);

	if (argc > 0)
	{
		rc.registerLispFunction("menu", menu);
		rc.registerLispFunction("submenu", submenu);
		rc.registerLispFunction("menu_item", menu_item);
		rc.registerSymbol("cMyCounter", INT, (void *)(&i));

		rc.registerSymbolValue("EVENT_SCREENSHOT",   10);
		rc.registerSymbolValue("EVENT_QUIT",         11);
		rc.registerSymbolValue("EVENT_SHOWBONES",    12);
		rc.registerSymbolValue("EVENT_WIREFRAME",    13);
		rc.registerSymbolValue("EVENT_FULLSCREEN",   14);
		rc.registerSymbolValue("EVENT_RES640x480",   15);
		rc.registerSymbolValue("EVENT_RES800x600",   16);
		rc.registerSymbolValue("EVENT_RES1024x768",  17);
		rc.registerSymbolValue("EVENT_RES1280x1024", 18);

		if (argc > 1 && rc.evalFile(argv[1]) < 0)
		{
			printf("Couldn't load file!\n");
		}
		else
		{
			// rc.evalBuffer("(+ 23 42 54 23)");
		}

		rc.dumpSymbols();
	}
	else
	{
		printf("%s FILENAME.LISP\n", argv[0]);
	}

	return -1;
}


int main(int argc, char *argv[])
{
	printf("[Resource class test]\n");

	return runUnitTest(argc, argv);
}
#endif
/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*==========================================================================
 * 
 * Project : Midgard, MTK, Freyja, OpenRaider
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440
 * Email   : stu7440@westga.edu
 * Object  : Texture
 * Comments: This is the Texture class.
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

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef DEBUG_MEMEORY
#   include "memeory_test.h"
#endif

#ifdef USING_MTK_TGA
#   include "mtk_tga.h"
#endif

#ifdef USING_MTK_PNG
#   include "mtk_png.h"
#endif

#ifdef HAVE_SDL_TTF
#   include <SDL/SDL_ttf.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>

//#include "Texture.h"


#ifdef USE_ARB_MULTITEXTURE
#   define glActiveTexture glActiveTextureARB
#   define glMultiTexCoord2f glMultiTexCoord2fARB
#endif


/* Texture * Texture::mSingleton = 0x0; */
gl_font_t *gFontTest = 0x0;


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

Texture::Texture()
{
	mTextureIds = NULL;

	mFlags = 0;
	mTextureId = -1;
	mTextureId2 = -1;
	mTextureCount = 0;
	mTextureLimit = 0;

	//mSingleton = this;

	initSDL_TTF();
}


Texture::~Texture()
{
	if (gFontTest)
	{
		glDeleteLists(gFontTest->drawListBase, gFontTest->count);
		delete gFontTest;
	}

	reset();
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////

unsigned char *Texture::generateColorTexture(unsigned char rgba[4],
											 unsigned int width, 
											 unsigned int height)
{
	unsigned char *image;
	unsigned int i, size;


	image = new unsigned char[height*width*4];

	for (i = 0, size = width*height; i < size; ++i)
	{
		/* RBGA */
		image[i*4]   = rgba[0];
		image[i*4+1] = rgba[1];
		image[i*4+2] = rgba[2];
		image[i*4+3] = rgba[3];
	}

	return image;
}


void glEnterMode2d(unsigned int width, unsigned int height)
{
	glPushAttrib(GL_ENABLE_BIT);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);

	/* This allows alpha blending of 2D textures with the scene */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	glOrtho(0.0, (GLdouble)width, (GLdouble)height, 0.0, 0.0, 1.0);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}


void glExitMode2d()
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	glPopAttrib();

	glMatrixMode(GL_MODELVIEW);
}


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

int Texture::loadColorTexture(unsigned char rgba[4],
										unsigned int width, unsigned int height)
{
	unsigned char *image;
	int id = -1;

	image = generateColorTexture(rgba, width, height);
	id = loadBuffer(image, width, height, RGBA, 32);
	delete [] image;

	return id;
}


void Texture::initSDL_TTF()
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
#endif
}


void Texture::setFlag(TextureFlag flag)
{
	mFlags |= flag;
}


void Texture::clearFlag(TextureFlag flag)
{
	mFlags |= flag;
	mFlags ^= flag;
}


void Texture::reset()
{
	if (mTextureIds)
	{
		glDeleteTextures(mTextureLimit, mTextureIds);
		delete [] mTextureIds;
	}

	mTextureIds = 0x0;
	mTextureCount = 0;
	mTextureLimit = 0;
}


void Texture::disableMultiTexture()
{
	mFlags ^= fUseMultiTexture;
	mTextureId = -1;
	mTextureId2 = -1;

	glDisable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0_ARB);
}


void Texture::useMultiTexture(float aU, float aV, float bU, float bV)
{
	if (!(mFlags & fUseMultiTexture))
		return;

	glMultiTexCoord2f(GL_TEXTURE0_ARB, aU, aV);
	glMultiTexCoord2f(GL_TEXTURE1_ARB, bU, bV);
}


void Texture::useMultiTexture(float u, float v)
{
	if (!(mFlags & fUseMultiTexture))
		return;

	glMultiTexCoord2f(GL_TEXTURE0_ARB, u, v);
	glMultiTexCoord2f(GL_TEXTURE1_ARB, u, v);
}


void Texture::bindMultiTexture(int texture0, int texture1)
{
	if (//(int)a == mTextureId && (int)b == mTextureId2 ||
		 !mTextureIds || 
		 texture0 < 0 || texture0 > (int)mTextureCount ||
		 texture1 < 0 || texture1 > (int)mTextureCount)
   {
		return;
	}

	mFlags |= fUseMultiTexture;
	mTextureId  = texture0;
	mTextureId2 = texture1;

	glActiveTexture(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mTextureIds[texture0]);

	glActiveTexture(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, mTextureIds[texture1]);
}


void Texture::setMaxTextureCount(unsigned int n)
{
	mTextureLimit = n;

	mTextureIds = new unsigned int[n];

	glGenTextures(n, mTextureIds);
}


int Texture::getTextureCount()
{
	return (mTextureCount-1);
}



int Texture::loadBuffer(unsigned char *image, 
								unsigned int width, unsigned int height,
								ColorMode mode, unsigned int bpp)
{
	int id;


	id = loadBufferSlot(image, width, height, mode, bpp, mTextureCount++);

	if (id < 0)
	{
		return id;
	}

	return ++id;
}


void convertARGB32bppToRGBA32bpp(unsigned char *image, 
											unsigned int w, unsigned int h)
{
	unsigned int i, size = w*h;
	unsigned char swap;


	for (i = 0; i < size; ++i)
	{
		/* 32-bit ARGB to RGBA */
		swap = image[i*4+3];
		image[i*4]   = image[i*4+1];
		image[i*4+1] = image[i*4+2];
		image[i*4+2] = image[i*4+3];
		image[i*4+3] = swap;
	}
}


int Texture::loadBufferSlot(unsigned char *image, 
									 unsigned int width, unsigned int height,
									 ColorMode mode, unsigned int bpp,
									 unsigned int slot)
{
	unsigned char bytes;
	unsigned int glcMode;


	if (!mTextureIds || slot >= mTextureLimit)
	{
		printf("Texture::Load> ERROR Not initalizied or out of free slots\n");
		return -1000;
	}

	if (!width || !height || !image)
	{
		printf("Texture::Load> ERROR Assertion 'image is valid' failed\n");
		return -1;
	}

	switch (mode)
	{
	case GREYSCALE:
		if (bpp != 8)
		{
			printf("Texture::Load> ERROR Unsupported GREYSCALE, %i bpp\n", bpp);
			return -2;
		}
		
		bytes = 1;
		glcMode = GL_LUMINANCE;
		break;
	case RGB:
		if (bpp != 24)
		{
			printf("Texture::Load> ERROR Unsupported RGBA, %i bpp\n", bpp);
			return -2;
		}
		
		bytes = 3;
		glcMode = GL_RGB;
		break;
	case ARGB:
		if (bpp == 32)
		{
			convertARGB32bppToRGBA32bpp(image, width, height);
		}
	case RGBA:
		if (bpp != 32)
		{
			printf("Texture::Load> ERROR Unsupported RGBA/ARGB, %i bpp\n", bpp);
			return -2;
		}
		
		bytes = 4;
		glcMode = GL_RGBA;
		break;
	default:
		printf("Texture::Load> ERROR Unknown color mode %i\n", mode);
		return -2;
	}

	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
  
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glBindTexture(GL_TEXTURE_2D, mTextureIds[slot]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (mFlags & fUseMipmaps)
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
							 GL_NEAREST_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, 
							 GL_LINEAR_MIPMAP_LINEAR);
		
		gluBuild2DMipmaps(GL_TEXTURE_2D, bytes, width, height,
								glcMode, GL_UNSIGNED_BYTE, image);
	}
	else
	{
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		
		glTexImage2D(GL_TEXTURE_2D, 0, glcMode, width, height, 0,
						 glcMode, GL_UNSIGNED_BYTE, image);
	}

	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	return slot;
}


void Texture::bindTextureId(unsigned int n)
{
	if ((int)n == mTextureId || !mTextureIds || n > mTextureCount)
   {
		return;
	}

	mTextureId = n;

	glEnable(GL_TEXTURE_2D);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glBindTexture(GL_TEXTURE_2D, mTextureIds[n]);
}


void Texture::glScreenShot(char *base, unsigned int width, unsigned int height)
{
#ifdef USING_MTK_PNG_SSHOT
  FILE *f;
  int sz = width*height;
  unsigned char *image = new unsigned char[sz*3];
  char filename[1024];
  static int count = 0;
  bool done = false;


  if (!image)
  {
    printf("glScreenShot> ERROR: Couldn't allocate image!\n");
    return;
  }

  while (!done)
  {
    snprintf(filename, 1024, "%s-%03i.png", base, count++);
    
    f = fopen(filename, "rb");

    if (f)
      fclose(f);
    else
      done = true;
  }

  f = fopen(filename, "wb");

  if (!f)
  {
    printf("glScreenShot> ERROR: Couldn't write screenshot.\n");
    perror("glScreenShot> ERROR: ");
    return;
  }

  // Capture frame buffer
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);

  mtk_image__png_save(f, image, width, height, 3);
  fclose(f);
	
  delete [] image;

  printf("glScreenShot> Took screenshot '%s'.\n", filename);
#else
  FILE *f;
  int sz = width*height;
  unsigned char *image = new unsigned char[sz*3];
  unsigned char *swap_row = new unsigned char[width*3];
  char filename[1024];
  static int count = 0;
  bool done = false;
  int i, j, size;
  unsigned char comment_lenght;
  unsigned char colormap_type; 
  unsigned char image_type;    
  unsigned short colormap_index;      
  unsigned short colormap_lenght;     
  unsigned char colormap_bbp;         
  unsigned short origin_x;
  unsigned short origin_y;
  unsigned short swidth;   
  unsigned short sheight;  
  char comment[32] = "Mongoose TGA 0.0.1\0";
  unsigned char tmp, bpp, desc_flags;


  if (!image || !width || !height)
  {
    if (image)
    {
      delete [] image;
    }

    printf("glScreenShot> ERROR: Couldn't allocate image!\n");
    return;
  }

  // Don't overwrite files
  while (!done)
  {
    snprintf(filename, 1024, "%s-%04i.tga", base, count++);
    
    f = fopen(filename, "rb");

    if (f)
      fclose(f);
    else
      done = true;
  }

  f = fopen(filename, "wb");

  if (!f)
  {
    printf("glScreenShot> ERROR: Couldn't write screenshot.\n");
    perror("glScreenShot> ERROR: ");
    return;
  }

  // Capture frame buffer
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, image);

  // Flip vertical
  for (i = 0, j = (int)height-1; i < (int)height/2; ++i, --j)
  {
    memcpy(swap_row, &image[i*width*3], width*3);
    memcpy(&image[i*width*3], &image[j*width*3], width*3);
    memcpy(&image[j*width*3], swap_row, width*3);
  }

  delete [] swap_row;

  comment_lenght = strlen(comment);
  colormap_type = 0;
  image_type = 2;
  colormap_index = 0;
  colormap_lenght = 0;
  colormap_bbp = 0;
  origin_x = origin_y = 0;
  swidth = width;
  sheight = height;
  bpp = 24;
  desc_flags = 32;

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

  size = width * height * 3;
 
  for (i = 0; i < size; i += 3)
  {
    tmp = image[i];
    image[i] = image[i + 2];
    image[i + 2] = tmp;
  }

  // Write image data
  if (fwrite(image, size, 1, f) < 1)
  {
    perror("glScreenShot> Disk write failed.\n");
	 fclose(f);
    return;
  }

  fclose(f);

  delete [] image;

  printf("Took screenshot '%s'.\n", filename);
#endif
}


int Texture::loadPNG(char *filename)
{
#ifdef USING_MTK_PNG
	FILE *f;
	unsigned char *image = NULL;
	unsigned char *image2 = NULL;
	unsigned int w, h, bpp;
	int id = -1;


	f = fopen(filename, "rb");
  
	if (!f)
	{
		perror("Couldn't load file");
	}
	else if (!mtk_image__png_check(f))
	{
		mtk_image__png_load(f, &image, &w, &h, &bpp);

		image2 = scaleBuffer(image, w, h, bpp/8);

		if (image2)
		{
			image = image2;
			w = h = 256;
		}
		
		if (image)
		{
			id = loadBuffer(image, w, h, (bpp == 32) ? RGBA : RGB, bpp);

			printf("%c", (id == -1) ? 'x' : 'o');
			fflush(stdout);

			delete [] image;
		}

		if (f)
		{
			fclose(f);
		}
	}

	if (id == -1)
	{
		printf("Texture::loadPNG> ERROR: Failed to load '%s'\n", filename);
	}

	return id;
#else
	printf("ERROR: MTK PNG support not enabled in this build\n");
	return -1;
#endif
}


int Texture::loadTGA(char *filename)
{
#ifdef USING_MTK_TGA
	FILE *f;
	unsigned char *image = NULL;
	unsigned char *image2 = NULL;
	unsigned int w, h, bpp;
	int id = -1;


	f = fopen(filename, "rb");
  
	if (!f)
	{
		perror("Couldn't load file");
	}
	else if (!mtk_image__tga_check(f))
	{
		mtk_image__tga_load(f, &image, &w, &h, &bpp);
		image2 = scaleBuffer(image, w, h, bpp/8);

		if (image2)
		{
			image = image2;
			w = h = 256;
		}

		if (image)
		{
			id = loadBuffer(image, w, h, 
								 (bpp == 32) ? RGBA : RGB,
								 bpp);
			
			printf("%c", (id == -1) ? 'x' : 'o');
			fflush(stdout);

			delete [] image;
		}

		fclose(f);
	}

	if (id == -1)
	{
		printf("Texture::loadTGA> ERROR: Failed to load '%s'\n", filename);
	}

	return id;
#else
	printf("ERROR: MTK TGA support not enabled in this build\n");
	return -1;
#endif
}


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////

int Texture::nextPower(int seed)
{
  int i;

  for(i = 1; i < seed; i *= 2) 
    ;

  return i;
}


/* This code based off on gluScaleImage()  */
unsigned char *Texture::scaleBuffer(unsigned char *image, 
												int width,  int height,	
												int components)
{
   int i, j, k;
   float* tempin;
   float* tempout;
   float sx, sy;
	//   int components = 3;
   unsigned char *timage;
	int original_height = height;
	int original_width = width;


   if (!image || !width || !height)
	{
		return NULL;
	}

   height = nextPower(height);
   width = nextPower(width);

   if (height > 256) 
     height = 256;
    
   if (width > 256) 
     width = 256;

   // Check to see if scaling is needed
   if (height == original_height && width == original_width) 
		return NULL;

	//printf("%i\n", components);

   timage = new unsigned char[height * width * components];
   tempin = new float[original_width * original_height * components * sizeof(float)];
   tempout = new float[width * height * components * sizeof(float)];

   if (!tempout || !tempin)
   {
     if (tempout)
       delete [] tempout;

     if (tempin)
       delete [] tempin;

     printf("Oh shit out of memory!\n");
     return NULL;
   }

   // Copy user data to float format.
   for (i = 0; i < original_height * original_width * components; ++i) 
   {
     tempin[i] = (float)image[i];
   }

   // Determine which filter to use by checking ratios.
   if (width > 1) 
   {
     sx = (float)(original_width - 1) / (float)(width - 1);
   } 
   else 
   {
     sx = (float)(original_width - 1);
   }

   if (height > 1)
   {
     sy = (float)(original_height - 1) / (float) (height - 1);
   } 
   else 
   {
     sy = (float)(original_height - 1);
   }

   if (sx < 1.0 && sy < 1.0) 
   {
     /* Magnify both width and height:  use weighted sample of 4 pixels */
     int i0, i1, j0, j1;
     float alpha, beta;
     float* src00;
     float* src01;
     float* src10;
     float* src11;
     float s1, s2;
     float* dst;

     for(i = 0; i < height; ++i) 
     {
       i0 = (int)(i * sy);
       i1 = i0 + 1;

       if (i1 >= original_height) 
       {
			 i1 = original_height - 1;
       }

       alpha = i * sy - i0;

       for (j = 0; j < width; ++j) 
       {
			 j0 = (int) (j * sx);
			 j1 = j0 + 1;

			 if (j1 >= original_width) 
			 {
				 j1 = original_width - 1;
			 }

			 beta = j * sx - j0;

			 /* Compute weighted average of pixels in rect (i0,j0)-(i1,j1) */
			 src00 = tempin + (i0 * original_width + j0) * components;
			 src01 = tempin + (i0 * original_width + j1) * components;
			 src10 = tempin + (i1 * original_width + j0) * components;
			 src11 = tempin + (i1 * original_width + j1) * components;

			 dst = tempout + (i * width + j) * components;

			 for (k = 0; k < components; ++k) 
			 {
				 s1 = *src00++ * (1.0 - beta) + *src01++ * beta;
				 s2 = *src10++ * (1.0 - beta) + *src11++ * beta;
				 *dst++ = s1 * (1.0 - alpha) + s2 * alpha;
			 } 
       }
     }     
   }
   else 
   {
     /* Shrink width and/or height:  use an unweighted box filter */
     int i0, i1;
     int j0, j1;
     int ii, jj;
     float sum;
     float* dst;

     for (i = 0; i < height; ++i) 
     {
       i0 = (int) (i * sy);
       i1 = i0 + 1;

       if (i1 >= original_height) 
       {
			 i1 = original_height - 1;
       }

       for (j = 0; j < width; ++j) 
       {
			 j0 = (int) (j * sx);
			 j1 = j0 + 1;

			 if (j1 >= original_width) 
			 {
				 j1 = original_width - 1;
			 }

			 dst = tempout + (i * width + j) * components;

			 /* Compute average of pixels in the rectangle (i0,j0)-(i1,j1) */
			 for (k = 0; k < components; ++k) 
			 {
				 sum = 0.0;

				 for (ii = i0; ii <= i1; ++ii) 
				 {
					 for (jj = j0; jj <= j1; ++jj) 
					 {
						 sum += *(tempin + (ii * original_width + jj) 
									 * components + k);
					 }
				 }

				 sum /= ( j1 - j0 + 1 ) * ( i1 - i0 + 1 );
				 *dst++ = sum;
			 }
       } 
     }
   }

   // Copy to our results.
   for( i = 0; i < height * width * components; ++i) 
   {
     timage[i] = (unsigned char)tempout[i];
   }

   // Delete our temp buffers.
   delete[] tempin;
   delete[] tempout;
   delete[] image;

   return timage;
}


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : FreyjaControl
 * License : No use w/o permission (c) 2000-2004 Mongoose
 * Comments: This is the controler class for the client
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 *
 *-- TODO --------------------------------------------------- 
 *
 * + Remove Egg dependence ( planes ) 
 *
 * + 3d events
 *
 * + Command pattern event refactoring
 *
 *-- History ------------------------------------------------ 
 *
 * 2004.08.14:
 * Mongoose - Documentation update and Freyja coding guidelines ( finally )
 *
 * 2000.09.10:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_FREYJACONTROL_H_
#define GUARD__FREYJA_MONGOOSE_FREYJACONTROL_H_

#include <mstl/Map.h>
#include <mstl/Vector.h>
#include <freyja/FreyjaPlugin.h>

// FIXME: Replace Resource use with MLisp prototype
#include <mgtk/Resource.h>
//#include <mgtk/Mlisp.h>


#include "MaterialManager.h"
#include "OpenGLFreyjaScene.h"
#include "FreyjaEvent.h"
#include "FreyjaRender.h" // FIXME: Tmp test hack before FreyjaInterface
#include "freyja_events.h"



class FreyjaAppPrinter : public FreyjaPrinter
{
 public:

	virtual void errorArgs(char *format, va_list *args)
	{
		freyja_print_args(format, args);
	}


	virtual void messageArgs(char *format, va_list *args)
	{
		freyja_print_args(format, args);
	}
};


class FreyjaControl
{
 public:

	typedef enum {                              /* Editor modes */
		TEXTURE_EDIT_MODE = 0, 
		ANIMATION_EDIT_MODE,
		MAP_EDIT_MODE,
		MODEL_EDIT_MODE,
		MATERIAL_EDIT_MODE
		
	} EditorMode;

	typedef enum {                              /* Editor event modes */
		modeNone = 1,
		MESH_MOVE_CENTER,
		POINT_ADD_MODE,
		POINT_DEL_MODE,
		POLYGON_ADD_MODE,
		POLYGON_DEL_MODE,
		BONE_CONNECT_MODE,
		BONE_DISCONNECT_MODE,
		TAG_MOVE_CENTER,
		TEXEL_COMBINE,
		VERTEX_COMBINE,
		VERTEX_BBOX_SELECT_MODE,
		POLYGON_SELECT_MODE,
		
		/* Generic transforms */
		modeMove,
		modeRotate,
		modeScale,
		modeSelect
		
	} EventMode;


	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	FreyjaControl(Resource *rc);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : FreyjaControl object is constructed
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~FreyjaControl();
	/*------------------------------------------------------
	 * Pre  : FreyjaControl object is allocated
	 * Post : FreyjaControl object is deconstructed
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Accessors
	////////////////////////////////////////////////////////////

	float getZoom();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns current viewing zoom of scene
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25: 
	 * Mongoose - Created from GooseEgg
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void addRecentFilename(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Appends filename to recently opened file list
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.14: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void display();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Update the GL context
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	bool event(int event, unsigned int value);
	/*------------------------------------------------------
	 * Pre  : Generic event signal is valid and value is
	 *        valid for the generic event
	 *
	 * Post : Event is handled internally 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.10.23: 
	 * Mongoose - Created, pulled out of other event methods
	 ------------------------------------------------------*/

	bool event(int event, vec_t value);
	/*------------------------------------------------------
	 * Pre  : Generic event signal is valid and value is
	 *        valid for the generic event
	 *
	 * Post : Event is handled internally 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.10.23: 
	 * Mongoose - Created, pulled out of other event methods
	 ------------------------------------------------------*/

	bool eventMain(int command);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 200X.XX.XX: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	bool eventMisc(int command);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns true if event handled
	 *
	 *-- History ------------------------------------------
	 *
	 * 200X.XX.XX: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	bool handleEvent(int event, int command);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Process event input, such as from a menu
	 *
	 *        Returns true if event handled
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void handleFilename(const char *filename);
	/*------------------------------------------------------
	 * Pre  : filename is valid and mode was set
	 * Post : Executes a file operation depending on mode
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.05.22:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void handleTextEvent(int event, const char *text);
	/*------------------------------------------------------
	 * Pre  : text is valid
	 * Post : Processes event using text given
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.16:
	 * Mongoose - Created
	 ------------------------------------------------------*/

	bool motionEvent(int x, int y);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Process mouse motion input
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	bool mouseEvent(int btn, int state, int mod, int x, int y);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Process mouse input
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/
	
	void resizeDisplay(unsigned int width, unsigned int height);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Resize the GL context
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setCurrentPlane(FreyjaRender::plane_t plane);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets the current edit plane
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void setZoom(float zoom);
	/*------------------------------------------------------
	 * Pre  : ZOOM is a number greater than 0.0
	 * Post : Sets current viewing zoom of scene
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25: 
	 * Mongoose - Created from GooseEgg
	 ------------------------------------------------------*/

	void loadResource();
	void initOpenGL();

private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////

	void getScreenToWorldOBSOLETE(float *x, float *y);
	/*------------------------------------------------------
	 * Pre  : Emulates the old ScreenToWorld() method behavior
	 * Post : This function will be removed after the Model rewrite
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.15: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void getWorldFromScreen(float *x, float *y, float *z);
	/*------------------------------------------------------
	 * Pre  : X and Y are the mouse position
	 *
	 * Post : Converts x,y on screen to x,y,z in world
	 *        for whichever plane was last selected
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.08.25: 
	 * Mongoose - Created from ScreenToWorld from GooseEgg
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	void copySelectedObject();
	void pasteBufferedObject();

	void addObject();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.01: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void deleteSelectedObject();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.01: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void moveObject(int x, int y, FreyjaRender::plane_t plane);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.01: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void rotateObject(int x, int y, FreyjaRender::plane_t plane);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.01: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void selectObject(int x, int y, FreyjaRender::plane_t plane);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.01: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void scaleObject(int x, int y, FreyjaRender::plane_t plane);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.01: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void MotionEdit(int x, int y, FreyjaRender::plane_t plane);
	/*--------------------------------------------
	 * Created  : 2000-09-10 by Mongoose
	 * Modified : 
	 * Pre      : 
	 * Post     : Process mouse motion input
	 *            in edit mode
	 --------------------------------------------*/
	
	void MouseEdit(int btn, int state, int mod, int x, int y,
				   FreyjaRender::plane_t plane);
	/*--------------------------------------------
	 * Created  : 2000-09-10 by Mongoose
	 * Modified : 
	 * Pre      : 
	 * Post     : Process mouse input in edit mode
	 --------------------------------------------*/

	//	void loadResource();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Loads the Resource file from disk and sets cvars
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created from GooseEgg
	 ------------------------------------------------------*/

	void setupResource(Resource &r);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns current viewing zoom of scene
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.14: 
	 * Mongoose - Created from old stand alone function
	 ------------------------------------------------------*/


	OpenGLFreyjaScene *mScene;              /* Scene data model */

	Resource *mResource;                    /* Resource system */
	
	FreyjaRender *mRender;                  /* OpenGL scene renderer */

	FreyjaPlugin *mModel;


	Vector<FreyjaEvent *> mEvents;          /* Command pattern event system */

	Vector<char *> mRecentFiles;            /* Recently loaded model files */

	MaterialManager *mMaterial;             /* Material manager */

	unsigned int mLastEvent, mLastCommand;  /* The last command pair recieved*/

	unsigned int mMouseButton, mModKey;     /* Cached mouse button event */

	unsigned int mMouseState;

	EditorMode mEditorMode;                 /* Mode of the editor */

	EventMode mEventMode;                   /* Mode of generic event handler */
	
	int _tex_state;                         /* Mouse state on texture canvas */
	
	int _mouse_state;                       /* Mouse state on modeler canvas */

	unsigned int mFileDialogMode;

	bool mFullScreen;

	char mScratchTextBuffer[1024];

	freyja_transform_t mTransformMode;
};

#endif

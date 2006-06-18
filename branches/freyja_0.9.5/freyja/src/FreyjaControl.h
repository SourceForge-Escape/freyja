/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : FreyjaControl
 * License : No use w/o permission (C) 2000 Mongoose
 * Comments: This is the controler class for the client
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 *
 *-- TODO --------------------------------------------------- 
 *
 * + Remove Egg dependence ( planes ) 
 *
 * + 3d events to Model facade
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
#include <mgtk/Resource.h>

#include "FreyjaModel.h"
#include "FreyjaRender.h"
#include "freyja_events.h"
#include "Texture.h"


class FreyjaControl
{
 public:

	typedef enum {
		RESERVED = 1
	} OptionFlags;


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
		BONE_ADD_MODE,
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

	FreyjaControl(Resource *r);
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

	void takeScreenshot(const char *filename, 
						uint32 width, uint32 height);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	int32 loadTextureBuffer(unsigned char *image, 
							uint32 width, uint32 height, uint32 bpp,
							Texture::ColorMode type);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

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

	bool event(int event, unsigned int value);
	/*------------------------------------------------------
	 * Pre  : Generic event signal is valid and value is
	 *        valid for the generic event
	 *
	 * Post : Event is handled internally 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2005.03.22:
	 * Mongoose - Replaces 2000 handleEvent method
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

	bool handleRecentFile(unsigned int idx);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	bool event(int command);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
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

	bool mouseEvent(int btn, int state, int mod, int x, int y);
	/*--------------------------------------------
	 * Created  : 2000-09-10 by Mongoose
	 * Modified : 
	 * Pre      : 
	 * Post     : Process mouse input
	 --------------------------------------------*/
	
	bool motionEvent(int x, int y);
	/*--------------------------------------------
	 * Created  : 2000-09-10 by Mongoose
	 * Modified : 
	 * Pre      : 
	 * Post     : Process mouse motion input
	 --------------------------------------------*/
	
	bool Key(unsigned int key, int x, int y, int modifer_keys);
	/*--------------------------------------------
	 * Created  : 2000-09-10 by Mongoose
	 * Modified : 
	 * Pre      : 
	 * Post     : Process alphanumeric keyboard 
	 *            input
	 --------------------------------------------*/

	void loadResource();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Loads the Resource file from disk and sets cvars
	 *
	 *-- History ------------------------------------------
	 *
	 * 2000.09.10: 
	 * Mongoose - Created from GooseEgg
	 ------------------------------------------------------*/

	void resizeDisplay(unsigned int width, unsigned int height);

	void updateDisplay();


private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////

	void getPickRay(vec_t mouseX, vec_t mouseY, 
					double *rayOrigin, double *rayVector);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns pick ray in object cooridnates
	 ------------------------------------------------------*/

	void testPickRay(vec_t x, vec_t y);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : FIXME: Replace with ray picking object picker
	 ------------------------------------------------------*/

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

	bool copySelectedObject();
	bool pasteSelectedObject();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.11.30: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void moveObject(int x, int y, freyja_plane_t plane);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.01: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void rotateObject(int x, int y, freyja_plane_t plane);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.01: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void selectObject(int x, int y, freyja_plane_t plane);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.01: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void scaleObject(int x, int y, freyja_plane_t plane);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2004.08.01: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	void MotionEdit(int x, int y, freyja_plane_t plane);
	/*--------------------------------------------
	 * Created  : 2000-09-10 by Mongoose
	 * Modified : 
	 * Pre      : 
	 * Post     : Process mouse motion input
	 *            in edit mode
	 --------------------------------------------*/
	
	void MouseEdit(int btn, int state, int mod, int x, int y, 
				   freyja_plane_t plane);
	/*--------------------------------------------
	 * Created  : 2000-09-10 by Mongoose
	 * Modified : 
	 * Pre      : 
	 * Post     : Process mouse input in edit mode
	 --------------------------------------------*/

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

	void getFreeWorldFromScreen(int x, int y, vec3_t p);

	Vector<char *> mRecentFiles;            /* Recently loaded model files */

	Resource *mResource;                    /* Resource system */

	FreyjaModel *mModel;                    /* Data model */
	
	FreyjaRender *mRender;                  /* OpenGL renderer */

	unsigned int mLastEvent, mLastCommand;  /* The last command pair recieved*/

	unsigned int mMouseButton, mModKey;     /* Cached mouse button event */

	unsigned int mMouseState;

	EditorMode mEditorMode;                 /* Mode of the editor */

	EventMode mEventMode;                   /* Mode of generic event handler */
	
	int mUVMouseState;                      /* Mouse state on texture canvas */
	
	int mXYZMouseState;                     /* Mouse state on modeler canvas */

	unsigned int mFileDialogMode;

	bool mFullScreen;

	char mScratchTextBuffer[1024];

	unsigned int mTransformMode;

	bool mCleared;

	float mGenMeshHeight;
	unsigned long mGenMeshCount;
	unsigned long mGenMeshSegements;
};

#endif

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
 *-- History ------------------------------------------------ 
 *
 * 2000-09-10:
 * Mongoose - Created
 ==========================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_FREYJACONTROL_H_
#define GUARD__FREYJA_MONGOOSE_FREYJACONTROL_H_

#include <mstl/Map.h>
#include <mstl/Vector.h>

#include "MaterialManager.h"

#include "FreyjaModel.h"
#include "FreyjaRender.h"
#include "FreyjaEvent.h"
#include "Resource.h"
#include "freyja_events.h"


typedef enum freyja_control_mode             /* Minor control modes */
{
	NO_MODE = 1,
	SAVE_FILE_MODE,
	MESH_SAVE_MODE,
	MESH_ADD_MODE,
	MESH_DEL_MODE,
	MESH_MOVE_CENTER,
	MESH_LOAD_MODE,
	MESH_MOVE,
	MESH_COPY_MODE,
	MESH_CUT_MODE,
	MESH_PASTE_MODE,
	POINT_ADD_MODE,
	POINT_DEL_MODE,
	VERTEX_MOVE,
	POLYGON_ADD_MODE,
	POLYGON_DEL_MODE,
	BONE_CONNECT_MODE,
	BONE_DISCONNECT_MODE,
	TAG_MOVE,
	TAG_MOVE_CENTER,
	BONE_SELECT_MODE,
	MESH_SELECT_MODE,
	MESH_WHOLE_COPY_MODE,
	TEXEL_COMBINE,
	VERTEX_COMBINE,
	VERTEX_BBOX_SELECT_MODE,
	POLYGON_SELECT_MODE,

	// Generic transforms
	MODEL_ROTATE, 
	MODEL_SCALE

} freyja_control_mode_t;


typedef enum freyja_edit_mode                /* Major control modes */
{
	TEXTURE_EDIT_MODE = 0, 
	ANIMATION_EDIT_MODE,
	MAP_EDIT_MODE,
	MODEL_EDIT_MODE,
	MATERIAL_EDIT_MODE

} freyja_edit_mode_t;



class FreyjaControl
{
 public:

	////////////////////////////////////////////////////////////
	// Constructors
	////////////////////////////////////////////////////////////

	FreyjaControl();
	/*--------------------------------------------
	 * Created  : 2000-09-10 by Mongoose
	 * Modified : 
	 * Pre      : model and render must exist
	 * Post     : FreyjaControl object is constructed
	 --------------------------------------------*/

	~FreyjaControl();
	/*--------------------------------------------
	 * Created  : 2000-09-10 by Mongoose
	 * Modified : 
	 * Pre      : FreyjaControl object is allocated
	 * Post     : FreyjaControl object is deconstructed
	 --------------------------------------------*/


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
	 * ????.??.??: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	////////////////////////////////////////////////////////////
	// Public Mutators
	////////////////////////////////////////////////////////////

	void eventAnimation(int command);
	void eventBone(int command);
	void eventMain(int command);
	void eventMesh(int command);
	void eventMisc(int command);

	void handleEvent(int event, int command);
	/*--------------------------------------------
	 * Created  : 2000-09-10 by Mongoose
	 * Modified : 
	 * Pre      : 
	 * Post     : Process menu input
	 --------------------------------------------*/

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


	void ReadRC();
	/*--------------------------------------------
	 * Created  : 2000-09-10 by Mongoose
	 * Modified : 
	 * Pre      : 
	 * Post     : Reads RC file, updates the new 
	 *            keymap, color scheme, etc
	 --------------------------------------------*/

	void setZoom(float zoom);
	/*------------------------------------------------------
	 * Pre  : ZOOM is a number greater than 0.0
	 * Post : Sets current viewing zoom of scene
	 *
	 *-- History ------------------------------------------
	 *
	 * ????.??.??: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	
	bool Mouse(int btn, int state, int mod, int x, int y);
	/*--------------------------------------------
	 * Created  : 2000-09-10 by Mongoose
	 * Modified : 
	 * Pre      : 
	 * Post     : Process mouse input
	 --------------------------------------------*/
	
	bool Motion(int x, int y);
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

	void ScreenToWorld(float *x, float *y);
	/*-----------------------------------------
	 * Created  : 2000-08-25 by Mongoose
	 * Modified : 
	 *
	 * Pre  : x and y are valid float pointers
	 * Post : Convert from screen to 
	 *        world coordinates 
	 -----------------------------------------*/

private:

	////////////////////////////////////////////////////////////
	// Private Accessors
	////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////
	// Private Mutators
	////////////////////////////////////////////////////////////

	void rotateObject(int x, int y, Egg::egg_plane plane);
	void scaleObject(int x, int y, Egg::egg_plane plane);


	void MotionEdit(int x, int y, Egg::egg_plane plane);
	/*--------------------------------------------
	 * Created  : 2000-09-10 by Mongoose
	 * Modified : 
	 * Pre      : 
	 * Post     : Process mouse motion input
	 *            in edit mode
	 --------------------------------------------*/
	
	void MouseEdit(int btn, int state, int mod, int x, int y, 
						Egg::egg_plane plane);
	/*--------------------------------------------
	 * Created  : 2000-09-10 by Mongoose
	 * Modified : 
	 * Pre      : 
	 * Post     : Process mouse input in edit mode
	 --------------------------------------------*/

	//	Vector<Event *> mEvents;

	unsigned int mLastEvent, mLastCommand;

	unsigned int mMouseButton, mModKey;

	Resource mResource;                     /* Resource system */

	MaterialManager *mMaterial;             /* Material manager */

	FreyjaModel *mModel;                    /* Data model */
	
	FreyjaRender *mRender;                  /* OpenGL renderer */
	
	freyja_control_mode_t _minor_mode;      /* Minor mode of the controler */

	freyja_edit_mode_t _major_mode;         /* Major mode of the controler */
	
	int _tex_state;                         /* Mouse state on texture canvas */
	
	int _mouse_state;                       /* Mouse state on modeler canvas */

	unsigned int mFileDialogMode;

	bool mFullScreen;

	char mScratchTextBuffer[1024];
};

#endif

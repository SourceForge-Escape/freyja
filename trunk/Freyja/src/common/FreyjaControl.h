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

#include "FreyjaModel.h"
#include "FreyjaRender.h"
#include "FreyjaResource.h"
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
	TEXTURE_EDIT_MODE  = 0,
	MODEL_VIEW_MODE    = 1,
	MAP_VIEW_MODE      = 2,
	MODEL_EDIT_MODE    = 3,
	MATERIAL_EDIT_MODE = 4

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


	FreyjaResource _resource;               /* Resource agent */
	
	FreyjaModel *_model;                    /* Data model agent */
	
	FreyjaRender *_render;                  /* GL Renderer agent */
	
	freyja_control_mode_t _minor_mode;      /* Minor mode of the controler */

	freyja_edit_mode_t _major_mode;         /* Major mode of the controler */
	
	int _tex_state;                         /* Mouse state on texture canvas */
	
	int _mouse_state;                       /* Mouse state on modeler canvas */

	bool mFullScreen;
};

#endif

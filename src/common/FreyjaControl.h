/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
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
 ================================================================*/


#ifndef __FREYJA_MONGOOSE_FREYJACONTROL_H_
#define __FREYJA_MONGOOSE_FREYJACONTROL_H_

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


// Mongoose 2002.02.21, HHhhhhmmm.... Tracer event testing
class FreyjaEvent
{
public:
	FreyjaEvent();

	~FreyjaEvent();

	void AddEvent(int e, int c);
	
	void Emit();

private:

	void *_func_event;

	bool _pair_active;

	int _pair_event[2];
};


class FreyjaControl
{
 public:

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
	
	void Event(int event, int command);
	/*--------------------------------------------
	 * Created  : 2000-09-10 by Mongoose
	 * Modified : 
	 * Pre      : 
	 * Post     : Process menu input
	 --------------------------------------------*/
	
	void AddKeyCommand(int key, int event, int command);
	/*------------------------------------------------------
	 * Pre  : Key is valid ascii code, Event, Command valid pair
	 * Post : Makes <ctrl> key press generate event pair
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.02.21:
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

	Map <int, FreyjaEvent *> _key;          /* Keyboard commands */

	FreyjaResource _resource;               /* Resource agent */
	
	FreyjaModel *_model;                    /* Data model agent */
	
	FreyjaRender *_render;                  /* GL Renderer agent */
	
	freyja_control_mode_t _minor_mode;      /* Minor mode of the controler */

	freyja_edit_mode_t _major_mode;         /* Major mode of the controler */
	
	int _tex_state;                         /* Mouse state on texture canvas */
	
	int _mouse_state;                       /* Mouse state on modeler canvas */
};

#endif

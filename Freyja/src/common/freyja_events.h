/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : SDL/GLUT template
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : 
 * License : No use w/o permission (C) 2001 Mongoose
 * Comments: This is the common event system interface
 *           for freyja
 *
 *           It's independent of the widget set, etc
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2001.06.26:
 * Mongoose - Created
 =================================================================*/

#ifndef GUARD__FREYJA_MONGOOSE_EVENT_H_
#define GUARD__FREYJA_MONGOOSE_EVENT_H_

#define EVENT_FREYJA         -1000
#define EVENT_REDO_LAST      -5
#define CMD_FREYJA_EXIT      0
#define CMD_FREYJA_GL_INIT   1
#define CMD_NULL             0


typedef struct callback_bone_s 
{
	int id;
	char name[64];
	unsigned int numChildren;
	callback_bone_s *parent;
	callback_bone_s *children;

} callback_bone_t;

void callback_update_skeleton(callback_bone_t *bone);


typedef enum
{
	KEY_LSHIFT =   1,
	KEY_RSHIFT =   2,
	KEY_LCTRL  =   4,
	KEY_RCTRL  =   8,
	KEY_LALT   =  16,
	KEY_RALT   =  32

} key_modifers_t;


typedef enum
{
	KEY_UP         = 256,
	KEY_DOWN       = 257,
	KEY_RIGHT      = 258,
	KEY_LEFT       = 259,
	KEY_PAGE_DOWN  = 260,
	KEY_PAGE_UP    = 261,

	KEY_F1         = 500,
	KEY_F2         = 501,
	KEY_F3         = 502,
	KEY_F4         = 503,
	KEY_F5         = 504,
	KEY_F6         = 505,
	KEY_F7         = 506,
	KEY_F8         = 507,
	KEY_F9         = 508,
	KEY_F10        = 509,
	KEY_F11        = 510,
	KEY_F12        = 511,

   KEY_SPACE      = 600

} key_std_t;

typedef enum
{
	MOUSE_BTN_RIGHT    =   1,
	MOUSE_BTN_LEFT     =   2,
	MOUSE_BTN_MIDDLE   =   4,
	MOUSE_BTN_4        =   8,
	MOUSE_BTN_5        =  16

} mouse_key_t;

typedef enum
{
	MOUSE_BTN_STATE_NONE      = 0,
	MOUSE_BTN_STATE_PRESSED   = 1

} mouse_key_state_t;

//////////////////////////////////////////////////////////

enum freyja_event_type
{
	EVENT_MAIN          =  0,
	EVENT_BONE          =  1,
	EVENT_MESH          =  2,
	EVENT_POLYGON       =  3,
	EVENT_POINT         =  4,
	EVENT_MISC          =  5,
	EVENT_GROUP         =  6,
	EVENT_TRANSFORM     =  7,
	EVENT_ANIMATION     =  8,
	EVENT_FREYJA_MODE   = 10,
	EVENT_MATERIAL      = 11
};

enum freyja_event_animation_cmd
{
	CMD_ANIMATION_NEXT = 1,
	CMD_ANIMATION_PREV = 2,
	CMD_ANIMATION_PLAY = 3,
	CMD_ANIMATION_STOP = 4
};

enum freyja_event_transform_cmd
{
	CMD_TRANSFORM_SCALE_DOWN_X = 1,
	CMD_TRANSFORM_SCALE_UP_X   = 2,
	CMD_TRANSFORM_SCALE_DOWN_Y = 3,
	CMD_TRANSFORM_SCALE_UP_Y   = 4,
	CMD_TRANSFORM_SCALE_DOWN_Z = 5,
	CMD_TRANSFORM_SCALE_UP_Z   = 6,
	
	CMD_TRANSFORM_ROT_DOWN_X  = 11,
	CMD_TRANSFORM_ROT_UP_X    = 12,
	CMD_TRANSFORM_ROT_DOWN_Y  = 13,
	CMD_TRANSFORM_ROT_UP_Y    = 14,
	CMD_TRANSFORM_ROT_DOWN_Z  = 15,
	CMD_TRANSFORM_ROT_UP_Z    = 16,
	
	CMD_TRANSFORM_TRANS_DOWN_X = 21,
	CMD_TRANSFORM_TRANS_UP_X   = 22,
	CMD_TRANSFORM_TRANS_DOWN_Y = 23,
	CMD_TRANSFORM_TRANS_UP_Y   = 24,
	CMD_TRANSFORM_TRANS_DOWN_Z = 25,
	CMD_TRANSFORM_TRANS_UP_Z   = 26,

	CMD_TRANSFORM_SCALE_DOWN_XYZ = 30, 
	CMD_TRANSFORM_SCALE_UP_XYZ   = 31
};

enum freyja_event_main_cmd
{
	CMD_MAIN_SHUTDOWN = 1,
	CMD_MAIN_NEW_MODEL,
	CMD_MAIN_OPEN_MODEL,
	CMD_MAIN_SAVE_MODEL,
	CMD_MAIN_OPEN_TEXTURE,
	CMD_MAIN_SELECTED_DIR,
	CMD_MAIN_SELECTED_FILE,
	CMD_MAIN_SAVE_MATERIAL,
	CMD_MAIN_OPEN_MATERIAL,
	CMD_MAIN_NEW_MATERIAL,
	CMD_MAIN_UNUSED1,
	CMD_MAIN_DEBUG_EGG,
	CMD_MAIN_OPEN_PALETTE,
	CMD_TOGGLE_FULLSCREEN
};

enum freyja_event_polygon_cmd
{
	CMD_POLYGON_ADD = 1,
	CMD_POLYGON_DELETE,
	CMD_POLYGON_SELECT
};

enum freyja_event_point_cmd
{
	CMD_POINT_ADD     = 1,
	CMD_POINT_DELETE,
	CMD_POINT_MOVE,
	CMD_POINT_COMBINE
};

enum freyja_event_bone_cmd
{
	CMD_BONE_PREV            =  1,
	CMD_BONE_NEXT            =  2,
	CMD_BONE_NEW             =  5,

	CMD_BONE_ADD_MESH        = 10,
	CMD_BONE_DELETE_MESH     = 15,

	CMD_BONE_SELECT          = 20,
	CMD_BONE_CONNECT         = 25,

	CMD_BONE_DISCONNECT      = 30,
	CMD_BONE_MOVE_PIVOT      = 35,

	CMD_BONE_DELETE          = 40,
	
	CMD_BONE_ROTATE_UP_X     = 60,
	CMD_BONE_ROTATE_UP_Y     = 61,
	CMD_BONE_ROTATE_UP_Z     = 62,
	CMD_BONE_ROTATE_DOWN_X   = 63,
	CMD_BONE_ROTATE_DOWN_Y   = 64,
	CMD_BONE_ROTATE_DOWN_Z   = 65
};

enum freyja_event_misc_cmd
{
	CMD_MISC_ZOOM_IN         =  1,
	CMD_MISC_ZOOM_OUT        =  2,
	CMD_MISC_SCROLL_UP_X     =  3,
	CMD_MISC_SCROLL_DOWN_X   =  4,
	CMD_MISC_SCROLL_UP_Y     =  5,
	CMD_MISC_SCROLL_DOWN_Y   =  6,
	CMD_MISC_SCROLL_UP_Z     =  7,
	CMD_MISC_SCROLL_DOWN_Z   =  8,
	CMD_MISC_SCROLL_UP       =  9,

	CMD_MISC_SCROLL_DOWN     = 10,
	CMD_MISC_SCROLL_LEFT     = 11,
	CMD_MISC_SCROLL_RIGHT    = 12,
	CMD_MISC_SCREENSHOT      = 13,
	CMD_MISC_SIDES_P         = 14,
	CMD_MISC_SIDES_M         = 15,
	CMD_MISC_DUMPTEXTURE     = 16,
	CMD_MISC_INFO            = 17,
	CMD_MISC_FPS             = 18,
	CMD_MISC_WIDGETSHOWN     = 19,

	CMD_MISC_CAMMODE         = 20,
	CMD_MISC_EDITMODE        = 21,
	CMD_MISC_TEXTURE_PREV    = 22,
	CMD_MISC_TEXTURE_NEXT    = 23,
	CMD_MISC_PAL_NEXT        = 24,
	CMD_MISC_RENDER_ROT_Z_P  = 25,
	CMD_MISC_RENDER_ROT_Z_M  = 26,
	CMD_MISC_RENDER_ROT_Y_P  = 27,
	CMD_MISC_RENDER_ROT_Y_M  = 28,
	CMD_MISC_RENDER_ROT_X_P  = 29,

	CMD_MISC_RENDER_ROT_X_M  = 30,
	CMD_MISC_TEXEL_COMBINE   = 31,
	CMD_MISC_BBOX_SELECT     = 32,
	CMD_MISC_SELECT          = 33,
	CMD_MISC_GEN_TEXMAP_XY   = 34,
	CMD_MISC_VERTEX_UV       = 35,
	CMD_MISC_SCENE_ROTATE    = 36,
	CMD_MISC_TEXTURE_SET     = 37,
	CMD_MISC_TEX_SLOT_LOAD   = 38,
	CMD_MISC_RENDER_BBOX     = 39,

	CMD_MISC_RENDER_GRID     = 40,
	CMD_MISC_LOAD_MAP        = 41,
	CMD_MISC_DISPLAY_INFO    = 42,
	
	CMD_MISC_GEN_NORMALS
};

enum freyja_event_mesh_cmd
{
	CMD_MESH_MIRROR = 1,
	CMD_MESH_DUP_FRAME,
	CMD_MESH_DUP_SCENE,
	CMD_MESH_ADD,
	CMD_MESH_DELETE,
	CMD_MESH_MOVE,
	CMD_MESH_MOVE_CENTER,
	CMD_MESH_GEN_FROM_FRAME,
	CMD_MESH_COPY,
	CMD_MESH_SPLIT,
	CMD_MESH_MERGE,
	CMD_MESH_SELECT,
	CMD_MESH_FULLCOPY,
	CMD_MESH_PREV,
	CMD_MESH_NEXT,
	CMD_MESH_ROTATE,
	CMD_MESH_SCALE,

	CMD_MESH_CLONE   = 50,

	CMD_MESH_COPY2   = 100,
	CMD_MESH_CUT     = 101,
	CMD_MESH_PASTE   = 102,

	CMD_MESH_GENERATE_CUBE   = 200,
	CMD_MESH_GENERATE_TRIS   = 201,
	CMD_MESH_GENERATE_SPHERE = 202,
	CMD_MESH_GENERATE_CAN    = 203
};

enum freyja_event_group_cmd
{
	CMD_GRP_SCALE_DOWN_XYZ   = 1,
	CMD_GRP_SCALE_UP_XYZ     = 2,
	CMD_GRP_SCALE_DOWN_X     = 3,
	CMD_GRP_SCALE_UP_X       = 4,
	CMD_GRP_SCALE_DOWN_Y     = 5,
	CMD_GRP_SCALE_UP_Y       = 6,
	CMD_GRP_SCALE_DOWN_Z     = 7,
	CMD_GRP_SCALE_UP_Z       = 8,
	CMD_GRP_PREV             = 9,

	CMD_GRP_NEXT             = 10,
	CMD_GRP_MIRROR           = 20
};

enum freyja_event_freyja_mode
{
	FREYJA_MODE_MODEL_EDIT          = 1,
	FREYJA_MODE_MODEL_VIEW          = 2,
	FREYJA_MODE_TEXTURE_EDIT        = 3,
	FREYJA_MODE_ANIMATION           = 4,

	FREYJA_MODE_MODEL_VIEW2         = 6,
	FREYJA_MODE_PLANE_XY            = 7,
	FREYJA_MODE_PLANE_XZ            = 8,
	FREYJA_MODE_PLANE_YZ            = 9,

	FREYJA_MODE_RENDER_TEXTURE      = 10,
	FREYJA_MODE_RENDER_FACE         = 11,
	FREYJA_MODE_RENDER_LINELOOP     = 12,
	FREYJA_MODE_RENDER_WIREFRAME    = 13,
	FREYJA_MODE_RENDER_LIGHTING     = 14,
	FREYJA_MODE_RENDER_POINTS       = 15,
	FREYJA_MODE_RENDER_BLENDING     = 17,
	FREYJA_MODE_RENDER_MATERIAL     = 18,
	FREYJA_MODE_MATERIAL_EDIT       = 19,
	FREYJA_MODE_RENDER_NORMAL       = 20,
	FREYJA_MODE_RENDER_BONETAG      = 21,

	FREYJA_MODE_LOAD_MODEL        = 101,
	FREYJA_MODE_SAVE_MODEL        = 102,
	FREYJA_MODE_LOAD_TEXTURE      = 103,
	FREYJA_MODE_SAVE_MATERIAL     = 104,
	FREYJA_MODE_LOAD_MATERIAL     = 105,
	FREYJA_MODE_LOAD_EMITTER      = 106,
	FREYJA_MODE_SAVE_EMITTER      = 107,
	FREYJA_MODE_LOAD_PALETTE      = 108
};

enum custom_color                   /* Custom GUI colors */
{
  COLOR_EDIT_POLYGON             = 1,
  COLOR_EDIT_LINE                = 2,
  COLOR_EDIT_LINE_HIGHLIGHT      = 4,
  COLOR_EDIT_VERTEX              = 8,
  COLOR_EDIT_VERTEX_HIGHLIGHT    = 16,
  COLOR_EDIT_BBOX                = 32,
  COLOR_EDIT_BG                  = 64,
  COLOR_EDIT_GRID                = 128,
  COLOR_EDIT_GRID_AXIS_X         = 256,
  COLOR_EDIT_TEXT                = 1024,
  COLOR_VIEW_BG                  = 2048,
  COLOR_VIEW_TEXT                = 4096,
  COLOR_EDIT_GRID_AXIS_Y         = 8192,
  COLOR_EDIT_GRID_AXIS_Z         = 16384,
  COLOR_EDIT_GRID_8              = 32768
};

//////////////////////////////////////////////

int query_mouse_active();
int query_mouse_button();
int query_load_texture_to_slot();

void event_set_mouse_active(bool b);
void event_set_mouse_button(int i);
void event_set_load_texture_to_slot(int i);
void event_new_key_cmd(int key, int event, int cmd);

//////////////////////////////////////////////

	void freyja_event2i(int event, int cmd);
	/*------------------------------------------------------
	 * Pre  : Event and Cmd are valid event pair
	 * Post : Broadcasts event to all freyja agents/listeners
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.19:
	 *  Mongoose - Created
	 ------------------------------------------------------*/


	void freyja_event2i_interface_listener(int event, int cmd);
	/*------------------------------------------------------
	 * Pre  : Event and Cmd are valid event pair
	 *        broadcast from freyja_event2i
	 *
	 *        This listener is implmented by interface
	 *
	 * Post : Interface recieves event, so it may handle it 
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.19:
	 *  Mongoose - Created
	 ------------------------------------------------------*/

   void refresh_material_interface();
	/*------------------------------------------------------
	 * Pre  : This listener is implmented by interface
	 * Post : Interface syncs interface to backend values
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.02.12:
	 * Mongoose - Created
	 ------------------------------------------------------*/

   void refresh_emitter_interface();
	/*------------------------------------------------------
	 * Pre  : This listener is implmented by interface
	 * Post : Interface syncs interface to backend values
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.02.12:
	 * Mongoose - Created
	 ------------------------------------------------------*/


char *freyja_rc_map(char *s);


void event_swap_buffers();
void event_shutdown(int code);
void event_print(char *s, ...);
void event_filename(int mode, char *filename);
void event_load_model(char *filename);
void event_popup_menu(int x, int y);
void event_refresh();
void event_custom_color(int custom_color_flags, float r, float g, float b);
void event_init(unsigned int *width, unsigned int *height, 
					 bool *fullscreen, char **driver);
void event_render_init(unsigned int width, unsigned int height);
void event_display();
void event_shutdown();
void event_resize(int width, int height);
void event_key(int key, int mod);
void event_motion(int x_delta, int y_delta);
void event_mouse(int button, int state, int mod, int x, int y);


#endif

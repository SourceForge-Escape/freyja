/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : (C) 2001-2007 Terry 'Mongoose' Hendrix II
 * Comments: This is the event system facade for mgtk for freyja.
 *
 *           It's independent of the widget set, etc
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongooseichiban@gmail.com>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2001.06.26:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__FREYJA3D_FREYJA_EVENT_H
#define GUARD__FREYJA3D_FREYJA_EVENT_H

#include <stdarg.h>

#include <mstl/String.h>

#include <mgtk/mgtk_keys.h>
#include <mgtk/mgtk_events.h>

#include <freyja/freyja.h>
#include <freyja/QueryABI.h>

typedef enum {

	/* Special events */
	eNone  = 0,           // Event to be ignored.
	eNop   = 2,           // Event with no action.
	eMode  = 3,           // These events change modes ( modify states )
	eEvent = 4,           // These events request handling ( perform actions )

	/* Event ids ( minors ) */
	
	// Colors
	eColorMaterialAmbient,
	eColorMaterialDiffuse,
	eColorMaterialSpecular,
	eColorMaterialEmissive,
	eColorLightAmbient,
	eColorLightDiffuse,
	eColorLightSpecular,
	eColorBackground,
	eColorGrid,
	eColorVertex,
	eColorVertexHighlight,
	eColorMesh,
	eColorMeshHighlight,
	eColorBone,
	eColorBoneHighlight,
	eColorJoint,
	eColorJointHighlight,

	// Menus
	ePluginMenu,
	eViewportModeMenu,
	eTransformMenu,
	eObjectMenu,
	eBlendDestMenu,
	eBlendSrcMenu,

	// Special base id for plugins to avoid conflicts
	ePluginEventBase = 10000

} event_subject_id;

#include <freyja/Scene.h>
extern freyja::Scene* gScene;


//////////////////////////////////////////////////////////////////////////////
// Freyja mgtk interface
//////////////////////////////////////////////////////////////////////////////

#define mgtk_event_set_float mgtk_spinbutton_value_set

#define freyja_event_notify_observer1f mgtk_event_notify_observer1f
//void freyja_event_notify_observer1f(event_subject_id e, float value);
/*------------------------------------------------------
 * Pre  : 
 * Post : This back propagates value changes to widgets
 *
 ------------------------------------------------------*/

#define freyja_append_item_to_menu mgtk_append_item_to_menu
//int freyja_append_item_to_menu(int event, const char *label, int item_event);
/*------------------------------------------------------
 * Pre  : 
 * Post : Appends menuitem <label>, <item_event> to menu <event>
 *
 ------------------------------------------------------*/

#define freyja_remove_all_items_from_menu mgtk_remove_all_items_to_menu
//void freyja_remove_all_items_from_menu(int event);
/*------------------------------------------------------
 * Pre  : <event> is the Id of the menu subject 
 * Post : Removes all submenus and menuitems from menu
 *
 ------------------------------------------------------*/

void freyja_append_eventid(char *symbol, int eventid);
/*------------------------------------------------------
 * Pre  : 
 * Post : Aliases another <symbol> for existing <eventid>
 *
 ------------------------------------------------------*/

void freyja_application_window_move(int x, int y);
/*------------------------------------------------------
 * Pre  : 
 * Post : Moves application window without user input
 *
 ------------------------------------------------------*/

void freyja_callback_get_image_data_rgb24(const char *filename, 
										  unsigned char **image, 
										  int *width, int *height);
/*------------------------------------------------------
 * Pre  : 
 * Post : Callback retrieves image data from filename
 *
 ------------------------------------------------------*/

void freyja_set_dialog_visible(const char *name);
/*------------------------------------------------------
 * Pre  : 
 * Post : Show() dialog by symbol name.
 *
 ------------------------------------------------------*/

int freyja_event2i(int event, int cmd);
/*------------------------------------------------------
 * Pre  : <event> and <cmd> are an valid event pair
 * Post : Passes event to freyja master control
 *
 *        Returns -1 on unhandled event
 *
 ------------------------------------------------------*/

void freyja_event_exit();
/*------------------------------------------------------
 * Pre  : 
 * Post : Calls shutdown and exits GUI
 *
 ------------------------------------------------------*/

void freyja_event_fullscreen();
/*------------------------------------------------------
 * Pre  : 
 * Post : Make application fullscreen
 *
 ------------------------------------------------------*/

void freyja_event_gl_refresh();
/*------------------------------------------------------
 * Pre  : 
 * Post : Display callback
 *
 ------------------------------------------------------*/

void freyja_event_info_dialog( const char* icon, const char* message );
/*------------------------------------------------------
 * Pre  : 
 * Post : Spawns info dialog with <message> and <icon>
 *
 ------------------------------------------------------*/

void freyja_event_set_float(int event, float value);
/*------------------------------------------------------
 * Pre  : 
 * Post : Set float <value> for widget <event>
 *
 ------------------------------------------------------*/

void freyja3d_shutdown();
/*------------------------------------------------------
 * Pre  : 
 * Post : Cleans up Freyja subsystems
 *
 ------------------------------------------------------*/

void freyja_event_start();
/*------------------------------------------------------
 * Pre  : 
 * Post : Starts up Freyja subsystems
 *
 ------------------------------------------------------*/

void freyja_event_unfullscreen();
/*------------------------------------------------------
 * Pre  : 
 * Post : Restore application from fullscreen
 *
 ------------------------------------------------------*/

long freyja_get_new_plugin_eventid();
/*------------------------------------------------------
 * Pre  : 
 * Post : Gets next UID for a plugin event
 *
 ------------------------------------------------------*/

void freyja_get_pixmap_filename(char *dest, unsigned int size, char *icon_name);
/*------------------------------------------------------
 * Pre  : 
 * Post : Gets full path filename for icons
 *
 ------------------------------------------------------*/

void freyja_get_rc_path(char *s, long sz);
/*------------------------------------------------------
 * Pre  : 
 * Post : Gets user resource dir
 *
 ------------------------------------------------------*/

void freyja_get_rc_filename(char *s, const char *filename, long sz);
/*------------------------------------------------------
 * Pre  : 
 * Post : Gets user resource dir relative filename
 *
 ------------------------------------------------------*/

void freyja_get_share_path(char *s, long sz);
/*------------------------------------------------------
 * Pre  : 
 * Post : Gets system share dir
 *
 ------------------------------------------------------*/

void freyja_get_share_filename(char *s, const char *filename, long sz);
/*------------------------------------------------------
 * Pre  : 
 * Post : Gets system share dir relative filename
 *
 ------------------------------------------------------*/

void freyja_handle_color(int id, float r, float g, float b, float a);
/*------------------------------------------------------
 * Pre  : 
 * Post : mgtk callback handles color button value change
 *
 ------------------------------------------------------*/

void freyja_handle_resource_start();
/*------------------------------------------------------
 * Pre  : 
 * Post : mgtk callback inits the modeler application
 *
 ------------------------------------------------------*/

void freyja_install_user();
/*------------------------------------------------------
 * Pre  : 
 * Post : Copies files from system share dir to user dir
 *
 ------------------------------------------------------*/

char freyja_is_user_installed_data_old();
/*------------------------------------------------------
 * Pre  : 
 * Post : Returns non zero if it can find a user resource
 *        directory or data is out of date.
 *
 ------------------------------------------------------*/

void freyja_load_texture_buffer(byte *image, uint32 w, uint32 h, uint32 bpp);
/*------------------------------------------------------
 * Pre  : Arguments are componets of a valid pixel buffer
 * Post : Pixbuf is loaded as a texture or surface
 *
 ------------------------------------------------------*/

void freyja_plugin_generic(const char *symbol, void *something);
/*------------------------------------------------------
 * Pre  : <symbol> is the query 
 * Post : Returns <something> which is the thing symbol
 *         is bound to... mostly functions and data  ;)
 *
 ------------------------------------------------------*/

void freyja_print(char *format, ...);
/*------------------------------------------------------
 * Pre  : (<format>, ...) just like printf()  
 * Post : Prints to status bar, and will also print to
 *        console and log with '!' as first char in format
 *
 ------------------------------------------------------*/

mstl::String freyja_get_resource_path();
mstl::String freyja_rc_map_string(const char *filename);
/*------------------------------------------------------
 * Pre  : <s> is a valid filename or dir in resource dir
 * Post : Returns fullpath filename as string
 *
 ------------------------------------------------------*/

const char* freyja_rc_map(const char* s);
/*------------------------------------------------------
 * Pre  : <s> is a valid filename or dir in resource dir
 * Post : Returns fullpath filename as C string or NULL
 *
 *        Call delete [] on this string once you're done
 *
 *        mgtk indirectly queries this to setup things
 *        such as user icons, etc
 *
 ------------------------------------------------------*/

void freyja_set_main_window_title(char *title);
/*------------------------------------------------------
 * Pre  : 
 * Post : <title> is set as the new window title
 *
 ------------------------------------------------------*/

void freyja_swap_buffers();
/*------------------------------------------------------
 * Pre  : 
 * Post : Called at the end of display, but does nothing
 *        you'd expect in mgtk builds
 *
 ------------------------------------------------------*/

void freyjaQueryCallbackHandler(unsigned int size, freyja_query_t *array);
/*------------------------------------------------------
 * Pre  : 
 * Post : Legacy function that handles generated queries.
 *
 ------------------------------------------------------*/


// 'freyja3d' 

void freyja3d_scenegraph_init();
/*------------------------------------------------------
 * Pre  : 
 * Post : Initialize the scenegraph widget(s).
 *
 ------------------------------------------------------*/

void freyja3d_update_scenegraph();
/*------------------------------------------------------
 * Pre  : 
 * Post : Update the scenegraph widgets.
 *
 ------------------------------------------------------*/

int freyja3d_get_event_id(const char* symbol);
/*------------------------------------------------------
 * Pre  : 
 * Post : Returns symbol's numeric id or -1.
 *
 ------------------------------------------------------*/

void freyja3d_record_saved_model(const char* filename);
/*------------------------------------------------------
 * Pre  : 
 * Post : Update client title, recent list, etc for a
 *        newly saved model.
 *
 ------------------------------------------------------*/

int freyja3d_load_texture(const char* filename);
/*------------------------------------------------------
 * Pre  : 
 * Post : Texture callback load function.
 *
 ------------------------------------------------------*/

int freyja3d_load_shader(const char* filename);
/*------------------------------------------------------
 * Pre  : 
 * Post : Shader callback load function.
 *
 ------------------------------------------------------*/

bool freyja3d_execute_lua_script( const char* filename );
/*------------------------------------------------------
 * Pre  : 
 * Post : 
 *
 ------------------------------------------------------*/

bool freyja3d_save_user_preferences( );
/*------------------------------------------------------
 * Pre  : 
 * Post : 
 *
 ------------------------------------------------------*/


void freyja3d_query_callback_handler(unsigned int size, freyja_query_t *array);
void freyja_handle_application_window_close( );
void freyja_handle_resource_start( );
const char* freyja_rc_map( const char* filename );
void freyja_handle_command(int command);
void freyja_handle_key_press(int key, int mod);
void freyja_handle_event1u(int event, unsigned int value);
void freyja_handle_command2i(int event, int command);
void freyja_handle_event1f(int event, float value);
void freyja_handle_gldisplay();
void freyja_handle_glresize(unsigned int width, unsigned int height);
void freyja_handle_text(int event, char *text);
void freyja_handle_text_array(int event, unsigned int count, char **text);
const char *freyja_get_resource_path_callback( );
void freyja_handle_motion(int x, int y);
void freyja_handle_mouse(int button, int state, int mod, int x, int y);


#endif // GUARD__FREYJA3D_FREYJA_EVENT_H

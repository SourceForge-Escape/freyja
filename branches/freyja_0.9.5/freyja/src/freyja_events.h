/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://icculus.org/mongoose/
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : (C) 2001-2007 Mongoose
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
#include <mgtk/mgtk_keys.h>
#include <mgtk/mgtk_events.h>
#include <mstl/String.h>
#include <freyja/freyja.h>

#define FREYJA_LOG_FILE "Freyja-dev.log"
#define FREYJA_RECENT_FILES "recent_files-dev"


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

	// Menus
	ePluginMenu,
	eViewportModeMenu,
	eTransformMenu,
	eObjectMenu,
	eBlendDestMenu,
	eBlendSrcMenu,

	// Old file dialog marker event ids
	FREYJA_MODE_LOAD_MODEL,
	FREYJA_MODE_SAVE_MODEL,
	FREYJA_MODE_LOAD_TEXTURE,
	FREYJA_MODE_SAVE_MATERIAL,
	FREYJA_MODE_LOAD_MATERIAL,

	// Special base id for plugins to avoid conflicts
	ePluginEventBase = 10000

} event_subject_id;


//////////////////////////////////////////////////////////////////////////////
// Freyja mgtk interface
//////////////////////////////////////////////////////////////////////////////

// These are here to keep the old transparent API, without using a facade.
#define freyja_event_file_dialog mgtk_event_file_dialog
#define freyja_event_get_color mgtk_event_get_color
#define freyja_event_set_color mgtk_event_set_color
#define freyja_event_get_float mgtk_event_get_float
#define freyja_event_set_float mgtk_spinbutton_value_set
#define freyja_event_set_range mgtk_event_set_range
#define freyja_event_gl_refresh mgtk_event_gl_refresh
#define freyja_set_main_window_title mgtk_application_window_title
#define freyja_swap_buffers mgtk_event_swap_gl_buffers
#define freyja_application_window_move mgtk_application_window_move
#define freyja_event_info_dialog mgtk_create_info_dialog
#define freyja_event_fullscreen mgtk_application_window_fullscreen
#define freyja_event_unfullscreen mgtk_application_window_unfullscreen
#define freyja_event_exit mgtk_event_shutdown

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

void freyja_event_info_dialog(char *icon, char *message);
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

void freyja_event_shutdown();
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

char freyja_is_user_installed();
/*------------------------------------------------------
 * Pre  : 
 * Post : Returns non zero if it can find a user resource
 *        directory.
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

char *freyja_rc_map(char *s);
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


#endif // GUARD__FREYJA3D_FREYJA_EVENT_H

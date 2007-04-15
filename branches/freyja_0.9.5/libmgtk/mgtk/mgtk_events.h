/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libmgtk
 * Author  : Mongoose
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (c) 2000-2004 Mongoose
 * Comments: This is the generic mlisp bridge to the mgtk widget system.
 *
 *           It provides a 2-way event system w/o requiring direct bindings.
 *           ( eg You can make this a dlopen'able interface facade. )
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.10.27:
 * Mongoose - Major API clean up, documentation update
 *
 *            Created, from old mlisp and mgtk source headers
 ==========================================================================*/

#ifndef GUARD__MGTK_MONGOOSE_MGTK_EVENT_H_
#define GUARD__MGTK_MONGOOSE_MGTK_EVENT_H_

#define ACCEL_SUPPORT_ON

// Arg list types for Resource use
#define ARG_GTK_WIDGET           64
#define ARG_GTK_BOX_WIDGET       128
#define ARG_GTK_WINDOW           256
#define ARG_GTK_MENU_WIDGET      1024
#define ARG_GTK_NOTEBOOK         2048
#define ARG_GTK_TOOLBOX_WIDGET   4096


/* Handle mgtk widget events by implementing these functions in your code */
/* Repeat: You have to implement these following call backs */

extern "C" {

void mgtk_callback_get_image_data_rgb24(const char *filename, 
										unsigned char **image, 
										int *width, int *height);
void mgtk_handle_application_window_close();
void mgtk_handle_color(int id, float r, float g, float b, float a);
void mgtk_handle_command(int command);
void mgtk_handle_command2i(int event, int command);
void mgtk_handle_event1u(int event, unsigned int value);
void mgtk_handle_event1f(int event, float value);
void mgtk_handle_gldisplay();
void mgtk_handle_glresize(unsigned int width, unsigned int height);
void mgtk_handle_key_press(int key, int mod);
//void mgtk_handle_key_release(int key, int mod);
void mgtk_handle_motion(int x_delta, int y_delta);
void mgtk_handle_mouse(int button, int state, int mod, int x, int y);
void mgtk_handle_resource_start();
void mgtk_handle_slider1u(int event, unsigned int value);
void mgtk_handle_text(int event, char *text);

void mgtk_print(char *format, ...);

void mgtk_get_pixmap_filename(char *dest, unsigned int size, char *icon_name);
char *mgtk_rc_map(char *filename_or_dirname);
const char *mgtk_get_resource_path();

void mgtk_toggle_value_set(int event, int val);
}


/* Call into special mgtk using these internal mgtk functions below */
float mgtk_event_get_float(int event);

int mgtk_event_set_range(int event, unsigned int value,
						 unsigned int min, unsigned int max);

void mgtk_event_notify_observer1f(unsigned int id, float r);

void mgtk_create_query_dialog_text(char *image, char *message, 
								   int eventId, const char *value);

float mgtk_create_query_dialog_float(char *image, char *message,
									 float value, float min, float max, 
									 float step, int digits);

void mgtk_create_info_dialog(char *icon, char *message);

int mgtk_create_confirm_dialog(const char *dialog_icon,
							   const char *information_message, 
							   const char *question_message,
							   const char *cancel_icon, const char *cancel_text,
							   const char *accept_icon, const char *accept_text);

/* File dialog events, now sends handle_text events */
void mgtk_event_file_dialog(int eventId, char *title);
void mgtk_event_fileselection_set_dir(int eventId, char *dir);
void mgtk_event_fileselection_append_pattern(int eventId, char *label, char *pattern);

void mgtk_event_dialog_visible_set(int dialog, int visible);
void mgtk_event_get_color(int id, float &r, float &g, float &b, float &a); // C++ exportable only -- fix this with the C ABI patch 2007.04.01
void mgtk_event_set_color(int id, float r, float g, float b, float a);
void mgtk_event_notify_observer1f(unsigned int event, float value);
void mgtk_event_notify_statusbar(const char *message);
void mgtk_event_swap_gl_buffers();
void mgtk_event_gl_refresh();
void mgtk_event_shutdown();

int mgtk_remove_all_items_to_menu(int event);
int mgtk_append_item_to_menu2i(int menuEvent, const char *label, short event, short command);
int mgtk_append_item_to_menu(int event, const char *label, int item_event);

int mgtk_init(int argc, char *argv[]);

void mgtk_start();

void mgtk_spinbutton_value_set(int event, float val);
void mgtk_textentry_value_set(int event, const char *s);
void mgtk_option_menu_value_set(int event, int index);
void mgtk_togglebutton_value_set(int event, bool val);

void mgtk_application_window_title(char *title);
void mgtk_application_window_move(int x, int y);
void mgtk_application_window_resize(int width, int height);
void mgtk_application_window_role(char *role);
void mgtk_application_window_fullscreen();
void mgtk_application_window_unfullscreen();


//////////////////////////////////////////////////////////////////
// mgtk_tree
// Use the functional API to avoid near future breakage...
// of your knees
//////////////////////////////////////////////////////////////////

#define mgtk_tree_label_size 64

typedef struct mgtk_tree_s 
{
	int id;
	char label[mgtk_tree_label_size];

	unsigned int numChildren;
	mgtk_tree_s *parent;
	mgtk_tree_s *children;

} mgtk_tree_t;

mgtk_tree_t *mgtk_event_tree_new(int id, const char *label);
/*------------------------------------------------------
 * Pre  : 
 * Post : Allocates a new <tree> and initializes members
 ------------------------------------------------------*/

void mgtk_event_tree_copy(mgtk_tree_t *src, mgtk_tree_t *dest);
/*------------------------------------------------------
 * Pre  : You have to know what you're doing by calling this.
 * Post : _Shallow_ copies members of <src> to <dest>
 ------------------------------------------------------*/

void mgtk_event_tree_add_child(mgtk_tree_t *parent, mgtk_tree_t *child);
/*------------------------------------------------------
 * Pre  : 
 * Post : Adds an existing child tree to an existing tree
 ------------------------------------------------------*/

void mgtk_event_tree_add_new_child(mgtk_tree_t *parent, int id, const char *label);
/*------------------------------------------------------
 * Pre  : 
 * Post : Adds a new child tree to an existing tree
 ------------------------------------------------------*/

void mgtk_event_tree_delete(mgtk_tree_t *tree);
/*------------------------------------------------------
 * Pre  : 
 * Post : Deallocates <tree> and all it's children
 ------------------------------------------------------*/

void mgtk_event_update_tree(unsigned int id, mgtk_tree_t *tree);
/*------------------------------------------------------
 * Pre  : <id> is valid mgtk event
 *        <tree> is a valid mgtk tree model 
 *
 * Post : This signals to update the view(s) for event <id> 
 *        using <tree> as the updated/new data model
 ------------------------------------------------------*/


typedef struct mgtk_mouse_event_s
{
	unsigned char type;       /* Click = 1, Motion = 2, Reserved */

	// Clicked
	int button;      /* mgtk_mouse_key_t */
	int state;       /* mgtk_mouse_key_state_t */
	int modifiers;   /* mgtk_key_modifers_t */
	int x;
	int y;

	// Motion
	int x_delta;
	int y_delta;

} mgtk_mouse_event_t;


#endif

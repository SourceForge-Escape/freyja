/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libmgtk
 * Author  : Mongoose
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (c) 2000-2004 Mongoose
 * Comments: This is the mlisp bridge to gtk+ widgets 
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.10.27:
 * Mongoose - Major API clean up, documentation update
 *
 * 2000.09.16:
 * Mongoose - Created
 ==========================================================================*/

#include <string.h>

#include <gtk/gtk.h>

#include <mstl/Map.h>
#include <mstl/SystemIO.h>

#include "Resource.h"

#include "mgtk_interface.h"
#include "mgtk_callbacks.h"
#include "mgtk_resource.h"
#include "mgtk_events.h"

#define ACCEL_SUPPORT_ON
#define EXTEND_FILE_TEST

// Arg list types for Resource use
#define ARG_GTK_WIDGET           64
#define ARG_GTK_BOX_WIDGET       128
#define ARG_GTK_WINDOW           256
#define ARG_GTK_MENU_WIDGET      1024
#define ARG_GTK_NOTEBOOK         2048
#define ARG_GTK_TOOLBOX_WIDGET   4096


using namespace mstl;

Map<long, GtkWidget *> gFileDialogWidgetMap;
Map<long, GtkWidget *> gFileDialogPatternWidgetMap;


// Could use mlisp to replace this, but let's not go there for this
// prototype merge of linux/win32/osx
#if defined (WIN32) || (MACOSX)
void (*win32_mgtk_callback_get_image_data_rgb24)(const char *, unsigned char **, int *, int *) = NULL;
void (*win32_mgtk_handle_color)(int, float, float, float, float) = NULL;
void (*win32_mgtk_handle_application_window_close)() = NULL;
void (*win32_mgtk_handle_command)(int) = NULL;
void (*win32_mgtk_handle_command2i)(int, int) = NULL;
void (*win32_mgtk_handle_event1u)(int, unsigned int) = NULL;
void (*win32_mgtk_handle_event1f)(int, float) = NULL;
void (*win32_mgtk_handle_file_dialog_selection)(int, char *) = NULL;
void (*win32_mgtk_handle_gldisplay)() = NULL;
void (*win32_mgtk_handle_glresize)(unsigned int, unsigned int) = NULL;
void (*win32_mgtk_handle_key_press)(int, int) = NULL;
void (*win32_mgtk_handle_motion)(int, int) = NULL;
void (*win32_mgtk_handle_mouse)(int, int, int, int, int) = NULL;
void (*win32_mgtk_handle_resource_start)() = NULL;
void (*win32_mgtk_handle_slider1u)(int event, unsigned int) = NULL;
void (*win32_mgtk_handle_text)(int, char *) = NULL;
void (*win32_mgtk_print)(const char*, ...) = NULL;
void (*win32_mgtk_get_pixmap_filename)(char *, unsigned int, char *) = NULL;
char *(*win32_mgtk_rc_map)(char *) = NULL;

void mgtk_win32_import(char *symbol, void *func)
{
	if (strncmp("win32_mgtk_callback_get_image_data_rgb24", symbol, 41) == 0)
	{
		win32_mgtk_callback_get_image_data_rgb24 = (void (*)(const char *, unsigned char **, int *, int *))func;
	}
	else if (strncmp("win32_mgtk_handle_color", symbol, 26) == 0)
	{
		win32_mgtk_handle_color = (void (*)(int, float, float, float, float))func;
	}
	else if (strncmp("win32_mgtk_handle_application_window_close", symbol, 26) == 0)
	{
		win32_mgtk_handle_application_window_close = (void (*)())func;
	}
	else if (strncmp("win32_mgtk_handle_command", symbol, 26) == 0)
	{
		win32_mgtk_handle_command = (void (*)(int))func;
	}
	else if (strncmp("win32_mgtk_handle_command2i", symbol, 28) == 0)
	{
		win32_mgtk_handle_command2i = (void (*)(int, int))func;
	}
	else if (strncmp("win32_mgtk_handle_event1u", symbol, 26) == 0)
	{
		win32_mgtk_handle_event1u = (void (*)(int, unsigned int))func;
	}
	else if (strncmp("win32_mgtk_handle_event1f", symbol, 26) == 0)
	{
		win32_mgtk_handle_event1f = (void (*)(int, float))func;
	}
	else if (strncmp("win32_mgtk_handle_file_dialog_selection", symbol, 40) == 0)
	{
		win32_mgtk_handle_file_dialog_selection = (void (*)(int, char*))func;
	}
	else if (strncmp("win32_mgtk_handle_gldisplay", symbol, 28) == 0)
	{
		win32_mgtk_handle_gldisplay = (void (*)())func;
	}
	else if (strncmp("win32_mgtk_handle_glresize", symbol, 27) == 0)
	{
		win32_mgtk_handle_glresize = (void (*)(unsigned int,unsigned int))func;
	}
	else if (strncmp("win32_mgtk_handle_key_press", symbol, 28) == 0)
	{
		win32_mgtk_handle_key_press = (void (*)(int,int))func;
	}
	else if (strncmp("win32_mgtk_handle_motion", symbol, 25) == 0)
	{
		win32_mgtk_handle_motion = (void (*)(int,int))func;
	}
	else if (strncmp("win32_mgtk_handle_mouse", symbol, 24) == 0)
	{
		win32_mgtk_handle_mouse = (void (*)(int, int, int, int, int))func;
	}
	else if (strncmp("win32_mgtk_handle_resource_start", symbol, 33) == 0)
	{
		win32_mgtk_handle_resource_start = (void (*)())func;
	}
	else if (strncmp("win32_mgtk_handle_slider1u", symbol, 27) == 0)
	{
		win32_mgtk_handle_slider1u = (void (*)(int, unsigned int))func;
	}
	else if (strncmp("win32_mgtk_handle_text", symbol, 17) == 0)
	{
		win32_mgtk_handle_text = (void (*)(int, char*))func;
	}
	else if (strncmp("win32_mgtk_print", symbol, 17) == 0)
	{
		win32_mgtk_print = (void (*)(const char*, ...))func;
	}
	else if (strncmp("win32_mgtk_get_pixmap_filename", symbol, 31) == 0)
	{
		win32_mgtk_get_pixmap_filename = (void (*)(char *, unsigned int, char *))func;
	}
	else if (strncmp("win32_mgtk_rc_map", symbol, 18) == 0)
	{
		win32_mgtk_rc_map = (char *(*)(char *))func;
	}
	else
	{
		mgtk_print("mgtk_win32_import> No binding for %s:%p\n", symbol, func);
	}
}


void mgtk_callback_get_image_data_rgb24(const char *filename, 
										unsigned char **image, 
										int *width, int *height)
{
	if (mgtk_callback_get_image_data_rgb24 != NULL)
	{
		(*win32_mgtk_callback_get_image_data_rgb24)(filename, image, width, height);
	}
}


void mgtk_handle_application_window_close()
{
	if (win32_mgtk_handle_application_window_close != NULL)
	{
		(*win32_mgtk_handle_application_window_close)();
	}
}

void mgtk_handle_color(int id, float r, float g, float b, float a)
{
	if (win32_mgtk_handle_color != NULL)
	{
		(*win32_mgtk_handle_color)(id, r, g, b, a);
	}
}


void mgtk_handle_command(int command)
{
	if (win32_mgtk_handle_command != NULL)
	{
		(*win32_mgtk_handle_command)(command);
	}
}


void mgtk_handle_command2i(int event, int command)
{
	if (win32_mgtk_handle_command2i != NULL)
	{
		(*win32_mgtk_handle_command2i)(event, command);
	}
}


void mgtk_handle_event1u(int event, unsigned int value)
{
	if (win32_mgtk_handle_event1u != NULL)
	{
		(*win32_mgtk_handle_event1u)(event, value);
	}
}


void mgtk_handle_event1f(int event, float value)
{
	if (win32_mgtk_handle_event1f != NULL)
	{
		(*win32_mgtk_handle_event1f)(event, value);
	}
}

void mgtk_handle_file_dialog_selection(int event, char *filename)
{
	if (win32_mgtk_handle_file_dialog_selection != NULL)
	{
		(*win32_mgtk_handle_file_dialog_selection)(event, filename);
	}
}

void mgtk_handle_gldisplay()
{
	if (win32_mgtk_handle_gldisplay != NULL)
	{
		(*win32_mgtk_handle_gldisplay)();
	}
}

void mgtk_handle_glresize(unsigned int width, unsigned int height)
{
	if (win32_mgtk_handle_glresize != NULL)
	{
		(*win32_mgtk_handle_glresize)(width, height);
	}
}

void mgtk_handle_key_press(int key, int mod)
{
	if (win32_mgtk_handle_key_press != NULL)
	{
		(*win32_mgtk_handle_key_press)(key, mod);
	}
}


void mgtk_handle_motion(int x_delta, int y_delta)
{
	if (win32_mgtk_handle_motion != NULL)
	{
		(*win32_mgtk_handle_motion)(x_delta, y_delta);
	}
}


void mgtk_handle_mouse(int button, int state, int mod, int x, int y)
{
	if (win32_mgtk_handle_mouse != NULL)
	{
		(*win32_mgtk_handle_mouse)(button, state, mod, x, y);
	}
}

void mgtk_handle_resource_start()
{
	if (win32_mgtk_handle_resource_start != NULL)
	{
		(*win32_mgtk_handle_resource_start)();
	}
}

void mgtk_handle_slider1u(int event, unsigned int value)
{
	if (win32_mgtk_handle_slider1u != NULL)
	{
		(*win32_mgtk_handle_slider1u)(event, value);
	}
}

void mgtk_handle_text(int event, char *text)
{
	if (win32_mgtk_handle_text != NULL)
	{
		(*win32_mgtk_handle_text)(event, text);
	}
}

void mgtk_print(char *format, ...)
{
	char buffer[1024];
	va_list args;

	va_start(args, format);
	vsnprintf(buffer, 1024, format, args);
	va_end(args);

	if (win32_mgtk_print != NULL)
	{
		(*win32_mgtk_print)(buffer);
	}
}


void mgtk_get_pixmap_filename(char *dest, unsigned int size, char *icon_name)
{
	if (win32_mgtk_get_pixmap_filename != NULL)
	{
		(*win32_mgtk_get_pixmap_filename)(dest, size, icon_name);
	}
}


char *mgtk_rc_map(char *filename_or_dirname)
{
	if (win32_mgtk_rc_map != NULL)
	{
		return (*win32_mgtk_rc_map)(filename_or_dirname);
	}

	return NULL;
}

#endif


/* Mongoose 2004.10.28, 
 * FIXME This only allows one gobal tree widget and it's crappy to boot */
GtkTreeView *SKELETON_TREEVIEW = 0x0;

/* Mongoose 2004.10.28, 
 * These are the only gobals that should be used here */
//GtkWidget *GTK_FILESELECTION_DROP_DOWN_MENU = 0x0;
GtkWidget *GTK_MAIN_WINDOW = 0x0;
GtkWidget *GTK_GL_AREA_WIDGET = 0x0;
GtkWidget *GTK_STATUS_BAR_WIDGET = 0x0;


GtkWidget *mgtk_get_application_window()
{
	return GTK_MAIN_WINDOW;
}

GtkWidget *mgtk_get_gl_widget()
{
	return GTK_GL_AREA_WIDGET;
}

GtkWidget *mgtk_get_statusbar_widget()
{
	return GTK_STATUS_BAR_WIDGET;
}


GtkWidget *mgtk_get_fileselection_pattern_widget(int event)
{
	return gFileDialogPatternWidgetMap[event];
}


///////////////////////////////////////////////////////////////////////////
// File chooser init
///////////////////////////////////////////////////////////////////////////

void mgtk_filechooser_spawn_event(GtkWidget *widget, gpointer user_data)
{
	GtkWidget *dialog = gFileDialogWidgetMap[GPOINTER_TO_INT(user_data)];

	DEBUG_MSG("%p %i\n", widget, user_data, dialog);

	if (dialog)
		gtk_widget_show_all(dialog);
}

void mgtk_filechooser_close_event(GtkWidget *widget, gpointer user_data)
{
	GtkWidget *dialog = gFileDialogWidgetMap[GPOINTER_TO_INT(user_data)];

	DEBUG_MSG("%p %i %p\n", widget, user_data, dialog);

	if (dialog)
		gtk_widget_hide_all(dialog);
}


GtkWidget *mgtk_link_filechooser_from_rc(int event, char *title, char *option)
{
	GtkWidget *dialog = gFileDialogWidgetMap[event];

	if (dialog)
		return dialog;

	/* Didn't find it created, so create and then link it */
	dialog = mgtk_create_filechooser(event, title);
	gFileDialogWidgetMap.Add(event, dialog);

	/*Hide on calls to close */
	gtk_signal_connect(GTK_OBJECT(dialog), "close",
					   GTK_SIGNAL_FUNC(mgtk_filechooser_close_event), 
					   GINT_TO_POINTER(event));

	/* Hide on window manager calls to close */
	gtk_signal_connect(GTK_OBJECT(dialog), "delete_event",
					   GTK_SIGNAL_FUNC(mgtk_event_filechooser_cancel), 
					   GINT_TO_POINTER(event));


	/* 'Home' directory links */
	char *path = mgtk_rc_map("/");
		
	if (path)
	{
		gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), path);
		gtk_file_chooser_add_shortcut_folder(GTK_FILE_CHOOSER(dialog), path, NULL);
		delete [] path;
	}


	/* Uber string options */
	const char *buffer = option;
	long len = strlen(buffer), state = 0;
	char symbol[64];
	char value[64];

	for (long i = 0, j = 0; i < len; ++i)
	{
		if (j == 63) j = 0;
		
		switch ( state )
		{
		case 0:
			if (buffer[i] == '=')
			{
				state = 1;
				j = 0;
			}
			else
			{
				symbol[j++] = buffer[i];
				symbol[j] = 0;
			}
			break;
			
		case 1:
			if (buffer[i] == ',' || !buffer[i])
			{
				state = j = 0;
				
				if (strcmp(symbol, "mode") == 0)
				{
					if (strcmp(value, "save") == 0)
					{
						gtk_file_chooser_set_action(GTK_FILE_CHOOSER(dialog), 
													GTK_FILE_CHOOSER_ACTION_SAVE);
					}
					else if (strcmp(value, "open") == 0)
					{
						gtk_file_chooser_set_action(GTK_FILE_CHOOSER(dialog), 
													GTK_FILE_CHOOSER_ACTION_OPEN);
					}
					else if (strcmp(value, "select-folder") == 0)
					{
						gtk_file_chooser_set_action(GTK_FILE_CHOOSER(dialog), 
													GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER);
					}
					else if (strcmp(value, "create-folder") == 0)
					{
						gtk_file_chooser_set_action(GTK_FILE_CHOOSER(dialog), 
													GTK_FILE_CHOOSER_ACTION_CREATE_FOLDER);
					}
				}
				else if (strcmp(symbol, "confirm-overwrite") == 0)
				{
					if (strcmp(value, "true") == 0)
					{
#ifndef WIN32 // FIXME: Doesn't seem to have this feature in win32 env build
						gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);
#endif
					}
				}
			}
			else
			{
				value[j++] = buffer[i];
				value[j] = 0;
			}
			break;
		}
	}

	return dialog;
}


void mgtk_callback_free_image_data(guchar *pixels, gpointer data)
{
	if (pixels)
		delete [] (unsigned char *)pixels;
}


#ifndef USE_OLD_FILE_SELECTION_WIDGET
void mgtk_update_filechooser_preview(GtkFileChooser *file_chooser, 
									 gpointer data)
{
	GtkWidget *preview = GTK_WIDGET(data);
	char *filename = gtk_file_chooser_get_preview_filename(file_chooser);

	/*
	pixbuf = gdk_pixbuf_new_from_data(const guchar *image,
									  GdkColorspace colorspace,
									  gboolean has_alpha,
									  int bits_per_sample,
									  int width,
									  int height,
									  int rowstride,
									  GdkPixbufDestroyNotify destroy_fn,
									  gpointer destroy_fn_data);
	*/
	unsigned char *image;
	int width, height;

	mgtk_callback_get_image_data_rgb24(filename, &image, &width, &height);
	GdkPixbuf *pixbuf = gdk_pixbuf_new_from_data(image, 
												 GDK_COLORSPACE_RGB, FALSE, 8,
												 width, height, width*3, 
												 mgtk_callback_free_image_data,
												 data);
	gboolean have_preview = (pixbuf != NULL);
	g_free(filename);

	gtk_image_set_from_pixbuf(GTK_IMAGE(preview), pixbuf);

	if (pixbuf)
	{
		gdk_pixbuf_unref(pixbuf);
	}

	gtk_file_chooser_set_preview_widget_active(file_chooser, have_preview);
}
#endif


GtkWidget *mgtk_get_fileselection_widget(int event)
{
	GtkWidget *file = gFileDialogWidgetMap[event];
	char *path;

	if (!file)
	{
#ifdef USE_OLD_FILE_SELECTION_WIDGET
		file = mgtk_create_fileselection(event, "Select file");
#else
		file = mgtk_create_filechooser(event, "Select file");
#endif

		gFileDialogWidgetMap.Add(event, file);

		path = mgtk_rc_map("/");
		
		if (path)
		{
#ifdef USE_OLD_FILE_SELECTION_WIDGET
			gtk_file_selection_set_filename(GTK_FILE_SELECTION(file), path);
#else
			gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(file), path);
			gtk_file_chooser_add_shortcut_folder(GTK_FILE_CHOOSER(file), path,
												 NULL);
			gtk_file_chooser_set_action(GTK_FILE_CHOOSER(file), GTK_FILE_CHOOSER_ACTION_SAVE);
#endif
			delete [] path;
		}

#ifdef EXTEND_FILE_TEST
#   ifdef USE_OLD_FILE_SELECTION_WIDGET
		GtkWidget *homeButton;
		GtkWidget *vbox = GTK_FILE_SELECTION(file)->main_vbox;

		GtkWidget *hbox = mgtk_create_hbox(vbox, "hbox", 0, 0, 0, 0, 0);

		homeButton = gtk_button_new_with_label("Home");
		gtk_widget_ref(homeButton);
		gtk_object_set_data_full(GTK_OBJECT(hbox), 
								 "home_button", homeButton,
								 (GtkDestroyNotify)gtk_widget_unref);
		gtk_widget_show(homeButton);
		gtk_box_pack_start(GTK_BOX(hbox), homeButton, TRUE, TRUE, FALSE);
		
		gtk_signal_connect(GTK_OBJECT(homeButton), "pressed",
						   GTK_SIGNAL_FUNC(mgtk_event_fileselection_homedir), 
						   GINT_TO_POINTER(file));

		GtkWidget *optionmenu = gtk_option_menu_new();
		gtk_widget_ref(optionmenu);
		gtk_object_set_data_full(GTK_OBJECT(hbox), "optionmenu", optionmenu,
								 (GtkDestroyNotify)gtk_widget_unref);
		gtk_widget_show(optionmenu);
		gtk_box_pack_start(GTK_BOX(hbox), optionmenu, TRUE, TRUE, 0);

		GtkWidget *optionmenu_menu = gtk_menu_new();
		gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu), optionmenu_menu);
		//mgtk_event_subscribe_gtk_widget(0, optionmenu);
		GTK_FILESELECTION_DROP_DOWN_MENU = optionmenu_menu;

		GtkWidget *sep = gtk_menu_item_new();
		gtk_menu_append(GTK_MENU(GTK_FILESELECTION_DROP_DOWN_MENU), sep);
		gtk_widget_show(sep);
		gtk_widget_set_usize(sep, 164, 1);
#   else
		//GtkWidget *vbox = GTK_DIALOG(file)->vbox;
		GtkWidget *preview;

		preview = gtk_image_new();
		gtk_file_chooser_set_preview_widget(GTK_FILE_CHOOSER(file), preview);
		g_signal_connect(file, "update-preview",
						 G_CALLBACK (mgtk_update_filechooser_preview), preview);
#   endif
#endif
	}

	return file;
}



/////////////////////////////////////////////////////////////


void mgtk_application_window_fullscreen()
{
	if (GTK_MAIN_WINDOW)
		gtk_window_fullscreen(GTK_WINDOW(GTK_MAIN_WINDOW));
}

void mgtk_application_window_unfullscreen()
{
	if (GTK_MAIN_WINDOW)
		gtk_window_unfullscreen(GTK_WINDOW(GTK_MAIN_WINDOW));
}

void mgtk_application_window_move(int x, int y)
{
	if (GTK_MAIN_WINDOW)
		gtk_window_move(GTK_WINDOW(GTK_MAIN_WINDOW), x, y);
}

void mgtk_application_window_resize(int width, int height)
{
	if (GTK_MAIN_WINDOW)
		gtk_window_resize(GTK_WINDOW(GTK_MAIN_WINDOW), width, height);
}


void mgtk_application_window_role(char *role)
{
	if (GTK_MAIN_WINDOW)
		gtk_window_set_role(GTK_WINDOW(GTK_MAIN_WINDOW), role);
}


////////////////////////////////////////////////////////////////////


void rc_assertion_error(char *widget_name, char *error)
{
	printf("%s> ERROR: Assertion '%s' failed\n", widget_name, error);

#ifdef FATAL_ASSERTION_ERROR
	event_shutdown();
#endif
}


void *rc_gtk_event_func(int event)
{
	void (*agtk_event)(GtkWidget *, void *);


	switch (event)
	{
	default:
		agtk_event = mgtk_event_command;
	}

	return (void *)agtk_event;
}


#ifdef FIXME
int rc_assert_type(arg_list_t *arg, char *widget_name, char *arg_name, int type)
{
	arg_enforce_type(&arg, type);

	if (!arg)
	{
		switch (type)
		{
		case FLOAT:
			break;
		case ARG_GTK_WIDGET:
			rc_assertion_error(child_name, "== ARG_GTK_WIDGET");
			break;
		default:
			printf("rc_assert_type> ERROR doesn't handle %i %s:%i", 
					 type, __FILE__, __LINE__);
			break;
		}

		return 2;
	}

	return 0;
}
#endif


////////////////////////////////////////////////////////////////

void mgtk_event_close_window(GtkWidget *widget, gpointer user_data)
{
	mgtk_handle_application_window_close();
}


arg_list_t *mgtk_rc_window(arg_list_t *container)
{
	arg_list_t *ret, *title, *icon;
	GtkWidget *window;
	char icon_filename[1024];


	symbol_enforce_type(&title, CSTRING);
	symbol_enforce_type(&icon, CSTRING);
	ret = NULL;

	if (!title && !icon)
	{
		rc_assertion_error("window", "title != CSTRING");
		rc_assertion_error("window", "icon != CSTRING");
		return NULL;
	}

	mgtk_get_pixmap_filename(icon_filename, 1024, get_string(icon));
	icon_filename[1023] = 0;

	window = mgtk_create_window(BUILD_NAME, get_string(title), icon_filename);

	if (!GTK_MAIN_WINDOW)
	{
		GTK_MAIN_WINDOW = window;
		gtk_window_set_role(GTK_WINDOW(window), get_string(title));
	}

	gtk_widget_show(window);

	g_signal_connect(GTK_OBJECT(window), "delete_event",
					 G_CALLBACK(mgtk_event_close_window),
					 GINT_TO_POINTER(window));

	new_adt(&ret, ARG_GTK_WINDOW, (void *)window);

	return ret;
}


arg_list_t *mgtk_rc_gl_widget(arg_list_t *box)
{
	arg_list_t *width, *height, *ret;
	GtkWidget *gl, *table, *hbox;


	if (!box)
	{
		rc_assertion_error("gl_widget", "box != NULL");
		return NULL;
	}

	arg_enforce_type(&box,  ARG_GTK_BOX_WIDGET);

	if (!box)
	{
		rc_assertion_error("gl_widget", "box == ARG_GTK_BOX_WIDGET");
		return NULL;
	}

	symbol_enforce_type(&width, INT);
	symbol_enforce_type(&height, INT);
	ret = NULL;

	if (!width || !height)
	{
		if (!width)
			rc_assertion_error("gl_widget", "width == INT");
		
		if (!height)
			rc_assertion_error("gl_widget", "height == INT");

		return NULL;
	}

#if defined (HAVE_GTKGLAREA) || (HAVE_GTKGLEXT)
	/* Gtk GL Area widget */
	gl = mgtk_create_glarea(get_int(width), get_int(height));

	if (!gl)
	{
		mgtk_print("!ERROR: OpenGL display not supported by this system?\n");
	}
	else
	{
		mgtk_print("@Gtk+ GL context started...");
	}

	GTK_GL_AREA_WIDGET = gl;

//#   ifndef HAVE_GTKGLEXT
	mgtk_glarea_window_state_t *state;

	state = (mgtk_glarea_window_state_t*)gtk_object_get_data(GTK_OBJECT(gl),
															 "gl_window_state");
	state->appbar = NULL;

#   ifndef HAVE_GTKGLEXT
	gtk_signal_connect(GTK_OBJECT(mgtk_get_gl_widget()), "key_press_event",
					   GTK_SIGNAL_FUNC(mgtk_event_key_press), NULL);
	gtk_signal_connect(GTK_OBJECT(mgtk_get_gl_widget()), "key_release_event",
					   GTK_SIGNAL_FUNC(mgtk_event_key_release), NULL);
	gtk_signal_connect(GTK_OBJECT(mgtk_get_gl_widget()), "destroy",
					   GTK_SIGNAL_FUNC(mgtk_destroy_window), NULL);
#   endif

#else
#   warning "WARNING No gtkglarea widget support in this build"
	gl = label_create(GTK_WIDGET(box->data), "lab1", "No gtkglarea widget support in this build\nAdd -DHAVE_GTKGL to CONFIGURE_CFLAGS in Makefile to force build to use gtkglarea widget", 0.5, 0.5);
	gtk_widget_set_usize(gl, get_int(width), get_int(height));
#endif
	
	/* Editing window */
	//hbox = packed_hbox_create(GTK_WIDGET(box->data), "hb1", 0, 0, 1, 1, 1);
	hbox = mgtk_create_vbox(GTK_WIDGET(box->data), "hb1", 0, 0, 1, 1, 1);
	
	table = gtk_table_new(3, 3, FALSE);
	gtk_widget_ref(table);
	gtk_object_set_data_full(GTK_OBJECT(hbox), "table", table,
							 (GtkDestroyNotify)gtk_widget_unref);
	gtk_widget_show(table);
	gtk_box_pack_start(GTK_BOX(hbox), table, TRUE, TRUE, 0);
	
	gtk_table_attach(GTK_TABLE(table), GTK_WIDGET(gl), 1, 2, 1, 2,
					 (GtkAttachOptions)(GTK_EXPAND | GTK_FILL),
					 (GtkAttachOptions)(GTK_EXPAND | GTK_FILL), 0, 0);
	
	gtk_widget_show(gl);
	

	// Mongoose 2002.07.07, Add status bar here as simple label for now
	//GTK_STATUS_BAR_HACK = label_create(hbox, "foo", "status bar", 0.0, 0.0);

	GTK_STATUS_BAR_WIDGET = gtk_statusbar_new();
	gtk_widget_show(GTK_STATUS_BAR_WIDGET);

	gtk_box_pack_start(GTK_BOX(hbox), GTK_STATUS_BAR_WIDGET, 0, 0, 0);
	
	delete_arg(&width);
	delete_arg(&height);

	return ret;
}


arg_list_t *mgtk_rc_toolbar(arg_list_t *box)
{
	arg_list_t *ret;
	GtkWidget *toolbar;


	if (!box)
	{
		rc_assertion_error("toolbar", "container != NULL");
		return NULL;
	}

	arg_enforce_type(&box,  ARG_GTK_BOX_WIDGET);

	if (!box)
	{
		rc_assertion_error("toolbar", "container == ARG_GTK_BOX_WIDGET");
		return NULL;
	}

	ret = NULL;
 
	toolbar = mgtk_create_toolbar((GtkWidget *)box->data);
	gtk_widget_ref(toolbar);
	gtk_object_set_data_full(GTK_OBJECT((GtkWidget *)box->data), "tool", 
							 toolbar,
							 (GtkDestroyNotify)gtk_widget_unref);
	gtk_widget_show(toolbar);

	new_adt(&ret, ARG_GTK_TOOLBOX_WIDGET, (void *)toolbar);

	return ret;
}


arg_list_t *mgtk_rc_toolbar_box(arg_list_t *box)
{
	arg_list_t *ret;
	GtkWidget *vbox;


	if (!box)
	{
		rc_assertion_error("toolbar_box", "container != NULL");
		return NULL;
	}

	arg_enforce_type(&box,  ARG_GTK_TOOLBOX_WIDGET);

	if (!box)
	{
		rc_assertion_error("toolbar_box", "container == ARG_GTK_TOOLBOX_WIDGET");
		return NULL;
	}


	vbox = gtk_vbox_new(FALSE, 0);
	gtk_widget_ref(vbox);
	gtk_object_set_data_full(GTK_OBJECT((GtkWidget *)box->data), "box", vbox,
									 (GtkDestroyNotify)gtk_widget_unref);
	gtk_widget_show(vbox);

	gtk_toolbar_append_widget(GTK_TOOLBAR((GtkWidget *)box->data), 
									  vbox, NULL, NULL);

	new_adt(&ret, ARG_GTK_BOX_WIDGET, (void *)vbox);

	return ret;
}


arg_list_t *mgtk_rc_toolbar_togglebutton(arg_list_t *box)
{
	arg_list_t *ret, *icon, *label, *help, *toggled, *event, *cmd;
	GtkWidget *toggle_button;
	void *event_func;
	int event_cmd;


	if (!box)
	{
		rc_assertion_error("toolbar_togglebutton", "container != NULL");
		return NULL;
	}

	arg_enforce_type(&box,  ARG_GTK_TOOLBOX_WIDGET);

	if (!box)
	{
		rc_assertion_error("toolbar_togglebutton", 
								 "container == ARG_GTK_TOOLBOX_WIDGET");
		return NULL;
	}

	symbol_enforce_type(&toggled, INT);
	symbol_enforce_type(&icon, CSTRING);
	symbol_enforce_type(&label, CSTRING);
	symbol_enforce_type(&help, CSTRING);
	symbol_enforce_type(&event, INT);
	symbol_enforce_type(&cmd, INT);
	ret = NULL;

	if (!icon || !label || !help || !toggled || !event || !cmd)
	{
		if (!icon)
			rc_assertion_error("togglebutton", "icon == CSTRING");

		if (!label)
			rc_assertion_error("togglebutton", "label == CSTRING");

		if (!help)
			rc_assertion_error("togglebutton", "help == CSTRING");

		if (!toggled)
			rc_assertion_error("togglebutton", "toggled == INT");

		if (!event)
			rc_assertion_error("togglebutton", "event == INT");

		if (!cmd)
			rc_assertion_error("togglebutton", "cmd == INT");
	}
	else
	{
		event_func = rc_gtk_event_func(get_int(event));
		event_cmd = get_int(cmd);

		// FIXME: Should be removed and replaced with lisp function
		char icon_filename[1024];

		if (!strncmp(get_string(icon), "gtk", 3))
		{
			strncpy(icon_filename, get_string(icon), 1024);
			icon_filename[1023] = 0;
		}
		else
		{
			mgtk_get_pixmap_filename(icon_filename, 1024, get_string(icon));
			icon_filename[1023] = 0;
		}

		toggle_button =
		mgtk_create_toolbar_toogle_button((GtkWidget *)box->data, 
										  get_int(toggled), 
										  icon_filename, get_string(label),
										  get_string(help),
										  event_func, event_cmd);
		
		new_adt(&ret, ARG_GTK_WIDGET, (void *)toggle_button);
	}

	delete_arg(&toggled);
	delete_arg(&icon);
	delete_arg(&label);
	delete_arg(&help);
	delete_arg(&event);
	delete_arg(&cmd);

	return ret;
}


arg_list_t *mgtk_rc_toolbar_button(arg_list_t *box)
{
	arg_list_t *ret, *icon, *label, *help, *event, *cmd;
	GtkWidget *button;
	void *event_func;
	int event_cmd;


	if (!box)
	{
		rc_assertion_error("toolbar_button", "container != NULL");
		return NULL;
	}

	arg_enforce_type(&box,  ARG_GTK_TOOLBOX_WIDGET);

	if (!box)
	{
		rc_assertion_error("toolbar_button", 
						   "container == ARG_GTK_TOOLBOX_WIDGET");
		return NULL;
	}

	symbol_enforce_type(&icon, CSTRING);
	symbol_enforce_type(&label, CSTRING);
	symbol_enforce_type(&help, CSTRING);
	symbol_enforce_type(&event, INT);
	symbol_enforce_type(&cmd, INT);
	ret = NULL;

	if (!icon || !label || !help || !event || !cmd)
	{
		if (!icon)
			rc_assertion_error("togglebutton", "icon == CSTRING");

		if (!label)
			rc_assertion_error("togglebutton", "label == CSTRING");

		if (!help)
			rc_assertion_error("togglebutton", "help == CSTRING");

		if (!event)
			rc_assertion_error("togglebutton", "event == INT");

		if (!cmd)
			rc_assertion_error("togglebutton", "cmd == INT");
	}
	else
	{
		event_func = rc_gtk_event_func(get_int(event));
		event_cmd = get_int(cmd);

		// FIXME: Should be removed and replaced with lisp function
		char icon_filename[1024];

		if (!strncmp(get_string(icon), "gtk", 3))
		{
			strncpy(icon_filename, get_string(icon), 1024);
			icon_filename[1023] = 0;
		}
		else
		{
			mgtk_get_pixmap_filename(icon_filename, 1024, get_string(icon));
			icon_filename[1023] = 0;
		}

		button =
		mgtk_create_toolbar_button((GtkWidget *)box->data, icon_filename, 
								   get_string(label), get_string(help),
								   event_func, event_cmd);
		
		new_adt(&ret, ARG_GTK_WIDGET, (void *)button);
	}

	delete_arg(&icon);
	delete_arg(&label);
	delete_arg(&help);
	delete_arg(&event);
	delete_arg(&cmd);

	return ret;
}



// (filechoosertoolbar_button "icon" "name" "tooltip" "Title..." eEvent "your_mgtk_rc_pattern_callback") 
arg_list_t *mgtk_rc_filechoosertoolbar_button(arg_list_t *box)
{
	arg_list_t *ret, *icon, *label, *help, *title, *event, *options;
	GtkWidget *button;

	if (!box)
	{
		rc_assertion_error("toolbar_button", "container != NULL");
		return NULL;
	}

	arg_enforce_type(&box,  ARG_GTK_TOOLBOX_WIDGET);

	if (!box)
	{
		rc_assertion_error("toolbar_button", 
						   "container == ARG_GTK_TOOLBOX_WIDGET");
		return NULL;
	}

	symbol_enforce_type(&icon, CSTRING);
	symbol_enforce_type(&label, CSTRING);
	symbol_enforce_type(&help, CSTRING);
	symbol_enforce_type(&title, CSTRING);
	symbol_enforce_type(&event, INT);
	symbol_enforce_type(&options, CSTRING);
	ret = NULL;

	if (!icon || !label || !help || !title || !event || !options)
	{
		if (!icon)
			rc_assertion_error("togglebutton", "icon == CSTRING");

		if (!label)
			rc_assertion_error("togglebutton", "label == CSTRING");

		if (!help)
			rc_assertion_error("togglebutton", "help == CSTRING");

		if (!title)
			rc_assertion_error("togglebutton", "title == CSTRING");

		if (!event)
			rc_assertion_error("togglebutton", "event == INT");

		if (!options)
			rc_assertion_error("togglebutton", "options == CSTRING");
	}
	else
	{
		// FIXME: Should be removed and replaced with lisp function
		char icon_filename[1024];

		if (!strncmp(get_string(icon), "gtk", 3))
		{
			strncpy(icon_filename, get_string(icon), 1024);
			icon_filename[1023] = 0;
		}
		else
		{
			mgtk_get_pixmap_filename(icon_filename, 1024, get_string(icon));
			icon_filename[1023] = 0;
		}

		mgtk_link_filechooser_from_rc(get_int(event), get_string(title), get_string(options));

		button =
		mgtk_create_toolbar_button((GtkWidget *)box->data, icon_filename, 
								   get_string(label), get_string(help),
								   NULL, GPOINTER_TO_INT(get_int(event)));
		

		gtk_signal_connect(GTK_OBJECT(button), "pressed",
						   GTK_SIGNAL_FUNC(mgtk_filechooser_spawn_event), 
						   GINT_TO_POINTER(get_int(event)));

		new_adt(&ret, ARG_GTK_WIDGET, (void *)button);
	}

	delete_arg(&icon);
	delete_arg(&label);
	delete_arg(&help);
	delete_arg(&title);
	delete_arg(&event);
	delete_arg(&options);

	return ret;
}


void mgtk_destroy_notebook(GtkWidget *widget)
{
	mgtk_notebook_eventmap_t *emap;


	emap = (mgtk_notebook_eventmap_t*)gtk_object_get_data(GTK_OBJECT(widget), 
														  "notebook_eventmap");

	if (emap) 
	{
		if (emap->events)
			delete [] emap->events;

		delete emap;
	}
}


arg_list_t *mgtk_rc_notebook(arg_list_t *box)
{
	arg_list_t *ret, *width, *height, *event;
	GtkWidget *notebook;


	if (!box)
	{
		rc_assertion_error("notebook", "box != NULL");
		return NULL;
	}

	arg_enforce_type(&box,  ARG_GTK_BOX_WIDGET);

	if (!box)
	{
		rc_assertion_error("notebook", "notebook == ARG_GTK_BOX_WIDGET");
		return NULL;
	}

	symbol_enforce_type(&width, INT);
	symbol_enforce_type(&height, INT);
	symbol_enforce_type(&event, INT);
	ret = NULL;

	if (!width || !height || !event)
	{
		if (!width)
			rc_assertion_error("notebook", "width == INT");

		if (!height)
			rc_assertion_error("notebook", "height == INT");

		if (!event)
			rc_assertion_error("notebook", "event == INT");
	}
	else
	{
#ifdef FORCE_HANDLEBOX_ON_NOTEBOOKS
		GtkWidget *handlebox = gtk_handle_box_new();
		gtk_widget_show(handlebox);
		gtk_box_pack_start(GTK_BOX((GtkWidget *)box->data), 
						   handlebox, FALSE, TRUE, 0);
		gtk_handle_box_set_handle_position(GTK_HANDLE_BOX(handlebox), 
										   GTK_POS_LEFT); // TOP
#endif

		// Notebook widget
		notebook = gtk_notebook_new();
		gtk_widget_ref(notebook);
		gtk_object_set_data_full(GTK_OBJECT((GtkWidget *)box->data), 
								 "nb1", notebook,
								 (GtkDestroyNotify)gtk_widget_unref);
		gtk_widget_show(notebook);
		  
		// Options
		if (get_int(width) && get_int(height))
		{
			gtk_widget_set_usize(notebook, get_int(width), get_int(height));
		}

		gtk_notebook_set_scrollable(GTK_NOTEBOOK(notebook), TRUE);
		
		/* Event signal */
		gtk_signal_connect(GTK_OBJECT(notebook), "switch_page",
						   GTK_SIGNAL_FUNC(mgtk_event_notebook_switch_page),
						   GINT_TO_POINTER(get_int(event)));

		/* Hook up vector data to map tab page ids to events */
		mgtk_notebook_eventmap_t *emap = new mgtk_notebook_eventmap_t;
		emap->count = 0;
		emap->events = 0x0;
		gtk_object_set_data(GTK_OBJECT(notebook), "notebook_eventmap", emap);
		gtk_signal_connect(GTK_OBJECT(notebook), "destroy",
						   GTK_SIGNAL_FUNC(mgtk_destroy_notebook), NULL);

		/* Add to container */
#ifdef FORCE_HANDLEBOX_ON_NOTEBOOKS
		gtk_container_add(GTK_CONTAINER(handlebox), notebook);
		gtk_box_pack_start(GTK_BOX((GtkWidget *)box->data), notebook,
						   TRUE, TRUE, FALSE);
#else
		gtk_container_add(GTK_CONTAINER(box->data), notebook);
#endif

		new_adt(&ret, ARG_GTK_NOTEBOOK, (void *)notebook);
	}

	delete_arg(&width);
	delete_arg(&height);
	delete_arg(&event);

	return ret;
}


arg_list_t *mgtk_rc_tab(arg_list_t *notebook)
{
	arg_list_t *label, *event, *ret;
	GtkWidget *item, *vbox;
	mgtk_notebook_eventmap_t *emap;
	int *newEvents;
	unsigned int i;


	if (!notebook)
	{
		rc_assertion_error("tab", "notebook != NULL");
		return NULL;
	}

	arg_enforce_type(&notebook,  ARG_GTK_NOTEBOOK);

	if (!notebook)
	{
		rc_assertion_error("tab", "notebook == ARG_GTK_NOTEBOOK");
		return NULL;
	}

	symbol_enforce_type(&label, CSTRING);
	symbol_enforce_type(&event, INT);
	ret = NULL;

	if (!label || !event)
	{
		if (!label)
			rc_assertion_error("tab", "label == CSTRING");

		if (!event)
			rc_assertion_error("tab", "event == INT");
	}
	else
	{
		vbox = gtk_vbox_new(FALSE, 0);
		

		emap = (mgtk_notebook_eventmap_t*)gtk_object_get_data(GTK_OBJECT(notebook->data), 
															  "notebook_eventmap");
		if (!emap)
		{
			mgtk_print("mgtk_rc_tab> ERROR invalid notebook event system");
			delete_arg(&label);
			delete_arg(&event);
			return ret;
		}

		item = mgtk_create_tab((GtkWidget *)notebook->data, 
							   get_string(label), 
							   vbox,	
							   get_string(label), 
							   emap->count);

		/* Simple vector to map events */
		emap->count++;
		newEvents = new int[emap->count];

		if (emap->events)
		{
			for (i = 0; i < emap->count; ++i)
				newEvents[i] = emap->events[i];

			delete [] emap->events;
		}

		newEvents[emap->count-1] = get_int(event);
		emap->events = newEvents;

		new_adt(&ret, ARG_GTK_BOX_WIDGET, (void *)vbox);
	}

	delete_arg(&label);
	delete_arg(&event);

	return ret;
}


arg_list_t *mgtk_rc_vbox(arg_list_t *box)
{	
	arg_list_t *homogeneous, *spacing, *expand, *fill, *pading, *ret;
	GtkWidget *vbox;


	if (!box)
	{
		rc_assertion_error("vbox", "container != NULL");
		return NULL;
	}

	arg_enforce_type(&box,  ARG_GTK_BOX_WIDGET | ARG_GTK_WINDOW);

	if (!box)
	{
		rc_assertion_error("vbox", "container == ARG_GTK_BOX_WIDGET | ARG_GTK_WINDOW");
		return NULL;
	}

	symbol_enforce_type(&homogeneous, INT);
	symbol_enforce_type(&spacing, INT); 
	symbol_enforce_type(&expand, INT);
	symbol_enforce_type(&fill, INT);
	symbol_enforce_type(&pading, INT);
	ret = NULL;

	if (!homogeneous || !spacing || !expand || !fill || !pading)
	{
		if (!homogeneous)
			rc_assertion_error("vbox", "homogeneous == INT");

		if (!spacing)
			rc_assertion_error("vbox", "spacing == INT");

		if (!expand)
			rc_assertion_error("vbox", "expand == INT");

		if (!fill)
			rc_assertion_error("vbox", "fill == INT");

		if (!pading)
			rc_assertion_error("vbox", "pading == INT");
	}
	else
	{
		vbox = mgtk_create_vbox((GtkWidget *)box->data,
								"vbox", 
								get_int(homogeneous),
								get_int(spacing),
								get_int(expand),
								get_int(fill),
								get_int(pading));

		if (box->type == ARG_GTK_WINDOW)
		{
			//gtk_box_pack_start(GTK_BOX((GtkWidget *)box->data), vbox, TRUE, TRUE, 0);
		}

		new_adt(&ret, ARG_GTK_BOX_WIDGET, (void *)vbox);
	}

	delete_arg(&homogeneous);
	delete_arg(&spacing); 
	delete_arg(&expand);
	delete_arg(&fill);
	delete_arg(&pading);

	return ret;
}


arg_list_t *mgtk_rc_hbox(arg_list_t *box)
{	
	arg_list_t *homogeneous, *spacing, *expand, *fill, *pading, *ret;
	GtkWidget *hbox;


	if (!box)
	{
		rc_assertion_error("hbox", "container != NULL");
		return NULL;
	}

	arg_enforce_type(&box,  ARG_GTK_BOX_WIDGET);

	if (!box)
	{
		rc_assertion_error("hbox", "container == ARG_GTK_BOX_WIDGET");
		return NULL;
	}

	symbol_enforce_type(&homogeneous, INT);
	symbol_enforce_type(&spacing, INT); 
	symbol_enforce_type(&expand, INT);
	symbol_enforce_type(&fill, INT);
	symbol_enforce_type(&pading, INT);
	ret = NULL;

	if (!homogeneous || !spacing || !expand || !fill || !pading)
	{
		if (!homogeneous)
			rc_assertion_error("hbox", "homogeneous == INT");

		if (!spacing)
			rc_assertion_error("hbox", "spacing == INT");

		if (!expand)
			rc_assertion_error("hbox", "expand == INT");

		if (!fill)
			rc_assertion_error("hbox", "fill == INT");

		if (!pading)
			rc_assertion_error("hbox", "pading == INT");
	}
	else
	{
		hbox = mgtk_create_hbox((GtkWidget *)box->data,
								"hbox", 
								get_int(homogeneous),
								get_int(spacing),
								get_int(expand),
								get_int(fill),
								get_int(pading));

		new_adt(&ret, ARG_GTK_BOX_WIDGET, (void *)hbox);
	}

	delete_arg(&homogeneous);
	delete_arg(&spacing); 
	delete_arg(&expand);
	delete_arg(&fill);
	delete_arg(&pading);

	return ret;
}


void mgtk_event_hide_dialog(GtkWidget *widget, gpointer user_data)
{
	gtk_widget_hide_all(widget);
}


arg_list_t *mgtk_rc_dialog(arg_list_t *box)
{	
	arg_list_t *title, *eOpen, *eClose, *ret;
	GtkWidget *widget;


	symbol_enforce_type(&title, CSTRING);
	symbol_enforce_type(&eOpen, INT);
	symbol_enforce_type(&eClose, INT); 
	ret = NULL;

	if (!title || !eOpen || !eClose)
	{
		if (!title)
			rc_assertion_error("dialog", "title == CSTRING");

		if (!eOpen)
			rc_assertion_error("dialog", "eOpen == INT");

		if (!eClose)
			rc_assertion_error("dialog", "eClose == INT");

	}
	else
	{
		widget = mgtk_create_window(get_string(title), "", "");

		gtk_window_set_transient_for(GTK_WINDOW(widget), GTK_WINDOW(GTK_MAIN_WINDOW));

		g_signal_connect(GTK_OBJECT(widget), "delete_event",
						 G_CALLBACK(mgtk_event_hide_dialog),
						 GINT_TO_POINTER(0));
	
		new_adt(&ret, ARG_GTK_WINDOW, (void *)widget);

		/* Mongoose 2002.02.01, Add this widget to a special
		 *   lookup table by it's event id */
		mgtk_event_subscribe_gtk_widget(get_int(eOpen), widget);
		mgtk_event_subscribe_gtk_widget(get_int(eClose), widget);
		
		//gtk_widget_show(widget);
	}

	delete_arg(&eOpen);
	delete_arg(&eClose);

	return ret;
}


arg_list_t *mgtk_rc_handlebox(arg_list_t *box)
{
	arg_list_t *type, *ret;
	GtkWidget *handlebox;


	if (!box)
	{
		rc_assertion_error("handlebox", "container != NULL");
		return NULL;
	}

	arg_enforce_type(&box,  ARG_GTK_BOX_WIDGET);

	if (!box)
	{
		rc_assertion_error("handlebox", "container == ARG_GTK_BOX_WIDGET");
		return NULL;
	}

	symbol_enforce_type(&type, INT);
	ret = NULL;

	if (!type)
	{
		if (!type)
			rc_assertion_error("handlebox", "type == INT");
	}
	else
	{
		handlebox = gtk_handle_box_new();
		gtk_widget_show(handlebox);
		gtk_box_pack_start(GTK_BOX((GtkWidget *)box->data), 
						   handlebox, FALSE, TRUE, 0);
		gtk_handle_box_set_handle_position(GTK_HANDLE_BOX(handlebox), 
										   ((get_int(type) == 0) ?
											GTK_POS_TOP : 
											((get_int(type) == 1) ? 
											 GTK_POS_LEFT : GTK_POS_RIGHT)));

		new_adt(&ret, ARG_GTK_BOX_WIDGET, (void *)handlebox);
	}

	delete_arg(&type);

	return ret;
}


arg_list_t *mgtk_rc_togglebutton(arg_list_t *container)
{
	arg_list_t *label, *toggled, *event, *cmd, *ret;
	GtkWidget *item;
	//void (*agtk_event)(GtkWidget *, void *);
	void *agtk_event;


	if (!container)
	{
		rc_assertion_error("togglebutton", "container != NULL");
		return NULL;
	}

	arg_enforce_type(&container,  ARG_GTK_BOX_WIDGET);

	if (!container)
	{
		rc_assertion_error("togglebutton", "container == ARG_GTK_BOX_WIDGET");
		return NULL;
	}

	symbol_enforce_type(&label, CSTRING);
	symbol_enforce_type(&toggled, INT);
	symbol_enforce_type(&event, INT);
	symbol_enforce_type(&cmd, INT);
	ret = NULL;

	if (!label || !toggled || !event || !cmd)
	{
		if (!label)
			rc_assertion_error("togglebutton", "label == CSTRING");

		if (!toggled)
			rc_assertion_error("togglebutton", "toggled == INT");

		if (!event)
			rc_assertion_error("togglebutton", "event == INT");

		if (!cmd)
			rc_assertion_error("togglebutton", "cmd == INT");
	}
	else
	{
		item = gtk_toggle_button_new_with_label(get_string(label));
		gtk_widget_ref(item);
		gtk_object_set_data_full(GTK_OBJECT((GtkWidget *)container->data), 
										 "button1", item,
										 (GtkDestroyNotify)gtk_widget_unref);
		gtk_widget_show(item);

		gtk_box_pack_start(GTK_BOX((GtkWidget *)container->data), 
								 item, TRUE, TRUE, 0);

		agtk_event = rc_gtk_event_func(get_int(event));

		if (get_int(toggled))
		{
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(item), TRUE);
		}
		
		if (agtk_event)
		{
			gtk_signal_connect(GTK_OBJECT(item), "toggled",
									 GTK_SIGNAL_FUNC(agtk_event), 
									 GINT_TO_POINTER(get_int(cmd)));
		}
		
		new_adt(&ret, ARG_GTK_WIDGET, (void *)item); // ARG_GTK_SPINBUTTON_WIDGET
	}

	delete_arg(&label);
	delete_arg(&event);
	delete_arg(&cmd);

	return ret;
}


arg_list_t *mgtk_rc_colorbutton(arg_list_t *container)
{
	arg_list_t *event, *cmd, *ret;
	GtkWidget *item;


	if (!container)
	{
		rc_assertion_error("button", "container != NULL");
		return NULL;
	}

	arg_enforce_type(&container,  ARG_GTK_BOX_WIDGET);

	if (!container)
	{
		rc_assertion_error("button", "container == ARG_GTK_BOX_WIDGET");
		return NULL;
	}

	symbol_enforce_type(&event, INT);
	symbol_enforce_type(&cmd, INT);
	ret = NULL;

	if (!event || !cmd)
	{
		if (!event)
			rc_assertion_error("button", "event == INT");

		if (!cmd)
			rc_assertion_error("button", "cmd == INT");
	}
	else
	{
		item = mgtk_create_color_button((void*)mgtk_event_color, get_int(cmd));
		gtk_widget_ref(item);
		gtk_object_set_data_full(GTK_OBJECT((GtkWidget *)container->data), 
										 "button1", item,
										 (GtkDestroyNotify)gtk_widget_unref);
		gtk_widget_show(item);

		gtk_box_pack_start(GTK_BOX((GtkWidget *)container->data), 
								 item, TRUE, TRUE, 0);

		// Mongoose 2002.02.01, Add this widget to a special 
		//   lookup table by it's event id
		//index_add_gtk_widget(get_int(cmd), item);
		mgtk_event_subscribe_gtk_widget(get_int(cmd), item);

		new_adt(&ret, ARG_GTK_WIDGET, (void *)item);
	}

	delete_arg(&event);
	delete_arg(&cmd);

	return ret;
}


arg_list_t *mgtk_rc_button(arg_list_t *container)
{
	arg_list_t *label, *event, *cmd, *ret;
	GtkWidget *item;
	void *agtk_event;


	if (!container)
	{
		rc_assertion_error("button", "container != NULL");
		return NULL;
	}

	arg_enforce_type(&container,  ARG_GTK_BOX_WIDGET);

	if (!container)
	{
		rc_assertion_error("button", "container == ARG_GTK_BOX_WIDGET");
		return NULL;
	}

	symbol_enforce_type(&label, CSTRING);
	symbol_enforce_type(&event, INT);
	symbol_enforce_type(&cmd, INT);
	ret = NULL;

	if (!label || !event || !cmd)
	{
		if (!label)
			rc_assertion_error("button", "label == CSTRING");

		if (!event)
			rc_assertion_error("button", "event == INT");

		if (!cmd)
			rc_assertion_error("button", "cmd == INT");
	}
	else
	{
		item = gtk_button_new_with_label(get_string(label));
		gtk_widget_ref(item);
		gtk_object_set_data_full(GTK_OBJECT((GtkWidget *)container->data), 
										 "button1", item,
										 (GtkDestroyNotify)gtk_widget_unref);
		gtk_widget_show(item);

		gtk_box_pack_start(GTK_BOX((GtkWidget *)container->data), 
								 item, TRUE, TRUE, 0);

		// Mongoose 2002.02.01, Add this widget to a special 
		//   lookup table by it's event id
		//index_add_gtk_widget(get_int(cmd), item);
		mgtk_event_subscribe_gtk_widget(get_int(cmd), item);

		agtk_event = rc_gtk_event_func(get_int(event));

		if (agtk_event)
		{
			gtk_signal_connect(GTK_OBJECT(item), "pressed",
							   GTK_SIGNAL_FUNC(agtk_event), 
							   GINT_TO_POINTER(get_int(cmd)));
		}
		
		new_adt(&ret, ARG_GTK_WIDGET, (void *)item); // ARG_GTK_SPINBUTTON_WIDGET
	}

	delete_arg(&label);
	delete_arg(&event);
	delete_arg(&cmd);

	return ret;
}


arg_list_t *mgtk_rc_hslider(arg_list_t *container)
{
	arg_list_t *event, *min, *max;
	arg_list_t *ret = NULL;
	GtkWidget *item;
	GtkObject *adj;


	if (!container)
	{
		rc_assertion_error("vsep", "container != NULL");
		return NULL;
	}

	arg_enforce_type(&container,  ARG_GTK_BOX_WIDGET);

	if (!container)
	{
		rc_assertion_error("vsep", "container == ARG_GTK_BOX_WIDGET");
		return NULL;
	}

	symbol_enforce_type(&event, INT);
	symbol_enforce_type(&min, INT);
	symbol_enforce_type(&max, INT);

	if (!event || !min || !max)
	{
		if (!event)
			rc_assertion_error("hslider", "event == INT");

		if (!min)
			rc_assertion_error("hslider", "min == INT");

		if (!max)
			rc_assertion_error("hslider", "max == INT");

		return NULL;
	}

	/* slider */
	adj = gtk_adjustment_new(0, get_int(min), get_int(max), 1, 2, 0);
	item = gtk_hscale_new(GTK_ADJUSTMENT(adj));
	gtk_widget_ref(item);
	//gtk_widget_set_usize(item, 132, -2);
	gtk_scale_set_digits(GTK_SCALE(item), 0);

	gtk_signal_connect(GTK_OBJECT(adj), "value_changed",
					   GTK_SIGNAL_FUNC(mgtk_event_slider), 
					   GINT_TO_POINTER(get_int(event)));
	
	//	gtk_box_pack_start(GTK_BOX(box), hscale, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX((GtkWidget *)container->data),item,TRUE,TRUE,0);
	gtk_widget_show(item);
	
	new_adt(&ret, ARG_GTK_WIDGET, (void *)item); // ARG_GTK_SPINBUTTON_WIDGET

	delete_arg(&event);
	delete_arg(&min);
	delete_arg(&max);

	return ret;
}


arg_list_t *mgtk_rc_textbox(arg_list_t *container)
{
	arg_list_t *ret, *event;
	GtkWidget *item;


	if (!container)
	{
		rc_assertion_error("textbox", "container != NULL");
		return NULL;
	}

	arg_enforce_type(&container,  ARG_GTK_BOX_WIDGET);

	if (!container)
	{
		rc_assertion_error("textbox", "container == ARG_GTK_BOX_WIDGET");
		return NULL;
	}

	symbol_enforce_type(&event, INT);
	ret = NULL;

	if (!event)
	{
		rc_assertion_error("textbox", "event == INT");
	}
	else
	{
		item = mgtk_create_text_entry((GtkWidget *)container->data);
		new_adt(&ret, ARG_GTK_WIDGET, (void *)item); // ARG_GTK_TEXTENTRY_WIDGET

		gtk_signal_connect(GTK_OBJECT(item), "changed",
						   GTK_SIGNAL_FUNC(mgtk_event_text),
						   GINT_TO_POINTER(get_int(event)));

		// Mongoose 2002.02.14, Add this widget to a special 
		//   lookup table by it's event id
		//index_add_gtk_widget(get_int(event), item);
		mgtk_event_subscribe_gtk_widget(get_int(event), item);
	}

	return ret;
}


arg_list_t *mgtk_rc_hsep(arg_list_t *container)
{
	arg_list_t *ret;
	GtkWidget *item;


	if (!container)
	{
		rc_assertion_error("hsep", "container != NULL");
		return NULL;
	}

	arg_enforce_type(&container,  ARG_GTK_BOX_WIDGET);

	if (!container)
	{
		rc_assertion_error("hsep", "container == ARG_GTK_BOX_WIDGET");
		return NULL;
	}

	ret = NULL;

	item = gtk_hseparator_new();
	gtk_box_pack_start(GTK_BOX((GtkWidget *)container->data), 
							 item, TRUE, TRUE, 0);
	gtk_widget_show(item);
	
	new_adt(&ret, ARG_GTK_WIDGET, (void *)item); // ARG_GTK_SPINBUTTON_WIDGET

	return ret;
}


arg_list_t *mgtk_rc_vsep(arg_list_t *container)
{
	arg_list_t *ret;
	GtkWidget *item;


	if (!container)
	{
		rc_assertion_error("vsep", "container != NULL");
		return NULL;
	}

	arg_enforce_type(&container,  ARG_GTK_BOX_WIDGET);

	if (!container)
	{
		rc_assertion_error("vsep", "container == ARG_GTK_BOX_WIDGET");
		return NULL;
	}

	ret = NULL;

	item = gtk_vseparator_new();
	gtk_box_pack_start(GTK_BOX((GtkWidget *)container->data), 
							 item, TRUE, TRUE, 0);
	gtk_widget_show(item);
	
	new_adt(&ret, ARG_GTK_WIDGET, (void *)item); // ARG_GTK_SPINBUTTON_WIDGET

	return ret;
}


arg_list_t *mgtk_rc_label2(arg_list_t *container)
{
	arg_list_t *label, *ret;
	GtkWidget *item;


	if (!container)
	{
		rc_assertion_error("label", "container != NULL");
		return NULL;
	}

	arg_enforce_type(&container,  ARG_GTK_BOX_WIDGET);

	if (!container)
	{
		rc_assertion_error("label", "container == ARG_GTK_BOX_WIDGET");
		return NULL;
	}

	symbol_enforce_type(&label, CSTRING);
	ret = NULL;

	if (!label)
	{
		if (!label)
		{
			rc_assertion_error("label2", "label == CSTRING");
		}
	}
	else
	{
		item = gtk_label_new(NULL);
		gtk_label_set_markup(GTK_LABEL(item), get_string(label));

		gtk_box_pack_start(GTK_BOX((GtkWidget *)container->data), 
								 item, TRUE, TRUE, 0);

		gtk_widget_show(item);
	
		new_adt(&ret, ARG_GTK_WIDGET, (void *)item); // ARG_GTK_SPINBUTTON_WIDGET
	}

	delete_arg(&label);

	return ret;
}


arg_list_t *mgtk_rc_label(arg_list_t *container)
{
	arg_list_t *label, *x_align, *y_align, *ret;
	GtkWidget *item;


	if (!container)
	{
		rc_assertion_error("label", "container != NULL");
		return NULL;
	}

	arg_enforce_type(&container,  ARG_GTK_BOX_WIDGET);

	if (!container)
	{
		rc_assertion_error("label", "container == ARG_GTK_BOX_WIDGET");
		return NULL;
	}

	symbol_enforce_type(&label, CSTRING);
	symbol_enforce_type(&x_align, FLOAT);
	symbol_enforce_type(&y_align, FLOAT);
	ret = NULL;

	if (!label || !x_align || !y_align)
	{
		if (!label)
		{
			rc_assertion_error("label", "label == CSTRING");
		}

		if (!x_align) 
		{
			rc_assertion_error("label", "label == FLOAT");
		}

		if (!y_align)
		{
			rc_assertion_error("label", "label == FLOAT");
		}
	}
	else
	{
		item = mgtk_create_label((GtkWidget *)container->data, 
								 get_string(label), // name
								 get_string(label), // label text
								 get_float(x_align),
								 get_float(y_align));
		
		gtk_box_pack_start(GTK_BOX((GtkWidget *)container->data), 
								 item, TRUE, TRUE, 0);

		gtk_widget_show(item);
	
		new_adt(&ret, ARG_GTK_WIDGET, (void *)item); // ARG_GTK_SPINBUTTON_WIDGET
	}

	delete_arg(&label);
	delete_arg(&x_align);
	delete_arg(&y_align);

	return ret;
}


arg_list_t *mgtk_rc_icon(arg_list_t *container)
{
	arg_list_t *name, *size, *ret;
	GtkWidget *icon;


	if (!container)
	{
		rc_assertion_error("icon", "container != NULL");
		return NULL;
	}

	arg_enforce_type(&container,  ARG_GTK_BOX_WIDGET);

	if (!container)
	{
		rc_assertion_error("icon", "container == ARG_GTK_BOX_WIDGET");
		return NULL;
	}

	symbol_enforce_type(&name, CSTRING);
	symbol_enforce_type(&size, INT);
	ret = NULL;

	if (!name || !size)
	{
		if (!name)
		{
			rc_assertion_error("icon", "name == CSTRING");
		}

		if (!size) 
		{
			rc_assertion_error("icon", "size == INT");
		}
	}
	else
	{
		GtkIconSize sz;
		char icon_filename[4096];

		switch (get_int(size))
		{
		case 1:
			sz = GTK_ICON_SIZE_MENU; 
			break;
		case 2:
			sz = GTK_ICON_SIZE_SMALL_TOOLBAR; 
			break;
		case 3:
			sz = GTK_ICON_SIZE_LARGE_TOOLBAR; 
			break;
		case 4:
			sz = GTK_ICON_SIZE_BUTTON; 
			break;
		case 5:
			sz = GTK_ICON_SIZE_DIALOG; 
			break;
		default:
			sz = GTK_ICON_SIZE_DIALOG;
		}

		mgtk_get_pixmap_filename(icon_filename, 4095, get_string(name));
		icon_filename[4095] = 0;

		icon = mgtk_create_icon(icon_filename, sz);
		gtk_container_add(GTK_CONTAINER((GtkWidget *)container->data), icon);
		gtk_widget_show(icon);

		new_adt(&ret, ARG_GTK_WIDGET, (void *)icon); // ARG_GTK_ICON_WIDGET
	}

	delete_arg(&name);
	delete_arg(&size);

	return ret;
}


arg_list_t *mgtk_rc_spinbutton(arg_list_t *container)
{
	GtkWidget *item;
	arg_list_t *start, *min, *max, *event, *ret;
	//	void (*agtk_event)(GtkWidget *, void *);


	if (!container)
	{
		rc_assertion_error("spinbutton", "container != NULL");
		return NULL;
	}

	arg_enforce_type(&container,  ARG_GTK_BOX_WIDGET);

	if (!container)
	{
		rc_assertion_error("spinbutton", "container == ARG_GTK_BOX_WIDGET");
		return NULL;
	}

	symbol_enforce_type(&start, INT | FLOAT);
	symbol_enforce_type(&min,   INT | FLOAT);
	symbol_enforce_type(&max,   INT | FLOAT);
	symbol_enforce_type(&event, INT);
	ret = NULL;

	if (!start || !min || !max || !event)
	{
		if (!start)
			rc_assertion_error("spinbutton", "start == NUMERIC");

		if (!min)
			rc_assertion_error("spinbutton", "min == NUMERIC");

		if (!max)
			rc_assertion_error("spinbutton", "max == NUMERIC");

		if (!event)
			rc_assertion_error("spinbutton", "event == INT");
	}
	else
	{
		item = mgtk_create_spinbutton((GtkWidget *)container->data, 
									  "sbtn",
									  get_number(start),
									  get_number(min),
									  get_number(max));

		gtk_box_pack_start(GTK_BOX((GtkWidget *)container->data), 
								 item, TRUE, TRUE, 0);
		gtk_widget_show(item);


		/* Mongoose 2002.02.01, Add this widget to a special 
		 *   lookup table by it's event id */
		//index_add_gtk_widget(get_int(event), item);
		mgtk_event_subscribe_gtk_widget(get_int(event), item);

		/* Mongoose 2002.01.25, Float, unsigned int, and int data */
		if (start->type == FLOAT)
		{
			gtk_signal_connect(GTK_OBJECT(item), "changed",
							   GTK_SIGNAL_FUNC(mgtk_event_spinbutton_float), 
							   GINT_TO_POINTER(get_int(event)));
		}
		else if (get_int(min) >= 0)
		{
			gtk_signal_connect(GTK_OBJECT(item), "changed",
							   GTK_SIGNAL_FUNC(mgtk_event_spinbutton_uint), 
							   GINT_TO_POINTER(get_int(event)));
		}
		else
		{
			gtk_signal_connect(GTK_OBJECT(item), "changed",
							   GTK_SIGNAL_FUNC(mgtk_event_spinbutton_int), 
							   GINT_TO_POINTER(get_int(event)));
		}

		new_adt(&ret, ARG_GTK_WIDGET, (void *)item); // ARG_GTK_SPINBUTTON_WIDGET
	}

	delete_arg(&start);
	delete_arg(&min);
	delete_arg(&max);
	delete_arg(&event);

	return ret;
}


arg_list_t *mgtk_rc_spinbutton2(arg_list_t *container)
{
	GtkWidget *item;
	arg_list_t *start, *min, *max, *event, *ret;
	arg_list_t *step, *page, *page_sz, *digit;


	if (!container)
	{
		rc_assertion_error("spinbutton2", "container != NULL");
		return NULL;
	}

	arg_enforce_type(&container,  ARG_GTK_BOX_WIDGET);

	if (!container)
	{
		rc_assertion_error("spinbutton2", "container == ARG_GTK_BOX_WIDGET");
		return NULL;
	}

	symbol_enforce_type(&start, INT | FLOAT);
	symbol_enforce_type(&min,   INT | FLOAT);
	symbol_enforce_type(&max,   INT | FLOAT);
	symbol_enforce_type(&step, INT | FLOAT);
	symbol_enforce_type(&page, INT | FLOAT);
	symbol_enforce_type(&page_sz, INT | FLOAT);
	symbol_enforce_type(&digit, INT);
	symbol_enforce_type(&event, INT);
	ret = NULL;

	if (!start || !min || !max || !event || !step || !page || 
		 !page_sz || !digit)
	{
		if (!start)
			rc_assertion_error("spinbutton2", "start == NUMERIC");

		if (!min)
			rc_assertion_error("spinbutton2", "min == NUMERIC");

		if (!max)
			rc_assertion_error("spinbutton2", "max == NUMERIC");

		if (!event)
			rc_assertion_error("spinbutton2", "event == INT");

		if (!digit)
			rc_assertion_error("spinbutton2", "digit == INT");

		if (!step)
			rc_assertion_error("spinbutton2", "step == NUMERIC");

		if (!page)
			rc_assertion_error("spinbutton2", "page == NUMERIC");

		if (!page_sz)
			rc_assertion_error("spinbutton2", "page_sz == NUMERIC");
	}
	else
	{
		item = mgtk_create_spinbutton2((GtkWidget *)container->data, 
									   "sbtn2",
									   get_number(start),
									   get_number(min),
									   get_number(max),
									   get_number(step),
									   get_number(page),
									   get_number(page_sz),
									   get_int(digit));

		gtk_box_pack_start(GTK_BOX((GtkWidget *)container->data), 
								 item, TRUE, TRUE, 0);
		gtk_widget_show(item);


		// Mongoose 2002.02.01, Add this widget to a special 
		//   lookup table by it's event id
		//index_add_gtk_widget(get_int(event), item);
		mgtk_event_subscribe_gtk_widget(get_int(event), item);

		// Mongoose 2002.01.25, Float, unsigned int, and int data
		if (get_int(digit))
		{
			gtk_signal_connect(GTK_OBJECT(item), "changed",
							   GTK_SIGNAL_FUNC(mgtk_event_spinbutton_float), 
							   GINT_TO_POINTER(get_int(event)));
		}
		else if (get_int(min) >= 0)
		{
			gtk_signal_connect(GTK_OBJECT(item), "changed",
							   GTK_SIGNAL_FUNC(mgtk_event_spinbutton_uint), 
							   GINT_TO_POINTER(get_int(event)));
		}
		else
		{
			gtk_signal_connect(GTK_OBJECT(item), "changed",
							   GTK_SIGNAL_FUNC(mgtk_event_spinbutton_int), 
							   GINT_TO_POINTER(get_int(event)));
		}

		new_adt(&ret, ARG_GTK_WIDGET, (void *)item); //ARG_GTK_SPINBUTTON_WIDGET
	}

	delete_arg(&start);
	delete_arg(&min);
	delete_arg(&max);
	delete_arg(&event);
	delete_arg(&step);
	delete_arg(&page);
	delete_arg(&page_sz);
	delete_arg(&digit);

	return ret;
}


GtkWidget *append_toolbar_button(GtkWidget *box, GtkWidget *toolbar,
							const char *name, const char *buttonId,
							const gchar *stockId, GtkIconSize size)
{
	GtkWidget *button;
	GtkWidget *toolbar_icon;


	toolbar_icon = gtk_image_new_from_stock(stockId, size);
	button = gtk_toolbar_append_element(GTK_TOOLBAR(toolbar),
										GTK_TOOLBAR_CHILD_BUTTON,
										NULL,
										name,
										NULL, NULL,
										toolbar_icon, NULL, NULL);
	gtk_widget_ref(button);
	gtk_object_set_data_full(GTK_OBJECT(box), buttonId, button,
							 (GtkDestroyNotify)gtk_widget_unref);
	gtk_widget_show(button);

	return button;
}


/* Mongoose 2004.10.28, 
 * FIXME This only allows one gobal tree widget and it's crappy to boot */
void mgtk_resource_rebuild_treeview(GtkTreeModel *model)
{
	if (SKELETON_TREEVIEW && model)
	{
		gtk_tree_view_set_model(SKELETON_TREEVIEW, model);
	}
}

void mgtk_tree_cell_edited_callback (GtkCellRendererText *cell,
									 gchar               *path_string,
									 gchar               *new_text,
									 gpointer             userdata)
{
	mgtk_print("!Event %i: %s\n", GPOINTER_TO_INT(userdata), new_text);
	mgtk_handle_text(GPOINTER_TO_INT(userdata), new_text);
}



gboolean mgtk_GtkTreeSelectionFunc(GtkTreeSelection *selection,
								   GtkTreeModel *model,
								   GtkTreePath *path,
								   gboolean path_currently_selected,
								   gpointer data)
{
	mgtk_print("!$$$$$$$$$$$$$\n tree selected\n$$$$$$$$$$$$$$$$\n");

	return TRUE;
}


void  mgtk_treeview_onRowActivated (GtkTreeView        *treeview,
						   GtkTreePath        *path,
						   GtkTreeViewColumn  *col,
						   gpointer            userdata)
{
	GtkTreeModel *model;
	GtkTreeIter   iter;
	
    g_print ("A row has been double-clicked!\n");
	
    model = gtk_tree_view_get_model(treeview);
	
    if (gtk_tree_model_get_iter(model, &iter, path))
    {
		gchar *name;
		gint id;

		gtk_tree_model_get(model, &iter, 
						   NAME_COLUMN, &name, 
						   ID_COLUMN, &id,
						   -1);

		mgtk_print("!Event %i: Selected row { %s, %i }\n", GPOINTER_TO_INT(userdata), name, id);

		mgtk_handle_event1u(GPOINTER_TO_INT(userdata), id);

		g_free(name);
	}
}



/* Mongoose 2002.01.24,
 * Tree widget should be factored out for general widget use */
arg_list_t *mgtk_rc_animation_tab_hack(arg_list_t *container)
{
	arg_list_t *ret;
	GtkWidget *box = NULL;


	if (!container)
	{
		rc_assertion_error("animation_tab_hack", "container != NULL");
		return NULL;
	}

	arg_enforce_type(&container, ARG_GTK_BOX_WIDGET);

	if (!container)
	{
		rc_assertion_error("animation_tab_hack", "container == ARG_GTK_BOX_WIDGET");
		return NULL;
	}


	// Read in an event id
	arg_list_t *event = symbol();
	arg_enforce_type(&event, INT);
	if (!event)
	{
		rc_assertion_error("animation_tab_hack", "event == INT");
		return NULL;
	}

	arg_list_t *event2 = symbol();
	arg_enforce_type(&event2, INT);
	if (!event2)
	{
		rc_assertion_error("animation_tab_hack", "event2 == INT");
		return NULL;
	}	


	ret = NULL;

	box = (GtkWidget *)container->data;


    /* Tree: New for Gtk+ 2.0 working Animation Tree */
	GtkTreeViewColumn *col;
	GtkCellRenderer *renderer;
	GtkWidget *view;
	GtkTreeModel *model;
	GtkTreeStore *store;
	GtkTreeIter root, child;


	view = gtk_tree_view_new();	
	SKELETON_TREEVIEW = GTK_TREE_VIEW(view);

	g_signal_connect(view, "row-activated", (GCallback)mgtk_treeview_onRowActivated, GINT_TO_POINTER(get_int(event)));

	GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
	if (selection)
	{
		gtk_tree_selection_set_mode(selection, GTK_SELECTION_BROWSE);
		gtk_tree_selection_set_select_function(selection, mgtk_GtkTreeSelectionFunc, GINT_TO_POINTER(get_int(event)), NULL);
	}

	/* Column "Name" */
	col = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(col, "Name");
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_add_attribute(col, renderer, "text", NAME_COLUMN);
	g_object_set(renderer, "editable", TRUE, NULL);
	g_signal_connect(renderer, "edited", (GCallback)mgtk_tree_cell_edited_callback, GINT_TO_POINTER(get_int(event2)));

	/* Column "Id" */
	col = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(col, "Id");
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_add_attribute(col, renderer, "text", ID_COLUMN);

	/* Append dummy bone(s) */
	store = gtk_tree_store_new(N_COLUMNS, /* Total number of cols */
							   G_TYPE_STRING,   /* Bone Name */
                               G_TYPE_INT);     /* Bone Id */

	gtk_tree_store_append(store, &root, NULL);
	gtk_tree_store_set(store, &root,
                   	   NAME_COLUMN, "root",
                   	   ID_COLUMN, 0,
		               -1);

	gtk_tree_store_append(store, &child, &root);
	gtk_tree_store_set(store, &child,
                   	   NAME_COLUMN, "dummy",
                   	   ID_COLUMN, 1,
		               -1);

	model = GTK_TREE_MODEL(store);

	/* Add view to container */
	gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
	g_object_unref(model); 
	gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(view)),
                                GTK_SELECTION_NONE);
	gtk_widget_show(view);

#ifdef ANIM_TREE_IN_BOX
	gtk_box_pack_start(GTK_BOX(box), view, FALSE, FALSE, FALSE);
#else
	/* Animation tree inside scrolled window, instead of box */
	GtkWidget* scroll = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
											 GTK_POLICY_AUTOMATIC,
											 GTK_POLICY_AUTOMATIC);
	gtk_widget_show(scroll);
	gtk_widget_set_usize(scroll, 100, 200);
	gtk_box_pack_start(GTK_BOX(box), scroll, FALSE, FALSE, FALSE);
	gtk_container_add(GTK_CONTAINER(scroll), view);
#endif

	delete_arg(&event);
	delete_arg(&event2);

	return ret;
}


arg_list_t *mgtk_rc_optionmenu(arg_list_t *box)
{
	GtkWidget *optionmenu, *optionmenu_menu;
	arg_list_t *ret, *event, *cmd;
	

	if (!box)
	{
		rc_assertion_error("optionmenu", "container != NULL");
		return NULL;
	}

	arg_enforce_type(&box, ARG_GTK_BOX_WIDGET); // ARG_GTK_BOX_WIDGET

	if (!box)
	{
		rc_assertion_error("optionmenu", "container == ARG_GTK_BOX_WIDGET");
		return NULL;
	}

	event = symbol();
	cmd = symbol();
	ret = NULL;

	arg_enforce_type(&event, INT);
	arg_enforce_type(&cmd, INT);

	if (!event || !cmd)
	{
		if (!event)
			rc_assertion_error("optionmenu", "event == INT");

		if (!cmd)
			rc_assertion_error("optionmenu", "cmd == INT");
	}
	else
	{
		optionmenu = gtk_option_menu_new();
		gtk_widget_ref(optionmenu);

		gtk_object_set_data_full(GTK_OBJECT((GtkWidget *)box->data), 
										 "optionmenu", 
										 optionmenu,
										 (GtkDestroyNotify)gtk_widget_unref);
		gtk_widget_show(optionmenu);

		gtk_box_pack_start(GTK_BOX(box->data), optionmenu, TRUE, TRUE, 0);

		optionmenu_menu = gtk_menu_new();
		gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu), optionmenu_menu);
		
		new_adt(&ret, ARG_GTK_MENU_WIDGET, (void *)optionmenu_menu);

		// Mongoose 2002.02.14, FIXME should use both keys event:cmd, 
		//   not just command

		// Mongoose 2002.02.14, Add this widget to a special 
		//   lookup table by it's event id
		//index_add_gtk_widget(get_int(cmd), optionmenu);
		mgtk_event_subscribe_gtk_widget(get_int(cmd), optionmenu);
	}

	delete_arg(&event);
	delete_arg(&cmd);

	return ret;
}


// Hacky extention
arg_list_t *mgtk_rc_fileselection_drop_down_menu(arg_list_t *box)
{
	arg_list_t *event, *ret = NULL;
	
	mgtk_print("fileselection_drop_down_menu entry");

	event = symbol();
	arg_enforce_type(&event, INT);

	if (!event)
		rc_assertion_error("optionmenu", "event == INT");

	GtkWidget *dd = event ? gFileDialogPatternWidgetMap[get_int(event)] : NULL;

	if (!dd)
	{
		rc_assertion_error("fileselection_drop_down_menu", "container != NULL");
		return NULL;
	}

	new_adt(&ret, ARG_GTK_MENU_WIDGET, (void *)dd);

	delete_arg(&event);

	return ret;
}


// (filechooser* ... "Title..." eEvent "your_mgtk_rc_pattern_callback") 
// (filechooserbutton "Button label" "Title..." eFileDialogEvent "your_mgtk_rc_pattern_callback") 
arg_list_t *mgtk_rc_filechooserbutton(arg_list_t *box)
{
	arg_list_t *label, *event, *title, *pattern_func, *ret = NULL;

	if (!box)
	{
		rc_assertion_error("mgtk_rc_filechooserbutton", "container != NULL");
		return NULL;
	}

	arg_enforce_type(&box, ARG_GTK_BOX_WIDGET); // ARG_GTK_BOX_WIDGET

	if (!box)
	{
		rc_assertion_error("mgtk_rc_filechooserbutton", "container == ARG_GTK_BOX_WIDGET");
		return NULL;
	}

	label = symbol();
	arg_enforce_type(&label, CSTRING);
	if (!label)	rc_assertion_error("mgtk_rc_filechooserbutton", "label == CSTRING");

	title = symbol();
	arg_enforce_type(&title, CSTRING);
	if (!title)	rc_assertion_error("mgtk_rc_filechooserbutton", "title == STRING");

	event = symbol();
	arg_enforce_type(&event, INT);
	if (!event)	rc_assertion_error("mgtk_rc_filechooserbutton", "event == INT");

	pattern_func = symbol();
	arg_enforce_type(&pattern_func, CSTRING);
	if (!pattern_func)	rc_assertion_error("mgtk_rc_filechooserbutton", "pattern_func == CSTRING");


	GtkWidget *item = NULL;

	if (!label || !title || !event || !pattern_func)
	{
		return NULL;
	}
	else
	{
		mgtk_link_filechooser_from_rc(get_int(event), get_string(title), get_string(pattern_func));

		item = gtk_button_new_with_label(get_string(label));
		gtk_widget_ref(item);
		gtk_object_set_data_full(GTK_OBJECT((GtkWidget *)box->data), 
								 "filechooserbutton1", item,
								 (GtkDestroyNotify)gtk_widget_unref);
		gtk_widget_show(item);

		gtk_box_pack_start(GTK_BOX((GtkWidget *)box->data), 
								 item, TRUE, TRUE, 0);

		gtk_signal_connect(GTK_OBJECT(item), "pressed",
						   GTK_SIGNAL_FUNC(mgtk_filechooser_spawn_event), 
						   GINT_TO_POINTER(get_int(event)));
		
		new_adt(&ret, ARG_GTK_WIDGET, (void *)item); // ARG_GTK_SPINBUTTON_WIDGET
	}

	///////////////////////




	if (!title || !event || !pattern_func)
	{
		rc_assertion_error("mgtk_rc_filechooserbutton", "container != NULL");
		return NULL;
	}

	new_adt(&ret, ARG_GTK_MENU_WIDGET, (void *)item);

	delete_arg(&label);
	delete_arg(&title);
	delete_arg(&event);
	delete_arg(&pattern_func);

	return ret;
}


////////////////////////////////////////////////////////////////
// Gtk+ menu widgets
////////////////////////////////////////////////////////////////

// (filechoosermenu_item "Label" "Title..." [F1] eFileDialogEvent "your_mgtk_rc_pattern_callback" "icon") 
arg_list_t *mgtk_rc_filechoosermenu_item(arg_list_t *menu)
{
	GtkWidget *item;
	arg_list_t *ret = NULL;

	if (!menu)
	{
		rc_assertion_error("menu_item", "menu != NULL");
		return NULL;
	}

	arg_enforce_type(&menu,  ARG_GTK_MENU_WIDGET); // ARG_GTK_MENU_WIDGET

	if (!menu)
	{
		rc_assertion_error("menu_item", "menu == ARG_GTK_MENU_WIDGET");
		return NULL;
	}

	arg_list_t *text = symbol();
	arg_enforce_type(&text,  CSTRING);
	if (!text)	rc_assertion_error("mgtk_rc_filechooserbutton", "text == STRING");

	arg_list_t *title = symbol();
	arg_enforce_type(&title, CSTRING);
	if (!title)	rc_assertion_error("mgtk_rc_filechooserbutton", "title == STRING");


	// Accel is optional
	arg_list_t *accel = symbol();
	arg_list_t *event;

	if (accel->type == CSTRING)
	{
		event = symbol();
	}
	else
	{
		event = accel;
		accel = 0x0;
	}

	arg_enforce_type(&event, INT);
	if (!event)	rc_assertion_error("mgtk_rc_filechooserbutton", "event == INT");

	arg_list_t *pattern_func = symbol();
	arg_enforce_type(&pattern_func, CSTRING);
	if (!pattern_func)	rc_assertion_error("mgtk_rc_filechooserbutton", "pattern_func == CSTRING");

	arg_list_t *icon = symbol();
	arg_enforce_type(&icon, CSTRING);
	if (!icon)	rc_assertion_error("mgtk_rc_filechooserbutton", "icon == STRING");

	if (!text || !event || !pattern_func || !icon)
	{
		return NULL;
	}
	else
	{
		item = gtk_image_menu_item_new_with_mnemonic((char *)text->data);
		gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(item),
									  mgtk_create_icon((char *)icon->data, 
													   GTK_ICON_SIZE_MENU));

#ifdef ACCEL_SUPPORT_ON
		if (accel != 0 &&
			accel->data != 0 &&
			((char *)accel->data)[0] != 0)
		{  
			GtkAccelGroup *accel_group;
			unsigned int i, len, key, mod;
			char *s = (char *)(accel->data);
			
			mgtk_print("Key accel %s", (char *)(accel->data));
			
			len = strlen(s);
			
			for (mod = 0, i = 0; i < len; ++i)
			{
				switch (s[i])
				{
				case 'C':
					mod |= GDK_CONTROL_MASK;
					break;
				case 'S':
					mod |= GDK_SHIFT_MASK;
					break;
				case 'M':
					mod |= GDK_MOD1_MASK;
					break;
				case 'E':
					key = GDK_Return;
					
					i = len + 8;
					break;
				case 'F':
					switch (s[i+1])
					{
					case '1':
						switch (s[i+2])
						{
						case '0':
							key = GDK_F10;
							break;
						case '1':
							key = GDK_F11;
							break;
						case '2':
							key = GDK_F12;
							break;
						default:
							key = GDK_F1;
							break;
						}
						break;
					case '2':
						key = GDK_F2;
						break;
					case '3':
						key = GDK_F3;
						break;
					case '4':
						key = GDK_F4;
						break;
					case '5':
						key = GDK_F5;
						break;
					case '6':
						key = GDK_F6;
						break;
					case '7':
						key = GDK_F7;
						break;
					case '8':
						key = GDK_F8;
						break;
					case '9':
						key = GDK_F9;
						break;
					}
					
					i = len + 8;
					break;
				case '-':
					break;
				default:
					key = gdk_unicode_to_keyval(s[i]);
					break;
				}
			}
				
			/* Add code here to translate accel string to GDK key / mods */

			accel_group = gtk_accel_group_new();
			
			// GDK_CONTROL_MASK | GDK_SHIFT_MASK | GDK_MOD1_MASK

			gtk_widget_add_accelerator(item, "activate", 
									   accel_group,
									   key, (GdkModifierType)mod,
									   GTK_ACCEL_VISIBLE);
			
			gtk_window_add_accel_group(GTK_WINDOW(mgtk_get_application_window()),
									   accel_group);
		}
#endif

		new_adt(&ret, ARG_GTK_MENU_WIDGET, (void *)item); // ARG_GTK_MENU_WIDGET
		
		gtk_menu_append(GTK_MENU(menu->data), item);
		gtk_widget_show(item);
		

		gtk_signal_connect(GTK_OBJECT(item), "activate",
						   GTK_SIGNAL_FUNC(mgtk_filechooser_spawn_event), 
						   GINT_TO_POINTER(get_int(event)));

		mgtk_link_filechooser_from_rc(get_int(event), get_string(title), get_string(pattern_func));
	}

	delete_arg(&text);
	delete_arg(&title);
	delete_arg(&event);
	delete_arg(&accel);
	delete_arg(&pattern_func);
	delete_arg(&icon);

	return ret;
}


arg_list_t *mgtk_rc_menu_seperator(arg_list_t *container)
{
	GtkWidget *sep;
	arg_list_t *ret;


	if (!container)
	{
		rc_assertion_error("seperator", "menu != NULL");
		return NULL;
	}

	arg_enforce_type(&container,  ARG_GTK_MENU_WIDGET);

	if (!container)
	{
		rc_assertion_error("seperator", "menu == ARG_GTK_MENU_WIDGET");
		return NULL;
	}


	sep = gtk_menu_item_new();

	if (GTK_IS_MENU_BAR(container->data))
	{
		gtk_menu_append(GTK_MENU_BAR(container->data), sep);
	}
	else
	{
		gtk_menu_append(GTK_MENU(container->data), sep);
	}

	gtk_widget_show(sep);

	new_adt(&ret, ARG_GTK_MENU_WIDGET, (void *)sep);

	return ret;
}


// (menu_item_toggle "Label" eToggleEvent InitStateBool)
void mgtk_check_menu_item_handler(GtkCheckMenuItem *item, gpointer e)
{
	mgtk_handle_event1u(GPOINTER_TO_INT(e), 
						gtk_check_menu_item_get_active(item));
}

arg_list_t *mgtk_rc_check_menu_item(arg_list_t *menu)
{
	GtkWidget *item;
	arg_list_t *text, *event, *cmd, *ret;


	if (!menu)
	{
		rc_assertion_error("check_menu_item", "menu != NULL");
		return NULL;
	}

	arg_enforce_type(&menu,  ARG_GTK_MENU_WIDGET); // ARG_GTK_MENU_WIDGET

	if (!menu)
	{
		rc_assertion_error("check_menu_item", "menu == ARG_GTK_MENU_WIDGET");
		return NULL;
	}

	text = symbol();
	event = symbol();
	cmd = symbol();
	ret = NULL;

	arg_enforce_type(&text,  CSTRING);
	arg_enforce_type(&event, INT);
	arg_enforce_type(&cmd, INT);


	if (!text || !event || !cmd)
	{
		if (!text)
			rc_assertion_error("menu_item", "text == CSTRING");

		if (!event)
			rc_assertion_error("menu_item", "event == INT");

		if (!cmd)
			rc_assertion_error("menu_item", "cmd == INT");
	}
	else
	{
		item = gtk_check_menu_item_new_with_mnemonic((char *)text->data);

		new_adt(&ret, ARG_GTK_MENU_WIDGET, (void *)item); // ARG_GTK_MENU_WIDGET
		
		gtk_menu_append(GTK_MENU(menu->data), item);
		gtk_widget_show(item);
		
		gtk_signal_connect(GTK_OBJECT(item), "toggled",
						   GTK_SIGNAL_FUNC(mgtk_check_menu_item_handler), 
						   GINT_TO_POINTER(get_int(event)));

		if (get_int(cmd))
		{
			gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item), TRUE);
		}

		// Mongoose 2002.02.01, Add this widget to a special 
		//   lookup table by it's event id
		//index_add_gtk_widget(get_int(event), item);
		mgtk_event_subscribe_gtk_widget(get_int(event), item);
	}

	delete_arg(&text);
	delete_arg(&event);
	delete_arg(&cmd);

	return ret;
}


arg_list_t *mgtk_rc_menu_item(arg_list_t *menu)
{
	GtkWidget *item;
	arg_list_t *text, *event, *cmd, *accel, *ret;
	void *agtk_event;


	if (!menu)
	{
		rc_assertion_error("menu_item", "menu != NULL");
		return NULL;
	}

	arg_enforce_type(&menu,  ARG_GTK_MENU_WIDGET); // ARG_GTK_MENU_WIDGET

	if (!menu)
	{
		rc_assertion_error("menu_item", "menu == ARG_GTK_MENU_WIDGET");
		return NULL;
	}

	text = symbol();
	accel = symbol();
	event = symbol();

	if (accel->type == CSTRING)
	{
		cmd = symbol();
	}
	else
	{
		cmd = event;
		event = accel;
		accel = 0x0;
	}
	ret = NULL;

	arg_enforce_type(&text,  CSTRING);
	arg_enforce_type(&event, INT);
	// arg_enforce_type(&cmd,   INT);

	if (!text || !event || cmd->type != INT && cmd->type != CSTRING)
	{
		if (!text)
			rc_assertion_error("menu_item", "text == CSTRING");

		if (!event)
			rc_assertion_error("menu_item", "event == INT");

		if (!cmd)
			rc_assertion_error("menu_item", "cmd == INT || CSTRING");
	}
	else
	{
		item = gtk_image_menu_item_new_with_mnemonic((char *)text->data);

		if (cmd->type == CSTRING)
		{
			gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(item),
										  mgtk_create_icon((char *)cmd->data, 
														   GTK_ICON_SIZE_MENU));

#ifdef ACCEL_SUPPORT_ON
			if (accel != 0 &&
				accel->data != 0 &&
				((char *)accel->data)[0] != 0)
			{  
				GtkAccelGroup *accel_group;
				unsigned int i, len, key, mod;
				char *s = (char *)(accel->data);

				mgtk_print("Key accel %s", (char *)(accel->data));

				len = strlen(s);

				for (mod = 0, i = 0; i < len; ++i)
				{
					switch (s[i])
					{
					case 'C':
						mod |= GDK_CONTROL_MASK;
						break;
					case 'S':
						mod |= GDK_SHIFT_MASK;
						break;
					case 'M':
						mod |= GDK_MOD1_MASK;
						break;
					case 'E':
						key = GDK_Return;
						
						i = len + 8;
						break;
					case 'F':
						switch (s[i+1])
						{
						case '1':
							switch (s[i+2])
							{
							case '0':
								key = GDK_F10;
								break;
							case '1':
								key = GDK_F11;
								break;
							case '2':
								key = GDK_F12;
								break;
							default:
								key = GDK_F1;
								break;
							}
							break;
						case '2':
							key = GDK_F2;
							break;
						case '3':
							key = GDK_F3;
							break;
						case '4':
							key = GDK_F4;
							break;
						case '5':
							key = GDK_F5;
							break;
						case '6':
							key = GDK_F6;
							break;
						case '7':
							key = GDK_F7;
							break;
						case '8':
							key = GDK_F8;
							break;
						case '9':
							key = GDK_F9;
							break;
						}

						i = len + 8;
						break;
					case '-':
						break;
					default:
						key = gdk_unicode_to_keyval(s[i]);
						break;
					}
				}
				
				/* Add code here to translate accel string to GDK key / mods */

				accel_group = gtk_accel_group_new();
  
				// GDK_CONTROL_MASK | GDK_SHIFT_MASK | GDK_MOD1_MASK

				gtk_widget_add_accelerator(item, "activate", 
										   accel_group,
										   key, (GdkModifierType)mod,
										   GTK_ACCEL_VISIBLE);

				gtk_window_add_accel_group(GTK_WINDOW(mgtk_get_application_window()),
										   accel_group);
			}
#endif
		}

		new_adt(&ret, ARG_GTK_MENU_WIDGET, (void *)item); // ARG_GTK_MENU_WIDGET
		
		gtk_menu_append(GTK_MENU(menu->data), item);
		gtk_widget_show(item);
		
		agtk_event = rc_gtk_event_func(get_int(event));

		if (agtk_event)
		{
			gtk_signal_connect(GTK_OBJECT(item), "activate",
							   GTK_SIGNAL_FUNC(agtk_event), 
							   GINT_TO_POINTER(((cmd->type == INT) ? 
												get_int(cmd) : get_int(event))));
		}
	}

	delete_arg(&text);
	delete_arg(&event);
	delete_arg(&cmd);

	return ret;
}


arg_list_t *mgtk_rc_submenu(arg_list_t *menu)
{
	GtkWidget *submenu = NULL;
	GtkWidget *item = NULL;
	arg_list_t *label, *ret;
	int id = -1;


	if (!menu)
	{
		rc_assertion_error("submenu", "menu != NULL");
		return NULL;
	}

	arg_enforce_type(&menu, ARG_GTK_MENU_WIDGET); // ARG_GTK_MENU_WIDGET

	if (!menu)
	{
		rc_assertion_error("submenu", "menu == ARG_GTK_MENU_WIDGET");
		return NULL;
	}

	label = symbol();

	if (label->type == INT)
	{
		id = get_int(label);
		label = symbol();
	}

	arg_enforce_type(&label, CSTRING);
	ret = NULL;

	if (!label)
	{
		if (!label)
			rc_assertion_error("submenu", "label == CSTRING");
	}
	else
	{
		item = gtk_image_menu_item_new_with_mnemonic((char *)label->data);
		submenu = gtk_menu_new();
		gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), submenu);
		//gtk_label_set_mnemonic_widget(GTK_MENU(item)->label, submenu);

		if (GTK_IS_MENU_BAR(menu->data))
		{
			gtk_menu_append(GTK_MENU_BAR(menu->data), item);
		}
		else
		{
			gtk_menu_append(GTK_MENU(menu->data), item);
		}

		gtk_widget_show(item);
		

		if (id != -1)
		{
			// Mongoose 2002.02.14, Add this widget to a special 
			//   lookup table by it's event id
			mgtk_event_subscribe_gtk_widget(id, submenu);
		}

		new_adt(&ret, ARG_GTK_MENU_WIDGET, (void *)submenu);
	}

	delete_arg(&label);

	return ret;
}


arg_list_t *mgtk_rc_popup_menu(arg_list_t *arg)
{
	arg_list_t *ret;
	GtkWidget *menu = NULL;
	GtkWidget *app = NULL;


	arg_enforce_type(&arg, ARG_GTK_BOX_WIDGET);
	//arg_enforce_type(&arg,  ARG_GTK_WINDOW);

	if (!arg)
	{
		rc_assertion_error("popup_menu", "window == ARG_GTK_WINDOW");
		return NULL;
	}

	app = (GtkWidget *)arg->data;

	if (!app)
	{
		rc_assertion_error("popup_menu", "window != NULL");
		return NULL;
	}

	menu = gtk_menu_new();

	new_adt(&ret, ARG_GTK_MENU_WIDGET, (void *)menu);
	
	gtk_menu_attach_to_widget(GTK_MENU(menu), 
							  GTK_WIDGET(app),
							  mgtk_event_popup_menu_detacher);
	
	gtk_signal_connect_object(GTK_OBJECT(app), "destroy",
							  GTK_SIGNAL_FUNC(gtk_menu_detach),
							  GTK_OBJECT(menu));
	
	gtk_signal_connect_object(GTK_OBJECT(app), "button_press_event",
							  GTK_SIGNAL_FUNC(mgtk_event_popup_menu_handler), 
							  GTK_OBJECT(menu));
	
	return ret;
}


arg_list_t *mgtk_rc_menubar(arg_list_t *arg)
{
	arg_list_t *ret;
	GtkWidget *app = NULL;


	arg_enforce_type(&arg, ARG_GTK_BOX_WIDGET);

	if (!arg)
	{
		rc_assertion_error("menubar", "window == ARG_GTK_BOX_WIDGET");
		return NULL;
	}

	app = (GtkWidget *)arg->data;

	if (!app)
	{
		rc_assertion_error("menubar", "box != NULL");
		return NULL;
	}

	//GtkWidget *handlebox = gtk_handle_box_new();
	//gtk_widget_show(handlebox);
	//gtk_box_pack_start(GTK_BOX((GtkWidget *)arg->data), 
	//				   handlebox, FALSE, TRUE, 0);
	//gtk_handle_box_set_handle_position(GTK_HANDLE_BOX(handlebox),GTK_POS_LEFT);

	GtkWidget *menubar = gtk_menu_bar_new();
	gtk_widget_show(menubar);
	gtk_container_add(GTK_CONTAINER(arg->data), menubar);

	new_adt(&ret, ARG_GTK_MENU_WIDGET, (void *)menubar);
	return ret;
}


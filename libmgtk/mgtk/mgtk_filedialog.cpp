/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libmgtk
 * Author  : Mongoose
 * Website : http://www.icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (c) 2000-2006 Mongoose
 * Comments: 
 * 
 *-- History ------------------------------------------------- 
 *
 * 2006.12.19:
 * Mongoose - Clean up of old mgtk ( Resource, gtk+ Type ) filedialogs
 ==========================================================================*/

#include <string.h>
#include <gtk/gtk.h>
#include <mstl/Map.h>
#include <mstl/SystemIO.h>

#include "Resource.h"
#include "ResourceEvent.h"
#include "mgtk_interface.h"
#include "mgtk_callbacks.h"
#include "mgtk_resource.h"
#include "mgtk_events.h"


#define FILEDIALOG_IMAGE_PREVIEW

void mgtk_accel_support(GtkWidget *item, arg_list_t *accel);

using namespace mstl;


Map<long, GtkWidget *> gFileDialogWidgetMap;
Map<long, GtkWidget *> gFileDialogPatternWidgetMap;


GtkWidget *mgtk_get_fileselection_pattern_widget(int event)
{
	return gFileDialogPatternWidgetMap[event];
}


/////////////////////////////////////////////////////////////////////
// Gtk+ Events
/////////////////////////////////////////////////////////////////////


// File Selection

void mgtk_event_fileselection_pattern(int event, const char *pattern)
{
	printf("mgtk_event_fileselection_pattern(%i, %s)\n", event, pattern);

	GtkWidget *file = mgtk_get_fileselection_widget(event);
	gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(file), 
								gtk_file_chooser_get_filter(GTK_FILE_CHOOSER(file)));

	
}


void mgtk_event_fileselection_homedir(GtkWidget *file, void *data)
{
	char *path = mgtk_rc_map("/");
	
	if (path)
	{
		gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(data), path);
		delete [] path;
	}
}


void mgtk_event_file_dialog(int event, const char *title)
{
	GtkWidget *file = mgtk_get_fileselection_widget(event);
	
	gtk_window_set_title(GTK_WINDOW(file), title);
	gtk_widget_show(file);
}


const char *mgtk_event_fileselection_get_filter_name(int event)
{
	GtkFileChooser *chooser = GTK_FILE_CHOOSER(mgtk_get_fileselection_widget(event));
	GtkFileFilter *filter = gtk_file_chooser_get_filter(chooser);

	return filter ? gtk_file_filter_get_name(filter) : NULL;
}


void mgtk_event_filechooser_action(GtkWidget *widget, gpointer user_data)
{
	GtkWidget *file = mgtk_get_fileselection_widget(GPOINTER_TO_INT(user_data));
	gtk_widget_hide(file);
	char *filename = (char *)gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(file));
	int event = GPOINTER_TO_INT(user_data);

	GtkFileFilter *filter = gtk_file_chooser_get_filter(GTK_FILE_CHOOSER(file));
	char *name = filter ? (char *)gtk_file_filter_get_name(filter) : NULL;
	//printf("*** '%s'\n", name);

	char *tmp[] = { filename, name };

	mgtk_handle_text_array(event, 2, tmp);
}


void mgtk_event_filechooser_cancel(GtkWidget *widget, gpointer user_data)
{
	GtkWidget *file = mgtk_get_fileselection_widget(GPOINTER_TO_INT(user_data));

	if (file)
		gtk_widget_hide(file);
}


void mgtk_event_fileselection_set_dir(int event, const char *dir)
{
	GtkWidget *file = mgtk_get_fileselection_widget(event);
	
	if (!dir || !dir[0])
	{
		return;
	}

	gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(file), dir);
}


void mgtk_event_fileselection_append_pattern(int event, const char *label, const char *pattern)
{
	GtkWidget *file = mgtk_get_fileselection_widget(event);
	GtkFileFilter *filter = gtk_file_filter_new();
	gtk_file_filter_add_pattern(filter, (char*)pattern);
	gtk_file_filter_set_name(filter, (char*)label);
	gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(file), filter);
}


void mgtk_callback_free_image_data(guchar *pixels, gpointer data)
{
	if (pixels)
		delete [] (unsigned char *)pixels;
}


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


/////////////////////////////////////////////////////////////////////
// Gtk+ Widgets
/////////////////////////////////////////////////////////////////////

extern GtkWidget *GTK_MAIN_WINDOW;

void mgtk_filechooser_blocking_free(char *filename)
{
	if (filename)
	{
		g_free(filename);	
	}
}

char *mgtk_filechooser_blocking(const char *title, 
								const char *path, int type, 
								const char *filter_label, 
								const char *filter_pattern)
{
	GtkWidget *dialog = 
	gtk_file_chooser_dialog_new(title,
								GTK_WINDOW(GTK_MAIN_WINDOW),
								(type == 0) ? 
								GTK_FILE_CHOOSER_ACTION_OPEN :
								GTK_FILE_CHOOSER_ACTION_SAVE,
								GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
								(type == 0) ?
								GTK_STOCK_OPEN :
								GTK_STOCK_SAVE, GTK_RESPONSE_ACCEPT,
								NULL);

	// Path
	if (path && path[0])
	{
		gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(dialog), path);
	}

	// Overwrite
#if GTK_MAJOR_VERSION == 2 && GTK_MINOR_VERSION >= 8
	gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(dialog), TRUE);
#endif

	// Shortcut
	char *spath = mgtk_rc_map("/");
		
	if (spath)
	{
		gtk_file_chooser_add_shortcut_folder(GTK_FILE_CHOOSER(dialog), spath, NULL);
		delete [] spath;
	}

	// File filter
	if (filter_label && filter_label[0] &&
		filter_pattern && filter_pattern[0])
	{
		// Custom filter
		GtkFileFilter *filter2 = gtk_file_filter_new();
		gtk_file_filter_set_name(filter2, "All Files (*.*)");
		gtk_file_filter_add_pattern(filter2, "*.*");
		gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter2);

		// Generic All Files filter
		GtkFileFilter *filter = gtk_file_filter_new();
		gtk_file_filter_set_name(filter, (char*)filter_label);
		gtk_file_filter_add_pattern(filter, (char*)filter_pattern);
		gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

		// Select custom filter
		gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dialog), filter);
	}

	char *filename = NULL;

	if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT)
	{
		filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
	}

	gtk_widget_destroy(dialog);
 
#if GTK_MAJOR_VERSION == 2 && GTK_MINOR_VERSION < 8
	// Custom overwrite dialog, since the Gtk+ for Windows doesn't support it.
	if (filename && type == 1 && mstl::SystemIO::File::DoesFileExist(filename))
	{
		mstl::String s;
		s.Set("You are about to overwrite '%s'.", filename);
		
		if (!mgtk_create_confirm_dialog("gtk-question", 
										s.c_str(),
										"Do you want to overwrite?",
										"gtk-cancel", "cancel",
										"gtk-ok", "Overwrite") )
		{
			// Returning NULL to cancel the overwriting the file.
			g_free(filename);
			filename = NULL;
		}
	}
#endif

	return filename;
}


GtkWidget *mgtk_create_filechooser(int event, const char *title)
{
	GtkWidget *filechooser = NULL;
	GtkWidget *vbox;
	GtkWidget *action_area;
	GtkWidget *cancel_button;
	GtkWidget *ok_button;


	filechooser = gtk_file_chooser_dialog_new(title, NULL, GTK_FILE_CHOOSER_ACTION_OPEN, NULL, NULL);
	//gtk_file_chooser_set_do_overwrite_confirmation(GTK_FILE_CHOOSER(filechooser), TRUE);
	gtk_window_set_type_hint(GTK_WINDOW(filechooser), GDK_WINDOW_TYPE_HINT_DIALOG);

	vbox = GTK_DIALOG(filechooser)->vbox;
	gtk_widget_show(vbox);

	action_area = GTK_DIALOG(filechooser)->action_area;
	gtk_widget_show(action_area);
	gtk_button_box_set_layout(GTK_BUTTON_BOX(action_area), GTK_BUTTONBOX_END);

	/* Add cancel button */
	cancel_button = gtk_button_new_from_stock("gtk-cancel");
	gtk_widget_show(cancel_button);
	gtk_dialog_add_action_widget(GTK_DIALOG(filechooser), cancel_button, GTK_RESPONSE_CANCEL);
	GTK_WIDGET_SET_FLAGS(cancel_button, GTK_CAN_DEFAULT);

	gtk_signal_connect(GTK_OBJECT(cancel_button), "clicked",
					   GTK_SIGNAL_FUNC(mgtk_event_filechooser_cancel),
					   GINT_TO_POINTER(event));

	/* Add ok button -- this is used due to multiuse of single filedialog in mtk */
	ok_button = gtk_button_new_from_stock("gtk-ok");
	gtk_widget_show(ok_button);
	gtk_dialog_add_action_widget(GTK_DIALOG(filechooser), ok_button, GTK_RESPONSE_OK);
	GTK_WIDGET_SET_FLAGS(ok_button, GTK_CAN_DEFAULT);

	gtk_signal_connect(GTK_OBJECT(ok_button), "clicked",
					   GTK_SIGNAL_FUNC(mgtk_event_filechooser_action),
					   GINT_TO_POINTER(event));

	gtk_widget_grab_default(ok_button);


	/*
	gtk_signal_connect(GTK_OBJECT(filechooser), "file-activated",
					   GTK_SIGNAL_FUNC(mgtk_event_filechooser_action),
					   GINT_TO_POINTER(event));
	*/

	/* Add preview widget for images */
	GtkWidget *preview = gtk_image_new();
	gtk_file_chooser_set_preview_widget(GTK_FILE_CHOOSER(filechooser), preview);
	g_signal_connect(filechooser, "update-preview",
					 G_CALLBACK(mgtk_update_filechooser_preview), preview);

	return filechooser;
}



/////////////////////////////////////////////////////////////////////
// Gtk+ bridge to mlisp
/////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////
// File chooser init
///////////////////////////////////////////////////////////////////////////

void mgtk_filechooser_spawn_event(GtkWidget *widget, gpointer user_data)
{
	GtkWidget *dialog = gFileDialogWidgetMap[GPOINTER_TO_INT(user_data)];
	//DEBUG_MSG("%p %i\n", widget, user_data, dialog);

	if (dialog)
		gtk_widget_show_all(dialog);
}

void mgtk_filechooser_close_event(GtkWidget *widget, gpointer user_data)
{
	GtkWidget *dialog = gFileDialogWidgetMap[GPOINTER_TO_INT(user_data)];
	//DEBUG_MSG("%p %i %p\n", widget, user_data, dialog);

	if (dialog)
		gtk_widget_hide_all(dialog);
}


GtkWidget *mgtk_link_filechooser_from_rc(int event, const char *title, const char *option)
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
				else if (strcmp(symbol, "ext") == 0)
				{
					// Custom filter
					GtkFileFilter *filter = gtk_file_filter_new();
					gtk_file_filter_set_name(filter, value);
					gtk_file_filter_add_pattern(filter, value);
					gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);
					// Select custom filter
					gtk_file_chooser_set_filter(GTK_FILE_CHOOSER(dialog), filter);
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


GtkWidget *mgtk_get_fileselection_widget(int event)
{
	GtkWidget *file = gFileDialogWidgetMap[event];
	char *path;

	if (!file)
	{
		file = mgtk_create_filechooser(event, "Select file");

		gFileDialogWidgetMap.Add(event, file);

		path = mgtk_rc_map("/");
		
		if (path)
		{
			gtk_file_chooser_set_current_folder(GTK_FILE_CHOOSER(file), path);
			gtk_file_chooser_add_shortcut_folder(GTK_FILE_CHOOSER(file), path,
												 NULL);
			gtk_file_chooser_set_action(GTK_FILE_CHOOSER(file), GTK_FILE_CHOOSER_ACTION_SAVE);
			delete [] path;
		}

#ifdef FILEDIALOG_IMAGE_PREVIEW
		GtkWidget *preview = gtk_image_new();
		gtk_file_chooser_set_preview_widget(GTK_FILE_CHOOSER(file), preview);
		g_signal_connect(file, "update-preview",
						 G_CALLBACK (mgtk_update_filechooser_preview), preview);
#endif
	}

	return file;
}


////////////////////////////////////////////////////////////
// MLISP RC implementations
////////////////////////////////////////////////////////////


// Hacky extention
arg_list_t *mgtk_rc_fileselection_drop_down_menu(arg_list_t *box)
{
	arg_list_t *event, *ret = NULL;
	
	mgtk_print("fileselection_drop_down_menu entry");

	event = symbol();
	arg_enforce_type(&event, INT);
	MGTK_ASSERTMSG(event, "event == INT");

	GtkWidget *dd = event ? gFileDialogPatternWidgetMap[get_int(event)] : NULL;
	MGTK_ASSERTMSG(dd, "fileselection_drop_down_menu != NULL");

	if (!dd)
	{
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

	arg_enforce_type(&box, ARG_GTK_BOX_WIDGET); // ARG_GTK_BOX_WIDGET

	if (!box)
	{
		MGTK_ASSERTMSG("mgtk_rc_filechooserbutton", "container == ARG_GTK_BOX_WIDGET");
		return NULL;
	}

	label = symbol();
	arg_enforce_type(&label, CSTRING);
	MGTK_ASSERTMSG(label, "label == CSTRING");

	title = symbol();
	arg_enforce_type(&title, CSTRING);
	MGTK_ASSERTMSG(title, "title == STRING");

	event = symbol();
	arg_enforce_type(&event, INT);
	MGTK_ASSERTMSG(event, "event == INT");

	pattern_func = symbol();
	arg_enforce_type(&pattern_func, CSTRING);
	MGTK_ASSERTMSG(pattern_func, "pattern_func == CSTRING");

	if (label && title && event && pattern_func)
	{
		mgtk_link_filechooser_from_rc(get_int(event), get_string(title), get_string(pattern_func));

		GtkWidget *item = gtk_button_new_with_label(get_string(label));
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
		
		new_adt(&ret, ARG_GTK_WIDGET, (void *)item); // ARG_GTK_WIDGET
	}

	delete_arg(&label);
	delete_arg(&title);
	delete_arg(&event);
	delete_arg(&pattern_func);

	return ret;
}


// (filechoosertoolbar_button "icon" "name" "tooltip" "Title..." eEvent "your_mgtk_rc_pattern_callback") 
arg_list_t *mgtk_rc_filechoosertoolbar_button(arg_list_t *box)
{
	arg_list_t *ret = NULL, *icon, *label, *help, *title, *event, *options;

	arg_enforce_type(&box, ARG_GTK_TOOLBOX_WIDGET);
	MGTK_ASSERTMSG(box, "box == ARG_GTK_TOOLBOX_WIDGET");

	if (!box)
	{
		return NULL;
	}

	symbol_enforce_type(&icon, CSTRING);
	MGTK_ASSERTMSG(icon, "icon == CSTRING");
	
	symbol_enforce_type(&label, CSTRING);
	MGTK_ASSERTMSG(label, "label == CSTRING");
	
	symbol_enforce_type(&help, CSTRING);
	MGTK_ASSERTMSG(help, "help == CSTRING");
	
	symbol_enforce_type(&title, CSTRING);
	MGTK_ASSERTMSG(title, "title == CSTRING");
	
	symbol_enforce_type(&event, INT);
	MGTK_ASSERTMSG(event, "event == INT");
	
	symbol_enforce_type(&options, CSTRING);
	MGTK_ASSERTMSG(options, "options == CSTRING");
			
	if (icon && label && help && title && event && options)
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

		GtkWidget *button =
		mgtk_create_toolbar_button((GtkWidget *)box->data, 0, icon_filename, 
								   get_string(label), get_string(help),
								   NULL, GPOINTER_TO_INT(get_int(event)));
		

		gtk_signal_connect(GTK_OBJECT(button), "clicked",
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



// (filechoosermenu_item "Label" "Title..." [F1] eFileDialogEvent "your_mgtk_rc_pattern_callback" "icon") 
arg_list_t *mgtk_rc_filechoosermenu_item(arg_list_t *menu)
{
	GtkWidget *item;
	arg_list_t *ret = NULL;

	arg_enforce_type(&menu,  ARG_GTK_MENU_WIDGET);
	MGTK_ASSERTMSG(menu, "menu == ARG_GTK_MENU_WIDGET");

	if (!menu)
	{
		return NULL;
	}

	arg_list_t *text = symbol();
	arg_enforce_type(&text,  CSTRING);
	MGTK_ASSERTMSG(text, "text == STRING");

	arg_list_t *title = symbol();
	arg_enforce_type(&title, CSTRING);
	MGTK_ASSERTMSG(title, "title == STRING");

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
	MGTK_ASSERTMSG(event, "event == INT");

	arg_list_t *pattern_func = symbol();
	arg_enforce_type(&pattern_func, CSTRING);
	MGTK_ASSERTMSG(pattern_func, "pattern_func == CSTRING");

	arg_list_t *icon = symbol();
	arg_enforce_type(&icon, CSTRING);
	MGTK_ASSERTMSG(icon, "icon == STRING");

	if (text && event && pattern_func && icon)
	{
		item = gtk_image_menu_item_new_with_mnemonic((char *)text->data);
		gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(item),
									  mgtk_create_icon((char *)icon->data, 
													   GTK_ICON_SIZE_MENU));

		// Accel support
		mgtk_accel_support(item, accel);

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

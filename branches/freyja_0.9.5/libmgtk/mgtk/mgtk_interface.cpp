/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libmgtk
 * Author  : Mongoose
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (c) 2000-2004 Mongoose
 * Comments: This is the gtk+ wrapper
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
 * 2004.06.01:
 * Mongoose - Last of freyja use removed, project desn changed
 *
 * 2004.04.11:
 * Mongoose - Removed GNOME support
 *
 * 2000.09.16:
 * Mongoose - Created
 ==========================================================================*/

#include <sys/types.h>
#include <sys/stat.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#include <mstl/String.h>
#include <mstl/SystemIO.h>

#include "mgtk_interface.h"
#include "mgtk_callbacks.h"
#include "mgtk_resource.h"
#include "mgtk_events.h"


GdkPixbuf* mgtk_create_pixbuf( const char* icon_filename )
{
	GdkPixbuf* icon = NULL;

	if ( icon_filename && icon_filename[0] )
	{
		GError* error = NULL;
		icon = gdk_pixbuf_new_from_file( icon_filename, &error );

		// Try to find missing icons via relative path in resource system.
		if ( !icon )
		{
			g_error_free( error );
			GError* error = NULL;

			const char* tmp = mgtk_rc_map( icon_filename );
			icon = gdk_pixbuf_new_from_file( tmp, &error );
			delete [] tmp;
		}

		if ( !icon )
		{
			fprintf(stderr, "Failed to load icon file: %s: %s\n",
					icon_filename, error->message);
			g_error_free( error );
		}
	}

	return icon;
}


void mgtk_create_window_icon(GtkWidget *window, GdkPixbuf *icon)
{
	if (window && icon)
	{
		gtk_window_set_icon(GTK_WINDOW(window), icon);
		gdk_pixbuf_unref(icon);
	}
}


void mgtk_destroy_pixbuf(GdkPixbuf *icon)
{
	if (icon)
	{
		gdk_pixbuf_unref(icon);
	}
}


GtkWidget* mgtk_create_icon(const char* icon_filename, GtkIconSize icon_size)
{
	GtkWidget* icon = NULL;

	/* If there is a leading "gtk" assume it's stock, since there is no path */
	if ( !strncmp("gtk", icon_filename, 3) )
	{
		icon = gtk_image_new_from_stock(icon_filename, icon_size);
	}
	else
	{
		if ( !mstl::SystemIO::File::DoesFileExist( icon_filename ) )
		{
			// Try to find missing icons via relative path in resource system.
			const char* tmp = mgtk_rc_map( icon_filename );
			icon = gtk_image_new_from_file( tmp );
			delete [] tmp;
		}
		else
		{
			icon = gtk_image_new_from_file( icon_filename );
		}
	}

	return icon;
}


/////////////////////////////////////////////////////////////
// Dialogs
/////////////////////////////////////////////////////////////

int mgtk_create_confirm_dialog(const char *dialog_icon,
							   const char *information_message, 
							   const char *question_message,
							   const char *cancel_icon, const char *cancel_text,
							   const char *accept_icon, const char *accept_text)
{
	GtkWidget *dialog = gtk_dialog_new();

	GtkWidget *hbox = gtk_hbox_new(FALSE, 0);
	gtk_widget_ref(hbox);
	gtk_object_set_data_full(GTK_OBJECT(GTK_DIALOG(dialog)->vbox), 
							 "hbox1", hbox, (GtkDestroyNotify)gtk_widget_unref);
	gtk_widget_show(hbox);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), 
					   hbox, FALSE, TRUE, 0);

	GtkWidget *icon = mgtk_create_icon(dialog_icon, GTK_ICON_SIZE_DIALOG);

	GtkWidget *info = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(info), information_message);
	gtk_label_set_selectable(GTK_LABEL(info), TRUE);

	GtkWidget *question = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(question), question_message);
	gtk_label_set_selectable(GTK_LABEL(question), TRUE);

	gtk_container_add(GTK_CONTAINER(hbox), icon);
	gtk_container_add(GTK_CONTAINER(hbox), info);
	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), question);

	if (cancel_text && cancel_text[0])
	{
		GtkWidget *cancel = gtk_dialog_add_button(GTK_DIALOG(dialog), cancel_text, 0);

		if (cancel)
		{
			icon = mgtk_create_icon(cancel_icon, GTK_ICON_SIZE_LARGE_TOOLBAR);

			if (icon)
				gtk_button_set_image(GTK_BUTTON(cancel), icon);
		}
	}

	GtkWidget *accept = gtk_dialog_add_button(GTK_DIALOG(dialog), accept_text, GTK_RESPONSE_ACCEPT);

	/* Add icons to buttons */
	icon = mgtk_create_icon(accept_icon, GTK_ICON_SIZE_LARGE_TOOLBAR);
	if (icon && accept)
		gtk_button_set_image(GTK_BUTTON(accept), icon);

	gtk_widget_show_all(dialog);

	/* Force user to close this dialog by stopping other events */
	int ret = 0;

	switch (gtk_dialog_run(GTK_DIALOG(dialog)))
	{      
	case GTK_RESPONSE_ACCEPT:
		ret = 1;
		break;
	default:
		ret = 0;
	}

	gtk_widget_destroy(dialog);

	return ret;
}



void mgtk_create_info_dialog(const char *dialog_icon, const char *message)
{
	GtkWidget* about = gtk_dialog_new();	
	GtkWidget* icon = mgtk_create_icon(dialog_icon, GTK_ICON_SIZE_DIALOG);
	GtkWidget* label = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(label), message);
	gtk_label_set_selectable(GTK_LABEL(label), TRUE);

	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(about)->vbox), icon);
	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(about)->vbox), label);
	gtk_dialog_add_button(GTK_DIALOG(about), GTK_STOCK_CLOSE, 1);
	gtk_widget_show_all(about);

	/* Force user to close this dialog by stopping other events */
	gtk_dialog_run(GTK_DIALOG(about));
	gtk_widget_destroy(about);
}


void mgtk_create_query_dialog_text(const char *image, const char *message, 
								   int eventId, const char *value)
{
	GtkWidget *dialog = gtk_dialog_new();	
	GtkWidget *icon = mgtk_create_icon(image, GTK_ICON_SIZE_DIALOG);
	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), icon);
	GtkWidget *label = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(label), message);
	gtk_label_set_selectable(GTK_LABEL(label), TRUE);
	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), label);

	/* Setup text view widget */
	GtkWidget *view = gtk_text_view_new();
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));

	if (value)
	{
		gtk_text_buffer_set_text(buffer, value, -1);	
	}

	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), view);
	gtk_dialog_add_button(GTK_DIALOG(dialog), GTK_STOCK_OK, 1);
	gtk_widget_show_all(dialog);

	/* Force user to close this dialog by stopping other events */
	gtk_dialog_run(GTK_DIALOG(dialog));

	/* Emit signal with text data copied via property... */
	gchar *text;
	g_object_get(GTK_OBJECT(buffer),
				 "text", &text,
				 NULL);

 	mgtk_handle_text(eventId, text);
	g_free(text);

	gtk_widget_destroy(dialog);
}


float mgtk_create_query_dialog_float(const char *image, const char *message, 
									 float value, float min, float max, 
									 float step, int digits)
{
	GtkWidget *dialog = gtk_dialog_new();	
	GtkWidget *icon = mgtk_create_icon(image, GTK_ICON_SIZE_DIALOG);
	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), icon);
	GtkWidget *label = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(label), message);
	gtk_label_set_selectable(GTK_LABEL(label), TRUE);
	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), label);

	GtkWidget *spinbutton = mgtk_create_spinbutton2(GTK_DIALOG(dialog)->vbox, "spin", value, min, max, step, step*10, step*10, digits);
	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), spinbutton);

	gtk_dialog_add_button(GTK_DIALOG(dialog), GTK_STOCK_OK, 1);
	gtk_widget_show_all(dialog);

	/* Force user to close this dialog by stopping other events */
	gtk_dialog_run(GTK_DIALOG(dialog));

	value = gtk_spin_button_get_value_as_float(GTK_SPIN_BUTTON(spinbutton));

	gtk_widget_destroy(dialog);

	return value;
}


GtkWidget *mgtk_create_color_picker_dialog(const char *title, void *event_func)
{
	GtkWidget *dialog =  gtk_color_selection_dialog_new(title);
	gtk_widget_show(dialog);

	if (event_func)
	{
		gtk_signal_connect(GTK_OBJECT(GTK_COLOR_SELECTION_DIALOG(dialog)->cancel_button),
						   "pressed",
						   GTK_SIGNAL_FUNC(event_func),
						   GINT_TO_POINTER(0));

		gtk_signal_connect(GTK_OBJECT(GTK_COLOR_SELECTION_DIALOG(dialog)->ok_button),
						   "pressed",
						   GTK_SIGNAL_FUNC(event_func),
						   GINT_TO_POINTER(1));

		gtk_signal_connect(GTK_OBJECT(GTK_COLOR_SELECTION_DIALOG(dialog)->help_button), 
						   "pressed",
						   GTK_SIGNAL_FUNC(event_func),
						   GINT_TO_POINTER(2));
	}

	return dialog;
}


/////////////////////////////////////////////////////////////
// Widgets
/////////////////////////////////////////////////////////////

GtkWidget *mgtk_create_text_entry(GtkWidget *box)
{
	GtkWidget *entry = gtk_entry_new();
	gtk_widget_ref(entry);
	gtk_object_set_data_full(GTK_OBJECT(box), "entry", entry,
									 (GtkDestroyNotify)gtk_widget_unref);
	gtk_widget_show(entry);
	gtk_container_add(GTK_CONTAINER(box), entry);

	return entry;
}


GtkWidget *mgtk_create_vbox(GtkWidget *box, char *name, 
							bool homogeneous, int spacing,
							bool expand, bool fill, int pading)
{
	GtkWidget *vbox = gtk_vbox_new(homogeneous ? TRUE : FALSE, spacing);
	gtk_widget_ref(vbox);
	gtk_object_set_data_full(GTK_OBJECT(box), name, vbox,
									 (GtkDestroyNotify)gtk_widget_unref);
	gtk_widget_show(vbox);

	gtk_container_add(GTK_CONTAINER(box), vbox);

	return vbox;
}


GtkWidget *mgtk_create_hbox(GtkWidget *box, char *name, 
							bool homogeneous, int spacing,
							bool expand, bool fill, int pading)
{
	GtkWidget* hbox = gtk_hbox_new((homogeneous ? TRUE : FALSE), spacing);
	gtk_widget_ref(hbox);
	gtk_object_set_data_full(GTK_OBJECT(box), name, hbox,
							 (GtkDestroyNotify) gtk_widget_unref);
	gtk_widget_show(hbox);

	gtk_box_pack_start(GTK_BOX(box), 
					   hbox, expand ? TRUE : FALSE, 
					   fill ? TRUE : FALSE, 
					   pading);

	return hbox;
}


GtkWidget *mgtk_create_spinbutton2(GtkWidget *master, char *name, 
								   float val, float min, float max,
								   float step, float page, float page_sz, 
								   int digits)
{  
	GtkObject *spinbutton_adj = gtk_adjustment_new(val, min, max, step, page, page_sz);
	GtkWidget *spinbutton = gtk_spin_button_new(GTK_ADJUSTMENT(spinbutton_adj), 1, digits);
	gtk_widget_ref(spinbutton);
	gtk_object_set_data_full(GTK_OBJECT(master), name, spinbutton,
							 (GtkDestroyNotify)gtk_widget_unref);
	gtk_widget_show(spinbutton);

	return spinbutton;
}


GtkWidget *mgtk_create_spinbutton(GtkWidget *master, char *name, 
								  float val, float min, float max)
{
	return mgtk_create_spinbutton2(master, name, val, min, max, 1, 10, 10, 0);
}


GtkWidget *mgtk_create_label(GtkWidget *master, const char *name, 
							 const char *text, float x_align, float y_align)
{
  GtkWidget *label = gtk_label_new(text);

  gtk_widget_ref(label);
  gtk_object_set_data_full(GTK_OBJECT(master), name, label,
			   (GtkDestroyNotify)gtk_widget_unref);
  gtk_widget_show(label);
  gtk_misc_set_alignment(GTK_MISC(label), x_align, y_align);

  return label;
}


GtkWidget *mgtk_create_tab(GtkWidget *notebook, const char *name, 
						   GtkWidget *tab_contents, const char *label_text,
						   int tab_num)
{
	// Setup tab_contents
	gtk_widget_ref(tab_contents);
	gtk_object_set_data_full(GTK_OBJECT(notebook), name, 
							 tab_contents,
							 (GtkDestroyNotify)gtk_widget_unref);
	gtk_widget_show(tab_contents);
	gtk_container_add(GTK_CONTAINER(notebook), tab_contents);
	

	// Tab setup in notebook
	GtkWidget *label = mgtk_create_label(notebook, name, label_text, 0, 0.5);
	gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook),
							   gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook),
														tab_num), label);
	return NULL;
}


GtkWidget *mgtk_create_toolbar(GtkWidget *box)
{
	GtkWidget *toolbar = gtk_toolbar_new();

	gtk_widget_ref(toolbar);
	gtk_object_set_data_full(GTK_OBJECT(box), "tbar", toolbar,
							 (GtkDestroyNotify)gtk_widget_unref);
	gtk_widget_show(toolbar);
	gtk_box_pack_start(GTK_BOX(box), toolbar, 0, 0, 0);

#ifdef FORCE_ICON_TOOLBAR
	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);
#endif
	gtk_toolbar_set_tooltips(GTK_TOOLBAR(toolbar), TRUE);

	return toolbar;
}


GtkWidget *mgtk_create_color_button(void *func, int id)
{
#ifdef DISABLE_GTK_COLORBUTTON	
	GtkWidget *label = gtk_label_new(NULL);

	gtk_label_set_markup(GTK_LABEL(label), "ColorButton disabled");
	gtk_label_set_selectable(GTK_LABEL(label), TRUE);
	return label;
#else
	GtkWidget *colorbutton = gtk_color_button_new();
	gtk_widget_show(colorbutton);
	gtk_color_button_set_use_alpha(GTK_COLOR_BUTTON(colorbutton), TRUE);

	//g_signal_connect((gpointer)colorbutton, "color_set",
	//				 G_CALLBACK(func), GINT_TO_POINTER(id));

	gtk_signal_connect(GTK_OBJECT(colorbutton), "color_set",
					   GTK_SIGNAL_FUNC(func), 
					   GINT_TO_POINTER(id));

	return colorbutton;
#endif
}


GtkWidget* mgtk_create_toolbar_toogle_button(GtkWidget *toolbar,  bool toggled,
											 const char* icon, const char* label,
											 const char* help, 
											 void* event_func, int event_cmd)
{
	GtkWidget *gicon = mgtk_create_icon(icon, GTK_ICON_SIZE_LARGE_TOOLBAR);
	GtkToolItem *item = gtk_toggle_tool_button_new();
	GtkWidget *togglebutton = (GtkWidget *)item;

	gtk_tool_button_set_label((GtkToolButton*)item, (!label[0]) ? NULL : label);
	gtk_tool_button_set_icon_widget((GtkToolButton*)item, gicon);
	gtk_widget_show(gicon);
	//gtk_toolbar_append_widget(GTK_TOOLBAR(toolbar), togglebutton, help, NULL);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1); // Append to the end

	GtkTooltips *tips = gtk_tooltips_new();
	gtk_tool_item_set_tooltip(item, tips, help, NULL);

	gtk_widget_ref(togglebutton);
	gtk_object_set_data_full(GTK_OBJECT(toolbar), "tb_tbtn",
							 togglebutton,
							 (GtkDestroyNotify)gtk_widget_unref);
	gtk_widget_show(togglebutton);

	return togglebutton;
}


GtkWidget *mgtk_create_toolbar_button(GtkWidget *toolbar, int is_menu,
									  const char *icon, const char *label, const char *help, 
									  void *event_func, int event_cmd)
{
	GtkWidget *gicon = mgtk_create_icon(icon, GTK_ICON_SIZE_LARGE_TOOLBAR);
	GtkToolItem *item = ((is_menu) ? 
						 gtk_menu_tool_button_new(gicon, (!label[0]) ? NULL : label) :
						 gtk_tool_button_new(gicon, (!label[0]) ? NULL : label) );

	GtkWidget *button = (GtkWidget *)item;
	gtk_widget_show(gicon);

	//gtk_toolbar_append_widget((GtkToolbar *)toolbar, button, help, NULL);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1); // Append to the end

	GtkTooltips* tips = gtk_tooltips_new();
	gtk_tool_item_set_tooltip(item, tips, help, NULL);

	if (event_func)
	{
		gtk_signal_connect(GTK_OBJECT(button), "clicked",
						   GTK_SIGNAL_FUNC(event_func),
						   GINT_TO_POINTER(event_cmd));
	}

	gtk_widget_ref(button);
	gtk_object_set_data_full(GTK_OBJECT(toolbar), "tb_btn",
							 button, (GtkDestroyNotify)gtk_widget_unref);
	gtk_widget_show(button);

	return button;
}


GtkWidget *mgtk_create_window(char *title, char *wmclass, char *icon_name)
{
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_object_set_data(GTK_OBJECT(window), "window", window);
	gtk_window_set_policy(GTK_WINDOW(window), FALSE, TRUE, FALSE);
	gtk_window_set_wmclass(GTK_WINDOW(window), wmclass, wmclass);
	gtk_window_set_title(GTK_WINDOW(window), title);

	/* Setup events for this window */
	gtk_widget_set_events(GTK_WIDGET(window),
						  GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK);

	GdkPixbuf *icon = mgtk_create_pixbuf(icon_name);
	mgtk_create_window_icon(window, icon);
	//mgtk_destroy_pixbuf(icon);

	return window;
}


void mgtk_accel_support(GtkWidget *item, const char* accel)
{
	if ( accel )
	{  
		GtkAccelGroup *accel_group;
		unsigned int i, len, key = 0, mod;
		const char* s = accel;
		
		mgtk_print("Key accel %s\n", accel);
		
		len = strlen(s);

		for (mod = 0, i = 0; i < len; ++i)
		{
			switch ( s[i] )
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
			case 'E': // Enter
				key = GDK_Return;
						
				i = len + 8;
				break;
			case 'D': // Delete
				key = GDK_Delete;
						
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
}

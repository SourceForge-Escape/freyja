/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : 
 * License : GPL See file COPYING, also (C) 2000, 2001 Mongoose
 * Comments: This is the main client entry.
 *
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.04.11:
 * Mongoose - Removed GNOME support
 *
 * ????.??.??:
 * Mongoose - Created
 =================================================================*/

#ifdef USING_GTK_1_2
#   include <gtk/gtk.h>
#elif USING_GTK_2_0
#   include <gtk/gtk.h>
#else
#   error "This MLISP wrapper only supports GTK+ 1.2 and GTK+ 2.x"
#endif

#include <sys/types.h>
#include <sys/stat.h> 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "gnome_callbacks.h"
#include "gnome_interface.h"
#include "gnome_gtkglarea.h"
#include "gnome_resource.h"
#include "freyja_events.h"

#define AUTHOR "Terry 'Mongoose' Hendrix II <mongoose@users.sourceforge.net>"
#define COPYRIGHT_NOTICE "Copyright (c) 2000-2002 by Terry 'Mongoose' Hendrix II"
#define ABOUT_RANT "Freyja is an Open Source 3d modeler, animator, and particle editor.\nSend bug reports and feature requests via email, irc, or use the bug forms at the web site:\n* http://gooseegg.sourceforge.net\n* irc://irc.linux.com/#freyja\n* mongoose@users.sourceforge.net"


// Mongoose 2002.01.21, FIXME should use rc_mapped pathname
void demangle_pixmap_name(char *dest, char *icon_name)
{
#ifdef unix
	snprintf(dest, 254, "%s/.freyja/icons/%s", 
			  (char *)getenv("HOME"), icon_name);
#else
	strcpy(dest, "data/icons/%s", icon_name);
#endif
}


/////////////////////////////////////////////////////////////


GtkWidget *text_entry_create(GtkWidget *box)
{
	GtkWidget *entry;


	entry = gtk_entry_new();
	gtk_widget_ref(entry);
	gtk_object_set_data_full(GTK_OBJECT(box), "entry", entry,
									 (GtkDestroyNotify)gtk_widget_unref);
	gtk_widget_show(entry);
	gtk_container_add(GTK_CONTAINER(box), entry);

	return entry;
}


GtkWidget *packed_vbox_create(GtkWidget *box, char *name, 
										bool homogeneous, int spacing,
										bool expand, bool fill, int pading)
{
	GtkWidget *vbox;

	
	vbox = gtk_vbox_new(homogeneous ? TRUE : FALSE, spacing);
	gtk_widget_ref(vbox);
	gtk_object_set_data_full(GTK_OBJECT(box), name, vbox,
									 (GtkDestroyNotify)gtk_widget_unref);
	gtk_widget_show(vbox);

	gtk_container_add(GTK_CONTAINER(box), vbox);

	return vbox;
}


GtkWidget *packed_hbox_create(GtkWidget *box, char *name, 
										bool homogeneous, int spacing,
										bool expand, bool fill, int pading)
{
	GtkWidget *hbox;
	

	hbox = gtk_hbox_new((homogeneous ? TRUE : FALSE), spacing);

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


GtkWidget *spinbutton2_create(GtkWidget *master, char *name, 
										float val, float min, float max,
										float step, float page, float page_sz, int digits)
{  
  GtkObject *spinbutton_adj;
  GtkWidget *spinbutton;


  spinbutton_adj = gtk_adjustment_new(val, min, max, step, page, page_sz);
  spinbutton = gtk_spin_button_new(GTK_ADJUSTMENT(spinbutton_adj), 1, digits);
  gtk_widget_ref(spinbutton);
  gtk_object_set_data_full(GTK_OBJECT(master), name, spinbutton,
			   (GtkDestroyNotify)gtk_widget_unref);
  gtk_widget_show(spinbutton);

  return spinbutton;
}


GtkWidget *spinbutton_create(GtkWidget *master, char *name, 
									  float val, float min, float max)
{
	return spinbutton2_create(master, name, val, min, max, 1, 10, 10, 0);
}


GtkWidget *label_create(GtkWidget *master, char *name, 
								char *text, float x_align, float y_align)
{
  GtkWidget *label;


  label = gtk_label_new(text);

  gtk_widget_ref(label);
  gtk_object_set_data_full(GTK_OBJECT(master), name, label,
			   (GtkDestroyNotify)gtk_widget_unref);
  gtk_widget_show(label);
  gtk_misc_set_alignment(GTK_MISC(label), x_align, y_align);

  return label;
}


GtkWidget *tab_create(GtkWidget *notebook,  char *name, 
					  GtkWidget *tab_contents, char *label_text, int tab_num)
{
	GtkWidget *label;


	// Setup tab_contents
	gtk_widget_ref(tab_contents);
	gtk_object_set_data_full(GTK_OBJECT(notebook), name, 
							 tab_contents,
							 (GtkDestroyNotify)gtk_widget_unref);
	gtk_widget_show(tab_contents);
	gtk_container_add(GTK_CONTAINER(notebook), tab_contents);
	

	// Tab setup in notebook
	label = label_create(notebook, name, label_text, 0, 0.5);
	gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook),
							   gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook),
														tab_num), label);
	return NULL;
}


GtkWidget *fileselection_create(char *title)
{
	GtkWidget *fileselection;
	GtkWidget *ok_button;
	GtkWidget *cancel_button;
	GtkWidget *main_vbox;


	fileselection = gtk_file_selection_new(title);

	gtk_object_set_data(GTK_OBJECT(fileselection), "fileselection", 
							  fileselection);
	gtk_container_set_border_width(GTK_CONTAINER(fileselection), 10);

	// Mongoose 2002.01.18, Don't allow resize/grow
	gtk_window_set_policy(&(GTK_DIALOG(fileselection)->window), 
								 FALSE, FALSE, TRUE);

	// OK button
	ok_button = GTK_FILE_SELECTION(fileselection)->ok_button;
	gtk_object_set_data(GTK_OBJECT(fileselection), "ok_button", ok_button);
	gtk_widget_show(ok_button);
	GTK_WIDGET_SET_FLAGS(ok_button, GTK_CAN_DEFAULT);

	gtk_signal_connect(GTK_OBJECT(ok_button), "clicked",
							 GTK_SIGNAL_FUNC(fileselection_open_event),
							 NULL);


	// Cancel button
	cancel_button = GTK_FILE_SELECTION(fileselection)->cancel_button;
	gtk_object_set_data(GTK_OBJECT(fileselection), "cancel_button", 
							  cancel_button);
	gtk_widget_show(cancel_button);
	GTK_WIDGET_SET_FLAGS(cancel_button, GTK_CAN_DEFAULT);
	
	gtk_signal_connect(GTK_OBJECT(cancel_button), "clicked",
							 GTK_SIGNAL_FUNC(fileselection_cancel_event),
							 NULL);

#define EXTEND_FILE_TEST
#ifdef EXTEND_FILE_TEST
	GtkWidget *widget;

	main_vbox = GTK_FILE_SELECTION(fileselection)->main_vbox;

	widget = label_create(main_vbox, "lab1", "This is a fileselection extention test",
								 0.0, 0.0);

	gtk_box_pack_start(GTK_BOX(main_vbox), widget, TRUE, TRUE, FALSE);
#endif

	return fileselection;
}


GtkWidget *toolbar_create(GtkWidget *box)
{
	GtkWidget *toolbar;


	toolbar = gtk_toolbar_new();

	gtk_widget_ref(toolbar);
	gtk_object_set_data_full(GTK_OBJECT(box), "tbar", toolbar,
									 (GtkDestroyNotify)gtk_widget_unref);
	gtk_widget_show(toolbar);

	gtk_box_pack_start(GTK_BOX(box), toolbar, 1, 1, 0);
	gtk_toolbar_set_style(GTK_TOOLBAR(toolbar), GTK_TOOLBAR_ICONS);

	return toolbar;
}


GtkWidget *toolbar_toogle_btn(GtkWidget *toolbar,  bool toggled,
							  char *icon, char *label, char *help, 
							  void *event_func, int event_cmd)
{
	char icon_name[256];
	GtkWidget *togglebutton;
	GtkWidget *toolbar_icon;
	

	demangle_pixmap_name(icon_name, icon);
	
	toolbar_icon = gtk_image_new_from_file(icon_name);
	togglebutton = gtk_toolbar_append_element(GTK_TOOLBAR(toolbar),
											  GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
											  NULL,
											  (!label[0]) ? NULL : label,
											  help, NULL,
											  toolbar_icon, NULL, NULL);

	
	gtk_widget_ref(togglebutton);
	gtk_object_set_data_full(GTK_OBJECT(toolbar), "tb_tbtn",
							 togglebutton,
							 (GtkDestroyNotify)gtk_widget_unref);
	gtk_widget_show(togglebutton);


	if (event_func)
	{
		gtk_signal_connect(GTK_OBJECT(togglebutton), "toggled",
						   GTK_SIGNAL_FUNC(event_func),
						   GINT_TO_POINTER(event_cmd));

		if (toggled)
		{
			gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(togglebutton), TRUE);
		}
	}

	return togglebutton;
}


GtkWidget *toolbar_btn(GtkWidget *toolbar,
					   char *icon, char *label, char *help, 
					   void *event_func, int event_cmd)
{
	char icon_name[256];
	GtkWidget *button;
	GtkWidget *toolbar_icon;
	

	demangle_pixmap_name(icon_name, icon);
	
	toolbar_icon = gtk_image_new_from_file(icon_name);
	button = gtk_toolbar_append_element(GTK_TOOLBAR(toolbar),
										GTK_TOOLBAR_CHILD_BUTTON,
										NULL,
										(!label[0]) ? NULL : label,
										help, NULL,
										toolbar_icon, NULL, NULL);

	
	gtk_widget_ref(button);
	gtk_object_set_data_full(GTK_OBJECT(toolbar), "tb_btn",
							 button,
							 (GtkDestroyNotify)gtk_widget_unref);
	gtk_widget_show(button);


	if (event_func)
	{
		gtk_signal_connect(GTK_OBJECT(button), "pressed",
						   GTK_SIGNAL_FUNC(event_func),
						   GINT_TO_POINTER(event_cmd));
	}

	return button;
}


#if defined(USING_GTK_1_2) || defined(USING_GTK_2_0)
GtkWidget* about_create()
{
	return NULL;
}


FILE *get_log_file()
{
	static FILE *f = fopen("/tmp/Freyja.log", "w");

	return f;
}


void close_log_file()
{
	if (get_log_file())
		fclose(get_log_file());
}


void event_print(char *s, ...)
{
	FILE *f = get_log_file();
	char buffer[1024];
	va_list args;
	unsigned int l;


	// Strip message of an trailing carrage return 
	//  and print to stdout and the status bar
	va_start(args, s);
	vsnprintf(buffer, 1024, s, args);
	
	l = strlen(buffer);
  
	if (!l || !buffer[0])
		return;

	if (buffer[l-1] == '\n')
		buffer[l-1] = 0;

	if (getGtkStatusBarWidget())
	{
		if (GTK_IS_STATUSBAR(getGtkStatusBarWidget()))
		{
			gtk_statusbar_push(GTK_STATUSBAR(getGtkStatusBarWidget()),
							   0, buffer);
		}
		else
		{
			gtk_label_set_text(GTK_LABEL(getGtkStatusBarWidget()), buffer);
		}
	}

#ifdef DEBUG_EVENT_PRINT
	fprintf(stdout, "> %s\n",  buffer);
#endif

	if (f)
	{
		fprintf(f, "> ");
		vfprintf(f, s, args);
		fprintf(f, "\n");
	}

	va_end(args);
}


GtkWidget *window_create()
{
	GtkWidget *window;


	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_object_set_data(GTK_OBJECT(window), "window", window);
	gtk_window_set_policy(GTK_WINDOW(window), FALSE, TRUE, FALSE);
	gtk_window_set_wmclass(GTK_WINDOW(window), "Freyja", "Freyja");
	gtk_window_set_title(GTK_WINDOW(window), BUILD_NAME);

	/* Setup events for this window */
	gtk_widget_set_events(GTK_WIDGET(window),
								 GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK);
  

#ifdef USING_GTK_1_2
	gtk_signal_connect(GTK_OBJECT(getGtkGLAreaWidget()), "key_press_event",
							 GTK_SIGNAL_FUNC(glarea_key_press_event), NULL);
	gtk_signal_connect(GTK_OBJECT(getGtkGLAreaWidget()), "key_release_event",
							 GTK_SIGNAL_FUNC(glarea_key_release_event), NULL);
	gtk_signal_connect(GTK_OBJECT(getGtkGLAreaWidget()), "destroy",
							 GTK_SIGNAL_FUNC(window_destroy), NULL);
#else
	char icon_filename[256];
	GdkPixbuf *icon = NULL;


	demangle_pixmap_name(icon_filename, "freyja.png");

	if (icon_filename && icon_filename[0])
	{
		GError *error = NULL;

		icon = gdk_pixbuf_new_from_file(icon_filename, &error);

		if (!icon)
		{
			fprintf(stderr, "Failed to load icon file: %s: %s\n",
					  icon_filename, error->message);
			g_error_free(error);
		}
	}

	if (icon)
	{
      gtk_window_set_icon(GTK_WINDOW(window), icon);
      gdk_pixbuf_unref(icon);
	}
#endif

	return window;
}
#endif


int main(int argc, char *argv[])
{
	unsigned int width, height;
	char *driver;
	bool fullscreen;


#ifdef ENABLE_NLS
	bindtextdomain(PACKAGE, PACKAGE_LOCALE_DIR);
	textdomain(PACKAGE);
#endif

	gtk_init(&argc, &argv);

#ifdef USING_GTK_1_2
	event_print("@GTK+ 1.2 interface started...");
#elif USING_GTK_2_0
	event_print("@GTK+ 2.0 interface started...");	
#endif

	event_print("Email bug reports to %s", EMAIL_ADDRESS);

	// Mongoose 2002.02.23, Start Freyja MVC which 
	//   builds the widgets from a script
	event_init(&width, &height, &fullscreen, &driver);

	// Mongoose 2002.02.23, Tell renderer to wake up
	event_render_init(width, height);
	event_resize(width, height);

	// Mongoose 2002.01.23, Switch to mesh mode
	freyja_event2i(EVENT_FREYJA_MODE, FREYJA_MODE_MODEL_EDIT);
	event_refresh();

	event_print("Welcome to Freyja %s-%s, %s", VERSION, BUILD_ID, __DATE__);

	// Mongoose 2002.02.23, Hook for exit() calls
	atexit(event_shutdown);

	// Mongoose 2002.02.23, Load file passed by command line
	if (argc > 1)
	{
		fileselection_dir_set_event(argv[1]);
		event_filename(FREYJA_MODE_LOAD_MODEL, argv[1]);
	}

	//gdk_threads_enter();
	gtk_main();
	//gdk_threads_leave();

	return 0;
}

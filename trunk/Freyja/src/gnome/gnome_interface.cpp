/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : mlisp-mgtk+
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : 
 * License : GPL See file COPYING, also (C) 2000-2004 Mongoose
 * Comments: This is the main client entry.
 *
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.06.01:
 * Mongoose - Last of freyja use removed, project desn changed
 *
 * 2004.04.11:
 * Mongoose - Removed GNOME support
 *
 * ????.??.??:
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

#ifdef HAVE_OPENGL
#   include <GL/gl.h>
#   include <GL/glu.h>

#   ifdef HAVE_GTKGL
#      include <gtkgl/gtkglarea.h>
#   endif
#endif

#include "gnome_interface.h"
#include "gnome_callbacks.h"
#include "gnome_resource.h"


#ifdef HAVE_GTKGL
void mgtk_refresh_glarea(GtkWidget *widget)
{
	GdkRectangle area;


	area.x = 0;
	area.y = 0;
	area.width  = widget->allocation.width;
	area.height = widget->allocation.height;

	gtk_widget_draw(widget, &area);
}


void mgtk_init_glarea(GtkWidget* widget)
{
	glarea_window_state_t *state;


	state = (glarea_window_state_t*)gtk_object_get_data(GTK_OBJECT(widget), 
														"gl_window_state");

	if (gtk_gl_area_make_current(GTK_GL_AREA(widget))) 
	{
		/* Initialization */
		if (!state->init) 
		{
			//event_render_init(state->width, state->height);
			//event_resize(state->width, state->height);
			state->init = true;
		}
	}
}


void mgtk_expose_glarea(GtkWidget *widget, GdkEventExpose *event)
{
	GtkGLArea *glarea = GTK_GL_AREA(widget);

	
	/* draw only last expose */
	if (event->count > 0) 
	{
		return;
	}

	/* OpenGL calls can be done only if make_current returns true */
	if (gtk_gl_area_make_current(glarea)) 
	{
		/* Call modeler's renderer */
		mgtk_event_gldisplay();
		
		/* Swap buffers to rasterize */
		gtk_gl_area_swapbuffers(glarea);
	}
}


void mgtk_resize_glarea(GtkWidget *widget, GdkEventConfigure *event)
{
	/* OpenGL calls can be done only if make_current returns true */
	if (gtk_gl_area_make_current(GTK_GL_AREA(widget))) 
	{
		//glViewport(0, 0, widget->allocation.width, widget->allocation.height);
		mgtk_event_glresize(widget->allocation.width, widget->allocation.height);
	}
}


void mgtk_destroy_glarea(GtkWidget *widget)
{
	glarea_window_state_t *state;


	state = (glarea_window_state_t*)gtk_object_get_data(GTK_OBJECT(widget),
														"gl_window_state");

	if (state) 
	{
		delete state;
	}
}


GtkWidget *mgtk_create_glarea(unsigned int width, unsigned int height)
{
	GtkWidget *glarea;
	glarea_window_state_t *state;


	if (gdk_gl_query() == FALSE)
	{
		return NULL;
	}

	glarea = gtk_gl_area_new_vargs(NULL,
								   GDK_GL_RGBA,
								   GDK_GL_RED_SIZE,1,
								   GDK_GL_GREEN_SIZE,1,
								   GDK_GL_BLUE_SIZE,1,
								   GDK_GL_DEPTH_SIZE,1,
								   GDK_GL_DOUBLEBUFFER,
								   GDK_GL_NONE);

	gtk_gl_area_size(GTK_GL_AREA(glarea), width, height);

	/* Set up events and signals for OpenGL widget */
	gtk_widget_set_events(GTK_WIDGET(glarea),
						  GDK_EXPOSURE_MASK |
						  GDK_BUTTON_PRESS_MASK |
						  GDK_BUTTON_RELEASE_MASK |
						  GDK_POINTER_MOTION_MASK |
						  GDK_POINTER_MOTION_HINT_MASK |
						  GDK_MOTION_NOTIFY);

	/* Mouse */
	gtk_signal_connect(GTK_OBJECT(glarea), "motion_notify_event",
					   GTK_SIGNAL_FUNC(mgtk_event_mouse_motion), NULL);
	gtk_signal_connect(GTK_OBJECT(glarea), "button_press_event",
					   GTK_SIGNAL_FUNC(mgtk_event_button_press), NULL);
	gtk_signal_connect(GTK_OBJECT(glarea), "button_release_event",
					   GTK_SIGNAL_FUNC(mgtk_event_button_release), NULL);

	/* Misc */
	gtk_signal_connect(GTK_OBJECT(glarea), "expose_event",
					   GTK_SIGNAL_FUNC(mgtk_expose_glarea), NULL);
	gtk_signal_connect(GTK_OBJECT(glarea), "configure_event",
					   GTK_SIGNAL_FUNC(mgtk_resize_glarea), NULL);
	gtk_signal_connect(GTK_OBJECT(glarea), "realize",
					   GTK_SIGNAL_FUNC(mgtk_init_glarea), NULL);
	gtk_signal_connect(GTK_OBJECT(glarea), "destroy",
					   GTK_SIGNAL_FUNC(mgtk_destroy_glarea), NULL);

	/* Setup GL window state */
	state = new glarea_window_state_t;
	state->init = false;
	state->plane = 0;
	state->mouse_x = 0;
	state->mouse_y = 0;
	state->width = width;
	state->height = height;
	gtk_object_set_data(GTK_OBJECT(glarea), "gl_window_state", state);

	return glarea;
}
#endif


GdkPixbuf *mgtk_create_pixbuf(char *icon_filename)
{
	GdkPixbuf *icon = NULL;

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


GtkWidget *mgtk_create_icon(char *icon_filename, GtkIconSize icon_size)
{
	GtkWidget *icon = NULL;


	/* If there is a leading "gtk" assume it's stock, since there is no path */
	if (!strncmp("gtk", icon_filename, 3))
		//		!strncmp("gnome-", icon_filename, 6))
	{
		icon = gtk_image_new_from_stock(icon_filename, icon_size);
	}
	else
	{
		icon = gtk_image_new_from_file(icon_filename);
	}

	return icon;
}


/////////////////////////////////////////////////////////////
// Dialogs
/////////////////////////////////////////////////////////////

int mgtk_create_confirm_dialog(char *dialog_icon,
							   char *information_message, 
							   char *question_message,
							   char *cancel_icon, char *cancel_text,
							   char *accept_icon, char *accept_text)
{
	GtkWidget *dialog, *icon, *info, *question, *cancel, *accept, *hbox;
	int ret = 0;

	dialog = gtk_dialog_new();


	hbox = gtk_hbox_new(FALSE, 0);
	gtk_widget_ref(hbox);
	gtk_object_set_data_full(GTK_OBJECT(GTK_DIALOG(dialog)->vbox), 
							 "hbox1", hbox, (GtkDestroyNotify)gtk_widget_unref);
	gtk_widget_show(hbox);
	gtk_box_pack_start(GTK_BOX(GTK_DIALOG(dialog)->vbox), 
					   hbox, FALSE, TRUE, 0);

	icon = mgtk_create_icon(dialog_icon, GTK_ICON_SIZE_DIALOG);

	info = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(info), information_message);
	gtk_label_set_selectable(GTK_LABEL(info), TRUE);

	question = gtk_label_new(NULL);
	gtk_label_set_markup(GTK_LABEL(question), question_message);
	gtk_label_set_selectable(GTK_LABEL(question), TRUE);

	gtk_container_add(GTK_CONTAINER(hbox), icon);
	gtk_container_add(GTK_CONTAINER(hbox), info);
	gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), question);
	cancel = gtk_dialog_add_button(GTK_DIALOG(dialog), cancel_text, 0);
	accept = gtk_dialog_add_button(GTK_DIALOG(dialog), accept_text, GTK_RESPONSE_ACCEPT);
	// FIXME: Add icons to buttons here, or roll own dialog button code
	gtk_widget_show_all(dialog);


	/* Force user to close this dialog by stopping other events */
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



void mgtk_create_info_dialog(char *dialog_icon, char *message)
{
	GtkWidget *about, *label, *icon;


	about = gtk_dialog_new();
	
	icon = mgtk_create_icon(dialog_icon, GTK_ICON_SIZE_DIALOG);
	
	label = gtk_label_new(NULL);
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


GtkWidget *mgtk_create_color_picker_dialog(char *title, void *event_func)
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
	GtkWidget *entry;


	entry = gtk_entry_new();
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
	GtkWidget *vbox;

	
	vbox = gtk_vbox_new(homogeneous ? TRUE : FALSE, spacing);
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


GtkWidget *mgtk_create_spinbutton2(GtkWidget *master, char *name, 
								   float val, float min, float max,
								   float step, float page, float page_sz, 
								   int digits)
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


GtkWidget *mgtk_create_spinbutton(GtkWidget *master, char *name, 
								  float val, float min, float max)
{
	return mgtk_create_spinbutton2(master, name, val, min, max, 1, 10, 10, 0);
}


GtkWidget *mgtk_create_label(GtkWidget *master, char *name, 
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


GtkWidget *mgtk_create_tab(GtkWidget *notebook, char *name, 
						   GtkWidget *tab_contents, char *label_text,
						   int tab_num)
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
	label = mgtk_create_label(notebook, name, label_text, 0, 0.5);
	gtk_notebook_set_tab_label(GTK_NOTEBOOK(notebook),
							   gtk_notebook_get_nth_page(GTK_NOTEBOOK(notebook),
														tab_num), label);
	return NULL;
}


GtkWidget *mgtk_create_fileselection(char *title)
{
	GtkWidget *fileselection;
	GtkWidget *ok_button;
	GtkWidget *cancel_button;


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
					   GTK_SIGNAL_FUNC(fileselection_action_event),
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

	return fileselection;
}


GtkWidget *mgtk_create_toolbar(GtkWidget *box)
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


GtkWidget *mgtk_create_color_button(void *func, int id)
{
#ifdef DISABLE_GTK_COLORBUTTON	
	GtkWidget *label = gtk_label_new(NULL);

	gtk_label_set_markup(GTK_LABEL(label), "ColorButton disabled");
	gtk_label_set_selectable(GTK_LABEL(label), TRUE);
	return label;
#else
	GtkWidget *colorbutton;

	colorbutton = gtk_color_button_new();
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


GtkWidget *mgtk_create_toolbar_toogle_button(GtkWidget *toolbar,  bool toggled,
											 char *icon, char *label,
											 char *help, 
											 void *event_func, int event_cmd)
{
	GtkWidget *togglebutton;
	GtkWidget *toolbar_icon;
	

	toolbar_icon = mgtk_create_icon(icon, GTK_ICON_SIZE_LARGE_TOOLBAR);

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


GtkWidget *mgtk_create_toolbar_button(GtkWidget *toolbar,
									  char *icon, char *label, char *help, 
									  void *event_func, int event_cmd)
{
	GtkWidget *button;
	GtkWidget *toolbar_icon;
	
	
	toolbar_icon = mgtk_create_icon(icon, GTK_ICON_SIZE_LARGE_TOOLBAR);

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


GtkWidget *mgtk_create_window(char *title, char *wmclass, char *icon_name)
{
	GtkWidget *window;
	GdkPixbuf *icon;


	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_object_set_data(GTK_OBJECT(window), "window", window);
	gtk_window_set_policy(GTK_WINDOW(window), FALSE, TRUE, FALSE);
	gtk_window_set_wmclass(GTK_WINDOW(window), wmclass, wmclass);
	gtk_window_set_title(GTK_WINDOW(window), title);

	/* Setup events for this window */
	gtk_widget_set_events(GTK_WIDGET(window),
						  GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK);

	icon = mgtk_create_pixbuf(icon_name);
	mgtk_create_window_icon(window, icon);
	//mgtk_destroy_pixbuf(icon);

	return window;
}

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
#include "ResourceEvent.h"

#include "mgtk_interface.h"
#include "mgtk_callbacks.h"
#include "mgtk_resource.h"
#include "mgtk_events.h"

using namespace mstl;

/* Mongoose 2004.10.28, 
 * These are the only gobals that should be used here */
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


////////////////////////////////////////////////////////////////

void mgtk_event_close_window(GtkWidget *widget, gpointer user_data)
{
	mgtk_handle_application_window_close();
}


arg_list_t *mgtk_rc_window(arg_list_t *container)
{
	arg_list_t *ret = NULL, *title, *icon;
	GtkWidget *window;
	char icon_filename[1024];

	symbol_enforce_type_assert(&title, CSTRING);
	symbol_enforce_type_assert(&icon, CSTRING);

	if (title && icon)
	{
		mgtk_get_pixmap_filename(icon_filename, 1024, get_string(icon));
		icon_filename[1023] = 0;

		window = mgtk_create_window(BUILD_NAME, get_string(title),
									icon_filename);

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
	}

	return ret;
}


arg_list_t *mgtk_rc_gl_widget(arg_list_t *box)
{
	arg_enforce_type(&box, ARG_GTK_BOX_WIDGET);
	MGTK_ASSERTMSG(box, "box == ARG_GTK_BOX_WIDGET");

	if (!box)
	{
		return NULL;
	}

	arg_list_t *width;
	symbol_enforce_type_assert(&width, INT);

	arg_list_t *height;
	symbol_enforce_type_assert(&height, INT);

	if (!width || !height)
	{
		return NULL;
	}

	/* Gtk GL Area widget */
	GtkWidget *gl = mgtk_create_glarea(get_int(width), get_int(height));

	if (gl)
	{
		gpointer gobj = gtk_object_get_data(GTK_OBJECT(gl), "gl_window_state");
		mgtk_glarea_window_state_t *state = (mgtk_glarea_window_state_t*)gobj;
		state->appbar = NULL;

		GTK_GL_AREA_WIDGET = gl;
		mgtk_print("@Gtk+ GL context started...");
	}
	else
	{
		gl = mgtk_create_label(GTK_WIDGET(box->data), "gl_err_label1", 
							   "Failed to create OpenGL context.", 0.5, 0.5);
		gtk_widget_set_usize(gl, get_int(width), get_int(height));
		mgtk_print("!ERROR: OpenGL display not supported by this system?\n");
	}

#if defined HAVE_GTKGLEXT
	gtk_signal_connect(GTK_OBJECT(mgtk_get_gl_widget()), "key_press_event",
					   GTK_SIGNAL_FUNC(mgtk_event_key_press), NULL);
	gtk_signal_connect(GTK_OBJECT(mgtk_get_gl_widget()), "key_release_event",
					   GTK_SIGNAL_FUNC(mgtk_event_key_release), NULL);
	gtk_signal_connect(GTK_OBJECT(mgtk_get_gl_widget()), "destroy",
					   GTK_SIGNAL_FUNC(mgtk_destroy_window), NULL);
#endif
	
	/* Editing window */
	GtkWidget *vbox = mgtk_create_vbox(GTK_WIDGET(box->data), "gl_vbox", 
									   0, 0, 1, 1, 1);
	gtk_box_pack_start(GTK_BOX(vbox), GTK_WIDGET(gl), TRUE, TRUE, 0);
	gtk_widget_show(gl);
		
	delete_arg(&width);
	delete_arg(&height);

	return NULL;
}


arg_list_t *mgtk_rc_textview(arg_list_t *box)
{
	arg_enforce_type(&box, ARG_GTK_BOX_WIDGET);
	MGTK_ASSERTMSG(box, "box == ARG_GTK_BOX_WIDGET");

	if (!box)
	{
		return NULL;
	}

	// Optional
	arg_list_t *s;
	symbol_enforce_type(&s, CSTRING);
	//MGTK_ASSERTMSG(s, "s == CSTRING");

	//arg_list_t *width;
	//symbol_enforce_type(&width, INT);
	//MGTK_ASSERTMSG(width, "width == INT");

	//arg_list_t *height;
	//symbol_enforce_type(&height, INT);
	//MGTK_ASSERTMSG(height, "height == INT");

	GtkWidget *view = gtk_text_view_new();
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
	//gtk_widget_set_usize(view, get_int(width), get_int(height));

	if (s)
	{
		gtk_text_buffer_set_text(buffer, get_string(s), -1);	
	}

	gtk_box_pack_start(GTK_BOX((GtkWidget *)box->data), view, TRUE, TRUE, 0);
	gtk_widget_show(view);

	return box; // Directly 'repeat', no restrictions.
}


arg_list_t *mgtk_rc_statusbar(arg_list_t *box)
{
	arg_enforce_type(&box, ARG_GTK_BOX_WIDGET);
	MGTK_ASSERTMSG(box, "box == ARG_GTK_BOX_WIDGET");

	if (!box)
	{
		return NULL;
	}

	GTK_STATUS_BAR_WIDGET = gtk_statusbar_new();
	GtkWidget *widget = (GtkWidget *)GTK_STATUS_BAR_WIDGET;
	gtk_box_pack_start(GTK_BOX(box->data), widget, 0, 0, 0);
	gtk_widget_show(widget);

	arg_list_t *ret = NULL;
	new_adt(&ret, ARG_GTK_WIDGET, (void *)widget);

	return ret;
}


arg_list_t *mgtk_rc_toolbar_separator(arg_list_t *box)
{
	arg_enforce_type(&box, ARG_GTK_TOOLBOX_WIDGET);
	MGTK_ASSERTMSG(box, "box != ARG_GTK_TOOLBOX_WIDGET");

	if (!box)
	{
		return NULL;
	}

	GtkToolItem *item = gtk_separator_tool_item_new();
	GtkWidget *widget = (GtkWidget *)item;
	//gtk_toolbar_append_widget((GtkToolbar *)box->data, widget, "", "");
	gtk_toolbar_insert(GTK_TOOLBAR(box->data), item, -1); // Append to the end
	gtk_widget_show(widget);

	arg_list_t *ret = NULL;
	new_adt(&ret, ARG_GTK_WIDGET, (void *)widget);

	return ret;
}


arg_list_t *mgtk_rc_toolbar(arg_list_t *box)
{
	arg_list_t *ret = NULL;
	GtkWidget *toolbar;

	arg_enforce_type(&box,  ARG_GTK_BOX_WIDGET);
	MGTK_ASSERTMSG(box, "box == ARG_GTK_BOX_WIDGET");

	if (!box)
	{
		return NULL;
	}
 
	toolbar = mgtk_create_toolbar((GtkWidget *)box->data);
	gtk_widget_ref(toolbar);
	gtk_object_set_data_full(GTK_OBJECT((GtkWidget *)box->data), "tool", 
							 toolbar,
							 (GtkDestroyNotify)gtk_widget_unref);
	gtk_widget_show(toolbar);

	new_adt(&ret, ARG_GTK_TOOLBOX_WIDGET, (void *)toolbar);

	return ret;
}


void mgtk_tool_toggle_button_dual_handler(GtkWidget *item, gpointer e)
{
	int val = 0;
	unsigned int e1 = GPOINTER_TO_INT(e);
	unsigned int e2 = 
	GPOINTER_TO_INT(gtk_object_get_data((GtkObject*)item, "mlisp_event"));
	MSTL_MSG("*** %i, %i", e1, e2);

	if (GTK_IS_TOGGLE_BUTTON(item))
	{
		val = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(item));
	}
	else if (GTK_IS_TOGGLE_TOOL_BUTTON(item))
	{ 
		val = gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(item));
	}
	else
	{
		mgtk_print("! unknown type %s:%i", __FILE__, __LINE__);
	}

	if (ResourceEvent::listen(e1 - ResourceEvent::eBaseEvent, e2, val))
	{
		// Update any other widgets sharing the same event
		//FIXME mgtk_toggle_dual_value_set(e1, e2, val);
		return;
	}
}


void mgtk_tool_toggle_button_handler(GtkWidget *item, gpointer e)
{
	int val = 0;

	if (GTK_IS_TOGGLE_BUTTON(item))
	{
		val = gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(item));
	}
	else if (GTK_IS_TOGGLE_TOOL_BUTTON(item))
	{ 
		val = gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(item));
	}
	else
	{
		mgtk_print("! unknown type %s:%i", __FILE__, __LINE__);
	}

	mgtk_handle_event1u(GPOINTER_TO_INT(e), val);

	// Update any other widgets sharing the same event
	mgtk_toggle_value_set(GPOINTER_TO_INT(e), val);
}

arg_list_t *mgtk_rc_toolbar_togglebutton(arg_list_t *box)
{
	arg_list_t *ret = NULL, *icon, *label, *help, *toggled, *event, *cmd;
	GtkWidget *toggle_button;
	void *event_func;
	int event_cmd;


	arg_enforce_type(&box,  ARG_GTK_TOOLBOX_WIDGET);
	MGTK_ASSERTMSG(box, "box == ARG_GTK_TOOLBOX_WIDGET");

	if (!box)
	{
		return NULL;
	}

	symbol_enforce_type_assert(&toggled, INT);
	//MGTK_ASSERTMSG(toggled, "toggled == INT");

	symbol_enforce_type_assert(&icon, CSTRING);
	//MGTK_ASSERTMSG(icon, "icon == CSTRING");

	symbol_enforce_type_assert(&label, CSTRING);
	//MGTK_ASSERTMSG(label, "label == CSTRING");

	symbol_enforce_type_assert(&help, CSTRING);
	//MGTK_ASSERTMSG(help, "help == CSTRING");

	symbol_enforce_type_assert(&event, INT);
	//MGTK_ASSERTMSG(event, "event == INT");

	symbol_enforce_type_assert(&cmd, INT);
	//MGTK_ASSERTMSG(cmd, "cmd == INT");

	if (toggled && icon && label && help && event && cmd)
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

		gtk_signal_connect(GTK_OBJECT(toggle_button), "toggled",
						   GTK_SIGNAL_FUNC(mgtk_tool_toggle_button_handler), 
						   GINT_TO_POINTER(get_int(cmd)));

		// Mongoose 2002.02.01, Add this widget to a special 
		//   lookup table by it's event id
		//index_add_gtk_widget(get_int(event), item);
		mgtk_event_subscribe_gtk_widget(get_int(cmd), toggle_button);
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
	arg_list_t *ret = NULL, *icon, *label, *help, *event, *cmd;
	GtkWidget *button;
	void *event_func;
	int event_cmd;

	arg_enforce_type(&box, ARG_GTK_TOOLBOX_WIDGET);
	MGTK_ASSERTMSG(box, "box != ARG_GTK_TOOLBOX_WIDGET");

	if (!box)
	{
		return NULL;
	}

	symbol_enforce_type_assert(&icon, CSTRING);
	symbol_enforce_type_assert(&label, CSTRING);
	symbol_enforce_type_assert(&help, CSTRING);
	symbol_enforce_type_assert(&event, INT);
	symbol_enforce_type_assert(&cmd, INT);

	if (icon && label && help && event && cmd)
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


arg_list_t *mgtk_rc_summonbox(arg_list_t *box)
{
	// Summons can be 'top level' containers like windows/dialogs
	bool parented = (box != NULL);
	arg_enforce_type(&box,  ARG_GTK_BOX_WIDGET);
	MGTK_ASSERTMSG(((parented && box) || !parented), 
				   "box == ARG_GTK_BOX_WIDGET\nMLISP (%s:%i)", 
				   mlisp_get_filename(), mlisp_get_line_num());

	if (parented && !box)
	{
		return NULL;
	}

	arg_list_t *name;
	symbol_enforce_type_assert(&name, CSTRING);
	//MGTK_ASSERTMSG(name, "name == CSTRING\nMLISP (%s:%i)", 
	//			   mlisp_get_filename(), mlisp_get_line_num());

	arg_list_t *ret = NULL;

	if (name)
	{
		void *summonbox = mlisp_recall((char *)name->data);
		new_adt(&ret, ARG_GTK_BOX_WIDGET, summonbox);
	}

	return ret;
}


arg_list_t *mgtk_rc_expander(arg_list_t *box)
{
	arg_enforce_type(&box,  ARG_GTK_BOX_WIDGET);
	MGTK_ASSERTMSG(box, "box == ARG_GTK_BOX_WIDGET\nMLISP (%s:%i)", 
				   mlisp_get_filename(), mlisp_get_line_num());

	if (!box)
	{
		return NULL;
	}

	arg_list_t *label;
	symbol_enforce_type(&label,  CSTRING);
	MGTK_ASSERTMSG(label, "label == CSTRING\nMLISP (%s:%i)", 
				   mlisp_get_filename(), mlisp_get_line_num());

	arg_list_t *show;
	symbol_enforce_type_assert(&show, INT);
	//MGTK_ASSERTMSG(show, "show == INT\nMLISP (%s:%i)", 
	//			   mlisp_get_filename(), mlisp_get_line_num());

	arg_list_t *name = NULL;
	if (mlisp_peek_for_vargs())
	{
		symbol_enforce_type_assert(&name, CSTRING);
		//MGTK_ASSERTMSG(name, "name == CSTRING\nMLISP (%s:%i)", 
		//			   mlisp_get_filename(), mlisp_get_line_num());
	}

	arg_list_t *ret = NULL;

	if (label && show)
	{
		GtkWidget *expander = gtk_expander_new(mlisp_get_string(label));
		GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
		gtk_container_add((GtkContainer *)box->data, expander);
		gtk_container_add((GtkContainer *)expander, vbox);
		gtk_widget_show_all(expander);

		gtk_expander_set_expanded((GtkExpander*)expander, 
								  get_int(show)? TRUE : FALSE);

		new_adt(&ret, ARG_GTK_BOX_WIDGET, (void *)vbox);

		if (name)
		{
			// This makes a copy of the arg with a box in it for binding
			// to the name symbol.  This is for S-Class rank only, since
			// it populates the symbol table directly.
			//
			// To get the box back use the (summonbox ...) function.
			arg_list_t *sealing_jutsu;
			new_adt(&sealing_jutsu, ARG_GTK_BOX_WIDGET, (void *)vbox);
			mlisp_bind(name, sealing_jutsu);
		}
	}

	delete_arg(&label);
	delete_arg(&show);
	//delete_arg(&name);

	return ret;
}


arg_list_t *mgtk_rc_expander_hbox(arg_list_t *box)
{
	arg_enforce_type(&box,  ARG_GTK_BOX_WIDGET);
	MGTK_ASSERTMSG(box, "box == ARG_GTK_BOX_WIDGET\nMLISP (%s:%i)", 
				   mlisp_get_filename(), mlisp_get_line_num());

	if (!box)
	{
		return NULL;
	}

	arg_list_t *label;
	symbol_enforce_type(&label,  CSTRING);
	MGTK_ASSERTMSG(label, "label == CSTRING\nMLISP (%s:%i)", 
				   mlisp_get_filename(), mlisp_get_line_num());

	arg_list_t *show;
	symbol_enforce_type_assert(&show, INT);
	//MGTK_ASSERTMSG(show, "show == INT\nMLISP (%s:%i)", 
	//			   mlisp_get_filename(), mlisp_get_line_num());

	arg_list_t *name = NULL;
	if (mlisp_peek_for_vargs())
	{
		symbol_enforce_type_assert(&name, CSTRING);
		//MGTK_ASSERTMSG(name, "name == CSTRING\nMLISP (%s:%i)", 
		//			   mlisp_get_filename(), mlisp_get_line_num());
	}

	arg_list_t *ret = NULL;

	if (label && show)
	{
		GtkWidget *expander = gtk_expander_new("");//mlisp_get_string(label));
		GtkWidget *hbox = gtk_hbox_new(FALSE, 0);
		gtk_container_add((GtkContainer *)box->data, expander);
		gtk_container_add((GtkContainer *)expander, hbox);
		gtk_widget_show_all(expander);

		gtk_expander_set_expanded((GtkExpander*)expander, 
								  get_int(show)? TRUE : FALSE);

		new_adt(&ret, ARG_GTK_BOX_WIDGET, (void *)hbox);

		if (name)
		{
			// This makes a copy of the arg with a box in it for binding
			// to the name symbol.  This is for S-Class rank only, since
			// it populates the symbol table directly.
			//
			// To get the box back use the (summonbox ...) function.
			arg_list_t *sealing_jutsu;
			new_adt(&sealing_jutsu, ARG_GTK_BOX_WIDGET, (void *)hbox);
			mlisp_bind(name, sealing_jutsu);
		}
	}

	delete_arg(&label);
	delete_arg(&show);
	//delete_arg(&name);

	return ret;
}


void mgtk_destroy_notebook(GtkWidget *widget)
{
	mgtk_notebook_eventmap_t *emap = (mgtk_notebook_eventmap_t*)gtk_object_get_data(GTK_OBJECT(widget), "notebook_eventmap");

	if (emap) 
	{
		if (emap->events)
			delete [] emap->events;

		delete emap;
	}
}


arg_list_t *mgtk_rc_notebook(arg_list_t *box)
{
	arg_list_t *ret = NULL, *width, *height, *event;
	GtkWidget *notebook;

	arg_enforce_type(&box,  ARG_GTK_BOX_WIDGET);
	MGTK_ASSERTMSG(box, "box == ARG_GTK_BOX_WIDGET");

	if (!box)
	{
		return NULL;
	}

	symbol_enforce_type_assert(&width, INT);
	symbol_enforce_type_assert(&height, INT);
	symbol_enforce_type_assert(&event, INT);

	if (width && height && event)
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
	arg_list_t *label, *event, *ret = NULL;
	GtkWidget *item, *vbox;
	mgtk_notebook_eventmap_t *emap;
	int *newEvents;
	unsigned int i;

	arg_enforce_type(&notebook,  ARG_GTK_NOTEBOOK);
	MGTK_ASSERTMSG(notebook, "notebook == ARG_GTK_NOTEBOOK\nMLISP (%s:%i)", 
				   mlisp_get_filename(), mlisp_get_line_num());

	if (!notebook)
	{
		return NULL;
	}

	symbol_enforce_type_assert(&label, CSTRING);
	//MGTK_ASSERTMSG(label, "label == CSTRING\nMLISP (%s:%i)", 
	//			   mlisp_get_filename(), mlisp_get_line_num());
	symbol_enforce_type_assert(&event, INT);
	//MGTK_ASSERTMSG(event, "event == INT\nMLISP (%s:%i)", 
	//			   mlisp_get_filename(), mlisp_get_line_num());

	if (label && event)
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
	arg_list_t *homogeneous, *spacing, *expand, *fill, *pading, *ret = NULL;

	arg_enforce_type(&box, ARG_GTK_BOX_WIDGET | ARG_GTK_WINDOW);
	MGTK_ASSERTMSG(box, "box == ARG_GTK_BOX_WIDGET | ARG_GTK_WINDOW\nMLISP (%s:%i)", 
				   mlisp_get_filename(), mlisp_get_line_num());

	if (!box)
	{
		return NULL;
	} 

	symbol_enforce_type_assert(&homogeneous, INT);
	//MGTK_ASSERTMSG(homogeneous, "homogeneous == INT\nMLISP (%s:%i)", 
	//			   mlisp_get_filename(), mlisp_get_line_num());

	symbol_enforce_type_assert(&spacing, INT); 
	//MGTK_ASSERTMSG(spacing, "spacing == INT\nMLISP (%s:%i)", 
	//			   mlisp_get_filename(), mlisp_get_line_num());

	symbol_enforce_type_assert(&expand, INT);
	//MGTK_ASSERTMSG(expand, "expand == INT\nMLISP (%s:%i)", 
	//			   mlisp_get_filename(), mlisp_get_line_num());

	symbol_enforce_type_assert(&fill, INT);
	//MGTK_ASSERTMSG(fill, "fill == INT\nMLISP (%s:%i)", 
	//			   mlisp_get_filename(), mlisp_get_line_num());

	symbol_enforce_type_assert(&pading, INT);
	//MGTK_ASSERTMSG(pading, "pading == INT\nMLISP (%s:%i)", 
	//			   mlisp_get_filename(), mlisp_get_line_num());

	if (homogeneous && spacing && expand && fill && pading)
	{
		GtkWidget *vbox;
		vbox = mgtk_create_vbox((GtkWidget *)box->data,
								"vbox", 
								get_int(homogeneous),
								get_int(spacing),
								get_int(expand),
								get_int(fill),
								get_int(pading));

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
	arg_list_t *homogeneous, *spacing, *expand, *fill, *pading, *ret = NULL;
	GtkWidget *hbox;

	arg_enforce_type(&box, ARG_GTK_BOX_WIDGET);
	MGTK_ASSERTMSG(box, "box == ARG_GTK_BOX_WIDGET\nMLISP (%s:%i)", 
				   mlisp_get_filename(), mlisp_get_line_num());

	if (!box)
	{
		return NULL;
	}

	symbol_enforce_type_assert(&homogeneous, INT);
	//MGTK_ASSERTMSG(homogeneous, "homogeneous == INT\nMLISP (%s:%i)", 
	//			   mlisp_get_filename(), mlisp_get_line_num());

	symbol_enforce_type_assert(&spacing, INT); 
	//MGTK_ASSERTMSG(spacing, "spacing == INT\nMLISP (%s:%i)", 
	//			   mlisp_get_filename(), mlisp_get_line_num());

	symbol_enforce_type_assert(&expand, INT);
	//MGTK_ASSERTMSG(expand, "expand == INT\nMLISP (%s:%i)", 
	//			   mlisp_get_filename(), mlisp_get_line_num());

	symbol_enforce_type_assert(&fill, INT);
	//MGTK_ASSERTMSG(fill, "fill == INT\nMLISP (%s:%i)", 
	//			   mlisp_get_filename(), mlisp_get_line_num());

	symbol_enforce_type_assert(&pading, INT);
	//MGTK_ASSERTMSG(pading, "pading == INT\nMLISP (%s:%i)", 
	//			   mlisp_get_filename(), mlisp_get_line_num());


	if (homogeneous && spacing && expand && fill && pading)
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
	arg_list_t *title, *eOpen, *eClose, *ret = NULL;
	GtkWidget *widget;

	// NOTE: box isn't used b/c in this implementation dialog is a window
	//       which is a top level container!
	symbol_enforce_type_assert(&title, CSTRING);
	//MGTK_ASSERTMSG(title, "title == CSTRING\nMLISP (%s:%i)", 
	//			   mlisp_get_filename(), mlisp_get_line_num());

	symbol_enforce_type_assert(&eOpen, INT);
	//MGTK_ASSERTMSG(eOpen, "eOpen == INT\nMLISP (%s:%i)", 
	//			   mlisp_get_filename(), mlisp_get_line_num());

	symbol_enforce_type_assert(&eClose, INT); 
	//MGTK_ASSERTMSG(eClose, "eClose == INT\nMLISP (%s:%i)", 
	//			   mlisp_get_filename(), mlisp_get_line_num());

	if (title && eOpen && eClose)
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
	arg_list_t *type, *ret = NULL;
	GtkWidget *handlebox;

	arg_enforce_type(&box, ARG_GTK_BOX_WIDGET);
	MGTK_ASSERTMSG(box, "box == ARG_GTK_BOX_WIDGET\nMLISP (%s:%i)", 
				   mlisp_get_filename(), mlisp_get_line_num());

	if (!box)
	{
		return NULL;
	}

	symbol_enforce_type_assert(&type, INT);
	//MGTK_ASSERTMSG(type, "type == INT\nMLISP (%s:%i)", 
	//			   mlisp_get_filename(), mlisp_get_line_num());

	if (type)
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
	arg_enforce_type(&container,  ARG_GTK_BOX_WIDGET);
	MGTK_ASSERTMSG(container, "container == ARG_GTK_BOX_WIDGET\nMLISP (%s:%i)", 
				   mlisp_get_filename(), mlisp_get_line_num());

	if (!container)
	{
		return NULL;
	}

	arg_list_t *label = NULL;
	symbol_enforce_type_assert(&label, CSTRING);
	//MGTK_ASSERTMSG(label, "label == CSTRING\nMLISP (%s:%i)", 
	//			   mlisp_get_filename(), mlisp_get_line_num());

	arg_list_t *cmd = NULL;
	symbol_enforce_type_assert(&cmd, INT);
	//MGTK_ASSERTMSG(cmd, "cmd == INT\nMLISP (%s:%i)", 
	//			   mlisp_get_filename(), mlisp_get_line_num());

	arg_list_t *event = NULL;
	if (mlisp_peek_for_vargs())
	{
		symbol_enforce_type_assert(&event, INT);
		//MGTK_ASSERTMSG(event, "event == INT\nMLISP (%s:%i)", 
		//		   mlisp_get_filename(), mlisp_get_line_num());
	}

	arg_list_t *ret = NULL;
	if (label && cmd)
	{
		GtkWidget *item = gtk_toggle_button_new_with_label(get_string(label));
		gtk_widget_ref(item);
		gtk_object_set_data_full(GTK_OBJECT((GtkWidget *)container->data), 
								 "button1", item,
								 (GtkDestroyNotify)gtk_widget_unref);
		gtk_widget_show(item);

		gtk_box_pack_start(GTK_BOX((GtkWidget *)container->data), 
						   item, TRUE, TRUE, 0);

		if (event)
		{
			gtk_object_set_data(GTK_OBJECT(item), "mlisp_event", 
								GINT_TO_POINTER(get_int(event)));

			gtk_signal_connect(GTK_OBJECT(item), "toggled",
							   GTK_SIGNAL_FUNC(mgtk_tool_toggle_button_dual_handler), 
							   GINT_TO_POINTER(get_int(cmd)));
		}
		else
		{
			gtk_signal_connect(GTK_OBJECT(item), "toggled",
							   GTK_SIGNAL_FUNC(mgtk_tool_toggle_button_handler), 
							   GINT_TO_POINTER(get_int(cmd)));
		}

		new_adt(&ret, ARG_GTK_WIDGET, (void *)item);
	}

	delete_arg(&label);
	delete_arg(&event);
	delete_arg(&cmd);

	return ret;
}


arg_list_t *mgtk_rc_colorbutton(arg_list_t *container)
{
	arg_list_t *event, *cmd, *ret = NULL;
	GtkWidget *item;

	arg_enforce_type(&container, ARG_GTK_BOX_WIDGET);
	MGTK_ASSERTMSG(container, "container == ARG_GTK_BOX_WIDGET");

	if (!container)
	{
		return NULL;
	}

	symbol_enforce_type_assert(&event, INT);
	//MGTK_ASSERTMSG(event, "event == INT");

	symbol_enforce_type_assert(&cmd, INT);
	//MGTK_ASSERTMSG(cmd, "cmd == INT");

	if (event && cmd)
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
	arg_list_t *label, *event, *cmd, *ret = NULL;
	GtkWidget *item;
	void *agtk_event;

	arg_enforce_type(&container,  ARG_GTK_BOX_WIDGET);
	MGTK_ASSERTMSG(container, "container == ARG_GTK_BOX_WIDGET");

	if (!container)
	{
		return NULL;
	}

	symbol_enforce_type_assert(&label, CSTRING);
	//MGTK_ASSERTMSG(label, "label == CSTRING");

	symbol_enforce_type_assert(&event, INT);
	//MGTK_ASSERTMSG(event, "event == INT");

	symbol_enforce_type_assert(&cmd, INT);
	//MGTK_ASSERTMSG(cmd, "cmd == INT");

	if (label && event && cmd)
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

	arg_enforce_type(&container,  ARG_GTK_BOX_WIDGET);
	MGTK_ASSERTMSG(container, "container == ARG_GTK_BOX_WIDGET");

	if (!container)
	{
		return NULL;
	}

	symbol_enforce_type_assert(&event, INT);
	//MGTK_ASSERTMSG(event, "event == INT");

	symbol_enforce_type_assert(&min, INT);
	//MGTK_ASSERTMSG(min, "min == INT");

	symbol_enforce_type_assert(&max, INT);
	//MGTK_ASSERTMSG(max, "max == INT");

	if (event && min && max)
	{
		/* slider */
		adj = gtk_adjustment_new(0, get_int(min), get_int(max), 1, 2, 0);
		item = gtk_hscale_new(GTK_ADJUSTMENT(adj));
		gtk_widget_ref(item);
		//gtk_widget_set_usize(item, 132, -2);
		gtk_scale_set_digits(GTK_SCALE(item), 0);

		gtk_signal_connect(GTK_OBJECT(adj), "value_changed",
						   GTK_SIGNAL_FUNC(mgtk_event_slider1u), 
						   GINT_TO_POINTER(get_int(event)));
	
		//	gtk_box_pack_start(GTK_BOX(box), hscale, FALSE, FALSE, 0);
		gtk_box_pack_start(GTK_BOX((GtkWidget *)container->data),
						   item, TRUE, TRUE, 0);
		gtk_widget_show(item);
	
		new_adt(&ret, ARG_GTK_WIDGET, (void *)item); // ARG_GTK_SPINBUTTON_WIDGET
	}

	delete_arg(&event);
	delete_arg(&min);
	delete_arg(&max);

	return ret;
}


arg_list_t *mgtk_rc_textbox(arg_list_t *container)
{
	arg_list_t *ret = NULL, *event;
	GtkWidget *item;

	arg_enforce_type(&container,  ARG_GTK_BOX_WIDGET);
	MGTK_ASSERTMSG(container, "container == ARG_GTK_BOX_WIDGET");

	if (!container)
	{
		return NULL;
	}

	symbol_enforce_type_assert(&event, INT);
	//MGTK_ASSERTMSG(event, "event == INT");

	if (event)
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

	delete_arg(&event);

	return ret;
}


arg_list_t *mgtk_rc_hsep(arg_list_t *container)
{
	arg_list_t *ret = NULL;
	GtkWidget *item;

	arg_enforce_type(&container,  ARG_GTK_BOX_WIDGET);
	MGTK_ASSERTMSG(container, "container == ARG_GTK_BOX_WIDGET");

	if (!container)
	{
		return NULL;
	}

	item = gtk_hseparator_new();
	gtk_box_pack_start(GTK_BOX((GtkWidget *)container->data), 
					   item, TRUE, TRUE, 0);
	gtk_widget_show(item);
	
	new_adt(&ret, ARG_GTK_WIDGET, (void *)item); // ARG_GTK_SPINBUTTON_WIDGET

	return ret;
}


arg_list_t *mgtk_rc_vsep(arg_list_t *container)
{
	arg_list_t *ret = NULL;
	GtkWidget *item;

	arg_enforce_type(&container,  ARG_GTK_BOX_WIDGET);
	MGTK_ASSERTMSG(container, "container == ARG_GTK_BOX_WIDGET");

	if (!container)
	{
		return NULL;
	}

	item = gtk_vseparator_new();
	gtk_box_pack_start(GTK_BOX((GtkWidget *)container->data), 
					   item, TRUE, TRUE, 0);
	gtk_widget_show(item);
	
	new_adt(&ret, ARG_GTK_WIDGET, (void *)item); // ARG_GTK_SPINBUTTON_WIDGET

	return ret;
}


arg_list_t *mgtk_rc_label2(arg_list_t *container)
{
	arg_list_t *label, *ret = NULL;
	GtkWidget *item;

	arg_enforce_type(&container,  ARG_GTK_BOX_WIDGET);
	MGTK_ASSERTMSG(container, "container == ARG_GTK_BOX_WIDGET");

	if (!container)
	{
		return NULL;
	}

	// optional
	symbol_enforce_type(&label, CSTRING);

	if (label)
	{
		item = gtk_label_new(NULL);
		gtk_label_set_markup(GTK_LABEL(item), get_string(label));

		gtk_box_pack_start(GTK_BOX((GtkWidget *)container->data), 
						   item, TRUE, TRUE, 0);

		gtk_widget_show(item);
	
		new_adt(&ret, ARG_GTK_WIDGET, (void *)item); 
	}

	delete_arg(&label);

	return ret;
}


arg_list_t *mgtk_rc_label(arg_list_t *container)
{
	arg_list_t *label, *x_align, *y_align, *ret = NULL;
	GtkWidget *item;

	arg_enforce_type(&container,  ARG_GTK_BOX_WIDGET);
	MGTK_ASSERTMSG(container, "container == ARG_GTK_BOX_WIDGET");

	if (!container)
	{
		return NULL;
	}

	symbol_enforce_type_assert(&label, CSTRING);
	//MGTK_ASSERTMSG(label, "label == CSTRING");

	symbol_enforce_type_assert(&x_align, FLOAT);
	//MGTK_ASSERTMSG(x_align, "x_align == FLOAT");

	symbol_enforce_type_assert(&y_align, FLOAT);
	//MGTK_ASSERTMSG(y_align, "y_align == FLOAT");

	if (label && x_align && y_align)
	{
		item = mgtk_create_label((GtkWidget *)container->data, 
								 get_string(label), // name
								 get_string(label), // label text
								 get_float(x_align),
								 get_float(y_align));
		
		gtk_box_pack_start(GTK_BOX((GtkWidget *)container->data), 
								 item, TRUE, TRUE, 0);

		gtk_widget_show(item);
	
		new_adt(&ret, ARG_GTK_WIDGET, (void *)item); //ARG_GTK_SPINBUTTON_WIDGET
	}

	delete_arg(&label);
	delete_arg(&x_align);
	delete_arg(&y_align);

	return ret;
}


arg_list_t *mgtk_rc_icon(arg_list_t *container)
{
	arg_list_t *name, *size, *ret = NULL;
	GtkWidget *icon;

	arg_enforce_type(&container,  ARG_GTK_BOX_WIDGET);
	MGTK_ASSERTMSG(container, "container == ARG_GTK_BOX_WIDGET");

	if (!container)
	{
		return NULL;
	}

	symbol_enforce_type_assert(&name, CSTRING);
	//MGTK_ASSERTMSG(name, "name == CSTRING");
	
	symbol_enforce_type_assert(&size, INT);
	//MGTK_ASSERTMSG(size, "size == INT");
	
	if (name && size)
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
	arg_list_t *start, *min, *max, *event, *ret = NULL;

	arg_enforce_type(&container,  ARG_GTK_BOX_WIDGET);
	MGTK_ASSERTMSG(container, "container == ARG_GTK_BOX_WIDGET");

	if (!container)
	{
		return NULL;
	}

	symbol_enforce_type_assert(&start, INT | FLOAT);
	//MGTK_ASSERTMSG(start, "start == NUMERIC");

	symbol_enforce_type_assert(&min,   INT | FLOAT);
	//MGTK_ASSERTMSG(min, "min == NUMERIC");

	symbol_enforce_type_assert(&max,   INT | FLOAT);
	//MGTK_ASSERTMSG(max, "max == NUMERIC");

	symbol_enforce_type_assert(&event, INT);
	//MGTK_ASSERTMSG(event, "event == INT");

	if (start && min && max && event)
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
	arg_list_t *start, *min, *max, *event, *ret = NULL;
	arg_list_t *step, *page, *page_sz, *digit;

	arg_enforce_type(&container,  ARG_GTK_BOX_WIDGET);
	MGTK_ASSERTMSG(container, "container == ARG_GTK_BOX_WIDGET");

	if (!container)
	{
		return NULL;
	}

	symbol_enforce_type_assert(&start, INT | FLOAT);
	//MGTK_ASSERTMSG(start, "start == NUMERIC");

	symbol_enforce_type_assert(&min,   INT | FLOAT);
	//MGTK_ASSERTMSG(min, "min == NUMERIC");

	symbol_enforce_type_assert(&max,   INT | FLOAT);
	//MGTK_ASSERTMSG(max, "max == NUMERIC");

	symbol_enforce_type_assert(&step, INT | FLOAT);
	//MGTK_ASSERTMSG(step, "step == NUMERIC");

	symbol_enforce_type_assert(&page, INT | FLOAT);
	//MGTK_ASSERTMSG(page, "page == NUMERIC");

	symbol_enforce_type_assert(&page_sz, INT | FLOAT);
	//MGTK_ASSERTMSG(page_sz, "page_sz == NUMERIC");	

	symbol_enforce_type_assert(&digit, INT);
	//MGTK_ASSERTMSG(digit, "digit == INT");

	symbol_enforce_type_assert(&event, INT);
	//MGTK_ASSERTMSG(event, "event == INT");

	if (start && min && max && event && step && page && page_sz && digit)
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
	GtkWidget *gicon = gtk_image_new_from_stock(stockId, size);
	GtkToolItem *item = gtk_tool_button_new(gicon, (!name[0]) ? NULL : name);
	gtk_widget_show(gicon);
	GtkWidget *button = (GtkWidget *)item;
	//gtk_toolbar_append_widget((GtkToolbar *)toolbar, button, NULL, NULL);
	gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1); // Append to the end	

	gtk_widget_ref(button);
	gtk_object_set_data_full(GTK_OBJECT(box), buttonId, button,
							 (GtkDestroyNotify)gtk_widget_unref);
	gtk_widget_show(button);

	return button;
}


arg_list_t *mgtk_rc_optionmenu(arg_list_t *box)
{
	GtkWidget *optionmenu, *optionmenu_menu;
	arg_list_t *ret = NULL, *event, *cmd;

	arg_enforce_type(&box, ARG_GTK_BOX_WIDGET); // ARG_GTK_BOX_WIDGET
	MGTK_ASSERTMSG(box, "container == ARG_GTK_BOX_WIDGET");

	if (!box)
	{
		return NULL;
	}

	symbol_enforce_type_assert(&event, INT);
	//MGTK_ASSERTMSG(event, "event == INT");

	symbol_enforce_type_assert(&cmd, INT);
	//MGTK_ASSERTMSG(cmd, "cmd == INT");

	if (event && cmd)
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


////////////////////////////////////////////////////////////////
// Gtk+ menu widgets
////////////////////////////////////////////////////////////////

arg_list_t *mgtk_rc_menu_seperator(arg_list_t *container)
{
	GtkWidget *sep;
	arg_list_t *ret = NULL;

	arg_enforce_type(&container,  ARG_GTK_MENU_WIDGET);
	MGTK_ASSERTMSG(container, "menu == ARG_GTK_MENU_WIDGET");

	if (!container)
	{
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


void mgtk_toggle_value_set(int event, int val);
arg_list_t *mgtk_func_toggle_set(arg_list_t *args)
{
	arg_list_t *event, *val;

	symbol_enforce_type_assert(&event, INT);
	//event = symbol();
	//arg_enforce_type(&event, INT);
	//MGTK_ASSERTMSG(event, "event == INT");

	symbol_enforce_type_assert(&val, INT);
	//val = symbol();
	//arg_enforce_type(&val, INT);
	//MGTK_ASSERTMSG(val, "val == INT");

	if (event && val)
	{
		mgtk_toggle_value_set(get_int(event), get_int(val));
	}

	delete_arg(&event);
	delete_arg(&val);

	return NULL;
}


// (menu_item_toggle "Label" eToggleEvent InitStateBool)
void mgtk_check_menu_item_handler(GtkCheckMenuItem *item, gpointer e)
{
	mgtk_handle_event1u(GPOINTER_TO_INT(e), 
						gtk_check_menu_item_get_active(item));

	// Update any other widgets sharing the same event
	mgtk_toggle_value_set(GPOINTER_TO_INT(e), 
						  gtk_check_menu_item_get_active(item));
}

arg_list_t *mgtk_rc_check_menu_item(arg_list_t *menu)
{
	GtkWidget *item;
	arg_list_t *text, *event, *cmd, *ret = NULL;

	arg_enforce_type(&menu,  ARG_GTK_MENU_WIDGET);
	MGTK_ASSERTMSG(menu, "menu == ARG_GTK_MENU_WIDGET");

	if (!menu)
	{
		return NULL;
	}

	symbol_enforce_type_assert(&text,  CSTRING);
	//text = symbol();
	//arg_enforce_type(&text,  CSTRING);
	//MGTK_ASSERTMSG(text, "text == CSTRING");

	symbol_enforce_type_assert(&event, INT);
	//event = symbol();
	//arg_enforce_type(&event, INT);
	//MGTK_ASSERTMSG(event, "event == INT");

	symbol_enforce_type_assert(&cmd, INT);
	//cmd = symbol();
	//arg_enforce_type(&cmd, INT);
	//MGTK_ASSERTMSG(cmd, "cmd == INT");

	if (text && event && cmd)
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


void mgtk_accel_support(GtkWidget *item, arg_list_t *accel)
{
#ifdef ACCEL_SUPPORT_ON

	if (accel != 0 && accel->data != 0 && ((char *)accel->data)[0] != 0)
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
#endif
}


// Four cases
//  (menu_item "Label"		  eEvent)
//  (menu_item "Label"		  eEvent       number-or-subevent)
//  (menu_item "Label"        eEvent	   "gtk-icon")
//  (menu_item "Label"        eEvent	   "accel"        "gtk-icon")
arg_list_t *mgtk_rc_menu_item(arg_list_t *menu)
{
	// 2006.11.13, Mongoose - I made this easier to read and better for you too!
	arg_enforce_type(&menu,  ARG_GTK_MENU_WIDGET);
	MGTK_ASSERTMSG(menu, "menu == ARG_GTK_MENU_WIDGET\nMLISP (%s:%i)", 
				   mlisp_get_filename(), mlisp_get_line_num());
	if (!menu)
	{
		return NULL;
	}

	arg_list_t *ret = NULL, *label, *event;

	symbol_enforce_type_assert(&label,  CSTRING);
	//arg_list_t *label = symbol();
	//arg_enforce_type(&label,  CSTRING);
	//MGTK_ASSERTMSG(label, "label == CSTRING\nMLISP (%s:%i)", 
	//			   mlisp_get_filename(), mlisp_get_line_num());

	symbol_enforce_type_assert(&event, INT);
	//arg_list_t *event = symbol();
	//arg_enforce_type(&event, INT);
	//MGTK_ASSERTMSG(event, "event == INT\nMLISP (%s:%i)", 
	//			   mlisp_get_filename(), mlisp_get_line_num());

	arg_list_t *subevent = NULL;
	arg_list_t *icon = NULL;
	if (mlisp_peek_for_vargs())
	{
		icon = symbol();

		if (mlisp_is_cstring(icon))
		{
			arg_enforce_type_assert(&icon, CSTRING);
			//arg_enforce_type(&icon, CSTRING);
			//MGTK_ASSERTMSG(icon, "icon|accel == CSTRING\nMLISP (%s:%i)", 
			//	   mlisp_get_filename(), mlisp_get_line_num());
		}
		else
		{
			subevent = icon;
			icon = NULL;
			arg_enforce_type_assert(&subevent, INT);
			//arg_enforce_type(&subevent, INT);
			//MGTK_ASSERTMSG(subevent, "subevent == INT\nMLISP (%s:%i)", 
			//	   mlisp_get_filename(), mlisp_get_line_num());
		}
	}

	arg_list_t *accel = NULL;
	if (mlisp_peek_for_vargs())
	{
		accel = icon;
		icon = symbol();
		arg_enforce_type_assert(&icon, CSTRING);
		//MGTK_ASSERTMSG(icon, "icon == CSTRING\nMLISP (%s:%i)", 
		//		   mlisp_get_filename(), mlisp_get_line_num());
	}

	if (label && event)
	{
		GtkWidget *item = gtk_image_menu_item_new_with_mnemonic((char *)label->data);

		if (icon)
		{
			gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(item),
										  mgtk_create_icon((char *)icon->data, 
														   GTK_ICON_SIZE_MENU));
		}

		if (accel)
		{
			mgtk_accel_support(item, accel);
		}

		new_adt(&ret, ARG_GTK_MENU_WIDGET, (void *)item);		
		gtk_menu_append(GTK_MENU(menu->data), item);
		gtk_widget_show(item);

		if (subevent)
		{
			gtk_object_set_data(GTK_OBJECT(item), "mlisp_event", 
								GINT_TO_POINTER(get_int(subevent)));
			gtk_signal_connect(GTK_OBJECT(item), "activate",
							   GTK_SIGNAL_FUNC(mgtk_event_dual_command), 
							   GINT_TO_POINTER(get_int(event)));
		}
		else
		{
			gtk_signal_connect(GTK_OBJECT(item), "activate",
							   GTK_SIGNAL_FUNC(mgtk_event_command), 
							   GINT_TO_POINTER(get_int(event)));
		}
	}

	delete_arg(&label);
	delete_arg(&event);
	delete_arg(&subevent);
	delete_arg(&icon);
	delete_arg(&accel);

	return ret;
}


arg_list_t *mgtk_rc_submenu(arg_list_t *menu)
{
	arg_list_t *label, *ret = NULL;
	int id = -1;

	arg_enforce_type(&menu, ARG_GTK_MENU_WIDGET);
	MGTK_ASSERTMSG(menu, "menu == ARG_GTK_MENU_WIDGET\nMLISP (%s:%i)", 
				   mlisp_get_filename(), mlisp_get_line_num());

	if (!menu)
	{
		return NULL;
	}

	label = symbol();

	if (label->type == INT)
	{
		id = get_int(label);
		delete_arg(&label);
		label = symbol();
	}

	arg_enforce_type_assert(&label, CSTRING);
	//MGTK_ASSERTMSG(label, "label == CSTRING\nMLISP (%s:%i)", 
	//			   mlisp_get_filename(), mlisp_get_line_num());

	if (label)
	{
		GtkWidget *item = gtk_image_menu_item_new_with_mnemonic((char *)label->data);
		GtkWidget *submenu = gtk_menu_new();
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
	MGTK_ASSERTMSG(arg, "window == ARG_GTK_BOX_WIDGET");

	if (!arg)
	{
		return NULL;
	}

	app = (GtkWidget *)arg->data;
	MGTK_ASSERTMSG(app, "app != NULL");

	if (!app)
	{
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
	arg_enforce_type(&arg, ARG_GTK_BOX_WIDGET);
	MGTK_ASSERTMSG(arg, "window == ARG_GTK_BOX_WIDGET");

	if (!arg)
	{
		return NULL;
	}

	arg_list_t *ret = NULL;
	//GtkWidget *app = (GtkWidget *)arg->data;
	MGTK_ASSERTMSG(arg->data, "app != NULL");

	if (arg && arg->data)
	{
		GtkWidget *menubar = gtk_menu_bar_new();
		gtk_widget_show(menubar);
		gtk_container_add(GTK_CONTAINER(arg->data), menubar);
		new_adt(&ret, ARG_GTK_MENU_WIDGET, (void *)menubar);
	}

	return ret;
}


/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : 
 * License : GPL See file COPYING, also (C) 2000, 2001 Mongoose
 * Comments: Gtk glarea widget utils
 *
 * 
 *-- History ------------------------------------------------- 
 *
 * 2000-09-16:
 * Mongoose - Created
 =================================================================*/

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

#ifdef GTK_GLAREA_GL
#   include <gtkgl/gtkglarea.h>
#endif

#include "gnome_callbacks.h"
#include "gnome_gtkglarea.h"
#include "gnome_interface.h"
#include "freyja_events.h"


#ifdef GTK_GLAREA_GL
GtkWidget *GTK_GLAREA_WINDOW = NULL;

///////////////////////////////////////////////////////////////////////////
// Mongoose 2002.01.12, Below here implement interfaces needed by some events
///////////////////////////////////////////////////////////////////////////


void event_swap_buffers()
{
	// Hhhmm...  nothing really needed here, swapping is handled in 
	// the gtk glarea interfacing code after draw requests

	usleep(1000);
}


void glarea_refresh(GtkWidget *widget);

void event_refresh()
{
	if (GTK_GLAREA_WINDOW)
	{
		glarea_refresh(GTK_GLAREA_WINDOW);
	}
}


//////////////////////////////////////////////////////////////////////
// Mongoose 2002.01.12, Move below into it's own file as it's reusable
//////////////////////////////////////////////////////////////////////


void glarea_refresh(GtkWidget *widget)
{
	GdkRectangle area;


	area.x = 0;
	area.y = 0;
	area.width  = widget->allocation.width;
	area.height = widget->allocation.height;

	gtk_widget_draw(widget, &area);
}


gint glarea_init(GtkWidget* widget)
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

	return TRUE;
}


gint glarea_expose(GtkWidget *widget, GdkEventExpose *event)
{
	GtkGLArea *glarea = GTK_GL_AREA(widget);

	
	/* draw only last expose */
	if (event->count > 0) 
	{
		return TRUE;
	}

	/* OpenGL calls can be done only if make_current returns true */
	if (gtk_gl_area_make_current(glarea)) 
	{
		/* Call modeler's renderer */
		event_display();
		
		/* Swap buffers to rasterize */
		gtk_gl_area_swapbuffers(glarea);
	}
	
	return TRUE;
}


gint glarea_resize(GtkWidget *widget, GdkEventConfigure *event)
{
	/* OpenGL calls can be done only if make_current returns true */
	if (gtk_gl_area_make_current(GTK_GL_AREA(widget))) 
	{
		//glViewport(0, 0, widget->allocation.width, widget->allocation.height);
		event_resize(widget->allocation.width, widget->allocation.height);
	}
	
	return TRUE;
}


gint glarea_destroy(GtkWidget *widget)
{
	glarea_window_state_t *state;


	state = (glarea_window_state_t*)gtk_object_get_data(GTK_OBJECT(widget),
																		 "gl_window_state");

	if (state) 
	{
		delete state;
	}
	
	return TRUE;
}


gint glarea_button_press(GtkWidget *widget, GdkEventButton *event)
{
	GdkModifierType state;
	glarea_window_state_t *gl_state;
	int x, y, mod, btn_state, button;


	gl_state = (glarea_window_state_t*)gtk_object_get_data(GTK_OBJECT(widget),
																			 "gl_window_state");
	
	/* Beginning of drag, reset mouse position */
	gdk_window_get_pointer(event->window, &x, &y, &state);
  
	gl_state->mouse_x = x;
	gl_state->mouse_y = y;

	btn_state = MOUSE_BTN_STATE_PRESSED;
	button = 0;
	mod = 0;

	if (event->state & GDK_CONTROL_MASK)
		mod |= KEY_RCTRL;

	if (state & GDK_BUTTON1_MASK)
	{
		button |= MOUSE_BTN_LEFT;
	}

	if (state & GDK_BUTTON2_MASK)
	{
		button |= MOUSE_BTN_MIDDLE;
	}

	if (state & GDK_BUTTON3_MASK)
	{
		button |= MOUSE_BTN_RIGHT;
	}

	if (state & GDK_BUTTON4_MASK)
	{
		button |= MOUSE_BTN_4;
	}

	if (state & GDK_BUTTON5_MASK)
	{
		button |= MOUSE_BTN_5;
	}

	event_set_mouse_button(button);

#define DEBUG_GTK_MOUSE
#ifdef DEBUG_GTK_MOUSE
	char foo[32];
	sprintf(foo, "MOUSE_BTN_%i", button);
	event_print("Sending mouse event to Freya; x: %i y: %i, %s, %s\n", 
					x, y, 
					(button == MOUSE_BTN_LEFT) ? "MOUSE_BTN_LEFT" :
					(button == MOUSE_BTN_RIGHT) ? "MOUSE_BTN_RIGHT" :
					(button == MOUSE_BTN_MIDDLE) ? "MOUSE_BTN_MIDDLE" :
					(button == MOUSE_BTN_4) ? "MOUSE_BTN_4" :
					(button == MOUSE_BTN_5) ? "MOUSE_BTN_5" :
					foo, 
					(btn_state==MOUSE_BTN_STATE_PRESSED)?"MOUSE_BTN_STATE_PRESSED":
					"MOUSE_BTN_STATE_NONE");
#endif
	event_mouse(button, btn_state, mod, x, y);
	glarea_refresh(widget);
	
	return FALSE;
}


gint glarea_button_release(GtkWidget *widget, GdkEventButton *event)
{
	event_set_mouse_button(0);

	return TRUE;
}


gint glarea_key_press_event(GtkWidget *widget, GdkEventKey *event)
{
	unsigned int mod = 0;
	int key = -1;


	//gtk_widget_add_accelerator(item, "activate_item", accel, 
	//                           tmp_key, GDK_MOD1_MASK, (GtkAccelFlags)0);

	if (event->state & GDK_CONTROL_MASK)
	{
		mod |= KEY_RCTRL;
	}

	if (event->state & GDK_SHIFT_MASK)
	{
		mod |= KEY_RSHIFT;
	}

	switch (event->keyval)
	{
	case GDK_Left:
		key = KEY_LEFT;
		break;
	case GDK_Right:
		key = KEY_RIGHT;
		break;
	case GDK_Up:
		key = KEY_UP;
		break;
	case GDK_space:
		key = KEY_SPACE;
		break;
	case GDK_Down:
		key = KEY_DOWN;
		break;
	case GDK_F1:
		key = KEY_F1;
		break;		
	case GDK_F2:
		key = KEY_F2;
		break;		
	case GDK_F3:
		key = KEY_F3;
		break;		
	case GDK_F4:
		key = KEY_F4;
		break;
	case GDK_F5:
		key = KEY_F5;
		break;		
	case GDK_F6:
		key = KEY_F6;
		break;		
	case GDK_F7:
		key = KEY_F7;
		break;
	case GDK_F8:
		key = KEY_F8;
		break;		
	case GDK_F9:
		key = KEY_F9;
		break;
	case GDK_F10:
		key = KEY_F10;
		break;		
	case GDK_F11:
		key = KEY_F11;
		break;		
	case GDK_F12:
		key = KEY_F12;
		break;
	default:
		if ((unsigned char)event->keyval < 128)
		{
			key = (unsigned char)event->keyval;
		}
	}

	/* Prevent the default handler from being run */
	//gtk_signal_emit_stop_by_name(GTK_OBJECT(widget), "key_press_event");

	if (key < 0)
	{
		return TRUE;
	}

#ifdef DEBUG_GTK_KEYS
	event_print("*** key_press_event> %i (%c) : %i\n", key, key, mod);
#endif
	event_key(key, mod);

	if (gtk_accelerator_valid(event->keyval, (GdkModifierType)0))
		return TRUE;
	
	return TRUE;
}


gint glarea_key_release_event(GtkWidget *widget, GdkEventKey *event)
{
	//event_print("*** key_release_event> %i : %i\n", key, mod);

  /* Prevent the default handler from being run */
  //gtk_signal_emit_stop_by_name(GTK_OBJECT(widget), "key_release_event");

  return TRUE;
}


gint glarea_motion_notify(GtkWidget *widget, GdkEventMotion *event)
{
	int x, y;
	GdkModifierType state;
	glarea_window_state_t *gl_state;


	if (!query_mouse_active())
		return TRUE;

	gl_state = (glarea_window_state_t*)gtk_object_get_data(GTK_OBJECT(widget),
																			 "gl_window_state");

	if (event->is_hint)
	{
		gdk_window_get_pointer(event->window, &x, &y, &state);
	} 
	else 
	{
		x = (int)event->x;
		y = (int)event->y;
		state = (GdkModifierType)event->state;
	}

	if (query_mouse_button())
	{
		if (query_mouse_button() & MOUSE_BTN_MIDDLE)
		{
			if (x > gl_state->mouse_x)
				freyja_event2i(EVENT_MISC, CMD_MISC_SCROLL_LEFT);
			else if (x < gl_state->mouse_x)
				freyja_event2i(EVENT_MISC, CMD_MISC_SCROLL_RIGHT);

			if (y < gl_state->mouse_y)
				freyja_event2i(EVENT_MISC, CMD_MISC_SCROLL_UP);
			else if (y > gl_state->mouse_y)
				freyja_event2i(EVENT_MISC, CMD_MISC_SCROLL_DOWN);

			//gtk_ruler_set_range(GTK_RULER(hruler), x1, x2, 0, x2);

			gl_state->mouse_x = x;
			gl_state->mouse_y = y;
			glarea_refresh(widget);			
		}
		else
		{
#ifdef DEBUG_GTK_MOUSE
			event_print("Gnome sending event_motion %i, %i\n", x, y);
#endif
			gl_state->mouse_x = x;
			gl_state->mouse_y = y;
			event_motion(x, y);
			glarea_refresh(widget);
		}
	}
	
	return TRUE;
}


GtkWidget *glarea_create(unsigned int width, unsigned int height)
{
	GtkWidget *glarea;
	glarea_window_state_t *state;


	event_print("@Gtk+ GL context started...");

	if (gdk_gl_query() == FALSE)
	{
		event_print("ERROR: OpenGL display not supported by this system?\n");
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

	gtk_signal_connect(GTK_OBJECT(glarea), "expose_event",
							 GTK_SIGNAL_FUNC(glarea_expose), NULL);
	gtk_signal_connect(GTK_OBJECT(glarea), "motion_notify_event",
							 GTK_SIGNAL_FUNC(glarea_motion_notify), NULL);
	gtk_signal_connect(GTK_OBJECT(glarea), "button_press_event",
							 GTK_SIGNAL_FUNC(glarea_button_press), NULL);
	gtk_signal_connect(GTK_OBJECT(glarea), "button_release_event",
							 GTK_SIGNAL_FUNC(glarea_button_release), NULL);
	gtk_signal_connect(GTK_OBJECT(glarea), "configure_event",
							 GTK_SIGNAL_FUNC(glarea_resize), NULL);
	gtk_signal_connect(GTK_OBJECT(glarea), "realize",
							 GTK_SIGNAL_FUNC(glarea_init), NULL);
	gtk_signal_connect(GTK_OBJECT(glarea), "destroy",
							 GTK_SIGNAL_FUNC(glarea_destroy), NULL);

	/* Setup GL window state */
	state = new glarea_window_state_t;
	state->init = false;
	state->plane = 0;
	state->mouse_x = 0;
	state->mouse_y = 0;
	state->width = width;
	state->height = height;
	state->h_adj = NULL;
	state->v_adj = NULL;
	gtk_object_set_data(GTK_OBJECT(glarea), "gl_window_state", state);

	GTK_GLAREA_WINDOW = glarea;
	return glarea;
}
#else
#   error "GTK_GLAREA_GL widget not defined!"
#endif

/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Mgtk
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2007 Mongoose
 * Comments: OpenGL canvas widget(s).
 *
 * 
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.08.19:
 * Mongoose - New API, Merged glarea / glext code per function.
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

#ifdef HAVE_OPENGL
#   include <GL/gl.h>
#   include <GL/glu.h>

#   ifdef HAVE_GTKGLEXT
#      include <gtk/gtkgl.h>
#      ifdef WIN32
#         include <gdk/win32/gdkglwin32.h>
#      endif
#   elif HAVE_GTKGLAREA
#      include <gtkgl/gtkglarea.h>
#   endif
#endif

#include <mstl/String.h>
#include <mstl/SystemIO.h>

#include "mgtk_interface.h"
#include "mgtk_callbacks.h"
#include "mgtk_resource.h"
#include "mgtk_keys.h"
#include "mgtk_events.h"
#include "mgtk_opengl_canvas.h"

#define DEBUG_OPENGL_CANVAS_MOUSE 0


// FIXME: This is the obsolete timing refresh code with just an API update.
//        Remove it and the globals when the new code is in place.
int gAnimate = 0;
static guint gTimeoutId = 0;

static gboolean mgtk_opengl_canvas_timeout(GtkWidget *widget)
{
  /* Invalidate the whole window. */
  gdk_window_invalidate_rect(widget->window, &widget->allocation, FALSE);

  /* Update synchronously. */
  gdk_window_process_updates(widget->window, FALSE);

  return TRUE;
}


static void mgtk_opengl_canvas_timeout_add(GtkWidget *widget)
{
  if (gTimeoutId == 0)
	{
		// TIMEOUT_INTERVAL = 10
		gTimeoutId = g_timeout_add(10, (GSourceFunc)mgtk_opengl_canvas_timeout, widget);
	}
}


static void mgtk_opengl_canvas_timeout_remove(GtkWidget *widget)
{
	if (gTimeoutId != 0)
	{
		g_source_remove(gTimeoutId);
		gTimeoutId = 0;
	}
}


static gboolean mgtk_opengl_canvas_map_event(GtkWidget *widget, GdkEventAny *event, gpointer     data)
{
	if (gAnimate)
		mgtk_opengl_canvas_timeout_add(widget);

	return TRUE;
}


static gboolean mgtk_opengl_canvas_unmap_event(GtkWidget *widget, GdkEventAny *event, gpointer data)
{
	mgtk_opengl_canvas_timeout_remove(widget);
	return TRUE;
}


static gboolean mgtk_opengl_canvas_visibility_notify_event(GtkWidget *widget, GdkEventVisibility *event, gpointer data)
{
	//mgtk_opengl_canvas_state_t* state =
	//(mgtk_opengl_canvas_state_t*)gtk_object_get_data(GTK_OBJECT(widget), "gl_window_state");

	if (gAnimate)
	{
		if (event->state == GDK_VISIBILITY_FULLY_OBSCURED)
			mgtk_opengl_canvas_timeout_remove(widget);
		else
			mgtk_opengl_canvas_timeout_add(widget);
	}

	return TRUE;
}


void mgtk_opengl_canvas_refresh(GtkWidget* widget)
{
#if HAVE_GTKGLEXT
	GdkGLContext *glcontext = gtk_widget_get_gl_context(widget);
	GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable(widget);

	/* OpenGL calls can be done only if make_current returns true */
	if (!gdk_gl_drawable_gl_begin(gldrawable, glcontext))
		return;

	/* Call modeler's renderer */
	mgtk_handle_gldisplay();

	/* Swap buffers to rasterize */
	if (gdk_gl_drawable_is_double_buffered(gldrawable))
		gdk_gl_drawable_swap_buffers(gldrawable);
	else
		glFlush ();

	gdk_gl_drawable_gl_end(gldrawable);
#elif HAVE_GTKGLAREA
	GdkRectangle area;
	area.x = 0;
	area.y = 0;
	area.width  = widget->allocation.width;
	area.height = widget->allocation.height;

	gtk_widget_draw(widget, &area);
#endif
}


void mgtk_opengl_canvas_init(GtkWidget* widget)
{
#if HAVE_GTKGLEXT
	GdkGLContext *glcontext = gtk_widget_get_gl_context(widget);
	GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable(widget);

	if (!gdk_gl_drawable_gl_begin(gldrawable, glcontext))
	{
		g_print("*** GL context failed to init for gtkglext!\n");
		return;
	}
	else
	{
		g_print("*** GL context init for gtkglext\n");
	}

	gdk_gl_drawable_gl_end(gldrawable);
#elif HAVE_GTKGLAREA
	mgtk_opengl_canvas_state_t* state =
	(mgtk_opengl_canvas_state_t*)gtk_object_get_data(GTK_OBJECT(widget), "gl_window_state");

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
#endif
}


void mgtk_opengl_canvas_expose(GtkWidget *widget, GdkEventExpose *event)
{
#if HAVE_GTKGLEXT
	GdkGLContext *glcontext = gtk_widget_get_gl_context(widget);
	GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable(widget);

	
	/* draw only last expose */
	//if (event->count > 0) 
	//{
	//	return;
	//}

	/* OpenGL calls can be done only if make_current returns true */
	if (!gdk_gl_drawable_gl_begin(gldrawable, glcontext))
	{
		g_print("*** GL context failed to expose for gtkglext!\n");
		return;
	}

	/* Call modeler's renderer */
	mgtk_handle_gldisplay();

	/* Swap buffers to rasterize */
	if (gdk_gl_drawable_is_double_buffered(gldrawable))
		gdk_gl_drawable_swap_buffers(gldrawable);
	else
		glFlush ();

	gdk_gl_drawable_gl_end(gldrawable);
#elif HAVE_GTKGLAREA
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
		mgtk_handle_gldisplay();
		
		/* Swap buffers to rasterize */
		gtk_gl_area_swapbuffers(glarea);
	}
#endif
}


void mgtk_opengl_canvas_resize(GtkWidget *widget, GdkEventConfigure *event)
{
#if HAVE_GTKGLEXT
	GdkGLContext *glcontext = gtk_widget_get_gl_context(widget);
	GdkGLDrawable *gldrawable = gtk_widget_get_gl_drawable(widget);

	/* OpenGL calls can be done only if make_current returns true */
	if (!gdk_gl_drawable_gl_begin(gldrawable, glcontext))
	{
		g_print("*** GL context failed to resize for gtkglext!\n");
		return;
	}

	mgtk_handle_glresize(widget->allocation.width, widget->allocation.height);
	gdk_gl_drawable_gl_end(gldrawable);
#elif HAVE_GTKGLAREA
	/* OpenGL calls can be done only if make_current returns true */
	if ( gtk_gl_area_make_current( GTK_GL_AREA(widget) ) ) 
	{
		//glViewport(0, 0, widget->allocation.width, widget->allocation.height);
		mgtk_handle_glresize( widget->allocation.width, widget->allocation.height );
	}
#endif
}


void mgtk_opengl_canvas_destroy(GtkWidget* widget)
{
#if HAVE_GTKGLEXT
#elif HAVE_GTKGLAREA
	mgtk_opengl_canvas_state_t* state =
	(mgtk_opengl_canvas_state_t*)gtk_object_get_data(GTK_OBJECT(widget), "gl_window_state");

	if (state) 
	{
		delete state;
	}
#endif
}


GtkWidget* mgtk_opengl_canvas_new(unsigned int width, unsigned int height)
{
	GtkWidget* canvas = NULL;

#if HAVE_GTKGLEXT
	gint major, minor;
	gdk_gl_query_version(&major, &minor);
	g_print ("OpenGL %d.%d\n", major, minor);

	//FIXME: I bet this casting breaks the bitmap -- 
	//       thanks to whoever that wrote the bad arg types for gdk_gl_config_new_by_mode

	/* Get a double buffered gtk_gl context handle */
	GdkGLConfig* glconfig = NULL;
#   ifdef WIN32
	// Must do this since gdk_gl_config_new_by_mode has broken lib / headers ( arg is for an enum not int! )
	int args[] = {
		GDK_GL_RGBA, 
		GDK_GL_RED_SIZE, 1,  // Not bpp or color depth? 
		GDK_GL_GREEN_SIZE, 1,
  		GDK_GL_BLUE_SIZE, 1,
		GDK_GL_DEPTH_SIZE, 1,
		GDK_GL_STENCIL_SIZE, 1, 
		GDK_GL_DOUBLEBUFFER, 
		GDK_GL_ATTRIB_LIST_NONE};  

	g_print("*** Spawning double-buffered visual...\n");
	glconfig = gdk_gl_config_new(args);
#   else
	glconfig = gdk_gl_config_new_by_mode((GdkGLConfigMode)(GDK_GL_MODE_RGB | GDK_GL_MODE_DEPTH | GDK_GL_MODE_STENCIL | GDK_GL_MODE_DOUBLE));
#   endif

	if (glconfig == NULL)
    {
		g_print("*** Cannot find the double-buffered visual.\n");
		g_print("*** Trying single-buffered visual.\n");

		/* Try single-buffered visual */
#   ifdef WIN32
		// Must do this since gdk_gl_config_new_by_mode has broken lib / headers ( arg is for an enum not int! )
		int args_single[] = {
			GDK_GL_RGBA, 
			GDK_GL_RED_SIZE, 1,  // Not bpp? 
			GDK_GL_GREEN_SIZE, 1,
	  		GDK_GL_BLUE_SIZE, 1,
			GDK_GL_DEPTH_SIZE, 1, 
			GDK_GL_STENCIL_SIZE, 1,
			GDK_GL_ATTRIB_LIST_NONE};  

		glconfig = gdk_gl_config_new(args_single);
#   else
		glconfig = gdk_gl_config_new_by_mode((GdkGLConfigMode)(GDK_GL_MODE_RGB |
					    GDK_GL_MODE_DEPTH | GDK_GL_MODE_STENCIL));
#   endif

		if (glconfig == NULL)
		{
			g_print ("*** No appropriate OpenGL-capable visual found.\n");
			return NULL;
		}
    }
	else
	{
		g_print("*** Created double-buffered visual for OpenGL context.\n");
	}

	/* Canvas for drawing OpenGL scene. */
	canvas = gtk_drawing_area_new();
	gtk_widget_set_size_request(canvas, width, height); 

	/* Set OpenGL-capability to the widget. */
	gtk_widget_set_gl_capability(canvas, glconfig, NULL,
								 TRUE, GDK_GL_RGBA_TYPE);

	/* Set up events and signals for OpenGL widget */
	gtk_widget_add_events(canvas,
						  GDK_EXPOSURE_MASK |
//                        GDK_KEY_PRESS_MASK |
//                        GDK_KEY_RELEASE_MASK |
						  GDK_BUTTON_RELEASE_MASK |
//                        GDK_POINTER_MOTION_MASK

						  GDK_POINTER_MOTION_MASK |
						  GDK_POINTER_MOTION_HINT_MASK |
						  GDK_MOTION_NOTIFY |
//						  GDK_BUTTON1_MOTION_MASK |
//						  GDK_BUTTON2_MOTION_MASK |
						  GDK_BUTTON_PRESS_MASK |
						  GDK_VISIBILITY_NOTIFY_MASK);

	g_signal_connect_after(G_OBJECT(canvas), "realize",
                           G_CALLBACK(mgtk_opengl_canvas_init), NULL);
	g_signal_connect(G_OBJECT(canvas), "configure_event",
		    		 G_CALLBACK(mgtk_opengl_canvas_resize), NULL);
	g_signal_connect(G_OBJECT(canvas), "expose_event",
		 			 G_CALLBACK(mgtk_opengl_canvas_expose), NULL);

	g_signal_connect(G_OBJECT(canvas), "button_press_event",
		    		 G_CALLBACK(mgtk_event_button_press), NULL);
	g_signal_connect(G_OBJECT(canvas), "motion_notify_event",
					 G_CALLBACK(mgtk_event_mouse_motion), NULL);
	g_signal_connect(G_OBJECT(canvas), "button_release_event",
					 G_CALLBACK(mgtk_event_button_release), NULL);

	g_signal_connect(G_OBJECT(canvas), "map_event",
					 G_CALLBACK(mgtk_opengl_canvas_map_event), NULL);
	g_signal_connect(G_OBJECT(canvas), "unmap_event",
					 G_CALLBACK(mgtk_opengl_canvas_unmap_event), NULL);
	g_signal_connect(G_OBJECT(canvas), "visibility_notify_event",
					 G_CALLBACK(mgtk_opengl_canvas_visibility_notify_event), NULL);

	g_signal_connect_swapped(G_OBJECT(canvas), "key_press_event",
							 G_CALLBACK(mgtk_event_key_press), NULL);
	g_signal_connect(G_OBJECT(canvas), "key_release_event",
					 G_CALLBACK(mgtk_event_key_release), NULL);

	/* Setup GL window state */
	mgtk_opengl_canvas_state_t *state = new mgtk_opengl_canvas_state_t;
	state->init = false;
	state->plane = 0;
	state->mouse_x = 0;
	state->mouse_y = 0;
	state->width = width;
	state->height = height;	
	state->animate = 0;
	state->timeout_interval = 10;
	state->timeout_id = 0;
	gtk_object_set_data(GTK_OBJECT(canvas), "gl_window_state", state);

	gtk_widget_show(canvas);

	/* Shared code pushed here from mgtk_resource / mgtk_lua. */
	g_signal_connect( G_OBJECT(canvas), "key_press_event",
					  G_CALLBACK(mgtk_event_key_press), NULL );
	g_signal_connect( G_OBJECT(canvas), "key_release_event",
					  G_CALLBACK(mgtk_event_key_release), NULL );
	g_signal_connect( G_OBJECT(canvas), "destroy",
					  G_CALLBACK(mgtk_destroy_window), NULL );

#elif HAVE_GTKGLAREA
	if ( gdk_gl_query() == FALSE )
	{
		return NULL;
	}

	canvas = gtk_gl_area_new_vargs(NULL,
								   GDK_GL_RGBA,
								   GDK_GL_RED_SIZE,1,
								   GDK_GL_GREEN_SIZE,1,
								   GDK_GL_BLUE_SIZE,1,
								   GDK_GL_DEPTH_SIZE,1,
								   GDK_GL_DOUBLEBUFFER,
								   GDK_GL_NONE);

	gtk_gl_area_size(GTK_GL_AREA(canvas), width, height);

	/* Set up events and signals for OpenGL widget */
	gtk_widget_set_events(GTK_WIDGET(canvas),
						  GDK_EXPOSURE_MASK |
						  GDK_BUTTON_PRESS_MASK |
						  GDK_BUTTON_RELEASE_MASK |
						  GDK_POINTER_MOTION_MASK |
						  GDK_POINTER_MOTION_HINT_MASK |
						  GDK_MOTION_NOTIFY);

	/* Mouse */
	g_signal_connect(G_OBJECT(canvas), "motion_notify_event",
					 G_CALLBACK(mgtk_event_mouse_motion), NULL);

	g_signal_connect(G_OBJECT(canvas), "button_press_event",
					 G_CALLBACK(mgtk_event_button_press), NULL);

	g_signal_connect(G_OBJECT(canvas), "button_release_event",
					 G_CALLBACK(mgtk_event_button_release), NULL);

	/* Misc */
	g_signal_connect(G_OBJECT(canvas), "expose_event",
					 G_CALLBACK(mgtk_expose_glarea), NULL);

	g_signal_connect(G_OBJECT(canvas), "configure_event",
					 G_CALLBACK(mgtk_resize_glarea), NULL);

	g_signal_connect(G_OBJECT(canvas), "realize",
					 G_CALLBACK(mgtk_init_glarea), NULL);

	g_signal_connect(G_OBJECT(canvas), "destroy",
					 G_CALLBACK(mgtk_destroy_glarea), NULL);

	/* Setup GL window state */
	mgtk_opengl_canvas_state_t* state = new mgtk_opengl_canvas_state_t;
	state->init = false;
	state->plane = 0;
	state->mouse_x = 0;
	state->mouse_y = 0;
	state->width = width;
	state->height = height;
	state->animate = 0;
	state->timeout_interval = 10;
	state->timeout_id = 0;
	gtk_object_set_data( GTK_OBJECT(canvas), "gl_window_state", state );
#else
#  warning "WARNING No gtkglarea widget support in this build"
#endif

	return canvas;
}


//////////////////////////////////////////////////////////////////
// Legacy util code.
//////////////////////////////////////////////////////////////////

void mgtk_draw_point_size1f(float size)
{
#ifdef HAVE_OPENGL
	glPointSize(size);
#endif
}


void mgtk_draw_color3f(float r, float g, float b)
{
#ifdef HAVE_OPENGL
	glColor3f(r, g, b);
#endif
}


void mgtk_draw_line6f(float x, float y, float z,
					  float x2, float y2, float z2)
{
#ifdef HAVE_OPENGL
	glBegin(GL_LINES);
	glVertex3f(x, y, z);
	glVertex3f(x2, y2, z2);
	glEnd();
#endif
}


void mgtk_draw_point3f(float x, float y, float z)
{
#ifdef HAVE_OPENGL
	glBegin(GL_POINTS);
	glVertex3f(x, y, z);
	glEnd();
#endif
}


//////////////////////////////////////////////////////////////////
// Legacy event code.
//////////////////////////////////////////////////////////////////

void mgtk_event_swap_gl_buffers()
{
	/* Hhhmm...  nothing really needed here, swapping is handled in 
	 * the gtk glarea interfacing code after draw requests */
	// Hhmm... avoid back to back redraws give time to finish
	//SystemIO::MicroSleep(250);
}


void mgtk_event_gl_refresh()
{
	if (mgtk_get_gl_widget())
	{
		mgtk_opengl_canvas_refresh( mgtk_get_gl_widget() );
	}
}


// FIXME: factor out key mapping from GDK and mgtk!
void mgtk_event_button_press(GtkWidget *widget, GdkEventButton *event)
{
	/* Beginning of drag, reset mouse position */
	GdkModifierType state;
	int x, y;
	gdk_window_get_pointer(event->window, &x, &y, &state);

	mgtk_opengl_canvas_state_t *gl_state = 
	(mgtk_opengl_canvas_state_t*)gtk_object_get_data(GTK_OBJECT(widget), "gl_window_state");  
	gl_state->mouse_x = x;
	gl_state->mouse_y = y;

	int btn_state = MOUSE_BTN_STATE_PRESSED;
	int button = 0;
	int mod = 0;

	if (event->state & GDK_CONTROL_MASK)
		mod |= KEY_LCTRL;

	if (event->state & GDK_SHIFT_MASK)
		mod |= KEY_LSHIFT;

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
		button |= MOUSE_BTN_UP;
	}

	if (state & GDK_BUTTON5_MASK)
	{
		button |= MOUSE_BTN_DOWN;
	}

#if DEBUG_OPENGL_CANVAS_MOUSE
	char foo[32];
	sprintf(foo, "MOUSE_BTN_%i", button);
	mgtk_print("Sending mouse event to Freya; x: %i y: %i, %s, %s\n", 
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

	mgtk_handle_mouse(button, btn_state, mod, x, y);

	/* Request expose events instead of forcing. */
	gtk_widget_queue_draw( widget );
	//mgtk_opengl_canvas_refresh(widget);
}


void mgtk_event_mouse_motion(GtkWidget *widget, GdkEventMotion *event)
{
	int x, y;
	GdkModifierType state;

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

	mgtk_opengl_canvas_state_t *gl_state = 
	(mgtk_opengl_canvas_state_t*)gtk_object_get_data(GTK_OBJECT(widget), "gl_window_state");

	gl_state->mouse_x = x;
	gl_state->mouse_y = y;
	mgtk_handle_motion(x, y);

	/* Request expose events instead of forcing. */
	gtk_widget_queue_draw( widget );
	//mgtk_opengl_canvas_refresh(widget);
}



/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libmgtk
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (c) 2000-2007 Mongoose
 * Comments: This is the time slider widget implementation.
 * 
 *-- History ------------------------------------------------- 
 *
 * 2007.12.07:
 * Mongoose - Created, from mgtk prototype.
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
#include "mgtk_timeslider.h"

using namespace mstl;

#define MGTK_TIMESLIDER_BG_FILL                   0

#define MGTK_TIMESLIDER_USE_HANDLE_FOR_SLIDER     0


// Store a list of all mgtk_time_slider_state_t states allocated.
Map<int, mgtk_time_slider_state_t*> gTimeSliderMap;


mgtk_time_slider_state_t* mgtk_time_slider_state_new( int event, unsigned int start, unsigned int end )
{
	/* FIXME: Reuse for now. */
	if ( mgtk_time_slider_get_state_by_event( event ) )
		return mgtk_time_slider_get_state_by_event( event );

	mgtk_time_slider_state_t* state = new mgtk_time_slider_state_t;
	state->event = event;
	state->start = start;
	state->end = end;
	state->inc = 5;
	state->current_key = 0;
	state->marker_count = 0;
	state->current_marker = -1;

	gTimeSliderMap.Add( event, state );

	return state;
}


mgtk_time_slider_state_t* mgtk_time_slider_get_state_by_event( int event )
{
	return gTimeSliderMap[ event ];
}


void mgtk_time_slider_reset_markers( int event )
{
	mgtk_time_slider_state_t* state = mgtk_time_slider_get_state_by_event( event );

	if ( state )
	{
		state->current_key = 0;
		state->marker_count = 0;
		state->current_marker = -1;
	}
}


void mgtk_time_slider_add_marker( int event, unsigned int marker )
{
	mgtk_time_slider_state_t* state = mgtk_time_slider_get_state_by_event( event );

	if ( state && state->marker_count < 500 )
	{
		unsigned int key = marker;
		state->markers[ state->marker_count ] = key;
		state->current_marker = state->marker_count;
		state->marker_count++;

		/* Request expose events. */
		gtk_widget_queue_draw( GTK_WIDGET( state->widget ) );
	}
}


void mgtk_time_slider_set_range( int event, unsigned int start, unsigned int end )
{
	mgtk_time_slider_state_t* state = mgtk_time_slider_get_state_by_event( event );

	if ( state )
	{
		state->start = start;
		state->end = end;

		/* Request expose events. */
		gtk_widget_queue_draw( GTK_WIDGET( state->widget ) );
	}
}


void mgtk_time_slider_set_marker_color( int event, float r, float g, float b )
{
	
}


void mgtk_time_slider_set_active_marker_color( int event, float r, float g, float b )
{
	
}


gboolean mgtk_time_slider_expose_handler(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	const unsigned int width = widget->allocation.width;
	const unsigned int height = widget->allocation.height;

	const unsigned int text_y = height;
	const unsigned int bar_y = 2; // 2px padding
	const unsigned int bar_w = 3; // 3px width
	const unsigned int bar_h = (height >> 1) + (height >> 2) - 2; // 2px padding
	const unsigned int marker_h = (height >> 1) + (height >> 2) - 6; // 6px padding


#if MGTK_TIMESLIDER_BG_FILL
	/* Slider background trough. */
	gtk_paint_slider( widget->style,
					  widget->window,
					  GTK_STATE_ACTIVE,
					  GTK_SHADOW_IN,
                      NULL,
					  widget,
                      NULL, // const gchar *detail,
					  0, 2, //channel_y - (channel_height >> 1) - 4,
					  width, channel_height,
					  GTK_ORIENTATION_HORIZONTAL);
#endif

	/* Time slider's state based widget componets. */
	mgtk_time_slider_state_t* state =
	(mgtk_time_slider_state_t*)gtk_object_get_data(GTK_OBJECT(widget), "mgtk_time_slider_state");

	if ( state )
	{
		const float end = state->end;
		const float inc_v = state->inc;
		const float end_v = 1.0f / ( end - state->start );
		GdkGC* gc = widget->style->fg_gc[GTK_WIDGET_STATE(widget)];

		/* Frame. */
		//gdk_draw_rectangle( widget->window, gc, FALSE, 0, 0, width-1, height-1 );

		/* Tick lines. */
		for (unsigned int i = 0; i < state->end; i+=inc_v)
		{
			int x = (int)(i * end_v * width);
			gdk_draw_rectangle( widget->window, gc, TRUE, x, bar_y, bar_w, bar_h );

			char s[16];
			snprintf( s, 16, "%i", i + state->start );
			gtk_paint_string( widget->style, widget->window,
							  GTK_STATE_ACTIVE,
							  NULL,
							  widget,
							  NULL, //const gchar *detail,
							  x, text_y, 
							  s );
		}

		/* Markers */
		{ 
			/* Work around for gtk_widget_modify bug.  */ 
			GdkGCValues mygcvalues;
			gdk_gc_get_values( gc, &mygcvalues );
			GdkColor old_color = mygcvalues.foreground;
			GdkColormap* mycolormap = gtk_widget_get_colormap( widget );
			GdkColor mycolor = { 0, 65535, 65535, 0 };
			gdk_colormap_alloc_color( mycolormap, &mycolor, TRUE, TRUE );

			for ( unsigned int i = 0; i < state->marker_count; ++i)
			{
				const unsigned int key = state->markers[i];

				if ( key < state->start || key > state->end )
					continue;

				const unsigned int x = (key - state->start) * end_v * width;

				gdk_gc_set_foreground( gc, &mycolor );
				gdk_draw_rectangle( widget->window, gc, TRUE, x, bar_y, bar_w, marker_h );
				gdk_gc_set_foreground( gc, &old_color );	
				gdk_draw_rectangle( widget->window, gc, FALSE, x, bar_y, bar_w-1, marker_h-1 );
			}
		}

		/* Currently selected marker. */
		if ( state->current_marker > -1 )
		{
			const unsigned int key = state->markers[ state->current_marker ];

			if ( key >= state->start && key <= state->end )
			{
				const unsigned int x = (key - state->start) * end_v * width;

				/* Work around for gtk_widget_modify bug.  */ 
				GdkGCValues mygcvalues;
				gdk_gc_get_values( gc, &mygcvalues );
				GdkColor old_color = mygcvalues.foreground;
				GdkColormap* mycolormap = gtk_widget_get_colormap( widget );
				GdkColor mycolor = { 0, 65535, 0, 65535 };
				gdk_colormap_alloc_color( mycolormap, &mycolor, TRUE, TRUE );

				gdk_gc_set_foreground( gc, &mycolor );
				gdk_draw_rectangle( widget->window, gc, TRUE, x, bar_y, bar_w, marker_h );
				gdk_gc_set_foreground( gc, &old_color );	
				gdk_draw_rectangle( widget->window, gc, FALSE, x, bar_y, bar_w-1, marker_h-1 );
			}
		}


		/* Render a slider with a handle grip at current_key. */
		{
			int x = ( state->current_key - state->start ) * end_v * width;
#if MGTK_TIMESLIDER_USE_HANDLE_FOR_SLIDER
			gtk_paint_box( widget->style,
						   widget->window,
						   GTK_STATE_ACTIVE,
						   GTK_SHADOW_OUT,
						   NULL, GTK_WIDGET(widget), 
						   "buttondefault",
						   x, channel_y - (channel_height >> 1) - 6, 10, 26 );
			
			gtk_paint_handle( widget->style,
							  widget->window,
							  GTK_STATE_ACTIVE,
							  GTK_SHADOW_OUT,
							  NULL,
							  widget,
							  "stepper",
							  x, channel_y - (channel_height >> 1) - 6,
							  10, 26,
							  GTK_ORIENTATION_HORIZONTAL );
#else
			/* Work around for gtk_widget_modify bug using gdk API.  */ 
			GdkGCValues mygcvalues;
			gdk_gc_get_values( gc, &mygcvalues );
			GdkColor old_color = mygcvalues.foreground;
			GdkColormap* mycolormap = gtk_widget_get_colormap( widget );
			GdkColor mycolor = { 0, 0, 65535, 0 };
			gdk_colormap_alloc_color( mycolormap, &mycolor, TRUE, TRUE );

			gdk_gc_set_foreground( gc, &mycolor );
			gdk_draw_rectangle( widget->window, gc, TRUE, x, bar_y, bar_w, marker_h );
			gdk_gc_set_foreground( gc, &old_color );	
			gdk_draw_rectangle( widget->window, gc, FALSE, x, bar_y, bar_w-1, marker_h-1 );
#endif
		}

		/* Store widget width for event system use. */
		state->width = width;
		state->widget = widget;
	}

	return TRUE;
}


void mgtk_time_slider_motion_handler(GtkWidget* widget, GdkEventMotion* event)
{
	int x, y;
	GdkModifierType state;

	if ( event->is_hint )
	{
		gdk_window_get_pointer( event->window, &x, &y, &state );
	} 
	else 
	{
		x = (int)event->x;
		y = (int)event->y;
		state = (GdkModifierType)event->state;
	}

	mgtk_time_slider_state_t* slider_state =
	(mgtk_time_slider_state_t*)gtk_object_get_data(GTK_OBJECT(widget), "mgtk_time_slider_state");

	if ( slider_state )
	{
		const float width = widget->allocation.width;
		if ( x < 0 ) x = 0;
		if ( x > width ) x = width;

		unsigned int key = slider_state->start + (x / width) * ( slider_state->end - slider_state->start );
		unsigned int max = slider_state->end - 1; // 'Trim' 1 key to keep slider on screen.
		if ( key > max ) key = max;
		if ( key < slider_state->start ) key = slider_state->start;

		if ( state & GDK_BUTTON1_MASK )
		{
			slider_state->current_key = key;
			//mgtk_print("%i %i -> %i", x, y, key);
			mgtk_handle_event1u(GPOINTER_TO_INT( slider_state->event ), key );
		}
		else if ( state & GDK_BUTTON3_MASK )
		{
			/* Drag event */
			if ( slider_state->current_marker > -1 )
			{
				// mgtk_marker_drag_event( slider_state->event, slider_state->marker[ slider_state->current_marker ], key );
				slider_state->markers[ slider_state->current_marker ] = key;
			}
		}

		/* Request expose events. */
		gtk_widget_queue_draw( widget );
	}
}


void mgtk_time_slider_button_press_handler(GtkWidget* widget, GdkEventButton* event)
{
	/* Beginning of drag, reset mouse position */
	GdkModifierType state;
	int x, y;
	gdk_window_get_pointer(event->window, &x, &y, &state);

	mgtk_time_slider_state_t* slider_state =
	(mgtk_time_slider_state_t*)gtk_object_get_data(GTK_OBJECT(widget), "mgtk_time_slider_state");

	if ( slider_state )
	{
		const float width = widget->allocation.width;
		unsigned int key = slider_state->start + (x / width) * ( slider_state->end - slider_state->start );
		unsigned int max = slider_state->end;
		if (key > max) key = max;
		if (key < slider_state->start) key = slider_state->start;

		if ( state & GDK_BUTTON1_MASK )
		{
			/* Create new marker. */
			if ( event->state & GDK_CONTROL_MASK )
			{
				slider_state->markers[slider_state->marker_count] = key;
				slider_state->marker_count++;
				// mgtk_marker_create_event( slider_state->event, key );
			}
			/* Select existing marker. */ 
			else if ( event->state & GDK_SHIFT_MASK )
			{
				for ( unsigned int i = 0; i < slider_state->marker_count; ++i)
				{
					if ( key == slider_state->markers[i] )
					{
						slider_state->current_marker = i;
						// mgtk_marker_selection_event( slider_state->event, key );
						break;
					}
				}
			}
		}
		else if ( state & GDK_BUTTON3_MASK )
		{
			
		}

		/* Request expose events. */
		gtk_widget_queue_draw( widget );
	}
}


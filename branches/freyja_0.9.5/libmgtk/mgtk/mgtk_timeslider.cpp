/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libmgtk
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (c) 2000-2006 Mongoose
 * Comments: 
 * 
 *-- History ------------------------------------------------- 
 *
 * 2006.10.30:
 * Mongoose - Clean up of old mgtk ( Resource, gtk+ Type ) tree / list code
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


// Store a list of all mgtk_time_slider_state_t states allocated.
Map<int, mgtk_time_slider_state_t*> gTimeSliderMap;


mgtk_time_slider_state_t* mgtk_time_slider_state_new( int event, unsigned int start, unsigned int end )
{
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
	}
}


void mgtk_time_slider_set_range( int event, unsigned int start, unsigned int end )
{
	mgtk_time_slider_state_t* state = mgtk_time_slider_get_state_by_event( event );

	if ( state )
	{
		state->start = start;
		state->end = end;
	}
}


gboolean mgtk_time_slider_expose_handler(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	GdkGC* gc = widget->style->fg_gc[GTK_WIDGET_STATE(widget)];
	const unsigned int width = widget->allocation.width;
	const unsigned int height = widget->allocation.height;
	const unsigned int channel_height = 18;
	const unsigned int channel_y = height >> 1;

	/* Slider background trough. */
	gtk_paint_slider( widget->style,
					  widget->window,
					  GTK_STATE_ACTIVE,
					  GTK_SHADOW_IN,
                      NULL,
					  widget,
                      NULL, // const gchar *detail,
					  0, channel_y - (channel_height >> 1) - 4,
					  width, channel_height,
					  GTK_ORIENTATION_HORIZONTAL);


	/* Time slider's state based widget componets. */
	mgtk_time_slider_state_t* state =
	(mgtk_time_slider_state_t*)gtk_object_get_data(GTK_OBJECT(widget), "mgtk_time_slider_state");

	if ( state )
	{
		state->width = width;

		/* Tick lines. */
		const float end = state->end;
		const float inc_v = state->inc;
		const float end_v = 1.0f / ( end - state->start );

		for (unsigned int i = 0; i < state->end; i+=inc_v)
		{
			float x = (float)i * end_v;
			x *= width;
			gdk_draw_line( widget->window, gc, (int)x, channel_y, (int)x, channel_y - (channel_height >> 1) );
			
			char s[16];
			snprintf( s, 16, "%i", i + state->start );
			gtk_paint_string( widget->style,
							  widget->window,
							  GTK_STATE_ACTIVE,
							  NULL,
							  widget,
							  NULL, //const gchar *detail,
							  (int)x,
							  channel_y<<1,
							  s);
		}


		/* Markers */
		{
			GdkColor color;
			color.red = 65535;
			color.blue = 0;
			color.green = 65535;
			gtk_widget_modify_bg( widget, GTK_STATE_INSENSITIVE, &color );

			for ( unsigned int i = 0; i < state->marker_count; ++i)
			{
				const unsigned int key = state->markers[i];

				if ( key < state->start || key > state->end )
					continue;

				const unsigned int x = (key - state->start) * end_v * width;

#if 0
				gdk_draw_line( widget->window, gc, 
							   x, channel_y + (channel_height >> 1), x, channel_y - (channel_height >> 1) );
#else
				gtk_paint_box( widget->style,
							   widget->window,
							   GTK_STATE_INSENSITIVE,
							   GTK_SHADOW_OUT,
							   NULL, GTK_WIDGET(widget), 
							   NULL,
							   x, channel_y - (channel_height >> 1) - 4, 3, 24 );
#endif
			}

			gtk_widget_modify_bg( widget, GTK_STATE_INSENSITIVE, NULL );
		}

		/* Currently selected marker. */
		if ( state->current_marker > -1 )
		{
			const unsigned int key = state->markers[ state->current_marker ];

			if ( key >= state->start && key <= state->end )
			{
				const unsigned int x = (key - state->start) * end_v * width;
				//gdk_draw_line( widget->window, gc, 
				//			   x, channel_y + (channel_height >> 1) + 4, x, channel_y - (channel_height >> 1) - 4 );
				
				// Green for selected marker.
				GdkColor color;
				color.red = 0;
				color.blue = 0;
				color.green = 65535;
				gtk_widget_modify_bg( widget, GTK_STATE_INSENSITIVE, &color );

#if 0
				gtk_paint_vline( widget->style,
								 widget->window,
								 GTK_STATE_INSENSITIVE,
								 NULL, // area
								 widget,
								 NULL, // detail
								 channel_y + (channel_height >> 1) + 4,
								 channel_y - (channel_height >> 1) - 4,
								 x );
#else
				gtk_paint_box( widget->style,
							   widget->window,
							   GTK_STATE_INSENSITIVE,
							   GTK_SHADOW_OUT,
							   NULL, GTK_WIDGET(widget), 
							   NULL,
							   x, channel_y - (channel_height >> 1) - 4, 3, 24 );		
#endif
				gtk_widget_modify_bg( widget, GTK_STATE_INSENSITIVE, NULL );
			}
		}

		/* Render a slider with a handle grip at current_key. */
		int x = ( state->current_key - state->start ) * end_v * width;
		gtk_paint_box( widget->style,
					   widget->window,
					   true ? GTK_STATE_ACTIVE : GTK_STATE_INSENSITIVE,
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

		//mgtk_print("%i", key);
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
		unsigned int key = slider_state->start + (x / width) * ( slider_state->end - slider_state->start );
		unsigned int max = slider_state->end;// - 5;// always 'trim' 5 to keep slider on screen.
		if (key > max) key = max;
		if (key < slider_state->start) key = slider_state->start;

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
		unsigned int max = slider_state->end;// - 5; // always 'trim' 5 to keep slider on screen.
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


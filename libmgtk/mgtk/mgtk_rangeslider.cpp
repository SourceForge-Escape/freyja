/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libmgtk
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (c) 2000-2007 Mongoose
 * Comments: This is the range slider widget implementation.
 * 
 *-- History ------------------------------------------------- 
 *
 * 2007.12.07:
 * Mongoose - Created, from mgtk prototype.
 ==========================================================================*/

#include <string.h>

#include <gtk/gtk.h>

#include "mgtk_interface.h"
#include "mgtk_callbacks.h"
#include "mgtk_resource.h"
#include "mgtk_events.h"
#include "mgtk_timeslider.h"
#include "mgtk_rangeslider.h"


mgtk_range_slider_state_t* mgtk_range_slider_state_new( int event, 
														unsigned int min, unsigned int max, 
														unsigned int start, unsigned int end )
{
	mgtk_range_slider_state_t* state = new mgtk_range_slider_state_t;
	state->event = event;
	state->min = min;
	state->max = max;
	state->start = start;
	state->end = end;

	return state;
}





gboolean mgtk_range_slider_expose_handler(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	GdkGC* gc = widget->style->fg_gc[GTK_WIDGET_STATE(widget)];
	const unsigned int width = widget->allocation.width;
	const unsigned int height = widget->allocation.height;

	/* Slider background trough. */
	gtk_paint_slider( widget->style,
					  widget->window,
					  GTK_STATE_ACTIVE,
					  GTK_SHADOW_IN,
                      NULL,
					  widget,
                      NULL, // const gchar *detail,
					  0, (height>>2)+(height>>3),
					  width, height>>2,
					  GTK_ORIENTATION_HORIZONTAL);


	/* Range slider's state based widget componets. */
	mgtk_range_slider_state_t* state =
	(mgtk_range_slider_state_t*)gtk_object_get_data(GTK_OBJECT(widget), "mgtk_range_slider_state");

	if ( state )
	{
		//state->width = width;
		const float end_v = 1.0f / ( state->max - state->min );

		/* Render a slider with a handle grip at current_key. */
		int x = ( state->start - state->min ) * end_v * width;
		int x2 = ( state->end - state->min ) * end_v * width;
		gtk_paint_box( widget->style,
					   widget->window,
					   true ? GTK_STATE_ACTIVE : GTK_STATE_INSENSITIVE,
					   GTK_SHADOW_OUT,
					   NULL, GTK_WIDGET(widget), 
					   "buttondefault",
					   x, 2, x2 - x, height - 4 );

		/* Ranged slider handles. */
		gtk_paint_handle( widget->style,
						  widget->window,
						  GTK_STATE_ACTIVE,
						  GTK_SHADOW_OUT,
						  NULL,
						  widget,
                          "stepper",
						  x+1, 4,
						  8, height - 8,
						  GTK_ORIENTATION_HORIZONTAL );

		gtk_paint_handle( widget->style,
						  widget->window,
						  GTK_STATE_ACTIVE,
						  GTK_SHADOW_OUT,
						  NULL,
						  widget,
                          "stepper",
						  x2-9, 4,
						  8, height - 8,
						  GTK_ORIENTATION_HORIZONTAL );

		/* Ranged slider start/end numeric strings. */
		char s[16];
		snprintf( s, 16, "%i", state->start );
		gtk_paint_string( widget->style,
						  widget->window,
						  GTK_STATE_ACTIVE,
						  NULL,
						  widget,
						  NULL, //const gchar *detail,
						  x+10, (height>>1)+(height>>2),
						  s);

		snprintf( s, 16, "%i", state->end );
		gtk_paint_string( widget->style,
						  widget->window,
						  GTK_STATE_ACTIVE,
						  NULL,
						  widget,
						  NULL, //const gchar *detail,
						  x2-26, (height>>1)+(height>>2),
						  s);
	}

	return TRUE;
}


void mgtk_range_slider_motion_handler(GtkWidget* widget, GdkEventMotion* event)
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

	mgtk_range_slider_state_t* slider_state =
	(mgtk_range_slider_state_t*)gtk_object_get_data(GTK_OBJECT(widget), "mgtk_range_slider_state");

	if ( slider_state )
	{
		const float width = widget->allocation.width;
		const unsigned int range = slider_state->end - slider_state->start;
		if ( x < 0 ) x = 0;
		if ( x > width ) x = width;

		unsigned int key = slider_state->min + (x / width) * ( slider_state->max - slider_state->min );
		if ( (key + range) > slider_state->max ) key = slider_state->max - range;
		if ( key < slider_state->min ) key = slider_state->min;

		if ( state & GDK_BUTTON1_MASK )
		{
			/* Drag range event. */
			slider_state->end = key + range;  
			slider_state->start = key;
			mgtk_time_slider_set_range( slider_state->event, slider_state->start, slider_state->end );
		}
		else if ( state & GDK_BUTTON3_MASK )
		{
			/* Drag start/end event. */
			if ( 0 )
			{
				// mgtk_marker_drag_event( slider_state->event, slider_state->marker[ slider_state->current_marker ], key );
				//slider_state->markers[ slider_state->current_marker ] = key;
			}
		}

		/* Request expose events. */
		gtk_widget_queue_draw( widget );
	}
}

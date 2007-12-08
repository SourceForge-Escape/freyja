/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libmgtk
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (c) 2000-2004 Mongoose
 * Comments: This is the generic mlisp bridge to the mgtk widget system.
 *
 *           It provides a 2-way event system w/o requiring direct bindings.
 *           ( eg You can make this a dlopen'able interface facade. )
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2004.10.27:
 * Mongoose - Major API clean up, documentation update
 *
 *            Created, from old mlisp and mgtk source headers
 ==========================================================================*/

#ifndef GUARD__MGTK_TIMESLIDER_H_
#define GUARD__MGTK_TIMESLIDER_H_

typedef struct {

	int event;

	// Range is always positive.
	unsigned int start;
	unsigned int end;
	unsigned int inc;

	unsigned int width;

	int current_key;
	int current_marker;

	// FIXME: Use map/list here when/if it's decided if this will be data driven.
	unsigned int marker_count;
	unsigned int markers[512]; 

} mgtk_time_slider_state_t;


mgtk_time_slider_state_t* mgtk_time_slider_state_new( int event, unsigned int start, unsigned int end );
/*------------------------------------------------------
 * Pre  : 
 * Post : Allocates and initializes a new time slider state.
 *
 ------------------------------------------------------*/

mgtk_time_slider_state_t* mgtk_time_slider_get_state_by_event( int event );
/*------------------------------------------------------
 * Pre  : 
 * Post : Returns state for given time slider event.
 *
 ------------------------------------------------------*/

void mgtk_time_slider_reset_markers( int event );
/*------------------------------------------------------
 * Pre  : 
 * Post : All markers for sliders using event are reset.
 *
 ------------------------------------------------------*/

void mgtk_time_slider_add_marker( int event, unsigned int marker );
/*------------------------------------------------------
 * Pre  : 
 * Post : All markers for sliders using event are reset.
 *
 ------------------------------------------------------*/

void mgtk_time_slider_set_range( int event, unsigned int start, unsigned int end );
/*------------------------------------------------------
 * Pre  : 
 * Post : All markers for sliders using event are reset.
 *
 ------------------------------------------------------*/


#   if EXPOSE_TIMESLIDER_GTK

gboolean mgtk_time_slider_expose_handler(GtkWidget *widget, GdkEventExpose *event, gpointer data);
/*------------------------------------------------------
 * Pre  : 
 * Post : Handles time slider redraw event.
 *
 ------------------------------------------------------*/

void mgtk_time_slider_motion_handler(GtkWidget* widget, GdkEventMotion* event);
/*------------------------------------------------------
 * Pre  : 
 * Post : Handles time slider mouse motion event.
 *
 ------------------------------------------------------*/

void mgtk_time_slider_button_press_handler(GtkWidget* widget, GdkEventButton* event);
/*------------------------------------------------------
 * Pre  : 
 * Post : Handles time slider mouse button press event.
 *
 ------------------------------------------------------*/

#   endif

#endif // GUARD__MGTK_TIMESLIDER_H_


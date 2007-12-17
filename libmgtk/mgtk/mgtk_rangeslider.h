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

#ifndef GUARD__MGTK_RANGESLIDER_H_
#define GUARD__MGTK_RANGESLIDER_H_

typedef struct {

	int event;

	unsigned int min;
	unsigned int max;

	unsigned int start;
	unsigned int end;

} mgtk_range_slider_state_t;


mgtk_range_slider_state_t* 
mgtk_range_slider_state_new( int event, 
							 unsigned int min, unsigned int max,
							 unsigned int start, unsigned int end );
/*------------------------------------------------------
 * Pre  : 
 * Post : Allocates and initializes a new range slider state.
 *
 ------------------------------------------------------*/


gboolean mgtk_range_slider_expose_handler(GtkWidget *widget, GdkEventExpose *event, gpointer data);
/*------------------------------------------------------
 * Pre  : 
 * Post : Handles time slider redraw event.
 *
 ------------------------------------------------------*/

void mgtk_range_slider_motion_handler(GtkWidget* widget, GdkEventMotion* event);
/*------------------------------------------------------
 * Pre  : 
 * Post : Handles time slider mouse motion event.
 *
 ------------------------------------------------------*/


#endif // GUARD__MGTK_RANGESLIDER_H_


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

#ifndef GUARD__MGTK_OPENGL_CANVAS_H_
#define GUARD__MGTK_OPENGL_CANVAS_H_

#include <gtk/gtk.h>

typedef struct 
{
	bool init;
	int plane;
	int mouse_x;
	int mouse_y;
	int width;
	int height;
	GtkWidget* appbar;

	int animate;
	int timeout_interval;
	guint timeout_id;

} mgtk_opengl_canvas_state_t;


GtkWidget* mgtk_opengl_canvas_new( unsigned int width, unsigned int height );
/*------------------------------------------------------
 * Pre : Parmeters width and height are greater than zero.
 * Post: Returns newly created canvas widget.
 *
 ------------------------------------------------------*/



#endif // GUARD__MGTK_OPENGL_CANVAS_H_

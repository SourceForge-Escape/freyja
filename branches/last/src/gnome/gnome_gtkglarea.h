/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : 
 * License : GPL, also (C) 2001, 2002 Mongoose
 * Comments: This is the gtk_glarea wrapper
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2001.??.??:
 * Mongoose - Created
 =================================================================*/

#ifndef __FREYJA_MONGOOSE_GTKGLAREA_H_
#define __FREYJA_MONGOOSE_GTKGLAREA_H_

typedef struct 
{
	bool init;
	int plane;
	int mouse_x;
	int mouse_y;
	int width;
	int height;
	GtkObject *v_adj;
	GtkObject *h_adj;
	GtkWidget *appbar;

} glarea_window_state_t;

GtkWidget *glarea_create(unsigned int width, unsigned int height);
gint glarea_motion_notify(GtkWidget *widget, GdkEventMotion *event);
gint glarea_key_release_event(GtkWidget *widget, GdkEventKey *event);
gint glarea_key_press_event(GtkWidget *widget, GdkEventKey *event);

#endif

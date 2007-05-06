/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libmgtk
 * Author  : Mongoose
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (c) 2000-2004 Mongoose
 * Comments: This is the mlisp bridge to gtk+ events 
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
 * 2004.04.11:
 * Mongoose - Removed GNOME support, lots of little clean ups
 *
 * 2002.01.25:
 * Mongoose - Spin spin sugar... replaced a hundred spin functions
 *            with generic calls
 *
 * 2002.01.12:
 * Mongoose - Toolkit functions for mapping widgets and events
 *            to use for external get/set actions
 *
 * 2000.09.16:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD_MONGOOSE__GNOME_CALLBACKS_H_
#define GUARD_MONGOOSE__GNOME_CALLBACKS_H_

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

void mgtk_event_button_release(GtkWidget *widget, GdkEventButton *event);

void mgtk_event_button_press(GtkWidget *widget, GdkEventButton *event);

void mgtk_event_color(GtkWidget *colorbutton, gpointer id);

void mgtk_event_command(GtkWidget *widget, gpointer user_data);



void mgtk_event_key_release(GtkWidget *widget, GdkEventKey *event);

void mgtk_event_key_press(GtkWidget *widget, GdkEventKey *event);

void mgtk_event_mouse_motion(GtkWidget *widget, GdkEventMotion *event);

void mgtk_event_notebook_switch_page(GtkNotebook *notebook, 
									 GtkNotebookPage *page,
									 gint page_num, gpointer user_data);

void mgtk_event_popup_menu_handler(GtkWidget *widget, GdkEventButton *event);

void mgtk_event_popup_menu_detacher(GtkWidget *attach_widget,GtkMenu *menu);

void mgtk_event_slider1u(GtkWidget *widget, gpointer event_id);

void mgtk_event_slider1f(GtkWidget *widget, gpointer event_id);

void mgtk_event_spinbutton_int(GtkSpinButton *spin, gpointer event_id);

void mgtk_event_spinbutton_uint(GtkSpinButton *spin, gpointer event_id);

void mgtk_event_spinbutton_float(GtkSpinButton *spin, gpointer event_id);

void mgtk_event_subscribe_gtk_widget(int id, GtkWidget *widget);
/*------------------------------------------------------
 * Pre  : All event ids must be unique, but the same id
 *        may be reused for another widget
 *
 * Post : <widget> will be mapped to event <id> to
 *        recieve signals from application to sync with
 *        internal state
 *
 *-- History ------------------------------------------
 *
 * 2002.01.12:
 * Mongoose - Created
 ------------------------------------------------------*/

void mgtk_event_text(GtkWidget *widget, gpointer user_data);

void mgtk_event_dual_command(GtkWidget *widget, gpointer user_data);

#endif

/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : 
 * License : GPL See file COPYING, also (C) 2000, 2001 Mongoose
 * Comments: This is the main client entry.
 *
 *-- TODO ---------------------------------------------------
 *
 * Merge this into unified Freyja class then
 * move gtk dependent code out into FreyjaGnome
 *
 *-- History ------------------------------------------------- 
 *
 * 2000-09-16:
 * Mongoose - Created
 =================================================================*/

#ifndef GUARD_MONGOOSE__GNOME_CALLBACKS_H_
#define GUARD_MONGOOSE__GNOME_CALLBACKS_H_

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>
#include "freyja_events.h"

void freyja_event_subscribe_gtk_widget(int id, GtkWidget *widget);
void freyja_event_file_dialog_pattern(char *pattern);

void mgtk_event_mouse_motion(GtkWidget *widget, GdkEventMotion *event);
void mgtk_event_button_release(GtkWidget *widget, GdkEventButton *event);
void mgtk_event_button_press(GtkWidget *widget, GdkEventButton *event);
void mgtk_event_key_release(GtkWidget *widget, GdkEventKey *event);
void mgtk_event_key_press(GtkWidget *widget, GdkEventKey *event);

// Mongoose 2002.01.12, Freyja event from Gnome widget callbacks
void main_event(GtkWidget *widget, gpointer user_data);
void mode_event(GtkWidget *widget, gpointer user_data);
void misc_event(GtkWidget *widget, gpointer user_data);
void tag_event(GtkWidget *widget, gpointer user_data);
void mesh_event(GtkWidget *widget, gpointer user_data);
void point_event(GtkWidget *widget, gpointer user_data);
void polygon_event(GtkWidget *widget, gpointer user_data);
void animation_event(GtkWidget *widget, gpointer user_data);
void material_event(GtkWidget *widget, gpointer user_data);
void rc_textbox_event(GtkWidget *widget, gpointer user_data);


void event_send_color(GtkWidget *colorbutton, gpointer id);

// Mongoose 2002.01.25, Spin spin sugar... replace a hundred spin functions
gint spinbutton_int_event(GtkSpinButton *spin, gpointer event_id);
gint spinbutton_uint_event(GtkSpinButton *spin, gpointer event_id);
gint spinbutton_float_event(GtkSpinButton *spin, gpointer event_id);

gint aframe_adj_event(GtkWidget *widget, GdkEventButton *event);

void fileselection_action_event();
void fileselection_cancel_event();
void notebook_switch_page_event(GtkNotebook *notebook, GtkNotebookPage *page,
										  gint page_num, gpointer user_data);
void animation_tree_item_selected_event(GtkWidget *item);

gint popup_menu_handler_event(GtkWidget *widget, GdkEventButton *event);
void popup_menu_detacher_event(GtkWidget *attach_widget,GtkMenu *menu);


// Mongoose 2002.01.12, Misc/toolkit functions
GtkWidget *setup_tree_root(char *label, GtkWidget *tree);
void fileselection_dir_set_event(char *dir);
void index_add_gtk_widget(int event, GtkWidget *widget);
void index_add_notebook_event(int page, int event);

void set_main_window_title(char *title);

#endif

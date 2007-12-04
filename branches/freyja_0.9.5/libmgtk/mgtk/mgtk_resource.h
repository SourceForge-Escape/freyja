/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : 
 * License : GPL See file COPYING, also (C) 2000, 2001 Mongoose
 * Comments: 
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


#ifndef GUARD__FREYJA_MONGOOSE_GNOME_RESOURCE_H_
#define GUARD__FREYJA_MONGOOSE_GNOME_RESOURCE_H_

#include <mstl/Vector.h>

using namespace mstl;


typedef enum {
   NAME_COLUMN = 0,
   ID_COLUMN,
   N_COLUMNS
} mgtk_temp_tree_columns_t;


typedef struct {
	unsigned int count;
	int *events;
} mgtk_notebook_eventmap_t;


/* Access gobal gtk widgets for this application */
GtkWidget *mgtk_get_application_window();
GtkWidget *mgtk_get_gl_widget();
GtkWidget *mgtk_get_statusbar_widget();
GtkWidget *mgtk_get_fileselection_widget(int event);
GtkWidget *mgtk_get_fileselection_pattern_widget(int event);

void mgtk_resource_rebuild_treeview(int event, GtkTreeModel *model);

void mgtk_tool_toggle_button_handler(GtkWidget *item, gpointer e);

void mgtk_destroy_notebook(GtkWidget *widget);

void mgtk_check_menu_item_handler(GtkCheckMenuItem *item, gpointer e);

void mgtk_event_hide_dialog(GtkWidget *widget, gpointer user_data);

void mgtk_tool_toggle_button_dual_handler(GtkWidget *item, gpointer e);

#endif

/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libmgtk
 * Author  : Mongoose
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

#define ACCEL_SUPPORT_ON
#define EXTEND_FILE_TEST

// Arg list types for Resource use
#define ARG_GTK_WIDGET           64
#define ARG_GTK_BOX_WIDGET       128
#define ARG_GTK_WINDOW           256
#define ARG_GTK_MENU_WIDGET      1024
#define ARG_GTK_NOTEBOOK         2048
#define ARG_GTK_TOOLBOX_WIDGET   4096


using namespace mstl;


// Multiple trees widgets possibly sharing the same event id
//Map<int, Vector<GtkTreeView *> *> gTreeWidgetMap;

// Multiple tree widgets with unique event ids 
Map<int, GtkTreeView *> gTreeWidgetMap;


GtkTreeView *mgtk_tree_get_treeview(int event)
{
	return gTreeWidgetMap[event];
}


void mgtk_tree_set_treeview(int event, GtkTreeView *tree)
{
	gTreeWidgetMap.Add(event, tree);
}


void mgtk_tree_cell_edited_callback (GtkCellRendererText *cell,
									 gchar               *path_string,
									 gchar               *new_text,
									 gpointer             userdata)
{
	mgtk_print("!Event %i: %s\n", GPOINTER_TO_INT(userdata), new_text);
	mgtk_handle_text(GPOINTER_TO_INT(userdata), new_text);
}



gboolean mgtk_GtkTreeSelectionFunc(GtkTreeSelection *selection,
								   GtkTreeModel *model,
								   GtkTreePath *path,
								   gboolean path_currently_selected,
								   gpointer data)
{
	mgtk_print("!$$$$$$$$$$$$$\n tree selected\n$$$$$$$$$$$$$$$$\n");

	return TRUE;
}


void mgtk_resource_rebuild_treeview(int event, GtkTreeModel *model)
{
	GtkTreeView *view = mgtk_tree_get_treeview(event);

	if (view && model)
	{
		gtk_tree_view_set_model(view, model);
	}
}


void  mgtk_treeview_onRowActivated (GtkTreeView        *treeview,
						   GtkTreePath        *path,
						   GtkTreeViewColumn  *col,
						   gpointer            userdata)
{
	GtkTreeModel *model;
	GtkTreeIter   iter;

	MSTL_MSG("A row has been double-clicked!\n");
	
    model = gtk_tree_view_get_model(treeview);

    if (gtk_tree_model_get_iter(model, &iter, path))
    {
		gchar *name;
		gint id;

		gtk_tree_model_get(model, &iter, 
						   NAME_COLUMN, &name, 
						   ID_COLUMN, &id,
						   -1);

		mgtk_print("!Event %i: Selected row { %s, %i }\n", GPOINTER_TO_INT(userdata), name, id);

		mgtk_handle_event1u(GPOINTER_TO_INT(userdata), id);

		g_free(name);
	}
}


/* Mongoose 2002.01.24,
 * Tree widget should be factored out for general widget use */
arg_list_t *mgtk_rc_animation_tab_hack(arg_list_t *container)
{
	arg_enforce_type(&container, ARG_GTK_BOX_WIDGET);
	MSTL_ASSERTMSG(container, "container == ARG_GTK_BOX_WIDGET");

	if (!container)
	{
		return NULL;
	}

	// Read in an event id
	arg_list_t *event = symbol();
	arg_enforce_type(&event, INT);
	MSTL_ASSERTMSG(event, "event == INT");

	arg_list_t *event2 = symbol();
	arg_enforce_type(&event2, INT);
	MSTL_ASSERTMSG(event2, "event2 == INT");

	if (!event || !event2)
	{
		return NULL;
	}	


    /* Tree: New for Gtk+ 2.0 working Animation Tree */
	arg_list_t *ret = NULL;
	GtkWidget *box = (GtkWidget *)container->data;
	GtkTreeViewColumn *col;
	GtkCellRenderer *renderer;
	GtkWidget *view;
	GtkTreeModel *model;
	GtkTreeStore *store;

	view = gtk_tree_view_new();	
	mgtk_tree_set_treeview(get_int(event), GTK_TREE_VIEW(view));

	g_signal_connect(view, "row-activated", (GCallback)mgtk_treeview_onRowActivated, GINT_TO_POINTER(get_int(event)));

	GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
	if (selection)
	{
		gtk_tree_selection_set_mode(selection, GTK_SELECTION_BROWSE);
		gtk_tree_selection_set_select_function(selection, mgtk_GtkTreeSelectionFunc, GINT_TO_POINTER(get_int(event)), NULL);
	}

	/* Column "Name" */
	col = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(col, "Name");
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_add_attribute(col, renderer, "text", NAME_COLUMN);
	g_object_set(renderer, "editable", TRUE, NULL);
	g_signal_connect(renderer, "edited", (GCallback)mgtk_tree_cell_edited_callback, GINT_TO_POINTER(get_int(event2)));

	/* Column "Id" */
	col = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(col, "Id");
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_add_attribute(col, renderer, "text", ID_COLUMN);

	/* Append dummy bone(s) */
	store = gtk_tree_store_new(N_COLUMNS, /* Total number of cols */
							   G_TYPE_STRING,   /* Bone Name */
                               G_TYPE_INT);     /* Bone Id */

#if 0 // The new tree widgets don't need dummy stores
	GtkTreeIter root, child;
	gtk_tree_store_append(store, &root, NULL);
	gtk_tree_store_set(store, &root,
                   	   NAME_COLUMN, "root",
                   	   ID_COLUMN, 0,
		               -1);

	gtk_tree_store_append(store, &child, &root);
	gtk_tree_store_set(store, &child,
                   	   NAME_COLUMN, "dummy",
                   	   ID_COLUMN, 1,
		               -1);
#endif

	model = GTK_TREE_MODEL(store);

	/* Add view to container */
	gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
	g_object_unref(model); 
	gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(view)),
                                GTK_SELECTION_NONE);
	gtk_widget_show(view);

#ifdef ANIM_TREE_IN_BOX
	gtk_box_pack_start(GTK_BOX(box), view, FALSE, FALSE, FALSE);
#else
	/* Animation tree inside scrolled window, instead of box */
	GtkWidget* scroll = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
											 GTK_POLICY_AUTOMATIC,
											 GTK_POLICY_AUTOMATIC);
	gtk_widget_show(scroll);
	gtk_widget_set_usize(scroll, 100, 200);
	gtk_box_pack_start(GTK_BOX(box), scroll, FALSE, FALSE, FALSE);
	gtk_container_add(GTK_CONTAINER(scroll), view);
#endif

	delete_arg(&event);
	delete_arg(&event2);

	return ret;
}


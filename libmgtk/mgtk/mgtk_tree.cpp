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


void mgtk_treeview_onRowActivated(GtkTreeView *treeview,
								  GtkTreePath        *path,
								  GtkTreeViewColumn  *col,
								  gpointer            userdata)
{
	GtkTreeModel *model;
	GtkTreeIter   iter;

	//MSTL_MSG("A row has been double-clicked!\n");
	
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
	MGTK_ASSERTMSG(container, "container == ARG_GTK_BOX_WIDGET");

	if (!container)
	{
		return NULL;
	}

	// Read in an event id
	arg_list_t *event = symbol();
	arg_enforce_type_assert(&event, INT);
	//MGTK_ASSERTMSG(event, "event == INT");

	arg_list_t *event2 = symbol();
	arg_enforce_type_assert(&event2, INT);
	//MGTK_ASSERTMSG(event2, "event2 == INT");

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


//////////////////////////////////////////////////////////////////////
// mgtk event - tree model builder helper
//////////////////////////////////////////////////////////////////////

mgtk_tree_t *mgtk_event_tree_new(int id, const char *label)
{
	mgtk_tree_t *tree = new mgtk_tree_t;
	tree->parent = NULL;
	snprintf(tree->label, mgtk_tree_label_size, label);
	tree->label[mgtk_tree_label_size-1] = 0;
	tree->id = id;
	tree->numChildren = 0;
	tree->children = NULL;

	return tree;
}


void mgtk_event_tree_copy(mgtk_tree_t *src, mgtk_tree_t *dest)
{
	if (src && dest)
	{
		snprintf(dest->label, mgtk_tree_label_size, src->label);
		dest->label[mgtk_tree_label_size-1] = 0;
		dest->id = src->id;
		dest->parent = src->parent;

		// NOTE: Shallow copy
		dest->numChildren = src->numChildren;
		dest->children = src->children;
		//MSTL_MSG("*** '%s' %i", dest->label, dest->id);
	}
}


void mgtk_event_tree_add_child(mgtk_tree_t *parent, mgtk_tree_t *child)
{
	if (parent && child)
	{
		if (child->parent)
		{
			// Might want to do something here later
		}

		MGTK_ASSERTMSG(child->parent == NULL, "Overwriting tree->parent");
		child->parent = parent;

		// NOTE: Until the new mgtk_tree struct is introduced this will 
		//       be a horrible 'dumb vector' merry-go-round insertion
		mgtk_tree_t *array = parent->children;
		parent->children = new mgtk_tree_t[parent->numChildren+1];

		for (int i = 0, n = parent->numChildren; i < n; ++i)
		{
			//MSTL_MSG("*** '%s' %i", array[i].label, array[i].id);
			mgtk_event_tree_copy(array+i, parent->children+i);
		}
		
		if (array)
			delete [] array;

		mgtk_event_tree_copy(child, parent->children+parent->numChildren);
		parent->numChildren++;
		delete child;
	}
}


void mgtk_event_tree_add_new_child(mgtk_tree_t *parent, int id, const char *label)
{
	mgtk_event_tree_add_child(parent, mgtk_event_tree_new(id, label));
}


void mgtk_event_tree_children_delete(mgtk_tree_t *tree)
{
	if (tree)
	{
		if (tree->children)
		{
			for (unsigned int i = 0; i < tree->numChildren; ++i)
			{
				mgtk_event_tree_children_delete( tree->children + i );
			}

			delete [] tree->children;
		}
	}
}


void mgtk_event_tree_delete(mgtk_tree_t *tree)
{
	if (tree)
	{
		mgtk_event_tree_children_delete(tree);
		delete tree;
	}
}


void mgtk_event_update_gtk_tree(int event, mgtk_tree_t *tree,
								GtkTreeStore *store, GtkTreeIter root)
{
	if (!tree)
	{	
		GtkTreeIter root;
		GtkTreeStore *store = gtk_tree_store_new(N_COLUMNS,       /* Total number of cols */
								   G_TYPE_STRING,   /* Bone Name */
    	                           G_TYPE_INT);     /* Bone Id */

		gtk_tree_store_append(store, &root, NULL);
		gtk_tree_store_set(store, &root,
						   NAME_COLUMN, "Skeleton",
						   ID_COLUMN, -1,
						   -1);
		mgtk_resource_rebuild_treeview(event, GTK_TREE_MODEL(store));
		return;
	}

	GtkTreeIter child;
	unsigned int i;

#ifdef DEBUG_UPDATE_GTK_TREE
	mgtk_print("mgtk_event_update_gtk_tree %d::\n", tree->id);
#endif

	if (tree->id == -1 && tree->children) // 'Hidden root' to make a 'flat' list
	{
		for (i = 1; i < tree->numChildren; ++i)
		{
			mgtk_event_update_gtk_tree(event, &tree->children[i], store, child);
		}

		mgtk_resource_rebuild_treeview(event, GTK_TREE_MODEL(store));
	}
	else if (tree->id == 0) // Root bone
	{
		store = gtk_tree_store_new(N_COLUMNS,       /* Total number of cols */
								   G_TYPE_STRING,   /* Bone Name */
    	                           G_TYPE_INT);     /* Bone Id */

		gtk_tree_store_append(store, &root, NULL);
		gtk_tree_store_set(store, &root,
						   NAME_COLUMN, tree->label,
						   ID_COLUMN, tree->id,
						   -1);

		for (i = 0; i < tree->numChildren; ++i)
		{
			mgtk_event_update_gtk_tree(event, &tree->children[i], store, root);
		}

		mgtk_resource_rebuild_treeview(event, GTK_TREE_MODEL(store));
	}
	else if (!store)
	{
		mgtk_print("mgtk_event_update_gtk_tree> ERROR: Invalid tree model\n");
	}
	else
	{
		gtk_tree_store_append(store, &child, &root);
		gtk_tree_store_set(store, &child,
						   NAME_COLUMN, tree->label,
						   ID_COLUMN, tree->id,
						   -1);

		for (i = 0; i < tree->numChildren; ++i)
		{
			mgtk_event_update_gtk_tree(event, &tree->children[i], store, child);
		}
	}
}


void mgtk_event_update_tree(unsigned int id, mgtk_tree_t *tree)
{
	GtkTreeIter root;

	mgtk_event_update_gtk_tree(id, tree, NULL, root);
}



//////////////////////////////////////////////////////////////////////////
// new tree
//////////////////////////////////////////////////////////////////////////

Map<int, GtkTreeIter *> gTreeIterMap;

// GtkTreeView *treeview = mgtk_tree_get_treeview(event);
// GtkTreeModel *model = gtk_tree_view_get_model(treeview);
//void                gtk_tree_iter_free                  (GtkTreeIter *iter);
//gchar*              gtk_tree_model_get_string_from_iter (GtkTreeModel *tree_model,
//                                                         GtkTreeIter *iter);


GtkTreeStore *mgtk_tree_store_new(const char *types)
{
	if (!types || !types[0])
		return NULL;

	unsigned int len = strlen(types);
	GType array[len];

	for (unsigned int i = 0; i < len; ++i)
	{
		switch(types[i])
		{
		case 'f':
			array[i] = G_TYPE_FLOAT;
			break;

		case 'i':
			array[i] = G_TYPE_INT;
			break;

		case 's':
			array[i] = G_TYPE_STRING;
			break;

		default:
			;
		}
	}

	return gtk_tree_store_newv(len, array);
}


char *mgtk_tree_new(int widget_id, const char *types, ...)
{
	GtkTreeStore *store = mgtk_tree_store_new(types);
	GtkTreeIter root;
	va_list args;

	va_start(args, types);
	gtk_tree_store_append(store, &root, NULL);
	gtk_tree_store_set_valist(store, &root, args);
	mgtk_resource_rebuild_treeview(widget_id, GTK_TREE_MODEL(store));
	va_end(args);

	GtkTreeView *tree_view = mgtk_tree_get_treeview(widget_id);
	GtkTreeModel *tree_model = gtk_tree_view_get_model(tree_view);
	return gtk_tree_model_get_string_from_iter(tree_model, &root);
}


void mgtk_tree_free_path(char *path)
{
	if (path) g_free(path);
}


char *mgtk_tree_append(int widget_id, const char *path, const char *types, ...)
{
	GtkTreeView *tree_view = mgtk_tree_get_treeview(widget_id);
	GtkTreeModel *tree_model = gtk_tree_view_get_model(tree_view);
	GtkTreeIter root;

	if ( gtk_tree_model_get_iter_from_string(tree_model, &root, path) )
	{
		GtkTreeIter child;
		va_list args;

		GtkTreeStore *store = mgtk_tree_store_new(types); // not correct

		va_start(args, types);
		gtk_tree_store_append(store, &child, &root);
		gtk_tree_store_set_valist(store, &child, args);
		mgtk_resource_rebuild_treeview(widget_id, GTK_TREE_MODEL(store));
		va_end(args);

		return gtk_tree_model_get_string_from_iter(tree_model, &child);
	}

	return NULL;
}




// EXP: This is a test to see if you can run the other func via CLI-like
//      interface to avoid passing C-structs
void mgtk_event_tree_builder(mgtk_tree_t *&tree, const char *format, ...)
{
	// Removed
}

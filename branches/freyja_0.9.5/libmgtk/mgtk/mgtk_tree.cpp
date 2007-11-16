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
#include "mgtk_lua.h"
#include "mgtk_resource.h"
#include "mgtk_events.h"
#include "mgtk_tree.h"

using namespace mstl;


/* Multiple tree widgets with unique event ids per widget. */
Map<int, GtkTreeView *> gTreeWidgetMap;


//////////////////////////////////////////////////////////////////////
// mgtk event - tree model builder helper
//////////////////////////////////////////////////////////////////////

mgtk_tree_t* mgtk_tree_new(const char* label, int id)
{
	mgtk_tree_t *tree = new mgtk_tree_t;

	snprintf(tree->label, mgtk_tree_label_size, label);
	tree->label[mgtk_tree_label_size-1] = '\0';
	tree->id = id;

	tree->parent = NULL;
	tree->child = NULL;
	tree->sibling = NULL;

	return tree;
}


void mgtk_tree_shallow_copy(mgtk_tree_t* src, mgtk_tree_t* dest)
{
	if ( src && dest )
	{
		dest->id = src->id;
		snprintf(dest->label, mgtk_tree_label_size, src->label);
		dest->label[ mgtk_tree_label_size - 1 ] = '\0';

		dest->parent = src->parent;
		dest->child = src->child;
		dest->sibling = src->sibling;
	}
}


void mgtk_tree_add_child(mgtk_tree_t* parent, mgtk_tree_t* child)
{
	/* Throw a warning dialog for debugging on invalid precondition. */
	//MGTK_ASSERTMSG( (parent && child), "Assertion failure.");

	if ( parent && child )
	{
		/* Throw a warning dialog for debugging on overwrite. */
		MGTK_ASSERTMSG(child->parent == NULL, "Overwriting tree->parent.");

		/* Set child record. */
		child->parent = parent;
		child->sibling = NULL;

		/* Append to parent's child list. */
		if ( !parent->child )
		{
			parent->child = child;
		}
		else
		{
			mgtk_tree_t* node = parent->child;

			if ( node )
			{
				while ( node->sibling )
				{
					node = node->sibling;
				}

				node->sibling = child;
			}
		}
	}
}


mgtk_tree_t* mgtk_tree_add_new_child(mgtk_tree_t* parent, const char* label, int id)
{
	mgtk_tree_t* child = mgtk_tree_new(label, id);
	mgtk_tree_add_child( parent, child );
	return child;
}


void mgtk_tree_delete_children(mgtk_tree_t* tree)
{
	if (tree)
	{
		tree->child = NULL;
		mgtk_tree_delete( tree->child );
	}
}


void mgtk_tree_delete(mgtk_tree_t* tree)
{
	if ( tree )
	{
		mgtk_tree_delete( tree->sibling );
		mgtk_tree_delete( tree->child );
		delete tree;
	}
}


void mgtk_tree_update_widget_helper(const char* name, int event, mgtk_tree_t* tree, 
									GtkTreeStore* store, GtkTreeIter root)
{
	if (!tree)
	{	
		GtkTreeIter root;
		GtkTreeStore *store = gtk_tree_store_new(N_COLUMNS,       /* Total number of cols */
												 G_TYPE_STRING,   /* Node name */
												 G_TYPE_INT);     /* Node id */

		gtk_tree_store_append(store, &root, NULL);
		gtk_tree_store_set(store, &root,
						   NAME_COLUMN, name,
						   ID_COLUMN, -1,
						   -1);
		mgtk_resource_rebuild_treeview(event, GTK_TREE_MODEL(store));
		return;
	}	

#ifdef DEBUG_UPDATE_GTK_TREE
	mgtk_print("mgtk_tree_update_widget_helper %d::\n", tree->id);
#endif

	/* 'Hidden root' to make a 'flat' list view. */
	if ( tree->id == -2 && tree->child )
	{
		GtkTreeIter child;
		mgtk_tree_t* node = tree->child;

		if ( node )
		{
			/* List view skips first child node. */
			while ( node->sibling )
			{
				mgtk_tree_update_widget_helper(name, event, node->sibling, store, child);
				node = node->sibling;
			}
		}

		mgtk_resource_rebuild_treeview( event, GTK_TREE_MODEL(store) );
	}
	else if ( tree->id == -1 ) // 'Root'
	{
		store = gtk_tree_store_new(N_COLUMNS,       /* Total number of cols */
								   G_TYPE_STRING,   /* Tree name */
    	                           G_TYPE_INT);     /* Tree node (root) id */

		gtk_tree_store_append(store, &root, NULL);
		gtk_tree_store_set(store, &root,
						   NAME_COLUMN, tree->label,
						   ID_COLUMN, tree->id,
						   -1);

		mgtk_tree_t* node = tree->child;

		if ( node )
		{
			mgtk_tree_update_widget_helper(name, event, node, store, root);	

			while ( node->sibling )
			{
				mgtk_tree_update_widget_helper(name, event, node->sibling, store, root);
				node = node->sibling;
			}
		}

		mgtk_resource_rebuild_treeview( event, GTK_TREE_MODEL(store) );
	}
	else if (!store)
	{
		mgtk_print("mgtk_tree_update_widget_helper> ERROR: Invalid tree model\n");
	}
	else
	{
		GtkTreeIter child;
		gtk_tree_store_append(store, &child, &root);
		gtk_tree_store_set(store, &child,
						   NAME_COLUMN, tree->label,
						   ID_COLUMN, tree->id,
						   -1);

		mgtk_tree_t* node = tree->child;

		if ( node )
		{
			mgtk_tree_update_widget_helper(name, event, node, store, child);	

			while ( node->sibling )
			{
				mgtk_tree_update_widget_helper(name, event, node->sibling, store, child);
				node = node->sibling;
			}
		}
	}
}


void mgtk_tree_update_widget(const char* name, unsigned int id, mgtk_tree_t* tree)
{
	GtkTreeIter root;
	mgtk_tree_update_widget_helper(name, id, tree, NULL, root);
}


//////////////////////////////////////////////////////////////////////
// mgtk widget resource implementation
//////////////////////////////////////////////////////////////////////

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

	if ( view && model )
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

arg_list_t *mgtk_rc_tree(arg_list_t* container)
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
	arg_list_t* ret = NULL;
	GtkWidget* box = (GtkWidget *)container->data;

	GtkWidget* view = gtk_tree_view_new();	
	mgtk_tree_set_treeview(get_int(event), GTK_TREE_VIEW(view));

	g_signal_connect(view, "row-activated", (GCallback)mgtk_treeview_onRowActivated, GINT_TO_POINTER(get_int(event)));

	GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
	if ( selection )
	{
		gtk_tree_selection_set_mode(selection, GTK_SELECTION_BROWSE);
		gtk_tree_selection_set_select_function(selection, mgtk_GtkTreeSelectionFunc, GINT_TO_POINTER(get_int(event)), NULL);
	}

	/* Column "Name" */
	GtkTreeViewColumn* col = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(col, "Name");
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
	GtkCellRenderer* renderer = gtk_cell_renderer_text_new();
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
	GtkTreeStore* store = gtk_tree_store_new(N_COLUMNS, /* Total number of cols */
											 G_TYPE_STRING,   /* Bone Name */
											 G_TYPE_INT);     /* Bone Id */

	GtkTreeModel* model = GTK_TREE_MODEL(store);

	/* Add view to container */
	gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
	g_object_unref(model); 
	gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(view)),
                                GTK_SELECTION_NONE);
	gtk_widget_show(view);


#ifdef MGTK_TREE_IN_BOX
	gtk_box_pack_start(GTK_BOX(box), view, TRUE, TRUE, FALSE);
#else
	/* Tree inside scrolled window, instead of box. */
	GtkWidget* scroll = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
								   GTK_POLICY_AUTOMATIC,
								   GTK_POLICY_AUTOMATIC);
	gtk_widget_show(scroll);
	gtk_widget_set_usize(scroll, 100, 250);
	gtk_box_pack_start(GTK_BOX(box), scroll, TRUE, TRUE, FALSE);
	gtk_container_add(GTK_CONTAINER(scroll), view);
#endif

	delete_arg(&event);
	delete_arg(&event2);

	return ret;
}


int mgtk_lua_rc_tree(lua_State* s)
{
	const char* label = lua_tostring(s, 1);
	int event = (int)lua_tonumber(s, 2);
	int event2 = (int)lua_tonumber(s, 3);

	GtkWidget* view = gtk_tree_view_new();	
	mgtk_tree_set_treeview( event, GTK_TREE_VIEW(view) );
	g_signal_connect( view, "row-activated", (GCallback)mgtk_treeview_onRowActivated, GINT_TO_POINTER(event) );

	GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
	if ( selection )
	{
		gtk_tree_selection_set_mode(selection, GTK_SELECTION_BROWSE);
		gtk_tree_selection_set_select_function(selection, mgtk_GtkTreeSelectionFunc, GINT_TO_POINTER(event), NULL);
	}

	/* Column "Name" */
	GtkTreeViewColumn* col = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(col, label);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
	GtkCellRenderer* renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_add_attribute(col, renderer, "text", NAME_COLUMN);
	g_object_set(renderer, "editable", TRUE, NULL);
	g_signal_connect( renderer, "edited", (GCallback)mgtk_tree_cell_edited_callback, GINT_TO_POINTER(event2) );

	/* Column "Id" */
	col = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(col, "Id");
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_add_attribute(col, renderer, "text", ID_COLUMN);

	/* Append dummy node(s) */
	GtkTreeStore* store = gtk_tree_store_new(N_COLUMNS,       /* Total number of cols */
											 G_TYPE_STRING,   /* Node name */
											 G_TYPE_INT);     /* Node id */
	GtkTreeModel* model = GTK_TREE_MODEL(store);

	/* Add view to container */
	gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
	g_object_unref(model); 
	gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(view)), GTK_SELECTION_NONE);
	gtk_widget_show(view);


	/* Tree inside scrolled window, instead of box. */
	GtkWidget* scroll = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
								   GTK_POLICY_AUTOMATIC,
								   GTK_POLICY_AUTOMATIC);
	gtk_widget_show(scroll);
	//gtk_widget_set_usize(scroll, 100, 250);
	gtk_container_add(GTK_CONTAINER(scroll), view);

	lua_pushlightuserdata(s, scroll);

	return 1;
}


/////////////////////////////////////////////////////////////////////
// mgtk_tree_path
/////////////////////////////////////////////////////////////////////

//Map<int, GtkTreeIter *> gTreeIterMap;

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


char *mgtk_tree_path_new(int widget_id, const char *types, ...)
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


void mgtk_tree_path_free(char *path)
{
	if (path) g_free(path);
}


char *mgtk_tree_path_append(int widget_id, const char *path, const char *types, ...)
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


/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libmgtk
 * Author  : Mongoose
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

#ifndef GUARD__MGTK_TREE_H_
#define GUARD__MGTK_TREE_H_


#define mgtk_tree_label_size 64

typedef struct mgtk_tree_s 
{
	int id;
	char label[mgtk_tree_label_size];
	mgtk_tree_s* parent;
	mgtk_tree_s* child;
	mgtk_tree_s* sibling;

} mgtk_tree_t;


mgtk_tree_t* mgtk_tree_new(const char* label, int id);
/*------------------------------------------------------
 * Pre  : 
 * Post : Allocates a new <tree> and initializes members
 *
 ------------------------------------------------------*/

void mgtk_tree_shallow_copy(mgtk_tree_t* src, mgtk_tree_t* dest);
/*------------------------------------------------------
 * Pre  : You're sure you want to shallow copy contents.
 * Post : Shallow copies members of <src> to <dest>.
 *
 ------------------------------------------------------*/

void mgtk_tree_add_child(mgtk_tree_t* parent, mgtk_tree_t* child);
/*------------------------------------------------------
 * Pre  : 
 * Post : Adds an existing child tree to an existing tree
 *
 ------------------------------------------------------*/

mgtk_tree_t* mgtk_tree_add_new_child(mgtk_tree_t* parent, const char* label, int id);
/*------------------------------------------------------
 * Pre  : 
 * Post : Adds a new child tree to an existing tree
 *
 ------------------------------------------------------*/

void mgtk_tree_delete(mgtk_tree_t* tree);
/*------------------------------------------------------
 * Pre  : 
 * Post : Deallocates <tree> and all it's children
 *
 ------------------------------------------------------*/

void mgtk_tree_delete_children(mgtk_tree_t* tree);
/*------------------------------------------------------
 * Pre  : 
 * Post : Deallocates <tree>'s children
 *
 ------------------------------------------------------*/

void mgtk_tree_update_widget(const char* name, unsigned int id, mgtk_tree_t* tree);
/*------------------------------------------------------
 * Pre  : <id> is valid mgtk event
 *        <tree> is a valid mgtk tree model 
 *
 * Post : This signals to update the view(s) for event <id> 
 *        using <tree> as the updated/new data model
 *
 ------------------------------------------------------*/


/////////////////////////////////////////////////////////////////////
// mgtk_tree_path
/////////////////////////////////////////////////////////////////////

#   ifdef USING_MGTK_TREE_PATH

char *mgtk_tree_path_new(int widget_id, const char *types, ...);

void mgtk_tree_path_free(char *path);

char *mgtk_tree_path_append(int widget_id, const char *path, const char *types, ...);

#   endif // USING_MGTK_TREE_PATH

#endif // GUARD__MGTK_TREE_H_

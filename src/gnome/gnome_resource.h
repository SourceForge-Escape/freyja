/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
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

GtkWidget *getGtkGLAreaWidget();
GtkWidget *getGtkStatusBarWidget();


enum
{
   NAME_COLUMN = 0,
   ID_COLUMN,
   N_COLUMNS
};

void resource_rebuild_skeleton_treeview(GtkTreeModel *model);

#endif

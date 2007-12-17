/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libmgtk
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (c) 2000-2006 Mongoose
 * Comments: 
 * 
 *-- History ------------------------------------------------- 
 *
 * 2006.12.19:
 * Mongoose - Clean up of old mgtk ( Resource, gtk+ Type ) filedialogs
 ==========================================================================*/

#ifndef GUARD__MGTK_FILEDIALOG_H_
#define GUARD__MGTK_FILEDIALOG_H_

#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h>

GtkWidget *mgtk_create_filedialog(int event, char *title);
/*------------------------------------------------------
 * Pre  : 
 * Post : 
 *
 ------------------------------------------------------*/

void mgtk_event_filedialog_action(GtkWidget *widget, gpointer user_data);
/*------------------------------------------------------
 * Pre  : 
 * Post : 
 *
 ------------------------------------------------------*/

void mgtk_event_filedialog_cancel(GtkWidget *widget, gpointer user_data);
/*------------------------------------------------------
 * Pre  : 
 * Post : 
 *
 ------------------------------------------------------*/

void mgtk_event_filedialog_homedir(int event, GtkWidget *file, void *data);
/*------------------------------------------------------
 * Pre  : 
 * Post : 
 *
 ------------------------------------------------------*/

void mgtk_event_filedialog_pattern(int event, char *pattern);
/*------------------------------------------------------
 * Pre  : 
 * Post : 
 *
 ------------------------------------------------------*/

void mgtk_filechooser_spawn_event(GtkWidget *widget, gpointer user_data);
/*------------------------------------------------------
 * Pre  : 
 * Post : 
 *
 ------------------------------------------------------*/

GtkWidget* mgtk_link_filechooser_from_rc(int event, const char *title, const char *option);
/*------------------------------------------------------
 * Pre  : 
 * Post : 
 *
 ------------------------------------------------------*/


#endif // GUARD__MGTK_FILEDIALOG_H_

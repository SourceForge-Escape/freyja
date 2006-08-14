/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libmgtk
 * Author  : Mongoose
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (c) 2000-2004 Mongoose
 * Comments: This is the gtk+ wrapper
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
 * 2004.06.01:
 * Mongoose - Last of freyja use removed, project desn changed
 *
 * 2004.04.11:
 * Mongoose - Removed GNOME support
 *
 * 2000.09.16:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__FREYJA_MONGOOSE_FREYJA_INTERFACE_H_
#define GUARD__FREYJA_MONGOOSE_FREYJA_INTERFACE_H_

#include <gtk/gtk.h>

typedef struct 
{
	bool init;
	int plane;
	int mouse_x;
	int mouse_y;
	int width;
	int height;
	GtkWidget *appbar;

} mgtk_glarea_window_state_t;


GtkWidget *mgtk_create_glarea(unsigned int width, unsigned int height);

void mgtk_create_info_dialog(char *icon, char *message);

GtkWidget *mgtk_create_color_picker_dialog(char *title, void *event_func);

GtkWidget *mgtk_create_window(char *title, char *wmclass, char *icon_filename);

GtkWidget *mgtk_create_fileselection(int event, char *title);

GtkWidget *mgtk_create_filechooser(int event, char *title);

GtkWidget *mgtk_create_text_entry(GtkWidget *box);

GtkWidget *mgtk_create_icon(char *icon_filename, GtkIconSize icon_size);

GtkWidget *mgtk_create_vbox(GtkWidget *box, char *name, 
							bool homogeneous, int spacing,
							bool expand, bool fill, int pading);

GtkWidget *mgtk_create_hbox(GtkWidget *box, char *name, 
							bool homogeneous, int spacing,
							bool expand, bool fill, int pading);

GtkWidget *mgtk_create_spinbutton(GtkWidget *master, char *name, 
								  float val, float min, float max);

GtkWidget *mgtk_create_spinbutton2(GtkWidget *master, char *name, 
								   float val, float min, float max, float step,
								   float page, float page_sz, int digits);

GtkWidget *mgtk_create_label(GtkWidget *master, char *name, 
							 char *text, float x_align, float y_align);

GtkWidget *mgtk_create_tab(GtkWidget *notebook,  char *name, 
						   GtkWidget *tab_contents, char *label_text,
						   int tab_num);

GtkWidget *mgtk_create_notebook(GtkWidget *app);

GtkWidget *mgtk_create_toolbar(GtkWidget *box);

GtkWidget *mgtk_create_toolbar_toogle_button(GtkWidget *toolbar,  bool toggled,
											 char *icon, char *label,
											 char *help, 
											 void *event_func, int event_cmd);

GtkWidget *mgtk_create_toolbar_button(GtkWidget *toolbar,
									  char *icon, char *label, char *help, 
									  void *event_func, int event_cmd);

GtkWidget *mgtk_create_color_button(void *func, int id);

void mgtk_destroy_window(GtkWidget *widget);

void mgtk_refresh_glarea(GtkWidget *glarea);
#endif

/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : 
 * License : GPL, also (C) 2001, 2002 Mongoose
 * Comments: This is the gtk_glarea wrapper
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2001.??.??:
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

} glarea_window_state_t;


GtkWidget *mgtk_create_glarea(unsigned int width, unsigned int height);

void mgtk_create_info_dialog(char *message);

GtkWidget *mgtk_create_color_picker_dialog(char *title, void *event_func);

GtkWidget *mgtk_create_window(char *title, char *wmclass, char *icon_filename);

GtkWidget *mgtk_create_fileselection(char *title);

GtkWidget *mgtk_create_text_entry(GtkWidget *box);

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

void mgtk_destroy_window(GtkWidget *widget);

void mgtk_refresh_glarea(GtkWidget *glarea);

/* You have to implement these call backs */
void mgtk_event_gldisplay();

void mgtk_event_glresize(unsigned int width, unsigned int height);

#endif

/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
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
 =================================================================*/

#ifndef __FREYJA_MONGOOSE_FREYJA_INTERFACE_H_
#define __FREYJA_MONGOOSE_FREYJA_INTERFACE_H_

#include <gtk/gtk.h>

void close_log_file();

GtkWidget* about_create(void);

GtkWidget* window_create(void);

GtkWidget* fileselection_create(char *title);

GtkWidget *text_entry_create(GtkWidget *box);

GtkWidget *packed_vbox_create(GtkWidget *box, char *name, 
										bool homogeneous, int spacing,
										bool expand, bool fill, int pading);

GtkWidget *packed_hbox_create(GtkWidget *box, char *name, 
										bool homogeneous, int spacing,
										bool expand, bool fill, int pading);

GtkWidget *spinbutton_create(GtkWidget *master, char *name, 
									  float val, float min, float max);

GtkWidget *spinbutton2_create(GtkWidget *master, char *name, 
										float val, float min, float max,
										float step, float page, float page_sz, int digits);

GtkWidget *label_create(GtkWidget *master, char *name, 
								char *text, float x_align, float y_align);

GtkWidget *tab_create(GtkWidget *notebook,  char *name, 
							 GtkWidget *tab_contents, char *label_text, int tab_num);

GtkWidget *notebook_create(GtkWidget *app);

GtkWidget *toolbar_create(GtkWidget *box);

GtkWidget *toolbar_toogle_btn(GtkWidget *toolbar,  bool toggled,
										char *icon, char *label, char *help, 
										void *event_func, int event_cmd);

GtkWidget *toolbar_btn(GtkWidget *toolbar,
							  char *icon, char *label, char *help, 
							  void *event_func, int event_cmd);

gint window_destroy(GtkWidget *widget);
gint h_scroll_event(GtkWidget *widget, GdkEventButton *event);
gint v_scroll_event(GtkWidget *widget, GdkEventButton *event);

#endif

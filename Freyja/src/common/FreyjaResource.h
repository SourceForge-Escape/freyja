/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : FreyjaResource
 * License : No use w/o permission (C) 2002 Mongoose
 * Comments: Resource system for Freyja
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------ 
 *
 * 2002.01.12:
 * Mongoose - Created
 ================================================================*/


#ifndef GUARD__FREYJA_MONGOOSE_FREYJARESOURCE_H_
#define GUARD__FREYJA_MONGOOSE_FREYJARESOURCE_H_

#include "Resource.h"

// Mongoose 2002.01.12, Interfaces for FreyjaResource
//   must be implmented for each interface ( gtk, win32, etc )
arg_list_t *freyja_rc_window(arg_list_t *args);
arg_list_t *freyja_rc_gl_widget(arg_list_t *args);
arg_list_t *freyja_rc_notebook(arg_list_t *args);
arg_list_t *freyja_rc_tab(arg_list_t *args);
arg_list_t *freyja_rc_textbox(arg_list_t *args);
arg_list_t *freyja_rc_hsep(arg_list_t *args);
arg_list_t *freyja_rc_vsep(arg_list_t *args);
arg_list_t *freyja_rc_hbox(arg_list_t *args);
arg_list_t *freyja_rc_vbox(arg_list_t *args);
arg_list_t *freyja_rc_handlebox(arg_list_t *args);
arg_list_t *freyja_rc_label(arg_list_t *args);
arg_list_t *freyja_rc_button(arg_list_t *args);
arg_list_t *freyja_rc_colorbutton(arg_list_t *args);
arg_list_t *freyja_rc_togglebutton(arg_list_t *args);
arg_list_t *freyja_rc_spinbutton(arg_list_t *args);
arg_list_t *freyja_rc_spinbutton2(arg_list_t *args);
arg_list_t *freyja_rc_submenu(arg_list_t *args);
arg_list_t *freyja_rc_menu_seperator(arg_list_t *args);
arg_list_t *freyja_rc_menu_item(arg_list_t *args);
arg_list_t *freyja_rc_optionmenu(arg_list_t *args);
arg_list_t *freyja_rc_popup_menu(arg_list_t *args);
arg_list_t *freyja_rc_menubar(arg_list_t *args);
arg_list_t *freyja_rc_animation_tab_hack(arg_list_t *args);
arg_list_t *freyja_rc_toolbar(arg_list_t *args);
arg_list_t *freyja_rc_toolbar_togglebutton(arg_list_t *args);
arg_list_t *freyja_rc_toolbar_button(arg_list_t *args);
arg_list_t *freyja_rc_toolbar_box(arg_list_t *args);


char *freyja_rc_map(char *s);
/*------------------------------------------------------
 * Pre  : s is a valid filename or path/filename 
 *        under resouce dir
 *
 * Post : Returns full path filename as string or 
 *        NULL on error
 *
 *-- History ------------------------------------------
 *
 * 2002.01.12: 
 * Mongoose - Created
 ------------------------------------------------------*/


class FreyjaResource : public Resource
{
 public:

	FreyjaResource();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Constructs an object of FreyjaResource
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.12: 
	 * Mongoose - Created
	 ------------------------------------------------------*/

	~FreyjaResource();
	/*------------------------------------------------------
	 * Pre  : FreyjaResource object is allocated
	 * Post : Deconstructs an object of FreyjaResource
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.12: 
	 * Mongoose - Created
	 ------------------------------------------------------*/


	void RegisterCPPInt(char *symbol, int value);
	/*------------------------------------------------------
	 * Pre  : C++ Symbol is valid and matches Value
	 * Post : Binds C++ to matching symbol name in resource
	 *
	 *-- History ------------------------------------------
	 *
	 * 2002.01.21:
	 *  Mongoose - Created
	 ------------------------------------------------------*/

 private:

	/* */
};

#endif

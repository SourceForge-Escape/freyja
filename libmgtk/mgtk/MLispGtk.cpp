/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libmgtk
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : MLispGtk
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: 
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2005.04.26:
 * Mongoose - Created
 ==========================================================================*/

#include "MLispGtk.h"



void mlisp_gtk_init(MLisp &rc)
{
	rc.registerLispFunction("window", mgtk_rc_window);
	rc.registerLispFunction("icon", mgtk_rc_icon);
	rc.registerLispFunction("gl_widget", mgtk_rc_gl_widget);
	rc.registerLispFunction("notebook", mgtk_rc_notebook);
	rc.registerLispFunction("dialog", mgtk_rc_dialog);
	rc.registerLispFunction("tab", mgtk_rc_tab);
	rc.registerLispFunction("hsep", mgtk_rc_hsep);
	rc.registerLispFunction("vsep", mgtk_rc_vsep);
	rc.registerLispFunction("vbox", mgtk_rc_vbox);
	rc.registerLispFunction("hbox", mgtk_rc_hbox);
	rc.registerLispFunction("handlebox", mgtk_rc_handlebox);
	rc.registerLispFunction("textbox", mgtk_rc_textbox);
	rc.registerLispFunction("label", mgtk_rc_label);
	rc.registerLispFunction("label2", mgtk_rc_label2);
	rc.registerLispFunction("colorbutton", mgtk_rc_colorbutton);
	rc.registerLispFunction("button", mgtk_rc_button);
	rc.registerLispFunction("togglebutton", mgtk_rc_togglebutton);
	rc.registerLispFunction("spinbutton", mgtk_rc_spinbutton);
	rc.registerLispFunction("spinbutton2", mgtk_rc_spinbutton2);
	rc.registerLispFunction("menu_item", mgtk_rc_menu_item);
	rc.registerLispFunction("submenu", mgtk_rc_submenu);
	rc.registerLispFunction("menu_seperator", mgtk_rc_menu_seperator);
	rc.registerLispFunction("optionmenu", mgtk_rc_optionmenu);
	rc.registerLispFunction("popup_menu", mgtk_rc_popup_menu);
	rc.registerLispFunction("menubar", mgtk_rc_menubar);
	rc.registerLispFunction("animation_tab_hack", mgtk_rc_animation_tab_hack);
	rc.registerLispFunction("toolbar", mgtk_rc_toolbar);
	rc.registerLispFunction("toolbar_box", mgtk_rc_toolbar_box);
	rc.registerLispFunction("toolbar_togglebutton", mgtk_rc_toolbar_togglebutton);
	rc.registerLispFunction("toolbar_button", mgtk_rc_toolbar_button);
	rc.registerLispFunction("hslider", mgtk_rc_hslider);
	rc.registerLispFunction("dialog", mgtk_rc_dialog);
	rc.registerLispFunction("fileselection_hack", mgtk_rc_fileselection_drop_down_menu);

	rc.registerSymbolValue("IconSize_Menu", 1);
	rc.registerSymbolValue("IconSize_ToolbarSmall", 2);
	rc.registerSymbolValue("IconSize_Toolbar", 3);
	rc.registerSymbolValue("IconSize_Button", 4);
	rc.registerSymbolValue("IconSize_Dialog", 5);
}


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

MLispGtk::MLispGtk()
{
}


MLispGtk::~MLispGtk()
{
}


////////////////////////////////////////////////////////////
// Public Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Public Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Accessors
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Private Mutators
////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Unit Test code
////////////////////////////////////////////////////////////

#ifdef UNIT_TEST_MLISPGTK
int runMLispGtkUnitTest(int argc, char *argv[])
{
	MLispGtk test;

	return 0;
}


int main(int argc, char *argv[])
{
	printf("[MLispGtk class test]\n");

	return runMLispGtkUnitTest(argc, argv);
}
#endif

/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : MLisp
 * Author  : #Author
 * Website : http://icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : MLispWX
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

#include <wx/wx.h>
#include <wx/glcanvas.h>

#ifndef WIN32
#   include <unistd.h>
#endif

#include "MLispWX.h"


/// Hooks //////////////////////////////////////////////////////

enum mwx_types
{
	MWX_WIDGET = 1024,
	MWX_FRAME,
	MWX_GL_CANVAS
};


/* Interfaces must be implmented for each interface ( gtk+, win32, wx, etc ) */
mObject *mwx_rc_window(mObjectList *args)
{
	mObject *title = objPop(&args);
	mObject *icon = objPop(&args);
	mObject *obj;
	wxFrame *window = new wxFrame((wxFrame *)NULL, -1, getString(title),
									wxPoint(50, 50), wxSize(512, 512));

	if (icon && icon->type == CSTRING)
	{
		// Load window icon
	}

	while (args && (obj = objPop(&args)))
	{
		if (obj->type == MWX_WIDGET)
		{
			((wxWindow*)obj->data)->SetParent(window);
		}
	}

	window->Show(TRUE);

	return newADTObj(MWX_FRAME, &window);
}


mObject *mwx_rc_gl_widget(mObjectList *args)
{
	wxGLCanvas *gl_widget = new wxGLCanvas(NULL, -1, wxPoint(0,0), wxSize(200,200), wxSUNKEN_BORDER, _("OpenGL"));

	return newADTObj(MWX_GL_CANVAS, &gl_widget);
}


mObject *mwx_rc_notebook(mObjectList *args) { return NULL; }
mObject *mwx_rc_tab(mObjectList *args) { return NULL; }
mObject *mwx_rc_textbox(mObjectList *args) { return NULL; }
mObject *mwx_rc_hsep(mObjectList *args) { return NULL; }
mObject *mwx_rc_vsep(mObjectList *args) { return NULL; }
mObject *mwx_rc_hbox(mObjectList *args) { return NULL; }
mObject *mwx_rc_vbox(mObjectList *args) { return NULL; }
mObject *mwx_rc_handlebox(mObjectList *args) { return NULL; }
mObject *mwx_rc_label(mObjectList *args) { return NULL; }
mObject *mwx_rc_button(mObjectList *args) { return NULL; }
mObject *mwx_rc_colorbutton(mObjectList *args) { return NULL; }
mObject *mwx_rc_togglebutton(mObjectList *args) { return NULL; }
mObject *mwx_rc_spinbutton(mObjectList *args) { return NULL; }
mObject *mwx_rc_spinbutton2(mObjectList *args) { return NULL; }
mObject *mwx_rc_submenu(mObjectList *args) { return NULL; }
mObject *mwx_rc_menu_seperator(mObjectList *args) { return NULL; }
mObject *mwx_rc_menu_item(mObjectList *args) { return NULL; }
mObject *mwx_rc_optionmenu(mObjectList *args) { return NULL; }
mObject *mwx_rc_popup_menu(mObjectList *args) { return NULL; }
mObject *mwx_rc_menubar(mObjectList *args) { return NULL; }
mObject *mwx_rc_animation_tab_hack(mObjectList *args) { return NULL; }
mObject *mwx_rc_toolbar(mObjectList *args) { return NULL; }
mObject *mwx_rc_toolbar_togglebutton(mObjectList *args) { return NULL; }
mObject *mwx_rc_toolbar_button(mObjectList *args) { return NULL; }
mObject *mwx_rc_toolbar_box(mObjectList *args) { return NULL; }
mObject *mwx_rc_color(mObjectList *args) { return NULL; }
mObject *mwx_rc_hslider(mObjectList *args) { return NULL; }
mObject *mwx_rc_icon(mObjectList *args) { return NULL; }
mObject *mwx_rc_dialog(mObjectList *args) { return NULL; }
mObject *mwx_rc_label2(mObjectList *args) { return NULL; }
mObject *mwx_rc_fileselection_drop_down_menu(mObjectList *box) { return NULL; }

////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////
// Constructors
////////////////////////////////////////////////////////////

MLispWX::MLispWX(MLisp *mlisp)
{
	mRC = mlisp;

	mRC->registerLispFunction("window", mwx_rc_window);
	mRC->registerLispFunction("icon", mwx_rc_icon);
	mRC->registerLispFunction("gl_widget", mwx_rc_gl_widget);
	mRC->registerLispFunction("notebook", mwx_rc_notebook);
	mRC->registerLispFunction("dialog", mwx_rc_dialog);
	mRC->registerLispFunction("tab", mwx_rc_tab);
	mRC->registerLispFunction("hsep", mwx_rc_hsep);
	mRC->registerLispFunction("vsep", mwx_rc_vsep);
	mRC->registerLispFunction("vbox", mwx_rc_vbox);
	mRC->registerLispFunction("hbox", mwx_rc_hbox);
	mRC->registerLispFunction("handlebox", mwx_rc_handlebox);
	mRC->registerLispFunction("textbox", mwx_rc_textbox);
	mRC->registerLispFunction("label", mwx_rc_label);
	mRC->registerLispFunction("label2", mwx_rc_label2);
	mRC->registerLispFunction("colorbutton", mwx_rc_colorbutton);
	mRC->registerLispFunction("button", mwx_rc_button);
	mRC->registerLispFunction("togglebutton", mwx_rc_togglebutton);
	mRC->registerLispFunction("spinbutton", mwx_rc_spinbutton);
	mRC->registerLispFunction("spinbutton2", mwx_rc_spinbutton2);
	mRC->registerLispFunction("menu_item", mwx_rc_menu_item);
	mRC->registerLispFunction("submenu", mwx_rc_submenu);
	mRC->registerLispFunction("menu_seperator", mwx_rc_menu_seperator);
	mRC->registerLispFunction("optionmenu", mwx_rc_optionmenu);
	mRC->registerLispFunction("popup_menu", mwx_rc_popup_menu);
	mRC->registerLispFunction("menubar", mwx_rc_menubar);
	mRC->registerLispFunction("animation_tab_hack", mwx_rc_animation_tab_hack);
	mRC->registerLispFunction("toolbar", mwx_rc_toolbar);
	mRC->registerLispFunction("toolbar_box", mwx_rc_toolbar_box);
	mRC->registerLispFunction("toolbar_togglebutton", mwx_rc_toolbar_togglebutton);
	mRC->registerLispFunction("toolbar_button", mwx_rc_toolbar_button);
	mRC->registerLispFunction("hslider", mwx_rc_hslider);
	mRC->registerLispFunction("dialog", mwx_rc_dialog);

	/* Prototype compatibility */
	mRC->registerLispFunction("fileselection_hack", mwx_rc_fileselection_drop_down_menu);


	/* MLispGTK compatibility */
	mRC->registerSymbolValue("IconSize_Menu", 1);
	mRC->registerSymbolValue("IconSize_ToolbarSmall", 2);
	mRC->registerSymbolValue("IconSize_Toolbar", 3);
	mRC->registerSymbolValue("IconSize_Button", 4);
	mRC->registerSymbolValue("IconSize_Dialog", 5);
}


MLispWX::~MLispWX()
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

#ifdef UNIT_TEST_MLISPWX
int runMLispWXUnitTest(int argc, char *argv[])
{
	MLisp base;
	MLispWX rc(&base);

	
	base.setDebugLevel(5);

	if (argc > 1)
	{
		if (argc > 1 && base.evalFile(argv[1]) < 0)
		{
			printf("Couldn't load file!\n");
		}

		base.dumpSymbols();
	}

	return 0;
}


class StubApp: public wxApp
{
  virtual bool OnInit();
};


IMPLEMENT_APP(StubApp)


bool StubApp::OnInit()
{
	printf("[MLispWX class test]\n");
	runMLispWXUnitTest(argc, argv);
	return true;
}

//int main(int argc, char *argv[])
//{
//	printf("[MLispWX class test]\n");
//
//	return runMLispWXUnitTest(argc, argv);
//}
#endif

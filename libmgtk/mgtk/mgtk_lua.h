/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Mgtk
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2007 Mongoose
 * Comments: Confirmation Dialog class
 *
 * 
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.08.18:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__MGTK_LUA_H_
#define GUARD__MGTK_LUA_H_

#ifdef LUAWRAPPER_FOUND

#   include "lua/Lua.h"

void mgtk_lua_register_functions(const Lua &vm);
/*------------------------------------------------------
 * Pre : lua is a valid VM wrapper.
 * Post: Register mgtk functions for lua use.
 *
 ------------------------------------------------------*/


/* Mongoose 2002.01.12, Interfaces for Resource
   must be implmented for each interface ( gtk, win32, etc ) */

int mgtk_lua_rc_window(lua_State *s);
/*------------------------------------------------------
 *  
 * mgtk_window("title" "icon/filename")
 *
 * Signal: None.
 *
 * Post  : 
 *
 ------------------------------------------------------*/

int mgtk_lua_is_null(lua_State *s);
/*------------------------------------------------------
 * Pre : 
 * Post: i = mgtk_is_null( widget )
 * 
 ------------------------------------------------------*/

int mgtk_lua_rc_gl_widget(lua_State *s);
int mgtk_lua_rc_notebook(lua_State *s);
int mgtk_lua_rc_tab(lua_State *s);
int mgtk_lua_rc_textbox(lua_State *s);
int mgtk_lua_rc_hsep(lua_State *s);
int mgtk_lua_rc_vsep(lua_State *s);
int mgtk_lua_rc_button(lua_State *s);
int mgtk_lua_rc_colorbutton(lua_State *s);
int mgtk_lua_rc_togglebutton(lua_State *s);

int mgtk_lua_rc_submenu(lua_State *s);
int mgtk_lua_rc_menu_seperator(lua_State *s);

int mgtk_lua_rc_menu_item(lua_State *s);
/*------------------------------------------------------
 * Pre : 
 * Post: 
 *
 * menuitem = 
 * mgtk_menu_item("label", eventId [, "icon" | "accel", "icon"])
 * 
 ------------------------------------------------------*/

int mgtk_lua_rc_check_menu_item(lua_State *s);
int mgtk_lua_rc_optionmenu(lua_State *s);
int mgtk_lua_rc_popup_menu(lua_State *s);
int mgtk_lua_rc_menubar(lua_State *s);

int mgtk_lua_rc_tree(lua_State *s);

int mgtk_lua_rc_toolbar(lua_State *s);
int mgtk_lua_rc_toolbar_togglebutton(lua_State *s);
int mgtk_lua_rc_toolbar_button(lua_State *s);
int mgtk_lua_rc_toolbar_menu_button(lua_State *s);
int mgtk_lua_rc_toolbar_box(lua_State *s);

int mgtk_lua_rc_color(lua_State *s);
int mgtk_lua_rc_hslider(lua_State *s);
int mgtk_lua_rc_icon(lua_State *s);
int mgtk_lua_rc_dialog(lua_State *s);
int mgtk_lua_rc_query_dialog(lua_State *s);
int mgtk_lua_rc_confirmation_dialog(lua_State *s);
int mgtk_lua_rc_label2(lua_State *s);
int mgtk_lua_rc_fileselection_drop_down_menu(lua_State *s);
int mgtk_lua_rc_filechooserbutton(lua_State *s);
int mgtk_lua_rc_filechoosermenu_item(lua_State *s);
int mgtk_lua_rc_filechoosertoolbar_button(lua_State *s);
int mgtk_lua_rc_toolbar_separator(lua_State *s);
int mgtk_lua_rc_toolbar_item(lua_State *s);
int mgtk_lua_rc_expander(lua_State *s);
int mgtk_lua_rc_expander_hbox(lua_State *s);


//////////////////////////////////////////////////////////////////////////////
// Containers
////////////////////////////////////////////////////////////////////////////// 

int mgtk_lua_rc_box_pack(lua_State *s);
/*------------------------------------------------------
 *  
 * mgtk_box_pack(box, widget, [ expand, fill, padding ])
 *
 * Signal: None.
 *
 * Post  : Pack a widget into a box and make it visible.
 *
 ------------------------------------------------------*/

int mgtk_lua_rc_hbox(lua_State *s);
/*------------------------------------------------------
 *  
 * mgtk_hbox( [homogeneous, spacing] )
 *
 * Signal: None.
 *
 * Post  : Construct a widget hortizontal container.
 *
 ------------------------------------------------------*/

int mgtk_lua_rc_vbox(lua_State *s);
/*------------------------------------------------------
 *  
 * mgtk_hbox( [homogeneous, spacing] )
 *
 * Signal: None.
 *
 * Post  : Construct a widget vertical container.
 *
 ------------------------------------------------------*/

int mgtk_lua_rc_handlebox(lua_State *s);
/*------------------------------------------------------
 *  
 * mgtk_handlebox( [pos] )
 *
 * Signal: None.
 * Pre   : pos = {0 Top}, {1 Left}, {2 Right}
 * Post  : Construct a detachable widget container.
 *
 ------------------------------------------------------*/

int mgtk_lua_rc_summonbox(lua_State *s);
/*------------------------------------------------------
 *  
 * mgtk_handlebox( symbol )
 *
 * Signal: None.
 * Pre   : Symbol - symbol name of a 'named' widget container.
 * Post  : Reuse a previous 'named' widget container.
 *
 ------------------------------------------------------*/

int mgtk_lua_rc_append_menu(lua_State *s);
/*------------------------------------------------------
 *  
 * mgtk_append_menu( menu, menuitem )
 *
 ------------------------------------------------------*/



//////////////////////////////////////////////////////////////////////////////
// Event widgets
////////////////////////////////////////////////////////////////////////////// 

int mgtk_lua_rc_spinbutton_uint(lua_State *s);
/*------------------------------------------------------
 *  
 * spinbutton = 
 * mgtk_spinbutton_uint(eventId, initialValue, min, max [, step, page, pageSize])
 *
 * Signal: mgtk_event_spinbutton_uint
 *
 ------------------------------------------------------*/

int mgtk_lua_rc_spinbutton_int(lua_State *s);
/*------------------------------------------------------
 *  
 * spinbutton = 
 * mgtk_spinbutton_int(eventId, initialValue, min, max [, step, page, pageSize])
 *
 * Signal: mgtk_event_spinbutton_int
 *
 ------------------------------------------------------*/

int mgtk_lua_rc_spinbutton_float(lua_State *s);
/*------------------------------------------------------
 *  
 * spinbutton = 
 * mgtk_spinbutton_float(eventId, initialValue, min, max, digits [, step, page, pageSize])
 *
 * Signal: mgtk_event_spinbutton_float
 *
 ------------------------------------------------------*/

int mgtk_lua_rc_statusbar(lua_State *s);

int mgtk_lua_rc_textview(lua_State *s);
/*------------------------------------------------------
 *  
 * textview = 
 * mgtk_textview( "text" )
 *
 * Signal: 
 *
 ------------------------------------------------------*/

int mgtk_lua_rc_label(lua_State *s);
/*------------------------------------------------------
 *  
 * label = 
 * mgtk_label( "text", x_align, y_align )
 *
 * Signal: 
 *
 ------------------------------------------------------*/

int mgtk_lua_func_toggle_set(lua_State *s);


#endif // LUAWRAPPER_FOUND

#endif // GUARD__MGTK_LUA_H_

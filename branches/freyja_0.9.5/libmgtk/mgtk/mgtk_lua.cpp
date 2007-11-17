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

#ifdef LUAWRAPPER_FOUND

#include <gtk/gtk.h>

#include "lua/Lua.h"

#include "Resource.h"        /* Hook into mlisp symbol table and event map for compatibility. */
#include "ResourceEvent.h"

#include "mgtk_callbacks.h"
#include "mgtk_events.h"
#include "mgtk_interface.h"
#include "mgtk_resource.h"

#include "mgtk_lua.h"


void mgtk_lua_register_functions(const Lua &lua)
{
	lua.RegisterFunction("mgtk_event", mgtk_lua_event);
	lua.RegisterFunction("mgtk_is_null", mgtk_lua_is_null);

	lua.RegisterFunction("mgtk_box_pack", mgtk_lua_rc_box_pack);

	lua.RegisterFunction("mgtk_window", mgtk_lua_rc_window);

	lua.RegisterFunction("mgtk_hbox", mgtk_lua_rc_hbox);
	lua.RegisterFunction("mgtk_vbox", mgtk_lua_rc_vbox);
	lua.RegisterFunction("mgtk_handlebox", mgtk_lua_rc_handlebox);
	lua.RegisterFunction("mgtk_summonbox", mgtk_lua_rc_summonbox);
	lua.RegisterFunction("mgtk_expander", mgtk_lua_rc_expander);
	lua.RegisterFunction("mgtk_notebook", mgtk_lua_rc_notebook);
	lua.RegisterFunction("mgtk_tab", mgtk_lua_rc_tab);

	lua.RegisterFunction("mgtk_tree", mgtk_lua_rc_tree);

	lua.RegisterFunction("mgtk_opengl_canvas", mgtk_lua_rc_opengl_canvas);

	lua.RegisterFunction("mgtk_menu_item", mgtk_lua_rc_menu_item);
	lua.RegisterFunction("mgtk_append_menu", mgtk_lua_rc_append_menu);
	lua.RegisterFunction("mgtk_menubar", mgtk_lua_rc_menubar);
	lua.RegisterFunction("mgtk_submenu", mgtk_lua_rc_submenu);
	lua.RegisterFunction("mgtk_menu_separator", mgtk_lua_rc_menu_separator);
	lua.RegisterFunction("mgtk_optionmenu", mgtk_lua_rc_optionmenu);

	lua.RegisterFunction("mgtk_button", mgtk_lua_rc_button);
	lua.RegisterFunction("mgtk_colorbutton", mgtk_lua_rc_colorbutton);
	lua.RegisterFunction("mgtk_spinbutton_uint", mgtk_lua_rc_spinbutton_uint);
	lua.RegisterFunction("mgtk_spinbutton_int", mgtk_lua_rc_spinbutton_int);
	lua.RegisterFunction("mgtk_spinbutton_float", mgtk_lua_rc_spinbutton_float);

	lua.RegisterFunction("mgtk_toolbar", mgtk_lua_rc_toolbar);
	lua.RegisterFunction("mgtk_toolbar_button", mgtk_lua_rc_toolbar_button);
	lua.RegisterFunction("mgtk_toolbar_menubutton", mgtk_lua_rc_toolbar_menubutton);
	lua.RegisterFunction("mgtk_toolbar_togglebutton", mgtk_lua_rc_toolbar_togglebutton);	
	lua.RegisterFunction("mgtk_toolbar_separator", mgtk_lua_rc_toolbar_separator);
	lua.RegisterFunction("mgtk_toolbar_box", mgtk_lua_rc_toolbar_box);

	lua.RegisterFunction("mgtk_statusbar", mgtk_lua_rc_statusbar);
	lua.RegisterFunction("mgtk_textview", mgtk_lua_rc_textview);
	lua.RegisterFunction("mgtk_label", mgtk_lua_rc_label);

	lua.RegisterFunction("mgtk_toggle_set", mgtk_lua_func_toggle_set);
	lua.RegisterFunction("mgtk_window_move", mgtk_lua_window_move);
}


int mgtk_lua_get_id(const char* symbol)
{
	return ResourceEvent::GetResourceIdBySymbol( symbol );
}


int mgtk_lua_event(lua_State* s)
{
	int id = -1;

	if ( lua_gettop(s) == 1 && lua_isstring(s, 1) )
	{
		// FIXME: Use legacy mlisp resource bindings for now.
		id = ResourceEvent::GetResourceIdBySymbol( lua_tostring(s, 1) );
	}	
	else if ( lua_gettop(s) == 2 && 
			  lua_isstring(s, 1) && lua_isstring(s, 2) )
	{
		const char* symbol = lua_tostring(s, 1);
		const char* script = lua_tostring(s, 2);

		// 1. Check to see if this script is already bound to this symbol.
		id = ResourceEvent::GetResourceIdBySymbol( symbol );

		// 2. If not bind it.
		if ( id == -1 )
		{
			ResourceEventCallbackLuaScript::add(symbol, s, script);

			// 3. Return event id for this newly bound script.
			id = mgtk_lua_get_id( symbol );
		}
	}	
	else if ( lua_gettop(s) == 4 && 
			  lua_isstring(s, 1) && lua_isstring(s, 2)  && 
			  lua_isnumber(s, 3)  && lua_isstring(s, 4) )
	{
		const char* symbol = lua_tostring(s, 1);
		const char* func = lua_tostring(s, 2);

		// 1. Check to see if this script is already bound to this symbol.
		id = ResourceEvent::GetResourceIdBySymbol( symbol );

		// 2. If not bind it.
		if ( id == -1 )
		{
#if FIXME
			// Find a good way to wedge function binding to strict legacy event system.
			ResourceEventCallbackLuaFunc::add(symbol, s, func, 0, "");

			ResourceEventCallbackLuaFunc1u::add(symbol, s, func, 1, "u");

			ResourceEventCallbackLuaFunc1f::add(symbol, s, func, 1, "f");

			ResourceEventCallbackLuaFunc1s::add(symbol, s, func, 1, "s");

			// 3. Return event id for this newly bound script.
			id = ResourceEvent::GetResourceIdBySymbol( symbol );
#endif
		}
	}	

	lua_pushinteger(s, id);
	return 1;
}


int mgtk_lua_get_widget_by_name(lua_State* s)
{
	GtkWidget* ptr;

	if ( lua_gettop(s) == 1 && lua_isstring(s, 1) )
	{
		// FIXME: Use legacy mlisp resource bindings for now.
		ptr = (GtkWidget*)mlisp_recall( lua_tostring(s, 1) );
	}

	lua_pushlightuserdata(s, (void*)ptr);
	return 1;
}


int mgtk_lua_is_null(lua_State* s)
{	
	if ( lua_gettop(s) == 1 && lua_islightuserdata(s, 1) )
	{
		void* ptr = (void*)lua_touserdata(s, 1);
		lua_pushinteger(s, (unsigned long int)ptr);
	}
	else
	{
		lua_pushinteger(s, 0);
	}

	return 1;
}


//////////////////////////////////////////////////////////////////////////////
// Containers
////////////////////////////////////////////////////////////////////////////// 

int mgtk_lua_rc_window(lua_State* s)
{
	GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_object_set_data(GTK_OBJECT(window), "window", window);
	gtk_window_set_policy(GTK_WINDOW(window), FALSE, TRUE, FALSE);

	if ( lua_gettop(s) >= 2 &&
		 lua_isstring(s, 1) && lua_isstring(s, 2) )
	{
		const char* title = lua_tostring(s, 1);
		gtk_window_set_title(GTK_WINDOW(window), title);

		/* Icon. */
		GdkPixbuf* icon = mgtk_create_pixbuf( lua_tostring(s, 2) );

		if ( window && icon )
		{
			gtk_window_set_icon(GTK_WINDOW(window), icon);
			gdk_pixbuf_unref(icon);
		}
	}

	gtk_widget_show(window);

	if ( lua_gettop(s) >= 4 &&
		 lua_isnumber(s, 3) && lua_isnumber(s, 4) )
	{
		int x = (int)lua_tonumber(s, 3);
		int y = (int)lua_tonumber(s, 4);
		gtk_window_move(GTK_WINDOW(window), x, y);
	}

#if LUA_ALL_WINDOWS_CLOSE_APP
	g_signal_connect(GTK_OBJECT(window), "delete_event",
					 G_CALLBACK(mgtk_event_close_window),
					 GINT_TO_POINTER(window));
#endif

	/* Push widget pointer unto the lua stack. */
#if 1
	GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), vbox);
	gtk_widget_ref(vbox);
	gtk_widget_show(vbox);

	lua_pushlightuserdata(s, vbox);
#else
	lua_pushlightuserdata(s, window);
#endif

	return 1;
}


int mgtk_lua_rc_box_pack(lua_State *s)
{
	if ( lua_gettop(s) >= 2 &&
		 lua_islightuserdata(s, 1) && lua_islightuserdata(s, 2) )
	{
		GtkWidget *box = (GtkWidget *)lua_touserdata(s, 1);
		GtkWidget *widget = (GtkWidget *)lua_touserdata(s, 2);

		MGTK_ASSERTMSG(box != NULL, "Invalid container.");

		// Handle 'non-box' containers.
		if ( GTK_IS_HANDLE_BOX(box) )
		{
			gtk_container_add((GtkContainer *)box, widget);
			return 0;
		}

		if ( lua_gettop(s) == 5 )
		{
			int expand = lua_toboolean(s, 3);
			int fill = lua_toboolean(s, 4);
			int padding = (int)lua_tonumber(s, 5);

			gtk_box_pack_start(GTK_BOX(box), widget, expand, fill, padding);
			gtk_widget_show(box);
			gtk_widget_show(widget);
		}
		else
		{
			gtk_box_pack_start(GTK_BOX(box), widget, FALSE, FALSE, 0 );//TRUE, TRUE, 0);
			gtk_widget_show(box);
			gtk_widget_show(widget);
		}
	}

	return 0;
}


int mgtk_lua_rc_expander(lua_State *s)
{
	GtkWidget *widget = NULL;

	if ( lua_gettop(s) >= 3 &&
		 lua_islightuserdata(s, 1) &&
		 lua_isstring(s, 2) && lua_isboolean(s, 3) )
	{
		GtkWidget* box = (GtkWidget*)lua_touserdata(s, 1);
		const char *label = lua_tostring(s, 2);
		int expand = lua_toboolean(s, 3);

		widget = gtk_expander_new(label);		
		gtk_expander_set_expanded((GtkExpander*)widget, expand ? TRUE : FALSE);

		gtk_container_add((GtkContainer *)box, widget);

		GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
		gtk_container_add((GtkContainer *)widget, vbox);
		gtk_widget_show_all(widget);

		// Optional mlisp symbol table binding.
		if ( lua_gettop(s) == 4 && lua_isstring(s, 4) )
		{
			const char *name = lua_tostring(s, 4);

			// This makes a copy of the arg with a box in it for binding
			// to the name symbol.  This is for S-Class rank only, since
			// it populates the symbol table directly.
			//
			// To get the box back use the (summonbox ...) function.

			arg_list_t *symbol, *sealing_jutsu;
			new_string(&symbol, (char *)name);
			new_adt(&sealing_jutsu, ARG_GTK_BOX_WIDGET, (void *)vbox);
			mlisp_bind(symbol, sealing_jutsu);
		}

		widget = vbox;
	}

	// Push widget pointer unto the lua stack.
	lua_pushlightuserdata(s, (void *)widget);

	return 1;
}


int mgtk_lua_rc_hbox(lua_State *s)
{	
	int homogeneous = 0;
	int spacing = 0;

	/* Optional arguments */
	if ( lua_gettop(s) >= 2 && 
		 lua_isboolean(s, 1) && lua_isnumber(s, 2) )
	{
		homogeneous = lua_toboolean(s, 1);
		spacing = (int)lua_tonumber(s, 2);
	}

	GtkWidget *widget = gtk_hbox_new(homogeneous ? TRUE : FALSE, spacing);

	// Push widget pointer unto the lua stack.
	lua_pushlightuserdata(s, (void *)widget);

	return 1;
}


int mgtk_lua_rc_vbox(lua_State *s)
{	
	int homogeneous = 0;
	int spacing = 0;

	/* Optional arguments */
	if ( lua_gettop(s) == 2 && 
		 lua_isboolean(s, 1) && lua_isnumber(s, 2) )
	{
		homogeneous = lua_toboolean(s, 1);
		spacing = (int)lua_tonumber(s, 2);
	}

	GtkWidget *vbox = gtk_vbox_new(homogeneous ? TRUE : FALSE, spacing);
	//gtk_widget_ref(vbox);

	// Push widget pointer unto the lua stack.
	lua_pushlightuserdata(s, vbox);

	return 1;
}


int mgtk_lua_rc_handlebox(lua_State *s)
{
	GtkWidget *handlebox = gtk_handle_box_new();

	if ( lua_gettop(s) >= 1 && lua_isnumber(s, 1) )
	{
		int pos = (int)lua_tonumber(s, 1);
		gtk_handle_box_set_handle_position(GTK_HANDLE_BOX(handlebox), 
										   ( pos == 0 ) ? GTK_POS_TOP : 
										   ( pos == 1 ) ? GTK_POS_LEFT : GTK_POS_RIGHT );
	}

	// Push pointer to the box.
	lua_pushlightuserdata(s, handlebox);

	return 1;
}


int mgtk_lua_rc_summonbox(lua_State *s)
{	
	// Required string argument.
	if ( lua_gettop(s) == 1 && lua_isstring(s, 1) )
	{
		const char *name = lua_tostring(s, 1);

		// FIXME: Lua wrapper needs it's own symbol table if mlisp becomes optional.
		void *summonbox = mlisp_recall( name );

		// Push pointer to the box.
		lua_pushlightuserdata(s, summonbox);
	}	
	else
	{
		// Push NULL pointer on error.
		lua_pushlightuserdata(s, NULL);
	}

	return 1;
}


int mgtk_lua_rc_opengl_canvas(lua_State *s)
{
	int width = (int)lua_tonumber(s, 1);
	int height = (int)lua_tonumber(s, 2);

	GtkWidget* canvas = mgtk_create_glarea(width, height);

	if ( canvas )
	{
		gpointer gobj = gtk_object_get_data(GTK_OBJECT(canvas), "gl_window_state");
		mgtk_glarea_window_state_t* state = (mgtk_glarea_window_state_t*)gobj;
		state->appbar = NULL;

#if defined HAVE_GTKGLEXT
	gtk_signal_connect(GTK_OBJECT(canvas), "key_press_event",
					   GTK_SIGNAL_FUNC(mgtk_event_key_press), NULL);
	gtk_signal_connect(GTK_OBJECT(canvas), "key_release_event",
					   GTK_SIGNAL_FUNC(mgtk_event_key_release), NULL);
	gtk_signal_connect(GTK_OBJECT(canvas), "destroy",
					   GTK_SIGNAL_FUNC(mgtk_destroy_window), NULL);
#endif

	// FIXME: This needs to hook into a 'refresh/display' callback separately now.

	//GTK_GL_AREA_WIDGET = canvas;
		mgtk_print("@Gtk+ GL context started...");
	}
	else
	{
		canvas = gtk_label_new("Failed to create OpenGL context for canvas.");
		gtk_widget_set_usize(canvas, width, height);
		mgtk_print("!ERROR: OpenGL context not supported by this system?\n");
	}
	
	/* Editing window */
	//GtkWidget *vbox = mgtk_create_vbox(GTK_WIDGET(box), "box", 0, 0, 1, 1, 1);
	//gtk_box_pack_start(GTK_BOX(box), GTK_WIDGET(canvas), TRUE, TRUE, 0);

	gtk_widget_show(canvas);

	lua_pushlightuserdata(s, (void*)canvas);

	return 1;
}


//////////////////////////////////////////////////////////////////////////////
// Event widgets
////////////////////////////////////////////////////////////////////////////// 

int mgtk_lua_rc_menu_separator(lua_State *s)
{
	GtkWidget* sep = gtk_menu_item_new();
	gtk_widget_show(sep);
	lua_pushlightuserdata(s, sep);
	return 1;
}


int mgtk_lua_rc_append_menu(lua_State *s)
{
	if ( lua_gettop(s) >= 2 &&
		 lua_islightuserdata(s, 1) && lua_islightuserdata(s, 2) )
	{
		GtkWidget *menu = (GtkWidget *)lua_touserdata(s, 1);
		GtkWidget *item = (GtkWidget *)lua_touserdata(s, 2);

		/* menu_items must be use their submenu member directly. */
		if ( GTK_IS_MENU_ITEM(menu) )
		{
			gtk_menu_append(GTK_MENU( gtk_menu_item_get_submenu( GTK_MENU_ITEM(menu) ) ), item);
		}
		else if ( GTK_IS_MENU_BAR(menu) )
		{
			gtk_menu_append(GTK_MENU_BAR(menu), item);
		}
		else
		{
			gtk_menu_append(GTK_MENU(menu), item);
		}
	}

	return 0;
}


int mgtk_lua_rc_menubar(lua_State *s)
{
	GtkWidget* menubar = gtk_menu_bar_new();
	gtk_widget_show(menubar);

	// Push widget pointer unto the lua stack.
	lua_pushlightuserdata(s, menubar);

	return 1;
}


int mgtk_lua_rc_optionmenu(lua_State* s)
{
	const char* label = NULL;
	int id = -1;

	if ( lua_isstring(s, 1) && lua_isnumber(s, 2) )
	{
		label = lua_tostring(s, 1);
		id = (int)lua_tonumber(s, 2);
	}

	GtkWidget* optionmenu = gtk_option_menu_new();
	gtk_widget_ref(optionmenu);
	gtk_widget_show(optionmenu);

	GtkWidget* optionmenu_menu = gtk_menu_new();
	gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu), optionmenu_menu);
	gtk_widget_show(optionmenu_menu);

	if ( id != -1)
		mgtk_event_subscribe_gtk_widget(id, optionmenu);

	lua_pushlightuserdata(s, (void *)optionmenu_menu);
	return 1;
}


int mgtk_lua_rc_submenu(lua_State *s)
{
	const char *label = NULL;
	int id = -1;

	if ( lua_gettop(s) > 0 && lua_isstring(s, 1) )
	{
		label = lua_tostring(s, 1);
	}

	if ( lua_gettop(s) > 1 )
	{
		if ( lua_isnumber(s, 2) )
			id = (int)lua_tonumber(s, 2);
		else if ( lua_isstring(s, 2) )
			id = mgtk_lua_get_id( lua_tostring(s, 2) );
	}

	GtkWidget *item = gtk_image_menu_item_new_with_mnemonic(label);
	GtkWidget *submenu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), submenu);
	gtk_widget_show(item);
	gtk_widget_show(submenu);

	if (id != -1)
	{
		// Mongoose 2002.02.14, Add this widget to a special 
		//   lookup table by it's event id
		mgtk_event_subscribe_gtk_widget(id, submenu);
	}

	if ( lua_gettop(s) == 3 && lua_isstring(s, 3) )
	{
		const char* symbol = lua_tostring(s, 3);

		if (symbol)
		{
			arg_list_t *symbol_arg, *binding; 
			new_string(&symbol_arg, (char *)symbol);
			new_adt(&binding, ARG_GTK_MENU_WIDGET, (void *)submenu);
			mlisp_bind(symbol_arg, binding);
		}
	}

	// Push widget pointer unto the lua stack.
	lua_pushlightuserdata(s, (void *)item);

	return 1;
}


int mgtk_lua_rc_menu_item(lua_State *s)
{
	const char* label = NULL;
	const char* icon = NULL;
	const char* accel = NULL;
	int id = -1;


	if ( lua_gettop(s) > 0 && lua_isstring(s, 1) )
	{
		label = lua_tostring(s, 1);
	}

	if ( lua_gettop(s) > 1 )
	{
		if ( lua_isnumber(s, 2) )
			id = (int)lua_tonumber(s, 2);
		else if ( lua_isstring(s, 2) )
			id = mgtk_lua_get_id( lua_tostring(s, 2) );
	}

	if ( lua_gettop(s) > 3 && lua_isstring(s, 3) && lua_isstring(s, 4) )
	{
		accel = lua_tostring(s, 3);
		icon = lua_tostring(s, 4);
	}
	else if ( lua_gettop(s) > 2 && lua_isstring(s, 3) )
	{
		icon = lua_tostring(s, 3);
	}


	GtkWidget *item = gtk_image_menu_item_new_with_mnemonic( label );

	if ( icon )
	{
		gtk_image_menu_item_set_image(GTK_IMAGE_MENU_ITEM(item),
									  mgtk_create_icon(icon, GTK_ICON_SIZE_MENU) );
	}

	if ( accel )
	{
		mgtk_accel_support(item, accel);
	}

	gtk_widget_show(item);

	if (id != -1)
	{
		// Mongoose 2002.02.14, Add this widget to a special 
		//   lookup table by it's event id
		mgtk_event_subscribe_gtk_widget(id, item);

		gtk_signal_connect(GTK_OBJECT(item), "activate",
						   GTK_SIGNAL_FUNC(mgtk_event_command), 
						   GINT_TO_POINTER(id) );
	}

#if 0

		new_adt(&ret, ARG_GTK_MENU_WIDGET, (void *)item);		
		gtk_menu_append(GTK_MENU(menu->data), item);
		gtk_widget_show(item);

		if (subevent)
		{
			gtk_object_set_data(GTK_OBJECT(item), "mlisp_event", 
								GINT_TO_POINTER(get_int(subevent)));
			gtk_signal_connect(GTK_OBJECT(item), "activate",
							   GTK_SIGNAL_FUNC(mgtk_event_dual_command), 
							   GINT_TO_POINTER(get_int(event)));
		}
		else
		{
			gtk_signal_connect(GTK_OBJECT(item), "activate",
							   GTK_SIGNAL_FUNC(mgtk_event_command), 
							   GINT_TO_POINTER(get_int(event)));
		}
#endif

	// Push widget pointer unto the lua stack.
	lua_pushlightuserdata(s, (void *)item);

	return 1;
}


int mgtk_lua_rc_button(lua_State *s)
{
	// FIXME: Asserts, checking...
	const char *label = lua_tostring(s, 1);
	int event = (int)lua_tonumber(s, 2);

	GtkWidget* widget = gtk_button_new_with_label(label);

	// Add this widget to an event id map.
	if ( event != -1 )
		mgtk_event_subscribe_gtk_widget(event, widget);

	gtk_signal_connect(GTK_OBJECT(widget), "pressed",
					   GTK_SIGNAL_FUNC(mgtk_event_command), 
					   GINT_TO_POINTER(event));
		
	lua_pushlightuserdata(s, widget);

	return 1;
}


int mgtk_lua_rc_colorbutton(lua_State *s)
{
	GtkWidget *widget = NULL;

	if ( lua_gettop(s) == 1 && lua_isnumber(s, 1) )
	{
		int event = (int)lua_tonumber(s, 1);
		widget = mgtk_create_color_button((void*)mgtk_event_color, event);

		if ( event != -1 )
			mgtk_event_subscribe_gtk_widget(event, widget);
	}

	// Push widget pointer unto the lua stack.
	lua_pushlightuserdata(s, (void *)widget);

	return 1;
}


int mgtk_lua_rc_spinbutton_uint(lua_State *s)
{
	GtkWidget *widget = NULL;

	if ( lua_gettop(s) >= 4 && 
		 lua_isnumber(s, 1) && lua_isnumber(s, 2) &&
		 lua_isnumber(s, 3) && lua_isnumber(s, 4) )
	{
		int event = (int)lua_tonumber(s, 1);
		float initial = (float)lua_tonumber(s, 2);
		float min = (float)lua_tonumber(s, 3);
		float max = (float)lua_tonumber(s, 4);

		if (initial < 0) initial = 0.0f;
		if (min < 0) min = 0.0f;
		if (max < 0) max = 0.0f;

		// Optional
		float step = 1.0f;
		float page = 10.0f;
		float pageSize = 10.0f;

		if ( lua_gettop(s) == 7 )
		{
			step = (float)lua_tonumber(s, 5);
			page = (float)lua_tonumber(s, 6);
			pageSize = (float)lua_tonumber(s, 7);
		}
  
		GtkObject *spinbutton_adj = gtk_adjustment_new(initial, min, max, step, page, pageSize);
		GtkWidget *spinbutton = gtk_spin_button_new(GTK_ADJUSTMENT(spinbutton_adj), 1, 0);

		widget = spinbutton;

		/* Mongoose 2002.02.01, Add this widget to a special 
		 *   lookup table by it's event id */

		if ( event != -1 )
		{
			mgtk_event_subscribe_gtk_widget(event, spinbutton);	
			gtk_signal_connect(GTK_OBJECT(widget), "changed",
							   GTK_SIGNAL_FUNC(mgtk_event_spinbutton_uint), 
							   GINT_TO_POINTER(event) );
		}
	}

	// Push widget pointer unto the lua stack.
	lua_pushlightuserdata(s, (void *)widget);

	return 1;
}


int mgtk_lua_rc_spinbutton_int(lua_State *s)
{
	GtkWidget *widget = NULL;

	if ( lua_gettop(s) >= 4 && 
		 lua_isnumber(s, 1) && lua_isnumber(s, 2) &&
		 lua_isnumber(s, 3) && lua_isnumber(s, 4) )
	{
		int event = (int)lua_tonumber(s, 1);
		float initial = (float)lua_tonumber(s, 2);
		float min = (float)lua_tonumber(s, 3);
		float max = (float)lua_tonumber(s, 4);


		// Optional
		float step = 1.0f;
		float page = 10.0f;
		float pageSize = 10.0f;

		if ( lua_gettop(s) == 7 )
		{
			step = (float)lua_tonumber(s, 5);
			page = (float)lua_tonumber(s, 6);
			pageSize = (float)lua_tonumber(s, 7);
		}

  
		GtkObject *spinbutton_adj = gtk_adjustment_new(initial, min, max, step, page, pageSize);
		GtkWidget *spinbutton = gtk_spin_button_new(GTK_ADJUSTMENT(spinbutton_adj), 1, 0);

		widget = spinbutton;

		/* Mongoose 2002.02.01, Add this widget to a special 
		 *   lookup table by it's event id */
		if ( event != -1 )
		{
			mgtk_event_subscribe_gtk_widget(event, spinbutton);	
			gtk_signal_connect(GTK_OBJECT(widget), "changed",
							   GTK_SIGNAL_FUNC(mgtk_event_spinbutton_int), 
							   GINT_TO_POINTER(event) );
		}
	}

	// Push widget pointer unto the lua stack.
	lua_pushlightuserdata(s, (void *)widget);

	return 1;
}


int mgtk_lua_rc_spinbutton_float(lua_State *s)
{
	GtkWidget *widget = NULL;

	if ( lua_gettop(s) >= 4 && 
		 lua_isnumber(s, 1) && lua_isnumber(s, 2) &&
		 lua_isnumber(s, 3) && lua_isnumber(s, 4) )
	{
		int event = (int)lua_tonumber(s, 1);
		float initial = (float)lua_tonumber(s, 2);
		float min = (float)lua_tonumber(s, 3);
		float max = (float)lua_tonumber(s, 4);
		int digits = (int)lua_tonumber(s, 5);

		// Optional
		float step = 1.0f;
		float page = 10.0f;
		float pageSize = 10.0f;

		if ( lua_gettop(s) == 8 )
		{
			step = (float)lua_tonumber(s, 6);
			page = (float)lua_tonumber(s, 7);
			pageSize = (float)lua_tonumber(s, 8);
		}
  
		GtkObject *spinbutton_adj = gtk_adjustment_new(initial, min, max, step, page, pageSize);
		GtkWidget *spinbutton = gtk_spin_button_new(GTK_ADJUSTMENT(spinbutton_adj), 1, digits);

		widget = spinbutton;

		if (widget && event != -1)
		{
			/* Mongoose 2002.02.01, Add this widget to a special 
			 *   lookup table by it's event id */
			mgtk_event_subscribe_gtk_widget(event, widget);	
			gtk_signal_connect(GTK_OBJECT(widget), "changed",
							   GTK_SIGNAL_FUNC(mgtk_event_spinbutton_float), 
							   GINT_TO_POINTER(event) );
		}

	}

	// Push widget pointer unto the lua stack.
	lua_pushlightuserdata(s, (void *)widget);

	return 1;
}


int mgtk_lua_rc_statusbar(lua_State *s)
{
	GtkWidget *widget = gtk_statusbar_new();
	//GTK_STATUS_BAR_WIDGET = widget;

	// Push widget pointer unto the lua stack.
	lua_pushlightuserdata(s, (void *)widget);

	return 1;
}


int mgtk_lua_rc_toolbar_append(lua_State* s)
{
	if ( lua_gettop(s) >= 2 &&
		 lua_islightuserdata(s, 1) && lua_islightuserdata(s, 2) )
	{
		GtkWidget* toolbar = (GtkWidget*)lua_touserdata(s, 1);
		GtkToolItem* item = (GtkToolItem*)lua_touserdata(s, 2);

		// FIXME: If > 2 just keep appending... and check the last stack node for mode.

		int mode = -1; // Append to the end by default.
		if ( lua_gettop(s) == 3 && lua_isnumber(s, 3) )
		{
			mode = (int)lua_tonumber(s, 3);
		}

		gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, mode);
	}

	return 0;
}


int mgtk_lua_rc_toolbar_separator(lua_State* s)
{
	GtkToolItem* item = gtk_separator_tool_item_new();
	GtkWidget* widget = (GtkWidget *)item;
	gtk_widget_show(widget);

	if ( lua_gettop(s) >= 1 && lua_islightuserdata(s, 1) )
	{
		GtkWidget* toolbar = (GtkWidget *)lua_touserdata(s, 1);
		gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, -1);
	}

	lua_pushlightuserdata(s, (void *)widget);

	return 1;
}


int mgtk_lua_rc_toolbar_box(lua_State* s)
{
	GtkWidget* box = NULL;

	if ( lua_gettop(s) >= 1 && lua_islightuserdata(s, 1) )
	{
		GtkWidget* toolbar = (GtkWidget *)lua_touserdata(s, 1);
		int mode = -1; // Append to the end by default.

		if ( lua_gettop(s) == 2 && lua_isnumber(s, 2) )
		{
			mode = (int)lua_tonumber(s, 2);
		}
		
		GtkToolItem* item = gtk_tool_item_new();
		GtkWidget* widget = (GtkWidget*)item;
		gtk_toolbar_insert(GTK_TOOLBAR(toolbar), item, mode); 
		gtk_widget_show(widget);

		box = gtk_hbox_new(TRUE, 1);
		gtk_widget_ref(box);
		gtk_object_set_data_full(GTK_OBJECT(item), "box", box, (GtkDestroyNotify)gtk_widget_unref);
		gtk_container_add(GTK_CONTAINER(item), box);
		gtk_widget_show(box);
	}

	lua_pushlightuserdata(s, (void *)box);

	return 1;
}


int mgtk_lua_rc_toolbar(lua_State* s)
{
	GtkWidget* box = (GtkWidget *)lua_touserdata(s, 1);
	GtkWidget* toolbar = mgtk_create_toolbar( box );
	gtk_widget_ref(toolbar);
	gtk_object_set_data_full(GTK_OBJECT(box), "toolbar", toolbar, (GtkDestroyNotify)gtk_widget_unref);
	gtk_widget_show(toolbar);

	lua_pushlightuserdata(s, (void *)toolbar);

	return 1;
}


int mgtk_lua_rc_toolbar_togglebutton(lua_State* s)
{
	GtkWidget* toggle_button = NULL;

	if ( lua_gettop(s) == 6 )
	{
		GtkWidget* toolbar = (GtkWidget *)lua_touserdata(s, 1);
		const char* label = lua_tostring(s, 2);
		int event = (int)lua_tonumber(s, 3);
		int state = (int)lua_tonumber(s, 4);
		const char* icon = lua_tostring(s, 5);
		const char* tooltip = lua_tostring(s, 6);

		void* event_func = NULL; // No longer used // rc_gtk_event_func(event);
		
		toggle_button =
		mgtk_create_toolbar_toogle_button(toolbar, state, icon, label, tooltip, event_func, event);
		
		if ( event != -1 )
		{
			gtk_signal_connect(GTK_OBJECT(toggle_button), "toggled",
							   GTK_SIGNAL_FUNC(mgtk_tool_toggle_button_handler), 
							   GINT_TO_POINTER(event));

			/* Add this widget to a map by event id key. */
			mgtk_event_subscribe_gtk_widget(event, toggle_button);
		}
	}

	lua_pushlightuserdata(s, (void *)toggle_button);

	return 1;
}


int mgtk_lua_rc_toolbar_button(lua_State* s)
{
	GtkWidget* button = NULL;

	if ( lua_gettop(s) == 5 )
	{
		GtkWidget* toolbar = (GtkWidget *)lua_touserdata(s, 1);
		const char* label = lua_tostring(s, 2);
		int event = (int)lua_tonumber(s, 3);
		const char* icon = lua_tostring(s, 4);
		const char* tooltip = lua_tostring(s, 5);

		button = 
		mgtk_create_toolbar_button(toolbar, 0, icon, label, tooltip, (void*)mgtk_event_command, event);
	}

	lua_pushlightuserdata(s, (void *)button);

	return 1;
}


int mgtk_lua_rc_toolbar_menubutton(lua_State* s)
{
	GtkWidget* menu = NULL;

	if ( lua_gettop(s) == 5 )
	{
		GtkWidget* toolbar = (GtkWidget *)lua_touserdata(s, 1);
		const char* label = lua_tostring(s, 2);
		int event = -1;

		if ( lua_isnumber(s, 3) )
		{
			event = (int)lua_tonumber(s, 3);
			menu = gtk_menu_new();
			gtk_widget_show(menu);
		}
		else if ( lua_isstring(s, 3) )
		{
			// FIXME: Use legacy mlisp resource bindings via luastring for now.
			menu = (GtkWidget*)mlisp_recall( lua_tostring(s, 3) );
		}

		const char* icon = lua_tostring(s, 4);
		const char* tooltip = lua_tostring(s, 5);

		GtkWidget* button =
		mgtk_create_toolbar_button(toolbar, 1, icon, label, tooltip, (void*)mgtk_event_command, event);
		gtk_menu_tool_button_set_menu( GTK_MENU_TOOL_BUTTON(button), menu );
	}

	lua_pushlightuserdata(s, (void *)menu);

	return 1;
}



//////////////////////////////////////////////////////////////////////////////
// Status widgets
////////////////////////////////////////////////////////////////////////////// 

int mgtk_lua_rc_textview(lua_State *s)
{
	GtkWidget *view = gtk_text_view_new();
	GtkTextBuffer *buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));

	// Optional string argument
	if ( lua_gettop(s) == 1 && lua_isstring(s, 1) )
	{
		const char *text = lua_tostring(s, 1);
		gtk_text_buffer_set_text(buffer, text, -1);
	}

	// Push widget pointer unto the lua stack.
	lua_pushlightuserdata(s, (void *)view);

	return 1; 
}


int mgtk_lua_rc_label(lua_State *s)
{
	GtkWidget *label = NULL;

	if ( lua_gettop(s) == 3 && 
		 lua_isstring(s, 1) && lua_isnumber(s, 2) && lua_isnumber(s, 3) )
	{
		const char *text = lua_tostring(s, 1);
		float x_align = (float)lua_tonumber(s, 2); 
		float y_align = (float)lua_tonumber(s, 3);

		label = gtk_label_new(text);
		gtk_misc_set_alignment(GTK_MISC(label), x_align, y_align);
		gtk_widget_show(label);
	}

	MGTK_ASSERTMSG(label != NULL, "Invalid label.");

	// Push widget pointer unto the lua stack.
	lua_pushlightuserdata(s, (void *)label);
	return 1;
}


int mgtk_lua_rc_notebook(lua_State* s)
{
	GtkWidget* notebook = gtk_notebook_new();
	//gtk_widget_ref(notebook);
	//gtk_object_set_data_full(GTK_OBJECT(box), "notebook", notebook, (GtkDestroyNotify)gtk_widget_unref);
	gtk_widget_show(notebook);

	if ( lua_gettop(s) > 0 )
	{
		int event = (int)lua_tonumber(s, 1);

		// FIXME: Push this out as generic size event later.
		if ( lua_gettop(s) == 3 )
		{
			int width = (int)lua_tonumber(s, 2);
			int height = (int)lua_tonumber(s, 3);

			gtk_widget_set_usize(notebook, width, height);
		}

		gtk_notebook_set_scrollable(GTK_NOTEBOOK(notebook), TRUE);
		
		/* Event signal */
		gtk_signal_connect(GTK_OBJECT(notebook), "switch_page",
						   GTK_SIGNAL_FUNC(mgtk_event_notebook_switch_page),
						   GINT_TO_POINTER(event));

		/* Hook up vector data to map tab page ids to events */
		mgtk_notebook_eventmap_t* emap = new mgtk_notebook_eventmap_t;
		emap->count = 0;
		emap->events = 0x0;

		gtk_object_set_data(GTK_OBJECT(notebook), "notebook_eventmap", emap);
		gtk_signal_connect(GTK_OBJECT(notebook), "destroy",
						   GTK_SIGNAL_FUNC(mgtk_destroy_notebook), NULL);
	}

	lua_pushlightuserdata(s, (void*)notebook);

	return 1;
}


int mgtk_lua_rc_tab(lua_State* s)
{
	GtkWidget* vbox = NULL;

	if ( 1 )
	{
		GtkWidget* notebook = (GtkWidget *)lua_touserdata(s, 1);
		const char* label = lua_tostring(s, 2);
		int event = (int)lua_tonumber(s, 3);

		vbox = gtk_vbox_new(FALSE, 0);
		mgtk_notebook_eventmap_t* emap =
		(mgtk_notebook_eventmap_t*)gtk_object_get_data(GTK_OBJECT(notebook), "notebook_eventmap");

		if (!emap)
		{
			lua_pushlightuserdata(s, NULL);
			return 1;
		}

		//GtkWidget* item = 
		mgtk_create_tab(notebook, label, vbox, label, emap->count);

		/* Simple vector to map events */
		emap->count++;
		int* newEvents = new int[emap->count];

		if (emap->events)
		{
			for (unsigned int i = 0; i < emap->count; ++i)
			{
				newEvents[i] = emap->events[i];
			}

			delete [] emap->events;
		}

		newEvents[emap->count-1] = event;
		emap->events = newEvents;
	}

	lua_pushlightuserdata(s, (void*)vbox);
	return 1;
}



int mgtk_lua_func_toggle_set(lua_State *s)
{
	//symbol_enforce_type_assert(&event, INT);
	//symbol_enforce_type_assert(&val, INT);

	if ( lua_gettop(s) == 2 && 
		 lua_isnumber(s, 1) && lua_isnumber(s, 2) )
	{
		int event = (int)lua_tonumber(s, 1);
		int value = (int)lua_tonumber(s, 2);

		mgtk_toggle_value_set( event, value );
	}

	return 0;
}


int mgtk_lua_window_move(lua_State* s)
{
	GtkWidget* window = (GtkWidget*)lua_touserdata(s, 1);
	int x = (int)lua_tonumber(s, 2);
	int y = (int)lua_tonumber(s, 3);

	if ( window )
	{
		gtk_window_move(GTK_WINDOW(window), x, y);
	}

	return 0;
}


#endif // LUAWRAPPER_FOUND

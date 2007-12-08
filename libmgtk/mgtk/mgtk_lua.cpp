/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Mgtk
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2007 Mongoose
 * Comments: mgtk Lua language binding.
 *
 * 
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.08.18:
 * Mongoose - Created
 ==========================================================================*/

#ifdef LUAWRAPPER_FOUND

#define EXPOSE_TIMESLIDER_GTK 1

#include <gtk/gtk.h>

#include "lua/Lua.h"

#include "Resource.h"        /* Hook into mlisp symbol table and event map for compatibility. */
#include "ResourceEvent.h"
#include "ConfirmationDialog.h"
#include "QueryDialog.h"

#include "mgtk_callbacks.h"
#include "mgtk_events.h"
#include "mgtk_opengl_canvas.h"
#include "mgtk_interface.h"
#include "mgtk_filedialog.h"
#include "mgtk_resource.h"
#include "mgtk_timeslider.h"
#include "mgtk_tree.h"

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

	lua.RegisterFunction("mgtk_icon", mgtk_lua_rc_icon);

	/* Custom widgets */
	lua.RegisterFunction("mgtk_opengl_canvas", mgtk_lua_rc_opengl_canvas);
	lua.RegisterFunction( "mgtk_time_slider", mgtk_lua_rc_time_slider );
	lua.RegisterFunction( "mgtk_range_slider", mgtk_lua_rc_range_slider );

	lua.RegisterFunction("mgtk_menu_item", mgtk_lua_rc_menu_item);
	lua.RegisterFunction("mgtk_menu_item_check", mgtk_lua_rc_menu_item_check);
	lua.RegisterFunction("mgtk_append_menu", mgtk_lua_rc_append_menu);
	lua.RegisterFunction("mgtk_menubar", mgtk_lua_rc_menubar);
	lua.RegisterFunction("mgtk_submenu", mgtk_lua_rc_submenu);
	lua.RegisterFunction("mgtk_menu_separator", mgtk_lua_rc_menu_separator);
	lua.RegisterFunction("mgtk_optionmenu", mgtk_lua_rc_optionmenu);

	lua.RegisterFunction("mgtk_filechooserbutton", mgtk_lua_rc_filechooserbutton);

	lua.RegisterFunction("mgtk_hslider", mgtk_lua_rc_hslider);

	lua.RegisterFunction("mgtk_textbox", mgtk_lua_rc_textbox);

	lua.RegisterFunction("mgtk_button", mgtk_lua_rc_button);
	lua.RegisterFunction("mgtk_togglebutton", mgtk_lua_rc_togglebutton);
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

	// Dialogs
	lua.RegisterFunction( "mgtk_dialog", mgtk_lua_dialog );
	lua.RegisterFunction( "mgtk_confirmation_dialog", mgtk_lua_confirmation_dialog );
	lua.RegisterFunction( "mgtk_query_dialog", mgtk_lua_query_dialog );

	// Utiliy functions.
	lua.RegisterFunction("mgtk_boolean_set", mgtk_lua_boolean_set);
	lua.RegisterFunction("mgtk_color_set", mgtk_lua_color_set);
	lua.RegisterFunction("mgtk_integer_set", mgtk_lua_integer_set);
	lua.RegisterFunction("mgtk_float_set", mgtk_lua_float_set);
	lua.RegisterFunction("mgtk_string_set", mgtk_lua_string_set);

	lua.RegisterFunction( "mgtk_window_move", mgtk_lua_window_move );
	lua.RegisterFunction( "mgtk_optionmenu_set_by_id", mgtk_lua_optionmenu_set_by_id );
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

		// 1. Check to see if this script is already bound to this symbol.
		id = ResourceEvent::GetResourceIdBySymbol( symbol );

		// 2. If not bind it.
		if ( id == -1 )
		{
#if FIXME
			const char* func = lua_tostring(s, 2);

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
	
	// FIXME: This needs to hook into a 'refresh/display' callback separately now.
	// Old mlisp hook-up for testing.
	extern GtkWidget* GTK_MAIN_WINDOW;
	GTK_MAIN_WINDOW = window;

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

	// Must show before opengl_canvas is created
	gtk_widget_show(window);

	return 1;
}


int mgtk_lua_rc_icon(lua_State* s)
{
	GtkWidget* icon = NULL;
	
	if ( lua_gettop(s) == 2 && lua_isstring(s, 1) )
	{
		GtkIconSize sz = GTK_ICON_SIZE_DIALOG;
		int id = ( lua_isnumber(s, 2) ? (int)lua_tonumber(s, 2) :
					  lua_isstring(s, 2) ? mgtk_lua_get_id( lua_tostring(s, 2) ): -1 );

		switch ( id )
		{
		case 1:
			sz = GTK_ICON_SIZE_MENU; 
			break;

		case 2:
			sz = GTK_ICON_SIZE_SMALL_TOOLBAR; 
			break;

		case 3:
			sz = GTK_ICON_SIZE_LARGE_TOOLBAR; 
			break;

		case 4:
			sz = GTK_ICON_SIZE_BUTTON; 
			break;

		case 5:
			sz = GTK_ICON_SIZE_DIALOG; 
			break;

		default:
			;
		}

		icon = mgtk_create_icon( lua_tostring(s, 1), sz );
		gtk_widget_show( icon );
	}

	// Push widget pointer unto the lua stack.
	lua_pushlightuserdata(s, (void *)icon);

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
		//MGTK_ASSERTMSG(widget != NULL, "Invalid widget.");

		//if ( widget || !box )
		//	return 0;

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

		if ( GTK_IS_BOX(box) )
			 gtk_box_pack_start(GTK_BOX(box), widget, FALSE, FALSE, 0 );//TRUE, TRUE, 0);
		else
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
			mlisp_new_string(&symbol, (char *)name);
			mlisp_new_adt(&sealing_jutsu, ARG_GTK_BOX_WIDGET, (void *)vbox);
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
	if ( lua_gettop(s) >= 2 && 
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

	GtkWidget* canvas = mgtk_opengl_canvas_new(width, height);

	if ( canvas )
	{
		gpointer gobj = gtk_object_get_data(GTK_OBJECT(canvas), "gl_window_state");
		mgtk_opengl_canvas_state_t* state = (mgtk_opengl_canvas_state_t*)gobj;
		state->appbar = NULL;

		// FIXME: This needs to hook into a 'refresh/display' callback separately now.
		// Old mlisp hook-up for testing.
		extern GtkWidget* GTK_GL_AREA_WIDGET;
		GTK_GL_AREA_WIDGET = canvas;
		
		
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


int mgtk_lua_rc_hslider(lua_State *s)
{
	GtkWidget* slider = NULL;

	if ( lua_gettop(s) == 3 )
	{
		int event = ( lua_isnumber(s, 1) ? (int)lua_tonumber(s, 1) :
					  lua_isstring(s, 1) ? mgtk_lua_get_id( lua_tostring(s, 1) ): -1 );
		int min = (int)lua_tonumber(s, 2);
		int max = (int)lua_tonumber(s, 3);

		GtkObject* adj = gtk_adjustment_new( min, min, max, 1, 1, 10 );
		slider = gtk_hscale_new( GTK_ADJUSTMENT(adj) );
		gtk_widget_show(slider);
		gtk_scale_set_digits( GTK_SCALE(slider), 0);
		gtk_widget_ref(slider);

		gtk_signal_connect( GTK_OBJECT(adj), "value_changed",
							GTK_SIGNAL_FUNC(mgtk_event_slider1u), 
							GINT_TO_POINTER(event) );
	}

	lua_pushlightuserdata(s, slider);
	return 1;
}


int mgtk_lua_rc_textbox(lua_State* s)
{
	GtkWidget* textbox = NULL;

	{
		textbox = gtk_entry_new();
		gtk_widget_show( textbox );

		int event = ( lua_isnumber(s, 1) ? (int)lua_tonumber(s, 1) :
					  lua_isstring(s, 1) ? mgtk_lua_get_id( lua_tostring(s, 1) ): -1 );

		gtk_signal_connect( GTK_OBJECT( textbox ), "changed",
						    GTK_SIGNAL_FUNC( mgtk_event_text ),
						    GINT_TO_POINTER( event ) );

		// Mongoose 2002.02.14, Add this widget to a special 
		//   lookup table by it's event id
		//index_add_gtk_widget(get_int(event), item);
		mgtk_event_subscribe_gtk_widget( event, textbox );
	}

	lua_pushlightuserdata( s, textbox );
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

	if ( lua_gettop(s) > 2 &&
		 lua_isstring(s, 2) && lua_isnumber(s, 3) || lua_isstring(s, 3) )
	{
		label = lua_tostring(s, 2);
		id = ( lua_isnumber(s, 3) ? (int)lua_tonumber(s, 3) :
			   lua_isstring(s, 3) ? mgtk_lua_get_id( lua_tostring(s, 3) ): -1 );
	}

	GtkWidget* optionmenu = gtk_option_menu_new();
	gtk_widget_ref(optionmenu);
	gtk_widget_show(optionmenu);

	GtkWidget* optionmenu_menu = gtk_menu_new();
	gtk_option_menu_set_menu(GTK_OPTION_MENU(optionmenu), optionmenu_menu);
	gtk_widget_show(optionmenu_menu);

	gtk_container_add( GTK_CONTAINER(lua_touserdata(s, 1)), optionmenu );

	if ( id != -1)
	{
		mgtk_event_subscribe_gtk_widget( id, optionmenu );
	}

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
			mlisp_new_string(&symbol_arg, (char *)symbol);
			mlisp_new_adt(&binding, ARG_GTK_MENU_WIDGET, (void *)submenu);
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
		if ( lua_gettop(s) > 2 && lua_isnumber(s, 3) )
		{
			int subevent = (int)lua_tonumber(s, 3);
			gtk_object_set_data(GTK_OBJECT(item), "mlisp_event", 
								GINT_TO_POINTER(subevent));
			gtk_signal_connect(GTK_OBJECT(item), "activate",
							   GTK_SIGNAL_FUNC(mgtk_event_dual_command), 
							   GINT_TO_POINTER(id));
		}
		else
		{
			// Mongoose 2002.02.14, Add this widget to a special 
			//   lookup table by it's event id
			mgtk_event_subscribe_gtk_widget(id, item);

			gtk_signal_connect(GTK_OBJECT(item), "activate",
							   GTK_SIGNAL_FUNC(mgtk_event_command), 
							   GINT_TO_POINTER(id) );
		}
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


int mgtk_lua_rc_menu_item_check(lua_State *s)
{
	const char* label = NULL;
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

	GtkWidget *item = gtk_check_menu_item_new_with_mnemonic( label );
	gtk_widget_show(item);

	if ( lua_gettop(s) > 2 && lua_isnumber(s, 3) )
	{
		int value = (int)lua_tonumber(s, 3);

		if ( value )
		{
			gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(item), TRUE);
		}
	}

	//const char* accel = NULL;
	//if ( accel )
	//{
	//	mgtk_accel_support(item, accel);
	//}

	if (id != -1)
	{
		// Mongoose 2002.02.14, Add this widget to a special 
		//   lookup table by it's event id
		mgtk_event_subscribe_gtk_widget(id, item);

		gtk_signal_connect(GTK_OBJECT(item), "toggled",
						   GTK_SIGNAL_FUNC(mgtk_check_menu_item_handler), 
						   GINT_TO_POINTER(id) );
	}

	// Push widget pointer unto the lua stack.
	lua_pushlightuserdata(s, (void *)item);

	return 1;
}


int mgtk_lua_rc_togglebutton(lua_State *s)
{
	GtkWidget* button = NULL;

	if ( 1 )
	{
		button = gtk_toggle_button_new_with_label( lua_tostring(s, 1) );
		gtk_widget_show(button);
		gtk_widget_ref(button);

		int event = ( lua_isnumber(s, 2) ? (int)lua_tonumber(s, 2) :
					  lua_isstring(s, 2) ? mgtk_lua_get_id( lua_tostring(s, 2) ): -1 );
		int event2 = ( (lua_gettop(s) == 3) ? (int)lua_tonumber(s, 3) : -1 );

		if ( event2 != -1 )
		{
			gtk_object_set_data(GTK_OBJECT(button), "mlisp_event", 
								GINT_TO_POINTER(event2));

			gtk_signal_connect(GTK_OBJECT(button), "toggled",
							   GTK_SIGNAL_FUNC(mgtk_tool_toggle_button_dual_handler), 
							   GINT_TO_POINTER(event));
		}
		else
		{
			gtk_signal_connect(GTK_OBJECT(button), "toggled",
							   GTK_SIGNAL_FUNC(mgtk_tool_toggle_button_handler), 
							   GINT_TO_POINTER(event));
		}

		// Mongoose 2002.02.01, Add this widget to a special 
		//   lookup table by it's event id
		//mgtk_event_subscribe_gtk_widget(event, button);
	}

	lua_pushlightuserdata(s, (void *)button);
	return 1;
}


int mgtk_lua_rc_filechooserbutton(lua_State *s)
{
	GtkWidget* button = NULL;
	
	// FIXME: Asserts, checking...
	{
		const char* label = lua_tostring(s, 1);
		const char* title = lua_tostring(s, 1);
		int event = ( lua_isnumber(s, 3) ? (int)lua_tonumber(s, 3) :
					  lua_isstring(s, 3) ? mgtk_lua_get_id( lua_tostring(s, 3) ): -1 );	
		const char* pattern_func = lua_tostring(s, 4);

		mgtk_link_filechooser_from_rc( event, title, pattern_func );

		button = gtk_button_new_with_label(label);
		gtk_widget_show(button);

		gtk_signal_connect( GTK_OBJECT(button), "pressed",
							GTK_SIGNAL_FUNC( mgtk_filechooser_spawn_event ), 
							GINT_TO_POINTER(event) );
	}

	lua_pushlightuserdata(s, (void *)button);
	return 1;
}


int mgtk_lua_rc_button(lua_State *s)
{
	// FIXME: Asserts, checking...
	const char *label = lua_tostring(s, 1);
	//int event = (int)lua_tonumber(s, 2);
	int event = ( lua_isnumber(s, 2) ? (int)lua_tonumber(s, 2) :
				  lua_isstring(s, 2) ? mgtk_lua_get_id( lua_tostring(s, 2) ): -1 );


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

	if ( lua_gettop(s) == 1 && lua_isnumber(s, 1) || lua_isstring(s, 1) )
	{
		int event = ( lua_isnumber(s, 1) ? (int)lua_tonumber(s, 1) :
					  lua_isstring(s, 1) ? mgtk_lua_get_id( lua_tostring(s, 1) ): -1 );
		//int event = (int)lua_tonumber(s, 1);
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
		 lua_isnumber(s, 1) || lua_isstring(s, 1) && 
		 lua_isnumber(s, 2) && lua_isnumber(s, 3) && lua_isnumber(s, 4) )
	{

		int event = ( lua_isnumber(s, 1) ? (int)lua_tonumber(s, 1) :
					  lua_isstring(s, 1) ? mgtk_lua_get_id( lua_tostring(s, 1) ): -1 );
//		int event = (int)lua_tonumber(s, 1);
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
		 lua_isnumber(s, 1) || lua_isstring(s, 1) && lua_isnumber(s, 2) &&
		 lua_isnumber(s, 3) && lua_isnumber(s, 4) )
	{

		int event = ( lua_isnumber(s, 1) ? (int)lua_tonumber(s, 1) :
					  lua_isstring(s, 1) ? mgtk_lua_get_id( lua_tostring(s, 1) ): -1 );
		//		int event = (int)lua_tonumber(s, 1);
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
		 lua_isnumber(s, 1) || lua_isstring(s, 1) && lua_isnumber(s, 2) &&
		 lua_isnumber(s, 3) && lua_isnumber(s, 4) )
	{

		int event = ( lua_isnumber(s, 1) ? (int)lua_tonumber(s, 1) :
					  lua_isstring(s, 1) ? mgtk_lua_get_id( lua_tostring(s, 1) ): -1 );
		//		int event = (int)lua_tonumber(s, 1);
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
	
	// FIXME: Using legacy pointers.
	extern GtkWidget* GTK_STATUS_BAR_WIDGET;
	GTK_STATUS_BAR_WIDGET = widget;

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

		box = gtk_hbox_new(FALSE, 1);
		gtk_widget_show(box);

		gtk_widget_ref(box);
		gtk_object_set_data_full(GTK_OBJECT(item), "box", box, (GtkDestroyNotify)gtk_widget_unref);

		gtk_container_add(GTK_CONTAINER(item), box);
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

	if ( lua_gettop(s) == 2 )
	{
		int horz = (int)lua_tonumber( s, 2 );
		gtk_toolbar_set_orientation( GTK_TOOLBAR(toolbar), 
									 horz ? GTK_ORIENTATION_HORIZONTAL : GTK_ORIENTATION_VERTICAL );
	}

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
		int event = ( lua_isnumber(s, 3) ? (int)lua_tonumber(s, 3) :
					  lua_isstring(s, 3) ? mgtk_lua_get_id( lua_tostring(s, 3) ): -1 );
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
		int event = ( lua_isnumber(s, 3) ? (int)lua_tonumber(s, 3) :
					  lua_isstring(s, 3) ? mgtk_lua_get_id( lua_tostring(s, 3) ): -1 );
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

	if ( lua_gettop(s) > 4 )
	{
		GtkWidget* toolbar = (GtkWidget *)lua_touserdata(s, 1);
		const char* label = lua_tostring(s, 2);
		int event = ( lua_isnumber(s, 3) ? (int)lua_tonumber(s, 3) :
					  lua_isstring(s, 3) ? mgtk_lua_get_id( lua_tostring(s, 3) ): -1 );

		if ( lua_gettop(s) > 5 && lua_isstring(s, 6) )
		{
			// FIXME: Use legacy mlisp resource bindings via luastring for now.
			menu = (GtkWidget*)mlisp_recall( lua_tostring(s, 6) );
			mgtk_print("*** %p\n", menu);
		}

		if ( menu == NULL )
		{
			menu = gtk_menu_new();
			gtk_widget_show(menu);
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

	if ( lua_gettop(s) > 0 && lua_isstring(s, 1))
	{
		const char *text = lua_tostring(s, 1);
		label = gtk_label_new(text);
		gtk_widget_show(label);
	}

	if ( lua_gettop(s) > 1 && 
		 lua_isnumber(s, 2) && lua_isnumber(s, 3) )
	{
		float x_align = (float)lua_tonumber(s, 2); 
		float y_align = (float)lua_tonumber(s, 3);
		gtk_misc_set_alignment(GTK_MISC(label), x_align, y_align);
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
		int event = ( lua_isnumber(s, 1) ? (int)lua_tonumber(s, 1) :
					  lua_isstring(s, 1) ? mgtk_lua_get_id( lua_tostring(s, 1) ): -1 );

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

		int event = ( lua_isnumber(s, 3) ? (int)lua_tonumber(s, 3) :
					  lua_isstring(s, 3) ? mgtk_lua_get_id( lua_tostring(s, 3) ): -1 );
		//				int event = (int)lua_tonumber(s, 3);

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


extern void mgtk_tree_set_treeview(int event, GtkTreeView *tree);
extern void mgtk_tree_cell_edited_callback (GtkCellRendererText *cell,
											gchar               *path_string,
											gchar               *new_text,
											gpointer             userdata);
extern gboolean mgtk_GtkTreeSelectionFunc(GtkTreeSelection *selection,
										  GtkTreeModel *model,
										  GtkTreePath *path,
										  gboolean path_currently_selected,
										  gpointer data);
extern void mgtk_treeview_onRowActivated(GtkTreeView *treeview,
										 GtkTreePath        *path,
										 GtkTreeViewColumn  *col,
										 gpointer            userdata);

int mgtk_lua_rc_tree(lua_State* s)
{
	const char* label = lua_tostring(s, 1);
	int event = ( lua_isnumber(s, 2) ? (int)lua_tonumber(s, 2) :
				  lua_isstring(s, 2) ? mgtk_lua_get_id( lua_tostring(s, 2) ): -1 );
	int event2 = ( lua_isnumber(s, 3) ? (int)lua_tonumber(s, 3) :
				   lua_isstring(s, 3) ? mgtk_lua_get_id( lua_tostring(s, 3) ): -1 );

	GtkWidget* view = gtk_tree_view_new();	
	mgtk_tree_set_treeview( event, GTK_TREE_VIEW(view) );
	g_signal_connect( view, "row-activated", (GCallback)mgtk_treeview_onRowActivated, GINT_TO_POINTER(event) );

	GtkTreeSelection *selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(view));
	if ( selection )
	{
		gtk_tree_selection_set_mode(selection, GTK_SELECTION_BROWSE);
		gtk_tree_selection_set_select_function(selection, mgtk_GtkTreeSelectionFunc, GINT_TO_POINTER(event), NULL);
	}

	/* Column "Name" */
	GtkTreeViewColumn* col = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(col, label);
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
	GtkCellRenderer* renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_add_attribute(col, renderer, "text", NAME_COLUMN);
	g_object_set(renderer, "editable", TRUE, NULL);
	g_signal_connect( renderer, "edited", (GCallback)mgtk_tree_cell_edited_callback, GINT_TO_POINTER(event2) );

	/* Column "Event" */
	col = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(col, "Event");
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_add_attribute(col, renderer, "text", EVENT_COLUMN);

	/* Column "Id" */
	col = gtk_tree_view_column_new();
	gtk_tree_view_column_set_title(col, "Id");
	gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);
	renderer = gtk_cell_renderer_text_new();
	gtk_tree_view_column_pack_start(col, renderer, TRUE);
	gtk_tree_view_column_add_attribute(col, renderer, "text", ID_COLUMN);

	/* Append dummy node(s) */
	GtkTreeStore* store = gtk_tree_store_new(N_COLUMNS,       /* Total number of cols */
											 G_TYPE_STRING,   /* Node name */
											 G_TYPE_INT,      /* Node event */
											 G_TYPE_INT);     /* Node id */
	GtkTreeModel* model = GTK_TREE_MODEL(store);

	/* Add view to container */
	gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
	g_object_unref(model); 
	gtk_tree_selection_set_mode(gtk_tree_view_get_selection(GTK_TREE_VIEW(view)), GTK_SELECTION_NONE);
	gtk_widget_show(view);


	/* Tree inside scrolled window, instead of box. */
	GtkWidget* scroll = gtk_scrolled_window_new(NULL, NULL);
	gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
								   GTK_POLICY_AUTOMATIC,
								   GTK_POLICY_AUTOMATIC);
	gtk_widget_show(scroll);
	//gtk_widget_set_usize(scroll, 100, 250);
	gtk_container_add(GTK_CONTAINER(scroll), view);

	lua_pushlightuserdata(s, scroll);

	return 1;
}


int mgtk_lua_dialog(lua_State *s)
{
	GtkWidget* dialog = NULL;

	if ( lua_gettop(s) == 3 )
	{
		dialog = mgtk_create_window( (char*)lua_tostring(s, 1), "", "" );

		// Don't show by default, since these are 'cached/hidden' dialogs.
		//gtk_widget_show(dialog);

		// FIXME: Using legacy pointer
		extern GtkWidget* GTK_MAIN_WINDOW;
		gtk_window_set_transient_for( GTK_WINDOW(dialog), GTK_WINDOW(GTK_MAIN_WINDOW) );

		g_signal_connect( GTK_OBJECT(dialog), "delete_event",
						  G_CALLBACK(mgtk_event_hide_dialog),
						  GINT_TO_POINTER(0) );

		/* Mongoose 2002.02.01, Add this widget to a special
		 *   lookup table by it's event id */
		int open_id = ( lua_isnumber(s, 2) ? (int)lua_tonumber(s, 2) :
						lua_isstring(s, 2) ? mgtk_lua_get_id( lua_tostring(s, 2) ): -1 );
		mgtk_event_subscribe_gtk_widget( open_id, dialog );

		// FIXME: Not sure this is required anymore -- leaving it here for future review.
		//        'Bug for bug' compatibility is more important right now.
		int close_id = ( lua_isnumber(s, 3) ? (int)lua_tonumber(s, 3) :
						 lua_isstring(s, 3) ? mgtk_lua_get_id( lua_tostring(s, 3) ): -1 );
		mgtk_event_subscribe_gtk_widget( close_id, dialog );

		GtkWidget* vbox = gtk_vbox_new( FALSE, 0 );
		gtk_container_add( GTK_CONTAINER(dialog), vbox );
		gtk_widget_ref( vbox );
		gtk_widget_show( vbox );

		lua_pushlightuserdata( s, vbox );
	}
	else
	{
		lua_pushlightuserdata(s, dialog);
	}

	return 1;
}


int mgtk_lua_confirmation_dialog(lua_State *s)
{
	ConfirmationDialog d;
	d.mName = lua_tostring(s, 1);
	d.mDialogIcon = lua_tostring(s, 2);
	d.mInformationMessage = lua_tostring(s, 3);
	d.mQuestionMessage = lua_tostring(s, 4);
	d.mCancelIcon = lua_tostring(s, 5);
	d.mCancelText = lua_tostring(s, 6);
	d.mAcceptIcon = lua_tostring(s, 7);
	d.mAcceptText = lua_tostring(s, 8);
	d.AddToPool();

	return 0;
}


int mgtk_lua_query_dialog(lua_State *s)
{
	QueryDialog d;

	d.mName = lua_tostring(s, 1);
	d.mDialogIcon = lua_tostring(s, 2);
	d.mInformationMessage = lua_tostring(s, 3); 

	for (unsigned int i = 4, n = lua_gettop(s); i < n; )
	{
		mstl::String symbol = lua_tostring(s, i++);

		if ( symbol == "float" )
		{
			const char* symbol = lua_tostring(s, i++);
			const char* question = lua_tostring(s, i++);
			float value = (float)lua_tonumber(s, i++);
			float min = (float)lua_tonumber(s, i++);
			float max = (float)lua_tonumber(s, i++);
			QueryDialogValue<float> v(symbol, question, value);
			d.mFloats.push_back(v);
		}
		else if ( symbol == "int" )
		{
			const char* symbol = lua_tostring(s, i++);
			const char* question = lua_tostring(s, i++);
			int value = (int)lua_tonumber(s, i++);
			int min = (int)lua_tonumber(s, i++);
			int max = (int)lua_tonumber(s, i++);
			QueryDialogValue<int> v(symbol, question, value);
			d.mInts.push_back(v);
		}
		else if ( symbol == "textarea" )
		{
			const char* symbol = lua_tostring(s, i++);
			const char* question = lua_tostring(s, i++);
			const char* value = lua_tostring(s, i++);
			QueryDialogValue<mstl::String> v(symbol, question, mstl::String(value));
			d.mTextAreaStrings.push_back(v);
		}
		else if ( symbol == "textentry" )
		{
			const char *symbol = lua_tostring(s, i++);
			const char *question = lua_tostring(s, i++);
			const char *value = lua_tostring(s, i++);
			QueryDialogValue<mstl::String> v(symbol, question, mstl::String(value));
			d.mTextEntryStrings.push_back(v);

		}
		else 
		{
			d.mCancelIcon = symbol.c_str();
			d.mCancelText = lua_tostring(s, i++);
			d.mAcceptIcon = lua_tostring(s, i++);
			d.mAcceptText = lua_tostring(s, i++);
			break;
		}
	}

	d.AddToPool();

	//lua_pushlightuserdata(s, NULL);
	return 0;
}


int mgtk_lua_rc_time_slider(lua_State* s)
{
	GtkWidget* drawing_area = gtk_drawing_area_new();
	gtk_widget_show( drawing_area );

	if ( lua_gettop(s) > 0 )
	{
		int event = ( lua_isnumber(s, 1) ? (int)lua_tonumber(s, 1) :
					  lua_isstring(s, 1) ? mgtk_lua_get_id( lua_tostring(s, 1) ): -1 );

		if ( lua_gettop(s) > 2 )
		{
			int width = (int)lua_tonumber(s, 2);
			int height = (int)lua_tonumber(s, 3);

			gtk_widget_set_size_request( drawing_area, width, height );
		}

		gtk_widget_add_events( drawing_area,
							   GDK_EXPOSURE_MASK |
							   GDK_BUTTON_MOTION_MASK | 
							   GDK_BUTTON_PRESS_MASK |
							   GDK_BUTTON_RELEASE_MASK );

		g_signal_connect( G_OBJECT( drawing_area ), "expose_event",  
						  G_CALLBACK(mgtk_time_slider_expose_handler), NULL );

		g_signal_connect( G_OBJECT( drawing_area ), "motion_notify_event",
						  G_CALLBACK(mgtk_time_slider_motion_handler), NULL ); 

		g_signal_connect( G_OBJECT( drawing_area ), "button_press_event",
						  G_CALLBACK(mgtk_time_slider_button_press_handler), NULL );

		mgtk_time_slider_state_t* state = mgtk_time_slider_state_new( event, 0, 105 );
		gtk_object_set_data( GTK_OBJECT( drawing_area ), "mgtk_time_slider_state", state );
	}

	lua_pushlightuserdata(s, drawing_area);

	return 1;
}


typedef struct {

	int event;

	unsigned int min;
	unsigned int max;

	unsigned int start;
	unsigned int end;

} mgtk_range_slider_state_t;


mgtk_range_slider_state_t* mgtk_range_slider_state_new( int event, 
														unsigned int min, unsigned int max, 
														unsigned int start, unsigned int end )
{
	mgtk_range_slider_state_t* state = new mgtk_range_slider_state_t;
	state->event = event;
	state->min = min;
	state->max = max;
	state->start = start;
	state->end = end;

	return state;
}


gboolean mgtk_range_slider_expose_handler(GtkWidget *widget, GdkEventExpose *event, gpointer data)
{
	GdkGC* gc = widget->style->fg_gc[GTK_WIDGET_STATE(widget)];
	const unsigned int width = widget->allocation.width;
	const unsigned int height = widget->allocation.height;

	/* Slider background trough. */
	gtk_paint_slider( widget->style,
					  widget->window,
					  GTK_STATE_ACTIVE,
					  GTK_SHADOW_IN,
                      NULL,
					  widget,
                      NULL, // const gchar *detail,
					  0, (height>>2)+(height>>3),
					  width, height>>2,
					  GTK_ORIENTATION_HORIZONTAL);


	/* Range slider's state based widget componets. */
	mgtk_range_slider_state_t* state =
	(mgtk_range_slider_state_t*)gtk_object_get_data(GTK_OBJECT(widget), "mgtk_range_slider_state");

	if ( state )
	{
		//state->width = width;
		const float end_v = 1.0f / ( state->max - state->min );

		/* Render a slider with a handle grip at current_key. */
		int x = ( state->start - state->min ) * end_v * width;
		int x2 = ( state->end - state->min ) * end_v * width;
		gtk_paint_box( widget->style,
					   widget->window,
					   true ? GTK_STATE_ACTIVE : GTK_STATE_INSENSITIVE,
					   GTK_SHADOW_OUT,
					   NULL, GTK_WIDGET(widget), 
					   "buttondefault",
					   x, 2, x2 - x, height - 4 );

		gtk_paint_handle( widget->style,
						  widget->window,
						  GTK_STATE_ACTIVE,
						  GTK_SHADOW_OUT,
						  NULL,
						  widget,
                          "stepper",
						  x+1, 3,
						  8, height - 6,
						  GTK_ORIENTATION_HORIZONTAL );

		gtk_paint_handle( widget->style,
						  widget->window,
						  GTK_STATE_ACTIVE,
						  GTK_SHADOW_OUT,
						  NULL,
						  widget,
                          "stepper",
						  x2-9, 3,
						  8, height - 6,
						  GTK_ORIENTATION_HORIZONTAL );
		//mgtk_print("%i", key);

		/* Ranged slider handle. */
		char s[16];
		snprintf( s, 16, "%i", state->start );
		gtk_paint_string( widget->style,
						  widget->window,
						  GTK_STATE_ACTIVE,
						  NULL,
						  widget,
						  NULL, //const gchar *detail,
						  x+10, (height>>1)+(height>>2),
						  s);

		snprintf( s, 16, "%i", state->end );
		gtk_paint_string( widget->style,
						  widget->window,
						  GTK_STATE_ACTIVE,
						  NULL,
						  widget,
						  NULL, //const gchar *detail,
						  x2-26, (height>>1)+(height>>2),
						  s);
	}

	return TRUE;
}


int mgtk_lua_rc_range_slider(lua_State* s)
{
	GtkWidget* drawing_area = gtk_drawing_area_new();
	gtk_widget_show( drawing_area );

	if ( lua_gettop(s) > 0 )
	{
		int event = ( lua_isnumber(s, 1) ? (int)lua_tonumber(s, 1) :
					  lua_isstring(s, 1) ? mgtk_lua_get_id( lua_tostring(s, 1) ): -1 );

		if ( lua_gettop(s) > 2 )
		{
			int width = (int)lua_tonumber(s, 2);
			int height = (int)lua_tonumber(s, 3);

			gtk_widget_set_size_request( drawing_area, width, height );
		}

		gtk_widget_add_events( drawing_area,
							   GDK_EXPOSURE_MASK |
							   GDK_BUTTON_MOTION_MASK | 
							   GDK_BUTTON_PRESS_MASK |
							   GDK_BUTTON_RELEASE_MASK );

		g_signal_connect( G_OBJECT( drawing_area ), "expose_event",  
						  G_CALLBACK(mgtk_range_slider_expose_handler), NULL );

		//g_signal_connect( G_OBJECT( drawing_area ), "motion_notify_event",
		//				  G_CALLBACK(mgtk_time_slider_motion_handler), NULL ); 

		//g_signal_connect( G_OBJECT( drawing_area ), "button_press_event",
		//				  G_CALLBACK(mgtk_time_slider_button_press_handler), NULL );

		mgtk_range_slider_state_t* state = mgtk_range_slider_state_new( event, 0, 500, 0, 150 );
		gtk_object_set_data( GTK_OBJECT( drawing_area ), "mgtk_range_slider_state", state );
	}

	lua_pushlightuserdata(s, drawing_area);

	return 1;
}


int mgtk_lua_boolean_set(lua_State *s)
{
	if ( lua_gettop(s) == 2 && 
		 lua_isnumber(s, 1) || lua_isstring(s, 1) && 
		 lua_isnumber(s, 2) )
	{
		int event = ( lua_isnumber(s, 1) ? (int)lua_tonumber(s, 1) :
					  lua_isstring(s, 1) ? mgtk_lua_get_id( lua_tostring(s, 1) ): -1 );

		mgtk_toggle_value_set( event, (int)lua_tonumber(s, 2) );
	}

	return 0;
}


int mgtk_lua_color_set(lua_State *s)
{
	if ( lua_gettop(s) == 5 && 
		 lua_isnumber(s, 1) || lua_isstring(s, 1) && 
		 lua_isnumber(s, 2) && lua_isnumber(s, 3) && lua_isnumber(s, 4)&& lua_isnumber(s, 5) )
	{
		int event = ( lua_isnumber(s, 1) ? (int)lua_tonumber(s, 1) :
					  lua_isstring(s, 1) ? mgtk_lua_get_id( lua_tostring(s, 1) ): -1 );

		mgtk_event_set_color( event, 
							  (float)lua_tonumber(s, 2), 
							  (float)lua_tonumber(s, 3), 
							  (float)lua_tonumber(s, 4), 
							  (float)lua_tonumber(s, 5) );
	}

	return 0;
}


int mgtk_lua_integer_set(lua_State *s)
{
	if ( lua_gettop(s) == 2 && 
		 lua_isnumber(s, 1) || lua_isstring(s, 1) && 
		 lua_isnumber(s, 2) )
	{
		int event = ( lua_isnumber(s, 1) ? (int)lua_tonumber(s, 1) :
					  lua_isstring(s, 1) ? mgtk_lua_get_id( lua_tostring(s, 1) ): -1 );

		//mgtk_toggle_value_set( event, (int)lua_tonumber(s, 2) );
	}

	return 0;
}


int mgtk_lua_float_set(lua_State *s)
{
	if ( lua_gettop(s) == 2 && 
		 lua_isnumber(s, 1) || lua_isstring(s, 1) && 
		 lua_isnumber(s, 2) )
	{
		int event = ( lua_isnumber(s, 1) ? (int)lua_tonumber(s, 1) :
					  lua_isstring(s, 1) ? mgtk_lua_get_id( lua_tostring(s, 1) ): -1 );

		//mgtk_toggle_value_set( event, (float)lua_tonumber(s, 2) );
	}

	return 0;
}


int mgtk_lua_string_set(lua_State *s)
{
	if ( lua_gettop(s) == 2 && 
		 lua_isstring(s, 1) && lua_isstring(s, 2) )
	{
		//int event = ( lua_isnumber(s, 1) ? (int)lua_tonumber(s, 1) :
		//			  lua_isstring(s, 1) ? mgtk_lua_get_id( lua_tostring(s, 1) ): -1 );

		mlisp_bind_string( lua_tostring(s, 1), lua_tostring(s, 2) );
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


int mgtk_lua_optionmenu_set_by_id(lua_State* s)
{
	int id = ( lua_isnumber(s, 1) ? (int)lua_tonumber(s, 1) :
			   lua_isstring(s, 1) ? mgtk_lua_get_id( lua_tostring(s, 1) ) : -1 );
	int value = (int)lua_tonumber(s, 3);

	mgtk_option_menu_value_set( id, value );

	return 0;
}


#endif // LUAWRAPPER_FOUND

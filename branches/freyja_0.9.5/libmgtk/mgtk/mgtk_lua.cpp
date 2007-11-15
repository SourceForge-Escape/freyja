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
#include "mgtk_events.h"
#include "mgtk_interface.h"
#include "mgtk_callbacks.h"
#include "mgtk_lua.h"


void mgtk_lua_register_functions(const Lua &lua)
{
	lua.RegisterFunction("mgtk_is_null", mgtk_lua_is_null);

	lua.RegisterFunction("mgtk_box_pack", mgtk_lua_rc_box_pack);

	lua.RegisterFunction("mgtk_window", mgtk_lua_rc_window);

	lua.RegisterFunction("mgtk_hbox", mgtk_lua_rc_hbox);
	lua.RegisterFunction("mgtk_vbox", mgtk_lua_rc_vbox);
	lua.RegisterFunction("mgtk_handlebox", mgtk_lua_rc_handlebox);
	lua.RegisterFunction("mgtk_summonbox", mgtk_lua_rc_summonbox);
	lua.RegisterFunction("mgtk_expander", mgtk_lua_rc_expander);

	lua.RegisterFunction("mgtk_append_menu", mgtk_lua_rc_append_menu);
	lua.RegisterFunction("mgtk_menubar", mgtk_lua_rc_menubar);
	lua.RegisterFunction("mgtk_submenu", mgtk_lua_rc_submenu);

	lua.RegisterFunction("mgtk_button", mgtk_lua_rc_button);
	lua.RegisterFunction("mgtk_colorbutton", mgtk_lua_rc_colorbutton);
	lua.RegisterFunction("mgtk_spinbutton_uint", mgtk_lua_rc_spinbutton_uint);
	lua.RegisterFunction("mgtk_spinbutton_int", mgtk_lua_rc_spinbutton_int);
	lua.RegisterFunction("mgtk_spinbutton_float", mgtk_lua_rc_spinbutton_float);

	lua.RegisterFunction("mgtk_statusbar", mgtk_lua_rc_statusbar);
	lua.RegisterFunction("mgtk_textview", mgtk_lua_rc_textview);
	lua.RegisterFunction("mgtk_label", mgtk_lua_rc_label);

	lua.RegisterFunction("mgtk_toggle_set", mgtk_lua_func_toggle_set);
}


int mgtk_lua_is_null(lua_State *s)
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

int mgtk_lua_rc_window(lua_State *s)
{
	GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_object_set_data(GTK_OBJECT(window), "window", window);
	gtk_window_set_policy(GTK_WINDOW(window), FALSE, TRUE, FALSE);

	if ( lua_gettop(s) == 2 &&
		 lua_isstring(s, 1) && lua_isstring(s, 2) )
	{
		const char *title = lua_tostring(s, 1);
		gtk_window_set_title(GTK_WINDOW(window), title);

		// FIXME: Disabled
		//const char *icon = lua_tostring(s, 2);
		//char filename[1024];
		//mgtk_get_pixmap_filename(filename, 1024, icon);
		//icon_filename[1023] = 0;
	}

	gtk_widget_show(window);

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
			gtk_box_pack_start(GTK_BOX(box), widget, TRUE, TRUE, 0);
			gtk_widget_show(box);
			gtk_widget_show(widget);
		}
	}

	return 0;
}


int mgtk_lua_rc_expander(lua_State *s)
{
	GtkWidget *widget = NULL;

	if ( lua_gettop(s) >= 2 && 
		 lua_isstring(s, 1) && lua_isboolean(s, 2) )
	{
		const char *label = lua_tostring(s, 1);
		int expand = lua_toboolean(s, 2);

		widget = gtk_expander_new(label);		
		gtk_expander_set_expanded((GtkExpander*)widget, expand ? TRUE : FALSE);

		GtkWidget *vbox = gtk_vbox_new(FALSE, 0);
		gtk_container_add((GtkContainer *)widget, vbox);
		gtk_widget_show_all(widget);

		// Optional mlisp symbol table binding.
		if ( lua_gettop(s) == 3 && lua_isstring(s, 3) )
		{
			const char *name = lua_tostring(s, 3);

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
	if ( lua_gettop(s) == 2 && 
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



//////////////////////////////////////////////////////////////////////////////
// Event widgets
////////////////////////////////////////////////////////////////////////////// 

int mgtk_lua_rc_append_menu(lua_State *s)
{
	if ( lua_gettop(s) >= 2 &&
		 lua_islightuserdata(s, 1) && lua_islightuserdata(s, 2) )
	{
		GtkWidget *menu = (GtkWidget *)lua_touserdata(s, 1);
		GtkWidget *item = (GtkWidget *)lua_touserdata(s, 2);

		if ( GTK_IS_MENU_BAR(menu) )
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
	GtkWidget *menubar = gtk_menu_bar_new();

	// Push widget pointer unto the lua stack.
	lua_pushlightuserdata(s, menubar);

	return 1;
}


int mgtk_lua_rc_submenu(lua_State *s)
{
	const char *label = NULL;
	int id = -1;

	if ( lua_isnumber(s, 1) )
	{
		id = (int)lua_tonumber(s, 1);
		label = lua_tostring(s, 2);
	}
	else if ( lua_isstring(s, 1) )
	{
		label = lua_tostring(s, 1);
	}

	GtkWidget *item = gtk_image_menu_item_new_with_mnemonic(label);
	GtkWidget *submenu = gtk_menu_new();
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), submenu);

	if (id != -1)
	{
		// Mongoose 2002.02.14, Add this widget to a special 
		//   lookup table by it's event id
		mgtk_event_subscribe_gtk_widget(id, submenu);
	}

	if ( lua_gettop(s) == 3 && lua_isstring(s, 3) )
	{
		const char *symbol = lua_tostring(s, 3);

		if (symbol)
		{
			arg_list_t *symbol_arg, *binding; 
			new_string(&symbol_arg, (char *)symbol);
			new_adt(&binding, ARG_GTK_MENU_WIDGET, (void *)submenu);
			mlisp_bind(symbol_arg, binding);
		}
	}

	// Push widget pointer unto the lua stack.
	lua_pushlightuserdata(s, (void *)submenu);

	return 1;
}


int mgtk_lua_rc_button(lua_State *s)
{
	// FIXME: Asserts, checking...
	const char *label = lua_tostring(s, 1);
	int event = (int)lua_tonumber(s, 2);
	//int cmd = (int)lua_tonumber(s, 3);

	GtkWidget *widget = gtk_button_new_with_label(label);

	// Add this widget to an event id map. 
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
		mgtk_event_subscribe_gtk_widget(event, spinbutton);	
		gtk_signal_connect(GTK_OBJECT(widget), "changed",
						   GTK_SIGNAL_FUNC(mgtk_event_spinbutton_uint), 
						   GINT_TO_POINTER(event) );
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
		mgtk_event_subscribe_gtk_widget(event, spinbutton);	
		gtk_signal_connect(GTK_OBJECT(widget), "changed",
						   GTK_SIGNAL_FUNC(mgtk_event_spinbutton_int), 
						   GINT_TO_POINTER(event) );
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

		if (widget)
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


#endif // LUAWRAPPER_FOUND

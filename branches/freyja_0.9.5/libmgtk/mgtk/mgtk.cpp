/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Mgtk
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2007 Mongoose
 * Comments: Mgtk unit test
 *
 * 
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.08.18:
 * Mongoose - Created
 ==========================================================================*/

#include <mstl/String.h>

#include "ConfirmationDialog.h"
#include "Resource.h"
#include "ResourceEvent.h"
#include "mgtk_linker.h"
#include "mgtk_lua.h"
#include "mgtk_events.h"


#ifdef UNIT_TEST_MGTK

Lua gLua;
Resource gResource;


int mgtk_assert_dialog(const char *file, unsigned int line, 
					   const char *function,
					   const char *expression, const char *msg)
{
	mstl::String s;
	s.Set("Assert encountered:\n %s:%i %s()\n '%s'\n %s", 
		  file, line, function, expression, msg);

	ConfirmationDialog q("FreyjaAssertCallbackHandlerDialog", 
						 "gtk-dialog-error",
						 s.c_str(), 
						 "\nWould you like to continue with errors anyway?",
						 "gtk-quit", "_Exit", "gtk-ok", "_Continue");

	int action = q.Execute();

	return action;
}


void mgtk_test_handle_resource_start()
{
	// Setup the UI ( mlisp test )
	if ( !gResource.Load( "test.mlisp" ) )
	{
	}
}


void mgtk_test_handle_application_window_close()
{
	exit(0);
}


int mgtk_test_handle_command2i(int event, int cmd)
{
	if ( cmd > 0 && !ResourceEvent::listen(cmd - 10000) )
	{
		mgtk_print("%s(%i, %i): Event has no handler.\n",
					 __func__, event, cmd);
		return -1;
	}

	return 0;
}


void mgtk_test_handle_command(int command)
{
	if ( !ResourceEvent::listen(command - 10000) )
		mgtk_print("!Event(%d): Unhandled event.\n", command);
}


void mgtk_test_shutdown()
{
	exit(0);
}


int main(int argc, char* argv[])
{
	const char* ui = "test.lua";

	if ( argc > 1 )
	{
		ui = argv[1];
	}


	/* Setup callbacks. */
	mgtk_assert_handler( mgtk_assert_dialog );

	/* 'Link' up mgtk library stubs to these implementations. */
	mgtk_link_import("mgtk_print", (void*)printf);
	//mgtk_link_import("mgtk_callback_get_image_data_rgb24", (void*)freyja_callback_get_image_data_rgb24);
	//mgtk_link_import("mgtk_get_pixmap_filename", (void*)freyja_get_pixmap_filename);
	//mgtk_link_import("mgtk_rc_map", (void*)freyja_rc_map);
	//mgtk_link_import("mgtk_get_resource_path", (void*)freyja_get_resource_path_callback);
	
	/* Hookup resource to event system */
	//mgtk_link_import("mgtk_handle_color", (void*)freyja_handle_color);
	mgtk_link_import("mgtk_handle_application_window_close", (void*)mgtk_test_handle_application_window_close);
	mgtk_link_import("mgtk_handle_command", (void*)mgtk_test_handle_command);
	mgtk_link_import("mgtk_handle_command2i", (void*)mgtk_test_handle_command2i);
	//mgtk_link_import("mgtk_handle_event1u", (void*)freyja_handle_event1u);
	//mgtk_link_import("mgtk_handle_event1f", (void*)freyja_handle_event1f);
	//mgtk_link_import("mgtk_handle_gldisplay", (void*)freyja_handle_gldisplay);
	//mgtk_link_import("mgtk_handle_glresize", (void*)freyja_handle_glresize);
	//mgtk_link_import("mgtk_handle_key_press", (void*)freyja_handle_key_press);
	//mgtk_link_import("mgtk_handle_motion", (void*)freyja_handle_motion);
	//mgtk_link_import("mgtk_handle_mouse", (void*)freyja_handle_mouse);
	//mgtk_link_import("mgtk_handle_resource_start", (void*)mgtk_test_handle_resource_start);
	//mgtk_link_import("mgtk_handle_text_array", (void*)freyja_handle_text_array);
	//mgtk_link_import("mgtk_handle_text", (void*)freyja_handle_text);
	ResourceEvent::setResource( &gResource );
	ResourceEventCallback::add("eShutdown", mgtk_test_shutdown);

	/* Export Lua mgtk functions to libfreyja VM. */
	mgtk_lua_register_functions( gLua );

	/* Setup the user interface. */
	mgtk_init(argc, argv);

	/* Load the UI script here instead of using the 
	 * mgtk_handle_resource_start() callback. */
	if ( !gLua.ExecuteFile( ui ) )
	{
		mgtk_print("Failed to load test.lua\n");
	}

	/* Start the user interface. */
	mgtk_start();

	return 0;
}


#endif // UNIT_TEST_MGTK

/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * License : No use w/o permission (C) 2001-2007 Terry 'Mongoose' Hendrix II
 * Object  : 
 * Comments: 
 * 
 *-- History ------------------------------------------------- 
 *
 * 2007.12.30:
 * Mongoose - Refactored from freyja_events.
 * 
 * 2007.04.01:
 * Mongoose - All platforms use registered callbacks now, removed old code
 *
 * 2001.06.26:
 * Mongoose - Created
 ==========================================================================*/

#include "config.h"

#include <string.h>

#include <freyja/freyja.h>
#include <freyja/CameraABI.h>
#include <freyja/LightABI.h>
#include <freyja/LuaABI.h>
#include <freyja/MaterialABI.h>
#include <freyja/Scene.h>

#include <mstl/SystemIO.h>

#include <mgtk/mgtk_events.h>
#include <mgtk/mgtk_linker.h>
#include <mgtk/mgtk_lua.h>
#include <mgtk/ResourceEvent.h>

#include "Plugins.h"
#include "Control.h"
#include "FreyjaRender.h"
#include "freyja_events.h"

void freyja3d_misc_events_attach( );

freyja::Scene* gScene = NULL;
Lua gLuaVM;

using namespace freyja3d;


bool 
freyja3d_execute_lua_script( const char* filename )
{
	bool ret = true;

	if ( !mstl::SystemIO::File::DoesFileExist( filename ) )
	{
		FREYJA_ASSERTMSG(0, "Lua script '%s' could not be accessed.\n", filename );
	}
	else
	{
		if ( !gLuaVM.ExecuteFile( filename ) )
		{
			FREYJA_ASSERTMSG(0, "Lua error '%s':\n%s\n", filename, gLuaVM.GetLastError() );
		}
		else
		{
			ret = true;
		}
	}

	return ret;
}


int 
freyja3d_assert_handler( const char* file, 
						 unsigned int line, 
						 const char* function,
						 const char* expression, 
						 const char* msg )
{
	mstl::String s;
	s.Set("Assert encountered:\n %s:%i %s()\n '%s'\n %s", 
		  file, line, function, expression, msg);
	
#if ENABLE_ASSERT_LOG
	ControlPrinter::Log("[ASSERT] %s\n", s.c_str());
#endif

	// FIXME: Make a confirmation dialog as a locale template for this,
	//        and fill in the 'info' string here every time you call it.
	ConfirmationDialog q("FreyjaAssertCallbackHandlerDialog", 
						 "gtk-dialog-error", //"gtk-dialog-warning"
						 s.c_str(), 
						 "\nWould you like to continue with errors anyway?",
						 "gtk-quit", "_Exit", "gtk-ok", "_Continue");
	int action = q.Execute();
	
#if ENABLE_ASSERT_LOG
	ControlPrinter::Log("[ASSERT] User %s\n", action ? "continued" : "aborted");
#endif

	return action;
}


int 
freyja3d_debug_msg_handler( const char* file,
							unsigned int line, 
							const char* function,
							const char* expression,
							const char* msg )
{
	mstl::String s;
	s.Set("Called from:\n %s:%i %s()\n %s", 
		  file, line, function, msg); // Removed expression 

#if ENABLE_ASSERT_LOG
	ControlPrinter::Log("[INFO] %s\n", s.c_str());
#endif

	ConfirmationDialog q("FreyjaDebugInfoCallbackHandlerDialog", 
						 "gtk-dialog-info",//"gtk-dialog-warning",
						 s.c_str(), 
						 "",
						 "", "", "gtk-close", "_Close");
	int action = q.Execute();

#if ENABLE_ASSERT_LOG
	ControlPrinter::Log("[INFO] User %s\n", action ? "ok" : "cancel");
#endif

	return action;
}



void 
freyja3d_shutdown( )
{
#if 0
	if ( FreyjaControl::GetInstance() )
	{
		delete FreyjaControl::GetInstance();
	}
#endif

	freyja3d_save_user_preferences( );
	
	mgtk_event_shutdown( );
	freyja_shutdown( );

	freyja_print( "Thanks for using %s", PROGRAM_NAME );
	freyja_print( "   Build date: %s @ %s", __DATE__, __TIME__ );
	freyja_print( "   Build host: %s", BUILD_HOST );
	freyja_print( "   Email addr: %s", EMAIL_ADDRESS );
	freyja_print( "   Web site  : %s", PROJECT_URL );

	ControlPrinter::StopLogging( );
}


void 
freyja3d_quit( )
{
	// There is some bug with some Gtk+ builds that allow this function
	// to be called again before this exits.  
	// Also relates to the Cancel doesn't cancel bug.
	//FreyjaControl::GetInstance()->Shutdown();
	bool exiting = true;

	if ( gScene->GetModified( ) && !mgtk::ExecuteConfirmationDialog("ExitWarningDialog") )
	{
		exiting = false;
	}

	if ( exiting )
	{
		freyja3d_shutdown( );
	}
}


void freyja3d_print( const char* format, ...)
{
	if ( format && format[0] )
	{
		const unsigned int sz = 1024;
		char buf[sz];

		va_list args;
		va_start(args, format);
		int truncated = vsnprintf(buf, sz, format, args);
		buf[sz-1] = 0;
		va_end(args);

		/* More than 1k string was needed, so allocate one. */
		if ( truncated >= (int)sz )
		{
			unsigned int len = truncated + 1; // Doesn't include '\0'
			char* s = new char[ len ];

			va_start( args, format );
			vsnprintf( s, len, format, args );
			s[len-1] = '\0';
			va_end( args );

			ControlPrinter::Print( s );
			delete [] s;
		}
		else
		{
			ControlPrinter::Print( buf );
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
// Event callbacks.
////////////////////////////////////////////////////////////////////////////////

void freyja3d_handle_motion( int x, int y )
{
	//if (FreyjaControl::GetInstance())
	//FreyjaControl::GetInstance()->MotionEvent(x, y);
}


void freyja3d_handle_mouse( int button, int state, int mod, int x, int y )
{
	//if (FreyjaControl::GetInstance())
		//FreyjaControl::GetInstance()->MouseEvent(button, state, mod, x, y);
}


////////////////////////////////////////////////////////////////////////////////
// Library initialization.
////////////////////////////////////////////////////////////////////////////////

void 
freyja3d_init_libfreyja( )
{
	/* Setup libfreyja. */
	freyja_set_assert_handler( freyja3d_assert_handler );
	freyja_lua_register_functions( gLuaVM.GetState() );
	freyja_start( &Control::GetPrinter() );

	/* Spawn 0th light, and set the light iterator */
	vec4_t lightPos = { 12.0f, 35.0f, 8.0f, 0.0f }; // FIXME: Use mlisp cvars.
	freyjaCurrentLight(freyjaLightCreate());
	freyjaLightPosition4v(freyjaGetCurrentLight(), lightPos);

	/* Spawn 0th material, set the iterator, and make a default material */
	freyja::Material::mLoadTextureFunc = freyja3d_load_texture;
	freyja::Material::mLoadShaderFunc = freyja3d_load_shader;
	int32 mIndex = freyjaMaterialCreate();
	vec4_t rgba = { 0.0f, 0.0f, 0.0f, 1.0f };
	freyjaCurrentMaterial(mIndex);
	freyjaMaterialName(mIndex, "Boring default");
	freyjaMaterialEmissive(mIndex, rgba);
	rgba[0] = rgba[1] = rgba[2] = 0.79f;
	freyjaMaterialDiffuse(mIndex, rgba);
	rgba[0] = rgba[1] = rgba[2] = 0.2f;
	freyjaMaterialAmbient(mIndex, rgba);
	rgba[0] = rgba[1] = rgba[2] = 0.01f;
	freyjaMaterialSpecular(mIndex, rgba);
	freyjaMaterialShininess(mIndex, 0.0f);
	freyjaMaterialSetFlag( mIndex, fFreyjaMaterial_Texture );

	/* Setup camera0. */
	freyja_ptr camera0 = freyjaCameraCreate( "camera0" );
	//freyjaCameraPos3f( camera0, 64.0f, 64.0f, 64.0f );
	//freyjaCameraTarget3f( camera0, 0.0f, 32.0f, 0.0f );

	gScene = new freyja::Scene( );

	/* Set libfreyja query handler. */
	freyja_query_set_callback( freyja3d_query_callback_handler );
}


void 
freyja3d_resource_init( Resource& r )
{
	/* Attach singleton method listeners. */
	//FreyjaControl::GetInstance()->AttachMethodListeners( );
	FreyjaRender::GetInstance()->AttachMethodListeners( );

	/* Non-class listeners. */
	freyja3d_misc_events_attach( );

	/* Bind mlisp script functions to C/C++ functions. */
	//r.RegisterFunction("funcname", freyja_rc_funcname);


	////////////////////////////////////////////////////////////////////
	// Old style events
	////////////////////////////////////////////////////////////////////

	/* Mongoose 2002.01.21, 
	 * Bind some script vars to matching symbol in C/C++ */

	// Event types used for flow control of event ids
	r.RegisterInt("eMode", eMode);
	r.RegisterInt("eEvent", eEvent);
	r.RegisterInt("eNop", eNop);
	r.RegisterInt("eNone", eNone);

	// Menus
	r.RegisterInt("ePluginMenu", ePluginMenu);
	r.RegisterInt("eBlendDestMenu", eBlendDestMenu);
	r.RegisterInt("eBlendSrcMenu", eBlendSrcMenu);
	r.RegisterInt("eObjectMenu", eObjectMenu);
	r.RegisterInt("eViewportModeMenu", eViewportModeMenu);
	r.RegisterInt("eTransformMenu", eTransformMenu);

	// Colors
	r.RegisterInt("eColorMaterialAmbient", eColorMaterialAmbient);
	r.RegisterInt("eColorMaterialDiffuse", eColorMaterialDiffuse);
	r.RegisterInt("eColorMaterialSpecular", eColorMaterialSpecular);
	r.RegisterInt("eColorMaterialEmissive", eColorMaterialEmissive);
	r.RegisterInt("eColorLightAmbient", eColorLightAmbient);
	r.RegisterInt("eColorLightDiffuse", eColorLightDiffuse);
	r.RegisterInt("eColorLightSpecular", eColorLightSpecular);
	r.RegisterInt("eColorBackground", eColorBackground);
	r.RegisterInt("eColorGrid", eColorGrid);
	r.RegisterInt("eColorMesh", eColorMesh);
	r.RegisterInt("eColorVertex", eColorVertex);
	r.RegisterInt("eColorVertexHighlight", eColorVertexHighlight);
	r.RegisterInt("eColorMeshHighlight", eColorMeshHighlight);
	r.RegisterInt("eColorBone", eColorBone);
	r.RegisterInt("eColorBoneHighlight", eColorBoneHighlight);
	r.RegisterInt("eColorJoint", eColorJoint);
	r.RegisterInt("eColorJointHighlight", eColorJointHighlight);

	freyja_print( "%s: Core events {", __func__ );

	ResourceEventCallback::add( "eShutdown", &freyja3d_quit );

	freyja_print( "}" );
}


void 
freyja3d_init_mgtk( int argc, char* argv[] )
{
	/* 'Link' up mgtk library stubs to these implementations */
	mgtk_link_import( "mgtk_handle_motion", (void*)freyja3d_handle_motion );
	mgtk_link_import( "mgtk_handle_mouse", (void*)freyja3d_handle_mouse );
	mgtk_link_import( "mgtk_print", (void*)freyja3d_print );

	mgtk_link_import("mgtk_handle_color", (void*)freyja_handle_color);
	mgtk_link_import( "mgtk_handle_application_window_close", (void*)freyja3d_quit );
	mgtk_link_import("mgtk_handle_gldisplay", (void*)freyja_handle_gldisplay);
	mgtk_link_import("mgtk_handle_glresize", (void*)freyja_handle_glresize);
	mgtk_link_import("mgtk_handle_text_array", (void*)freyja_handle_text_array);
	mgtk_link_import("mgtk_callback_get_image_data_rgb24", (void*)freyja_callback_get_image_data_rgb24);
	mgtk_link_import("mgtk_get_pixmap_filename", (void*)freyja_get_pixmap_filename);
	mgtk_link_import("mgtk_get_resource_path", (void*)freyja_get_resource_path_callback);
	mgtk_link_import("mgtk_rc_map", (void*)freyja_rc_map);

	mgtk_assert_handler( freyja3d_assert_handler );

	/* Hookup legacy resource to legacy event system. */
	ResourceEvent::setResource( &Control::GetResource() );

	/* Export Lua mgtk functions. */ 
	mgtk_lua_register_functions( gLuaVM );

	/* Start the user interface backend. */
	mgtk_init( argc, argv );

	/* Initialize / hookup events. */
	freyja3d_resource_init( Control::GetResource() );

	/* Generate user interface from Lua script. */
	{
		String s = freyja_rc_map_string( FREYJA_UI_SCRIPT );
		freyja3d_execute_lua_script( s.c_str() );
	}

	/* Load user preferences from Lua script. */
	{
		String s = freyja_rc_map_string( FREYJA_USER_PREF_FILE );
		freyja3d_execute_lua_script( s.c_str() );
	}

	/* Start the OpenGL context. */
	freyja_handle_resource_start( );

	/* Start the user interface loop. */
	mgtk_start();
}


int 
main(int argc, char *argv[])
{
	/* Enable logging. */
	{
		mstl::String log = freyja_rc_map_string( FREYJA_LOG_FILE );
		ControlPrinter::StartLogging( log.c_str() );

		/* Report library versions to log. */	
		ControlPrinter::Log("@ freyja3d: %s", VERSION);
		ControlPrinter::Log("@ libhel: %s", helVersionInfo() );
		ControlPrinter::Log("@ libfreyja: %s", freyja_get_version() );
		ControlPrinter::Log("@ libmgtk: %s", mgtk_version() );
	}

	/* User install of scripts, icons, samples, configs, etc */
	if ( !freyja_is_user_installed_data_old() )
	{
		freyja_install_user();
	}

	/* Hookup assert handlers, note freyja assert is also used by this layer */
	freyjaDebugInfoHandler( freyja3d_debug_msg_handler );

	/* Setup libraries. */
	freyja3d_init_libfreyja( );
	freyja3d_init_mgtk( argc, argv );

	return 0;
}

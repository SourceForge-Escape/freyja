/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2001 Terry 'Mongoose' Hendrix II
 * Comments: This is the common event system interface
 *           for freyja
 *
 *           It's independent of the widget set, etc
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
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
#include <freyja/LightABI.h>
#include <freyja/MaterialABI.h>
#include <freyja/LuaABI.h>
#include <freyja/FreyjaImage.h>
#include <freyja/Metadata.h>

#include <mstl/SystemIO.h>

#include <mgtk/mgtk_events.h>
#include <mgtk/mgtk_linker.h>
#include <mgtk/mgtk_lua.h>
#include <mgtk/mgtk_tree.h>
#include <mgtk/ResourceEvent.h>
#include <mgtk/ConfirmationDialog.h>
#include <mgtk/QueryDialog.h>

#include "FreyjaRender.h"
#include "FreyjaControl.h"
#include "Plugins.h"
#include "freyja3d_scenegraph.h"
#include "freyja_events.h"

void freyja3d_misc_events_attach();


using namespace freyja3d;


int freyja3d_get_event_id(const char* symbol)
{
	return Control::GetEventIdByName( symbol );
}


void freyja3d_record_saved_model(const char* filename)
{
	FreyjaControl::GetInstance()->RecordSavedModel(filename);
}


void freyjaQueryCallbackHandler(unsigned int size, freyja_query_t *array)
{
	QueryDialog d;
	d.mName = "freyjaQueryCallbackHandler"; 
	d.mDialogIcon = "gtk-question"; 
	d.mInformationMessage = "Query Dialog"; 
	d.mCancelIcon = "gtk-cancel"; 
	d.mCancelText = "Cancel"; 
	d.mAcceptIcon = "gtk-ok";
	d.mAcceptText = "Answer"; 

	bool any = false;

	for (uint32 i = 0; i < size; ++i)
	{
		const char *type = array[i].type;
		const char *symbol = array[i].symbol;
		void *data = array[i].ptr;

		String s = type;

		if (s == "int")
		{
			s += " ";
			s += symbol;
			QueryDialogValue<int> v(symbol, s.c_str(), *(int *)data);
			d.mInts.push_back(v);
			any = true;
		}
		else if (s == "float")
		{
			s += " ";
			s += symbol;
			QueryDialogValue<float> v(symbol, s.c_str(), *(float *)data);
			d.mFloats.push_back(v);
			any = true;
		}
		else
		{
			FREYJA_INFOMSG(false, "FIXME: Unhandled query '%s' '%s' %p\n", 
						   type, symbol, data);
		}
	}


	// If the user chooses to set values, set them.
	if ( any && d.Execute() )
	{
		for (uint32 i = 0; i < size; ++i)
		{
			String s = array[i].type;

			if (s == "int")
			{
				(*(int *)array[i].ptr) = d.GetInt( array[i].symbol );
			}
			else if (s == "float")
			{
				(*(float *)array[i].ptr) = d.GetFloat( array[i].symbol );
			}
			else
			{
				// Not handled
			}
		}	
	}
}


void freyja_handle_application_window_close()
{
	//FREYJA_ASSERTMSG(FreyjaControl::mInstance, "FreyjaControl singleton not allocated");

	// There is some bug with some Gtk+ builds that allow this function
	// to be called again before this exits.  
	// Also relates to the Cancel doesn't cancel bug.
	FreyjaControl::GetInstance()->Shutdown();
}


void freyja_handle_color(int id, float r, float g, float b, float a)
{
	vec4_t color = { r, g, b, a };

	/* Color event listener */
	if (ResourceEvent::listen(id - ePluginEventBase, color, 4))
		return;

	switch (id)
	{
	case eColorMaterialAmbient: 
		freyjaMaterialAmbient(freyjaGetCurrentMaterial(), color);
		MaterialControl::GetInstance()->RefreshInterface();
		break;

	case eColorMaterialDiffuse:
		freyjaMaterialDiffuse(freyjaGetCurrentMaterial(), color);
		MaterialControl::GetInstance()->RefreshInterface();
		break;

	case eColorMaterialSpecular:
		freyjaMaterialSpecular(freyjaGetCurrentMaterial(), color);
		MaterialControl::GetInstance()->RefreshInterface();
		break;

	case eColorMaterialEmissive:
		freyjaMaterialEmissive(freyjaGetCurrentMaterial(), color);
		MaterialControl::GetInstance()->RefreshInterface();
		break;

	case eColorLightAmbient: 
		freyjaLightAmbient(freyjaGetCurrentLight(), color);
		freyja_event_set_color(eColorLightAmbient, r, g, b, a);
		break;

	case eColorLightDiffuse:
		freyjaLightDiffuse(freyjaGetCurrentLight(), color);
		freyja_event_set_color(eColorLightDiffuse, r, g, b, a);
		break;

	case eColorLightSpecular:
		freyjaLightSpecular(freyjaGetCurrentLight(), color);
		freyja_event_set_color(eColorLightSpecular, r, g, b, a);
		break;

	case eColorBackground:
		memcpy(FreyjaRender::mColorBackground, color, sizeof(vec4_t));
		freyja_event_set_color(eColorBackground, r, g, b, a);
		break;

	case eColorGrid:
		memcpy(FreyjaRender::mColorGridSeperator, color, sizeof(vec4_t));
		memcpy(FreyjaRender::mColorGridLine, color, sizeof(vec4_t));
		freyja_event_set_color(eColorGrid, r, g, b, a);
		break;

	case eColorVertex:
		memcpy(FreyjaRender::mColorVertex, color, sizeof(vec4_t));
		freyja_event_set_color(eColorVertex, r, g, b, a);
		break;

	case eColorVertexHighlight:
		memcpy(FreyjaRender::mColorVertexHighlight, color, sizeof(vec4_t));
		freyja_event_set_color(eColorVertexHighlight, r, g, b, a);
		break;

	case eColorMesh:
		memcpy(FreyjaRender::mColorWireframe, color, sizeof(vec4_t));
		freyja_event_set_color(eColorMesh, r, g, b, a);
		break;

	case eColorMeshHighlight:
		memcpy(FreyjaRender::mColorWireframeHighlight, color, sizeof(vec4_t));
		freyja_event_set_color(eColorMeshHighlight, r, g, b, a);
		break;

	case eColorBone:
		memcpy(FreyjaRender::mColorBone, color, sizeof(vec4_t));
		freyja_event_set_color(eColorBone, r, g, b, a);
		break;

	case eColorBoneHighlight:
		memcpy(FreyjaRender::mColorBoneHighlight, color, sizeof(vec4_t));
		freyja_event_set_color(eColorBoneHighlight, r, g, b, a);
		break;

	case eColorJoint:
		memcpy(FreyjaRender::mColorJoint, color, sizeof(vec4_t));
		freyja_event_set_color(eColorJoint, r, g, b, a);
		break;

	case eColorJointHighlight:
		memcpy(FreyjaRender::mColorJointHighlight, color, sizeof(vec4_t));
		freyja_event_set_color(eColorJointHighlight, r, g, b, a);
		break;

	default:
		return;
	}

	mgtk_event_gl_refresh();
}


///////////////////////////////////////////////////////////////////////
// Freyja wrappers
///////////////////////////////////////////////////////////////////////

const char *freyja_get_resource_path_callback()
{
	static String s;
	s = freyja_get_resource_path(); // In case it's changed refresh here.
	return s.c_str();
}


// FIXME remove duplicates
void freyja_handle_key_press(int key, int mod)
{
	freyja_print("mgtk_handle_key_press(%d, %d) not handled", key, mod);
}


void freyja_handle_command2i(int event, int command)
{
	freyja_event2i(event, command);
}


void freyja_handle_event1u(int event, unsigned int value)
{
	//FREYJA_ASSERTMSG(FreyjaControl::mInstance, "FreyjaControl singleton not allocated");

	// Removed legacy uint event codepath, kept old menu event fallback
	if (!ResourceEvent::listen(event - ePluginEventBase, value))
	{
		// Old style 2 ID menu events
		if (freyja_event2i(eEvent, event) == -1)
			freyja_print("!Event(%i, %i) dropped.", eEvent, event);
	}
}


void freyja_handle_event1f(int event, float value)
{
	//FREYJA_ASSERTMSG(FreyjaControl::mInstance, "FreyjaControl singleton not allocated");

	//if (!FreyjaControl::mInstance->event(event, value))
	if (!ResourceEvent::listen(event - ePluginEventBase, value))
	{
		if (freyja_event2i(eEvent, event) == -1)
			freyja_print("   mgtk_handle_event1f spawned previous unhandled event %i:%i", eEvent, event);
	}
}


void freyja_handle_gldisplay()
{
	FREYJA_ASSERTMSG(FreyjaRender::mInstance, "FreyjaRender Singleton not allocated");
	FreyjaRender::mInstance->Display();
}


void freyja_handle_glresize(unsigned int width, unsigned int height)
{
	//FREYJA_ASSERTMSG(FreyjaControl::mInstance, "FreyjaRender Singleton not allocated");
	FreyjaRender::mInstance->ResizeContext(width, height);
}


void freyja_handle_text_array(int event, unsigned int count, char **text)
{
	if (count != 2)
		return;

	if (!ResourceEvent::listen(event - ResourceEvent::eBaseEvent, 
							   text[0], text[1]))
	{
		// Not handled
	}	
}


void freyja_handle_text(int event, char *text)
{
	if (text == NULL || text[0] == 0)
		return;

	if (!ResourceEvent::listen(event - ePluginEventBase, text))
	{
		//if (!FreyjaControl::mInstance->handleTextEvent(event, text))
		freyja_print("%s(%i, '%s'): Unhandled event.", __func__, event, text);
	}
}


void freyja_callback_get_image_data_rgb24(const char *filename, 
										  unsigned char **image, 
										  int *width, int *height)
{
	*image = NULL;
	*width = 0;
	*height = 0;

	FreyjaImage img;

	if ( !img.loadImage( filename ) )
	{
		unsigned char* swap = NULL;
		img.setColorMode( FreyjaImage::RGB_24 );
		img.scaleImage(256, 256);
		img.getImage(&swap);
		*image = swap;
		*width = img.getWidth();
		*height = img.getHeight();
	}
}


void freyja_handle_command(int command)
{
	//FREYJA_ASSERTMSG(FreyjaControl::mInstance, "FreyjaControl singleton not allocated");

	if (!ResourceEvent::listen(command - 10000 /*ePluginEventBase*/))
		freyja_print("!Event(%d): Unhandled event.", command);
}


void freyja_set_dialog_visible(const char *name)
{
	int e = ResourceEvent::GetResourceIdBySymbol((char*)name);
	mgtk_event_dialog_visible_set(e, 1);	
}


void freyja_handle_resource_init(Resource &r)
{
	////////////////////////////////////////////////////////////////////
	// New freyja events
	////////////////////////////////////////////////////////////////////

	// Class listeners
	FREYJA_ASSERTMSG(FreyjaControl::GetInstance() != NULL, "FreyjaControl wasn't instanced before method listeners were attached.");
	FreyjaControl::GetInstance()->AttachMethodListeners();
	FreyjaRender::AttachMethodListeners();

	// Non-class listeners
	freyja3d_misc_events_attach();

	// Bind mlisp script functions to C/C++ functions.
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

	/* Load and init plugins */
	String dir = freyja_rc_map_string("plugins");	
	freyja3d_plugin_application_init( dir.c_str() );
}


void freyja_handle_resource_start()
{
	/* Mongoose 2002.02.02,  
	 * This is the backend entry called by the mgtk widget layer.
	 */

	/* Mongoose 2007.04.07,
	 * Here all the resources are setup, and the main interface starts. 
	 */

	freyja_print("!@Freyja started...");

	freyja_handle_resource_init(FreyjaControl::GetInstance()->GetResource());

	/* User install of icons, samples, configs, etc */
	if ( !freyja_is_user_installed() )
		freyja_install_user();

	/* Build the user interface from scripts and load user preferences. */
	FreyjaControl::GetInstance()->Init();
	freyja3d_plugin_init();
	freyja3d_plugin_application_widget_init();

	/* Setup scenegraph widget(s). */
	freyja3d_scenegraph_init();

	/* Setup material interface */
	MaterialControl::GetInstance()->RefreshInterface();

	/* Setup editor modes and drop-down menus */
	mgtk_option_menu_value_set(eViewportModeMenu, 0);
	FreyjaControl::GetInstance()->EvModeModel();

	mgtk_option_menu_value_set(eTransformMenu, 1);
	FreyjaControl::GetInstance()->SetObjectMode(FreyjaControl::tMesh);

	mgtk_option_menu_value_set(eObjectMenu, 0);
	FreyjaControl::GetInstance()->SetActionMode(FreyjaControl::aSelect);

	/* Set init window title, log welcome, and refresh OpenGL context */
	freyja_set_main_window_title(BUILD_NAME);
	freyja_print("Welcome to Freyja %s, %s", VERSION, __DATE__);
	mgtk_event_gl_refresh();

	/* Mongoose 2002.02.23, Hook for exit() calls */
	atexit(freyja_event_shutdown);
}


void freyja_append_eventid(char *symbol, int eventid)
{
	FreyjaControl::GetInstance()->GetResource().RegisterInt(symbol, eventid);
}


void freyja_get_rc_path(char *s, long sz)
{
	s[0] = 0;

#ifdef WIN32
	char cwd[1024];
	SystemIO::File::GetCurrentWorkingDir(cwd, 1024);

	long len = strlen(cwd);

	if (sz < len)
		return;
	
	snprintf(s, len, cwd);
	s[len] = 0;
#else
	char *env;

	env = getenv("HOME");

	if (!env || !env[0])
	{
		printf("ERROR: Bad HOME envronment\n");
		return;
	}

	long len = strlen(env) + strlen("/.freyja/");

	if (sz < len)
		return;

	snprintf(s, len, "%s/.freyja/", env);
	s[len] = 0;
#endif
}


void freyja_get_share_path(char *s, long sz)
{
	if (sz < 64)
		return;

#ifdef WIN32
	char cwd[1024];
	SystemIO::File::GetCurrentWorkingDir(cwd, 1024);
	snprintf(s, strlen(cwd), cwd);
#else
	snprintf(s, strlen("/usr/share/freyja/"), "/usr/share/freyja/");
#endif
}


void freyja_get_rc_filename(char *s, const char *filename, long sz)
{
	s[0] = 0;

#ifdef WIN32
	char cwd[1024];
	SystemIO::File::GetCurrentWorkingDir(cwd, 1024);

	long len = strlen(cwd) + strlen(filename);

	if (sz < len)
		return;
	
	snprintf(s, len, "%s/%s", cwd, filename);
	s[len] = 0;
#else
	char *env;

	env = getenv("HOME");

	if (!env || !env[0])
	{
		printf("ERROR: Bad HOME envronment\n");
		return;
	}

	long len = strlen(env) + strlen("/.freyja/") + strlen(filename);

	if (sz < len)
		return;

	snprintf(s, len, "%s/.freyja/%s", env, filename);
	s[len] = 0;
#endif
}


void freyja_get_share_filename(char *s, const char *filename, long sz)
{
	s[0] = 0;

#ifdef WIN32
	char cwd[1024];
	SystemIO::File::GetCurrentWorkingDir(cwd, 1024);

	long len = strlen(cwd) + strlen(filename);

	if (sz < len)
		return;
	
	snprintf(s, len, "%s/%s", cwd, filename);
	s[len] = 0;
#else
	long len = strlen("/usr/share/freyja/") + strlen(filename);

	if (sz < len)
		return;

	snprintf(s, len, "/usr/share/freyja/%s", filename);
	s[len] = 0;
#endif
}


char freyja_is_user_installed()
{
#ifdef WIN32
	return 1;
#else
	SystemIO::FileReader r;
	char path[128];

	freyja_get_rc_path(path, 128);

	return (r.DoesFileExist(path));
#endif
}


void freyja_install_user()
{
	SystemIO::FileReader r;
	SystemIO::FileWriter w;
	char rc[512];
	char share[512];
	const char *filename;


	/* Copy top level rc files */
	freyja_get_rc_path(rc, 512);
	freyja_get_share_path(share, 512);
	SystemIO::File::CreateDir(rc);

	if (r.OpenDir(share))
	{
		while ((filename = r.GetNextDirectoryListing()))
		{
			if (r.IsDirectory(filename))
				continue;

			SystemIO::CopyFileToPath(filename, rc);
		}
	}


	/* Copy icon files */
	freyja_get_rc_filename(rc, "icons/", 512);
	freyja_get_share_filename(share, "icons/", 512);
	SystemIO::File::CreateDir(rc);

	if (r.OpenDir(share))
	{
		while ((filename = r.GetNextDirectoryListing()))
		{
			if (r.IsDirectory(filename))
				continue;

			SystemIO::CopyFileToPath(filename, rc);
		}
	}


	/* Copy material files */
	freyja_get_rc_filename(rc, "materials/", 512);
	freyja_get_share_filename(share, "materials/", 512);
	SystemIO::File::CreateDir(rc);

	if (r.OpenDir(share))
	{
		while ((filename = r.GetNextDirectoryListing()))
		{
			if (r.IsDirectory(filename))
				continue;

			SystemIO::CopyFileToPath(filename, rc);
		}
	}


	/* Copy palettes files */
	freyja_get_rc_filename(rc, "palettes/", 512);
	freyja_get_share_filename(share, "palettes/", 512);
	SystemIO::File::CreateDir(rc);

	if (r.OpenDir(share))
	{
		while ((filename = r.GetNextDirectoryListing()))
		{
			if (r.IsDirectory(filename))
				continue;

			SystemIO::CopyFileToPath(filename, rc);
		}
	}


	/* Misc */
	freyja_get_rc_filename(rc, "models/", 512);
	SystemIO::File::CreateDir(rc);

	freyja_get_rc_filename(rc, "animations/", 512);
	SystemIO::File::CreateDir(rc);

	freyja_get_rc_filename(rc, "textures/", 512);
	SystemIO::File::CreateDir(rc);

	//freyja_get_rc_filename(rc, "particles/", 512);
	//SystemIO::File::CreateDir(rc);

	/* Copy plugins */
	freyja_get_rc_filename(rc, "plugins/", 512);
	freyja_get_share_filename(share, "plugins/", 512);
	SystemIO::File::CreateDir(rc);

	if (r.OpenDir(share))
	{
		while ((filename = r.GetNextDirectoryListing()))
		{
			if (r.IsDirectory(filename))
				continue; // Might want to recurse copy plugins later.

			SystemIO::CopyFileToPath(filename, rc);
		}
	}
}


void freyja_handle_motion(int x, int y)
{
	if (FreyjaControl::GetInstance())
	{
		FreyjaControl::GetInstance()->MotionEvent(x, y);
	}
}


void freyja_handle_mouse(int button, int state, int mod, int x, int y)
{
	if (FreyjaControl::GetInstance())
	{
		FreyjaControl::GetInstance()->MouseEvent(button, state, mod, x, y);
	}
}


int freyja_event2i(int event, int cmd)
{
	if ( event != eNop && 
		 !ResourceEvent::listen(cmd - ePluginEventBase) )
	{
		freyja_print("%s(%i, %i): Event has no handler.",
					 __func__, event, cmd);
		return -1;
	}

	return 0;
}


void freyja_print(char *format, ...)
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


void freyja_event_shutdown()
{
	if ( FreyjaControl::GetInstance() )
	{
		delete FreyjaControl::GetInstance();
	}

	freyja_print("Thanks for using %s", PROGRAM_NAME);
	freyja_print("   Build date: %s @ %s", __DATE__, __TIME__);
	freyja_print("   Build host: %s", BUILD_HOST);
	freyja_print("   Email addr: %s", EMAIL_ADDRESS);
	freyja_print("   Web site  : %s", PROJECT_URL);

	ControlPrinter::StopLogging();
}


String freyja_get_resource_path()
{
	mstl::String s;

#if WIN32
	char cwd[1024];
	SystemIO::File::GetCurrentWorkingDir(cwd, 1024);
	
	s = cwd;
	s += "/";
#else
	char *env = getenv("HOME");
	
	if (!env || !env[0])
	{
		MSTL_MSG("ERROR: Bad HOME envronment\n");
		s = "/usr/share/freyja";
	}
	else
	{
		s = env;
	}
	
	s += "/.freyja/";
#endif

	return s;
}


String freyja_rc_map_string(const char *filename)
{
	String s = freyja_get_resource_path();
	s += filename;
	return s;
}


const char* freyja_rc_map(const char* basename)
{
	String s = freyja_get_resource_path();
	s += basename;
	return mstl::String::Strdup( s.c_str() );
}


void freyja_get_pixmap_filename(char *dest, unsigned int size, char *icon_name)
{
	if (size < 1)
		return;

#if WIN32
	char cwd[1024];
	SystemIO::File::GetCurrentWorkingDir(cwd, 1024);
	snprintf(dest, size, "%s/icons/%s", cwd, icon_name);
#else
	snprintf(dest, size, "%s/.freyja/icons/%s",
			 (char *)getenv("HOME"), icon_name);
#endif

	dest[size-1] = 0;
}


int FreyjaAssertCallbackHandler(const char *file, unsigned int line, 
								const char *function,
								const char *expression, const char *msg)
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


int FreyjaDebugInfoCallbackHandler(const char *file, unsigned int line, 
								   const char *function,
								   const char *expression, const char *msg)
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


int main(int argc, char *argv[])
{
	/* 'Link' up mgtk library stubs to these implementations */
	mgtk_link_import("mgtk_handle_color", (void*)freyja_handle_color);
	mgtk_link_import("mgtk_handle_application_window_close", (void*)freyja_handle_application_window_close);
	mgtk_link_import("mgtk_handle_command", (void*)freyja_handle_command);
	mgtk_link_import("mgtk_handle_command2i", (void*)freyja_handle_command2i);
	mgtk_link_import("mgtk_handle_event1u", (void*)freyja_handle_event1u);
	mgtk_link_import("mgtk_handle_event1f", (void*)freyja_handle_event1f);
	mgtk_link_import("mgtk_handle_gldisplay", (void*)freyja_handle_gldisplay);
	mgtk_link_import("mgtk_handle_glresize", (void*)freyja_handle_glresize);
	mgtk_link_import("mgtk_handle_key_press", (void*)freyja_handle_key_press);
	mgtk_link_import("mgtk_handle_motion", (void*)freyja_handle_motion);
	mgtk_link_import("mgtk_handle_mouse", (void*)freyja_handle_mouse);
	mgtk_link_import("mgtk_handle_resource_start", (void*)freyja_handle_resource_start);
	mgtk_link_import("mgtk_handle_text_array", (void*)freyja_handle_text_array);
	mgtk_link_import("mgtk_handle_text", (void*)freyja_handle_text);
	mgtk_link_import("mgtk_print", (void*)freyja_print);
	mgtk_link_import("mgtk_callback_get_image_data_rgb24", (void*)freyja_callback_get_image_data_rgb24);
	mgtk_link_import("mgtk_get_pixmap_filename", (void*)freyja_get_pixmap_filename);
	mgtk_link_import("mgtk_rc_map", (void*)freyja_rc_map);
	mgtk_link_import("mgtk_get_resource_path", (void*)freyja_get_resource_path_callback);

	/* Enabled logging */
	mstl::String s = freyja_rc_map_string(FREYJA_LOG_FILE);
	ControlPrinter::StartLogging(s.c_str());

	/* Report library versions to log. */	
	ControlPrinter::Log("@ freyja: %s", VERSION);
	ControlPrinter::Log("@ libfreyja: %s", libfreyjaVersion() );
	ControlPrinter::Log("@ libhel: %s", helVersionInfo() );
	ControlPrinter::Log("@ libmgtk: %s", mgtk_version() );

	/* Hookup assert handlers, note freyja assert is also used by this layer */
	freyjaAssertHandler(FreyjaAssertCallbackHandler);
	mgtk_assert_handler(FreyjaAssertCallbackHandler);
	freyjaDebugInfoHandler(FreyjaDebugInfoCallbackHandler);

	/* Hookup resource to event system */
	ResourceEvent::setResource(&FreyjaControl::GetInstance()->GetResource());

	/* Export Lua mgtk functions to libfreyja VM. */
	mgtk_lua_register_functions( freyjaGetLuaVM() );

	mgtk_init(argc, argv);


	// FIXME: Move UI init here...


	/* Load file passed by command line args, CLI parser isn't really needed. */
	if (argc > 1 && argv[1][0] != '-')
	{
		FreyjaControl::GetInstance()->LoadModel(argv[1]);
	}

	mgtk_start();

	return 0;
}






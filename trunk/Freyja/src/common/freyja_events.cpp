/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : SDL/GLUT template
 * Author  : Mongoose
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : 
 * License : No use w/o permission (C) 2001 Mongoose
 * Comments: This is the common event system interface
 *           for freyja
 *
 *           It's independent of the widget set, etc
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2001.06.26:
 * Mongoose - Created
 ==========================================================================*/

#include "FreyjaModel.h"
#include "FreyjaRender.h"
#include "FreyjaControl.h"
#include "MaterialManager.h"
#include "freyja_events.h"


FreyjaModel   *gFreyjaModel = NULL;
FreyjaRender  *RENDER = NULL;
FreyjaControl *gFreyjaControl = NULL;


void freyja_handle_color(int id, float r, float g, float b, float a)
{
	vec4_t color;

	color[0] = r;
	color[1] = g;
	color[2] = b;
	color[3] = a;

	switch (id)
	{
	case 9000:
		gMaterialManager->setColor(MaterialManager::eAmbient, color);
		event_refresh();
		break;
	case 9001:
		gMaterialManager->setColor(MaterialManager::eDiffuse, color);
		event_refresh();
		break;
	case 9002:
		gMaterialManager->setColor(MaterialManager::eSpecular, color);
		event_refresh();
		break;
	case 9003:
		gMaterialManager->setColor(MaterialManager::eEmissive, color);
		event_refresh();
		break;
	case 10000:
		RENDER->setColor(COLOR_VIEW_BG, color);
		break;
	}
}


void freyja_event_new_key_cmd(int key, int event, int cmd)
{
	printf("freyja_event_new_key_cmd> Has been removed, %s:%d\n", 
			 __FILE__, __LINE__);
}


void event_register_control(FreyjaControl *c)
{
	gFreyjaControl = c;
}


void event_register_render(FreyjaRender *r)
{
	RENDER = r;
}


void event_register_model(FreyjaModel *m)
{
	gFreyjaModel = m;
}


void event_custom_color(int custom_color_flags, float r, float g, float b)
{
	if (RENDER)
		RENDER->CustomColor(custom_color_flags, r, g, b);
}


void freyja_event_file_dialog_notify(char *filename)
{
	if (gFreyjaControl)
		gFreyjaControl->handleFilename(filename);
}


void mgtk_event_gldisplay()
{
	if (RENDER)
	{
		RENDER->Display();
	}
	else
	{
		printf("event_display> Call to NULL Renderer\n");
	}
}


void mgtk_event_glresize(unsigned int width, unsigned int height)
{
	if (RENDER)
	{
		RENDER->Reshape(width, height);
	}
	else
	{
		printf("!event_resize> Call to NULL Renderer\n");
	}
}


void freyja_event_key_press(int key, int mod)
{
	printf("!freyja_event_key_press> Has been removed, %s:%d\n", 
			 __FILE__, __LINE__);
}


void event_motion(int x, int y)
{
	if (gFreyjaControl)
	{
		gFreyjaControl->Motion(x, y);
	}
}


void event_mouse(int button, int state, int mod, int x, int y)
{
	if (gFreyjaControl)
	{
		gFreyjaControl->Mouse(button, state, mod, x, y);
	}
}


void freyja_event2i(int event, int cmd)
{
	if (gFreyjaControl)
	{
		gFreyjaControl->handleEvent(event, cmd);
	}
}


FILE *get_log_file()
{
	static FILE *f = 0x0;

	if (!f)
	{
		f = fopen("/tmp/Freyja.log", "w");
	}

	return f;
}


void close_log_file()
{
	if (get_log_file())
		fclose(get_log_file());
}


void event_print_args(char *format, va_list *args)
{
	FILE *f = get_log_file();
	char buffer[1024];
	unsigned int l;


	// Strip message of an trailing carrage return 
	//  and print to stdout and the status bar
	vsnprintf(buffer, 1024, format, *args);
	
	l = strlen(buffer);
  
	if (!l || !buffer[0])
		return;

	if (buffer[l-1] == '\n')
		buffer[l-1] = 0;

#ifdef DEBUG_EVENT_PRINT
	fprintf(stdout, "DEBUG> %s\n", buffer);
#endif

	if (buffer[0] == '!')
	{
		buffer[0] = ' ';
		fprintf(stderr, "%s\n", buffer);
	}

	freyja_event_notify_view_log(buffer);

	if (f)
	{
		fprintf(f, "> ");
		vfprintf(f, format, *args);
		fprintf(f, "\n");
	}
}


void event_print(char *format, ...)
{
	va_list args;

	va_start(args, format);
	event_print_args(format, &args);
	va_end(args);
}


void freyja_event_shutdown()
{
	if (RENDER)
	{
		delete RENDER;
	}

	if (gFreyjaModel)
	{
		delete gFreyjaModel;
	}

	if (gFreyjaControl)
	{
		delete gFreyjaControl;
	}

	event_print("!Thanks for using %s", PROGRAM_NAME);
	event_print("!   Build date: %s @ %s", __DATE__, __TIME__);
	event_print("!   Build host: %s", BUILD_HOST);
	event_print("!   Email addr: %s", EMAIL_ADDRESS);
	event_print("!   Web site  : %s", PROJECT_URL);

	close_log_file();
}


// Mongoose 2002.02.02, This is the backend entry
//   for some damn reason it's started by the fucking
//   gtk_glarea window 'woo hoo'
//
//   Also needs no parms really
//
//   TODO, FIXME: Rewrite all code to focus on backend
//                as main entry and start interface from
//                it, most likey using resource for
//                total gui control
void freyja_event_start()
{
	event_print("@Freyja started...");
	gFreyjaControl = new FreyjaControl();

	/* Mongoose 2002.01.23, Switch to mesh mode */
	freyja_event2i(EVENT_FREYJA_MODE, FREYJA_MODE_MODEL_EDIT);
	event_refresh();

	event_print("Welcome to Freyja %s-%s, %s", VERSION, BUILD_ID, __DATE__);

	/* Mongoose 2002.02.23, Hook for exit() calls */
	atexit(freyja_event_shutdown);
}



char *freyja_rc_map(char *s)
{
	char *rc = NULL;
	char *path = "freyja";
	char *env;
	unsigned int len;
	
	
	if (!s || !s[0])
	{
		return NULL;
	}

	len = strlen(s) + strlen(path);
  
#ifdef unix
	env = getenv("HOME");

	if (!env || !env[0])
	{
		printf("ERROR: Bad HOME envronment\n");
		return NULL;
	}

	len += strlen(env) + 8;

   rc = new char[len + 1];
   snprintf(rc, len, "%s/.%s/%s", env, path, s);
#else
	len += 8;

   rc = new char[len + 1];
   snprintf(rc, len, "C:/%s/%s", path, s);
#endif
	
	return rc;
}


arg_list_t *freyja_rc_color(arg_list_t *args)
{
	arg_list_t *sym, *r, *g, *b;

	sym = symbol();
	symbol_enforce_type(&r, FLOAT);
	symbol_enforce_type(&g, FLOAT);
	symbol_enforce_type(&b, FLOAT);

	// Call event func here - simulated with printf in tests
	if (sym && r && g && b)
	{
#ifdef DEBUG_RESOURCE_COLOR
		printf("extern \"C\" { color(%s, %f, %f, %f); }\n",
				 get_string(sym),
				 get_float(r), get_float(g), get_float(b));
#endif

      if (strcmp(get_string(sym), "COLOR_EDIT_BBOX") == 0)
      {
			event_custom_color(COLOR_EDIT_BBOX, 
									  get_float(r), get_float(g), get_float(b));
      }
      else if (strcmp(get_string(sym), "COLOR_EDIT_BG") == 0)
      {			
			event_custom_color(COLOR_EDIT_BG, 
									  get_float(r), get_float(g), get_float(b));
      }
      else if (strcmp(get_string(sym), "COLOR_VIEW_BG") == 0)
      {
			event_custom_color(COLOR_VIEW_BG, 
									  get_float(r), get_float(g), get_float(b));
      }
      else if (strcmp(get_string(sym), "COLOR_EDIT_POLYGON") == 0)
      {
			event_custom_color(COLOR_EDIT_POLYGON,
									  get_float(r), get_float(g), get_float(b));
      }
      else if (strcmp(get_string(sym), "COLOR_EDIT_LINE") == 0)
      {
			event_custom_color(COLOR_EDIT_LINE, 
									  get_float(r), get_float(g), get_float(b));
      }
      else if (strcmp(get_string(sym), "COLOR_EDIT_TEXT") == 0)
      {
			event_custom_color(COLOR_EDIT_TEXT, 
									  get_float(r), get_float(g), get_float(b));
      }
      else if (strcmp(get_string(sym), "COLOR_EDIT_GRID_8") == 0)
      {
			event_custom_color(COLOR_EDIT_GRID_8, 
									  get_float(r), get_float(g), get_float(b));
      }
      else if (strcmp(get_string(sym), "COLOR_EDIT_LINE_HIGHLIGHT") == 0)
      {
			event_custom_color(COLOR_EDIT_LINE_HIGHLIGHT, 
									  get_float(r), get_float(g), get_float(b));
      }
      else if (strcmp(get_string(sym), "COLOR_EDIT_VERTEX") == 0)
      {
			event_custom_color(COLOR_EDIT_VERTEX, 
									  get_float(r), get_float(g), get_float(b));
      }
      else if (strcmp(get_string(sym), "COLOR_EDIT_VERTEX_HIGHLIGHT") == 0)
      {
			event_custom_color(COLOR_EDIT_VERTEX_HIGHLIGHT, 
									  get_float(r), get_float(g), get_float(b));
      }
	}
	else
	{
		printf("color> Failed to extract strict typed data from script\n");
	}

	delete_arg(&sym);
	delete_arg(&r);
	delete_arg(&g);
	delete_arg(&b);

	return NULL;
}


void freyja_get_pixmap_filename(char *dest, unsigned int size, char *icon_name)
{
	if (size < 1)
		return;

#ifdef unix
	snprintf(dest, size, "%s/.freyja/icons/%s",
			 (char *)getenv("HOME"), icon_name);
#else
	strcpy(dest, "data/icons/%s", icon_name);
#endif

	dest[size-1] = 0;
}




/* -*- Mode: C++; tab-width: 3; indent-tabs-mode: t; c-basic-offset: 3 -*- */
/*================================================================
 * 
 * Project : Freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.westga.edu/~stu7440/
 * Email   : stu7440@westga.edu
 * Object  : FreyjaResource
 * License : No use w/o permission (C) 2002 Mongoose
 * Comments: Resource system for Freyja
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <stu7440@westga.edu>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2002.01.12:
 * Mongoose - Created
 =================================================================*/

#include <stdlib.h>
#include <stdarg.h> 
#include <string.h>
#include "FreyjaResource.h"
#include "freyja_events.h"


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


// Mongoose 2002.01.21, FIXME, implment freyja use
arg_list_t *freyja_rc_key(arg_list_t *args)
{
	arg_list_t *code, *event, *cmd;


	code = symbol();
	event = symbol();
	cmd = symbol();

	arg_enforce_type(&code, INT);
	arg_enforce_type(&event, INT);
	arg_enforce_type(&cmd, INT);

	// Call event func here - simulated with printf in tests
	if (code && event && cmd)
	{
#ifdef DEBUG_RESOURCE_KEY
		printf("extern \"C\" { key(%i, %i, %i); }\n",
				 get_int(code), get_int(event), get_int(cmd));
#endif
		event_new_key_cmd(get_int(code), get_int(event), get_int(cmd));
	}
	else
	{
		printf("key> Failed to extract strict typed data from script\n");
	}

	delete_arg(&code);
	delete_arg(&event);
	delete_arg(&cmd);

	return NULL;
}


////////////////////////////////////////////////////////////


FreyjaResource::FreyjaResource()
{
	//printf("\nFreyjaResource> Bind Resource and C++ function names\n");

	// Mongoose 2002.01.12, Bind script functions to C/C++ functions
	RegisterFunction("window", freyja_rc_window);
	RegisterFunction("gl_widget", freyja_rc_gl_widget);
	RegisterFunction("notebook", freyja_rc_notebook);
	RegisterFunction("tab", freyja_rc_tab);
	RegisterFunction("hsep", freyja_rc_hsep);
	RegisterFunction("vsep", freyja_rc_vsep);
	RegisterFunction("vbox", freyja_rc_vbox);
	RegisterFunction("hbox", freyja_rc_hbox);
	RegisterFunction("handlebox", freyja_rc_handlebox);
	RegisterFunction("textbox", freyja_rc_textbox);
	RegisterFunction("label", freyja_rc_label);
	RegisterFunction("button", freyja_rc_button);
	RegisterFunction("togglebutton", freyja_rc_togglebutton);
	RegisterFunction("spinbutton", freyja_rc_spinbutton);
	RegisterFunction("spinbutton2", freyja_rc_spinbutton2);
	RegisterFunction("color", freyja_rc_color);
	RegisterFunction("key", freyja_rc_key);
	RegisterFunction("menu_item", freyja_rc_menu_item);
	RegisterFunction("submenu", freyja_rc_submenu);
	RegisterFunction("menu_seperator", freyja_rc_menu_seperator);
	RegisterFunction("optionmenu", freyja_rc_optionmenu);
	RegisterFunction("popup_menu", freyja_rc_popup_menu);
	RegisterFunction("menubar", freyja_rc_menubar);
	RegisterFunction("animation_tab_hack", freyja_rc_animation_tab_hack);
	RegisterFunction("toolbar", freyja_rc_toolbar);
	RegisterFunction("toolbar_box", freyja_rc_toolbar_box);
	RegisterFunction("toolbar_togglebutton", freyja_rc_toolbar_togglebutton);
	RegisterFunction("toolbar_button", freyja_rc_toolbar_button);
	//printf("FreyjaResource> Done\n");

	//printf("FreyjaResource> Bind Resource and C++ symbols\n");
	// Mongoose 2002.01.21, Bind some script vars to matching name in C/C++
	RegisterCPPInt("EVENT_MAIN", EVENT_MAIN);
	RegisterCPPInt("CMD_MAIN_NEW_MODEL",     CMD_MAIN_NEW_MODEL);
	RegisterCPPInt("CMD_MAIN_OPEN_MODEL",    CMD_MAIN_OPEN_MODEL);
	RegisterCPPInt("CMD_MAIN_SAVE_MODEL",    CMD_MAIN_SAVE_MODEL);
	RegisterCPPInt("CMD_MAIN_OPEN_TEXTURE",  CMD_MAIN_OPEN_TEXTURE);
	RegisterCPPInt("CMD_MAIN_OPEN_TEXTURE2",  CMD_MAIN_OPEN_TEXTURE2);
	RegisterCPPInt("CMD_MAIN_SHUTDOWN",      CMD_MAIN_SHUTDOWN);
	RegisterCPPInt("CMD_MAIN_NEW_MATERIAL",  CMD_MAIN_NEW_MATERIAL);
	RegisterCPPInt("CMD_MAIN_OPEN_MATERIAL", CMD_MAIN_OPEN_MATERIAL);
	RegisterCPPInt("CMD_MAIN_SAVE_MATERIAL", CMD_MAIN_SAVE_MATERIAL);
	RegisterCPPInt("CMD_MAIN_OPEN_PALETTE",  CMD_MAIN_OPEN_PALETTE);
	RegisterCPPInt("CMD_TOGGLE_FULLSCREEN",  CMD_TOGGLE_FULLSCREEN);

	RegisterCPPInt("EVENT_POINT", EVENT_POINT);
	RegisterCPPInt("CMD_POINT_ADD", CMD_POINT_ADD);
	RegisterCPPInt("CMD_POINT_MOVE", CMD_POINT_MOVE);
	RegisterCPPInt("CMD_POINT_DELETE", CMD_POINT_DELETE);
	RegisterCPPInt("CMD_POINT_COMBINE", CMD_POINT_COMBINE);

	RegisterCPPInt("EVENT_POLYGON", EVENT_POLYGON);
	RegisterCPPInt("CMD_POLYGON_ADD", CMD_POLYGON_ADD);
	RegisterCPPInt("CMD_POLYGON_SELECT", CMD_POLYGON_SELECT);
	RegisterCPPInt("CMD_POLYGON_DELETE", CMD_POLYGON_DELETE);	

	RegisterCPPInt("EVENT_MESH", EVENT_MESH);
	RegisterCPPInt("CMD_MESH_ADD", CMD_MESH_ADD);
	RegisterCPPInt("CMD_MESH_DELETE", CMD_MESH_DELETE);
	RegisterCPPInt("CMD_MESH_MOVE", CMD_MESH_MOVE);
	RegisterCPPInt("CMD_MESH_SELECT", CMD_MESH_SELECT);
	RegisterCPPInt("CMD_MESH_ROTATE", CMD_MESH_ROTATE);
	RegisterCPPInt("CMD_MESH_SCALE", CMD_MESH_SCALE);
	RegisterCPPInt("CMD_MESH_MIRROR", CMD_MESH_MIRROR);
	RegisterCPPInt("CMD_MESH_CLONE", CMD_MESH_CLONE);
	RegisterCPPInt("CMD_MESH_MOVE_CENTER", CMD_MESH_MOVE_CENTER);
	RegisterCPPInt("CMD_MESH_GENERATE_CUBE", CMD_MESH_GENERATE_CUBE);
	RegisterCPPInt("CMD_MESH_GENERATE_TRIS", CMD_MESH_GENERATE_TRIS);
	RegisterCPPInt("CMD_MESH_GENERATE_SPHERE", CMD_MESH_GENERATE_SPHERE);
	RegisterCPPInt("CMD_MESH_GENERATE_CAN", CMD_MESH_GENERATE_CAN);

	RegisterCPPInt("EVENT_BONE", EVENT_BONE);
	RegisterCPPInt("CMD_BONE_NEW", CMD_BONE_NEW);
	RegisterCPPInt("CMD_BONE_ADD_MESH", CMD_BONE_ADD_MESH);
	RegisterCPPInt("CMD_BONE_SELECT", CMD_BONE_SELECT);
	RegisterCPPInt("CMD_BONE_MOVE_PIVOT", CMD_BONE_MOVE_PIVOT);

	RegisterCPPInt("EVENT_MISC", EVENT_MISC);
	RegisterCPPInt("CMD_MISC_GEN_NORMALS", CMD_MISC_GEN_NORMALS);
	RegisterCPPInt("CMD_MISC_ZOOM_IN", CMD_MISC_ZOOM_IN);
	RegisterCPPInt("CMD_MISC_ZOOM_OUT", CMD_MISC_ZOOM_OUT);
	RegisterCPPInt("CMD_MISC_SCREENSHOT", CMD_MISC_SCREENSHOT);
	RegisterCPPInt("CMD_MISC_SELECT", CMD_MISC_SELECT);
	RegisterCPPInt("CMD_MISC_BBOX_SELECT", CMD_MISC_BBOX_SELECT);
	RegisterCPPInt("CMD_MISC_GEN_TEXMAP_XY", CMD_MISC_GEN_TEXMAP_XY);
	RegisterCPPInt("CMD_MISC_VERTEX_UV", CMD_MISC_VERTEX_UV);
	RegisterCPPInt("CMD_MISC_RENDER_GRID", CMD_MISC_RENDER_GRID);
	RegisterCPPInt("CMD_MISC_DISPLAY_INFO", CMD_MISC_DISPLAY_INFO);
	RegisterCPPInt("CMD_MISC_ABOUT", CMD_MISC_ABOUT);

	RegisterCPPInt("EVENT_MATERIAL", EVENT_MATERIAL);


	RegisterCPPInt("EVENT_FREYJA_MODE", EVENT_FREYJA_MODE);
	RegisterCPPInt("FREYJA_MODE_MODEL_EDIT", FREYJA_MODE_MODEL_EDIT);
	RegisterCPPInt("FREYJA_MODE_MATERIAL_EDIT", FREYJA_MODE_MATERIAL_EDIT);
	RegisterCPPInt("FREYJA_MODE_TEXTURE_EDIT", FREYJA_MODE_TEXTURE_EDIT);
	RegisterCPPInt("FREYJA_MODE_ANIMATION", FREYJA_MODE_ANIMATION);
	RegisterCPPInt("FREYJA_MODE_RENDER_WIREFRAME",FREYJA_MODE_RENDER_WIREFRAME);
	RegisterCPPInt("FREYJA_MODE_RENDER_FACE", FREYJA_MODE_RENDER_FACE);
	RegisterCPPInt("FREYJA_MODE_RENDER_POINTS", FREYJA_MODE_RENDER_POINTS);
	RegisterCPPInt("FREYJA_MODE_RENDER_NORMALS", FREYJA_MODE_RENDER_NORMALS);
	RegisterCPPInt("FREYJA_MODE_RENDER_TEXTURE", FREYJA_MODE_RENDER_TEXTURE);
	RegisterCPPInt("FREYJA_MODE_RENDER_LIGHTING", FREYJA_MODE_RENDER_LIGHTING);
	RegisterCPPInt("FREYJA_MODE_RENDER_MATERIAL", FREYJA_MODE_RENDER_MATERIAL);
	RegisterCPPInt("FREYJA_MODE_RENDER_BONETAG",FREYJA_MODE_RENDER_BONETAG);
	RegisterCPPInt("FREYJA_MODE_PLANE_XY", FREYJA_MODE_PLANE_XY);
	RegisterCPPInt("FREYJA_MODE_PLANE_YZ", FREYJA_MODE_PLANE_YZ);
	RegisterCPPInt("FREYJA_MODE_PLANE_XZ", FREYJA_MODE_PLANE_XZ);

	RegisterCPPInt("EVENT_TRANSFORM", EVENT_TRANSFORM);
	RegisterCPPInt("CMD_TRANSFORM_SCALE_DOWN_XYZ",CMD_TRANSFORM_SCALE_DOWN_XYZ);
	RegisterCPPInt("CMD_TRANSFORM_SCALE_UP_XYZ", CMD_TRANSFORM_SCALE_UP_XYZ);

	//printf("FreyjaResource> Done\n");
}


FreyjaResource::~FreyjaResource()
{
}


void FreyjaResource::RegisterCPPInt(char *symbol, int value)
{
	if (RegisterSymbol(symbol, INT, &value))
	{
		printf("ERROR couldn't register new resource symbol %s\n",
				 symbol);
	}
}

////////////////////////////////////////////////////////////
// Test code
////////////////////////////////////////////////////////////

#ifdef __TEST__
int FreyjaResource::_RegressionTest(int argc, char *argv[])
{
	return 0;
}


int main(int argc, char *argv[])
{
  FreyjaResource test;


  printf("[FreyjaResource class test]\n");

  return test._RegressionTest(argc, argv);
}
#endif

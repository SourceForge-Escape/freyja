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


int MOUSE_BTN_PRESSED = 0;
bool MOUSE_QUERY_ACTIVE = true;
FreyjaModel   *gFreyjaModel = NULL;
FreyjaRender  *RENDER = NULL;
FreyjaControl *CONTROL = NULL;



int query_mouse_active()
{
  return MOUSE_QUERY_ACTIVE;
}

int query_mouse_button()
{
  return MOUSE_BTN_PRESSED;
}

int query_load_texture_to_slot()
{
	unsigned int flags;

	flags = gMaterialManager->getGeneralFlags();
	return flags;
}

void event_set_mouse_active(bool b)
{
  MOUSE_QUERY_ACTIVE = b;
}

void event_set_mouse_button(int i)
{
  MOUSE_BTN_PRESSED = i;
}


/* Mongoose 2004.03.26, 
 * This is really a toggle action */
void event_set_load_texture_to_slot(int i)
{
	unsigned int flags;

	flags = gMaterialManager->getGeneralFlags();

	if (i)
	{
		gMaterialManager->setGeneralFlag(MaterialManager::fLoadTextureInSlot);
	}
	else
	{
		gMaterialManager->clearGeneralFlag(MaterialManager::fLoadTextureInSlot);
	}
}

void freyja_event_new_key_cmd(int key, int event, int cmd)
{
	printf("freyja_event_new_key_cmd> Has been removed, %s:%d\n", 
			 __FILE__, __LINE__);
}


void event_register_control(FreyjaControl *c)
{
	CONTROL = c;
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


// Mongoose 2002.02.02, TODO prompt to ask for overwrite ;0
void event_filename(int mode, char *filename)
{
	int failed = 1;
	int type = -1, type2 = -1;


	switch (mode)
	{
	case FREYJA_MODE_LOAD_MATERIAL:
		failed = gMaterialManager->load(filename);
		type = 0;
		type2 = 1;
		break;
	case FREYJA_MODE_SAVE_MATERIAL:
		failed = gMaterialManager->save(filename);
		type = 0;
		type2 = 0;
		break;
	case FREYJA_MODE_LOAD_MODEL:
		failed = gFreyjaModel->loadModel(filename);
		type = 2;
		type2 = 1;
		break;
	case FREYJA_MODE_LOAD_TEXTURE:
		failed = gMaterialManager->loadTexture(filename);
		type = 1;
		type2 = 1;
		event_refresh();
		break;
	case FREYJA_MODE_SAVE_MODEL:
		failed = gFreyjaModel->saveModel(filename);
		type = 2;
		type2 = 0;
		break;
	case FREYJA_MODE_LOAD_PALETTE:
		failed = 0;
		gMaterialManager->loadTexturePalette(filename);
		type = 4;
		type2 = 0;
		break;
	}

	// Mongoose 2002.02.02, Reduce text segment size some  =)
	event_print("%s '%s' %s%s", 
					(type == 0) ? "Material" : 
					(type == 1) ? "Texture" : 
					(type == 2) ? "Model" : 
					(type == 3) ? "Emitter" :
					(type == 4) ?"Palette" :
					"ERROR: No event for ",
					basename(filename),
					(type2 == 0) ? "save " : "load ",
					(failed == 0) ? "[sucess]" : "[failed]");
}


// Mongoose 2002.02.02, This is the backend entry
//   for some damn reason it's started by the fucking
//   gtk_glarea window 'woo hoo'
//
//   TODO, FIXME: Rewrite all code to focus on backend
//                as main entry and start interface from
//                it, most likey using resource for
//                total gui control
void event_init(unsigned int *width, unsigned int *height, 
					 bool *fullscreen, char **driver)
{
	*width  = 740;
	*height = 560;    
	*fullscreen = false;

	CONTROL = new FreyjaControl();

	/* Mongoose 2004.03.26, 
	 * Entry for MaterialManager test pattern */
	MaterialManager::Instance();
}


void event_shutdown()
{
	/* Mongoose 2004.03.26, 
	 * Entry for MaterialManager test pattern */
	MaterialManager::DestroyInstance();

	if (RENDER)
	{
		delete RENDER;
	}

	if (gFreyjaModel)
	{
		delete gFreyjaModel;
	}

	if (CONTROL)
	{
		delete CONTROL;
	}

	printf("\n\n\tThanks for using %s\n", PROGRAM_NAME);
	printf("\tBuild date: %s @ %s\n", __DATE__, __TIME__);
	printf("\tBuild host: %s\n", BUILD_HOST);
	printf("\tEmail addr: %s\n", EMAIL_ADDRESS);
	printf("\tWeb site  : %s\n\n", PROJECT_URL);
}


void event_render_init(unsigned int width, unsigned int height)
{
	if (RENDER)
	{
		RENDER->Init(width, height, true);
	}
	else
	{
		printf("event_render_init> Call to NULL Renderer\n");
	}
}


void event_display()
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


void event_resize(int width, int height)
{
	if (RENDER)
	{
		RENDER->Reshape(width, height);
	}
	else
	{
		printf("event_resize> Call to NULL Renderer\n");
	}
}


void freyja_event_key_press(int key, int mod)
{
	printf("freyja_event_key_press> Has been removed, %s:%d\n", 
			 __FILE__, __LINE__);
}


void event_motion(int x, int y)
{
	if (CONTROL)
	{
		CONTROL->Motion(x, y);
	}
}


void freyja_event2i(int event, int cmd)
{
	if (CONTROL)
	{
		freyja_event2i_interface_listener(event, cmd);
		CONTROL->Event(event, cmd);
	}
}


void event_mouse(int button, int state, int mod, int x, int y)
{
	if (CONTROL)
	{
		CONTROL->Mouse(button, state, mod, x, y);
	}
}


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
FreyjaControl *gFreyjaControl = NULL;



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


void freyja_event2i(int event, int cmd)
{
	if (gFreyjaControl)
	{
		gFreyjaControl->handleEvent(event, cmd);
	}
}


void event_mouse(int button, int state, int mod, int x, int y)
{
	if (gFreyjaControl)
	{
		gFreyjaControl->Mouse(button, state, mod, x, y);
	}
}

void event_shutdown()
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

	printf("\n\n\tThanks for using %s\n", PROGRAM_NAME);
	printf("\tBuild date: %s @ %s\n", __DATE__, __TIME__);
	printf("\tBuild host: %s\n", BUILD_HOST);
	printf("\tEmail addr: %s\n", EMAIL_ADDRESS);
	printf("\tWeb site  : %s\n\n", PROJECT_URL);
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
	atexit(event_shutdown);
}



/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : reverseengineer plugin
 * License : No use without permission (c) 2000-2005 Mongoose
 * Comments: This is the backend of the modeler
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 * 
 *-- History ------------------------------------------------- 
 *
 * 2005.07.23:
 * Mongoose - Created, Based on freyja prototype
 ==========================================================================*/

#include <string.h>
#include <math.h>
#include <hel/math.h>
#include <mstl/String.h>
#include <freyja/FreyjaPluginABI.h>
#include <freyja/FreyjaFileReader.h>
#include <freyja/FreyjaFileWriter.h>
#include <freyja/FreyjaImage.h>
#include <mgtk/ResourceEvent.h>
#include <mgtk/mgtk_events.h>


extern "C" {

	void freyja_reverseengineer_init(void (*func)(const char*, void*));
}

void ReverseEngineerEventsAttach();
void ReverseEngineerGUIAttach();

uint32 gReverseEngineerVOffset = 0;
uint32 gReverseEngineerVCount = 0;
uint32 gReverseEngineerVSkip = 0;
int32 gReverseEngineerMeshId = -1;
char *gReverseEngineerFilename = 0x0;

void freyja_reverseengineer_init(void (*func)(const char*, void*))
{
	ResourceAppPluginTest *plugin;
	plugin = new ResourceAppPluginTest(ReverseEngineerEventsAttach,
										ReverseEngineerGUIAttach);	
}


void eReverseEngineer()
{
	FreyjaFileReader r;
	uint32 i;
	vec_t x, y, z;


	if (r.openFile(gReverseEngineerFilename) == false)
		return;
	
	r.setFileOffset(gReverseEngineerVOffset);

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	gReverseEngineerMeshId = freyjaGetCurrent(FREYJA_MESH);

	for (i = 0; i < gReverseEngineerVCount && !r.endOfFile(); ++i)
	{
		x = r.readFloat32();
		y = r.readFloat32();
		z = r.readFloat32();

		if (gReverseEngineerVSkip > 0)
			r.setFileOffset(r.getFileOffset() + gReverseEngineerVSkip);

		freyjaVertex3f(x, y, z);
	}

	freyjaEnd();
	freyjaEnd();

	r.closeFile();

	mgtk_print("! eReverseEngineer> %ibytes read; %i %i %i", i*12,
               gReverseEngineerVOffset, gReverseEngineerVCount,
               gReverseEngineerVSkip);
	mgtk_event_gl_refresh();
}


void eReverseEngineerDelete()
{
	//freyjaMeshDelete(gReverseEngineerMeshId);
	freyjaModelClear(0);
	mgtk_event_gl_refresh();
}


void eReverseEngineerFilename(char *filename)
{
	if (gReverseEngineerFilename)
		delete [] gReverseEngineerFilename;

	gReverseEngineerFilename = String::strdup(filename);
}


void eReverseEngineerVOffset(unsigned int value)
{
	gReverseEngineerVOffset = value;
}


void eReverseEngineerVCount(unsigned int value)
{
	gReverseEngineerVCount = value;
}


void eReverseEngineerVSkip(unsigned int value)
{
	gReverseEngineerVSkip = value;
}


void eDialogReverseEngineer()
{
	mgtk_event_dialog_visible_set(Resource::mInstance->getIntByName("eDialogReverseEngineer"), 1);
} 


void ReverseEngineerEventsAttach()
{
	ResourceEventCallbackString::add("eReverseEngineerFilename", &eReverseEngineerFilename);
	ResourceEventCallbackUInt::add("eReverseEngineerVOffset", &eReverseEngineerVOffset);
	ResourceEventCallbackUInt::add("eReverseEngineerVCount", &eReverseEngineerVCount);
	ResourceEventCallbackUInt::add("eReverseEngineerVSkip", &eReverseEngineerVSkip);
	ResourceEventCallback::add("eReverseEngineer", &eReverseEngineer);
	ResourceEventCallback::add("eReverseEngineerDelete", &eReverseEngineerDelete);
	ResourceEventCallback::add("eDialogReverseEngineer", &eDialogReverseEngineer);
}


void ReverseEngineerGUIAttach()
{
	char *filename;
	char *basename = "plugins/reverseengineer.mlisp";
	int id, menuId;

	id = Resource::mInstance->getIntByName("eDialogReverseEngineer");
	menuId = Resource::mInstance->getIntByName("ePluginMenu");
	mgtk_append_item_to_menu(menuId, "Reverse Engineer", id);

	filename = mgtk_rc_map(basename);
	Resource::mInstance->Load(filename);
	delete [] filename;
}







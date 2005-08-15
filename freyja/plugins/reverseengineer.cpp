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

uint32 gReverseEngineerISz = 2;
uint32 gReverseEngineerVOffset = 0;
uint32 gReverseEngineerVCount = 0;
uint32 gReverseEngineerVSkip = 0;
uint32 gReverseEngineerFOffset = 0;
uint32 gReverseEngineerFCount = 0;
uint32 gReverseEngineerFSkip = 0;
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
	uint32 i, j, b, vcount, size;
	vec_t x, y, z;
	uint32 aa, bb, cc;


	if (r.openFile(gReverseEngineerFilename) == false)
		return;
	
	r.setFileOffset(gReverseEngineerVOffset);

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	gReverseEngineerMeshId = freyjaGetCurrent(FREYJA_MESH);

	size = r.getFileSize();

	mgtk_print("! eReverseEngineer> %ibytes in file", size);

	for (i = 0, b = 0, vcount=0; i < gReverseEngineerVCount && b < size; ++i)
	{
		x = r.readFloat32();
		y = r.readFloat32();
		z = r.readFloat32();
	
		b += 12;

		if (gReverseEngineerVSkip > 0)
		{
			for (j = 0; j < gReverseEngineerVSkip && b < size; ++j, ++b)
			{
				r.readInt8U();
			}
		}

		++vcount;
		freyjaVertexCreate3f(x, y, z);
	}

	r.setFileOffset(gReverseEngineerFOffset);

	for (i = 0, b = 0; i < gReverseEngineerFCount && b < size; ++i)
	{
		switch (gReverseEngineerISz)
		{
		case 2:
			aa = r.readInt16U();
			bb = r.readInt16U();
			cc = r.readInt16U();
			b += 3*2;
			break;

		case 4:
			aa = r.readInt32U();
			bb = r.readInt32U();
			cc = r.readInt32U();
			b += 3*4;
			break;

		default:
			i = gReverseEngineerFCount;
			continue;
		}

		if (aa > gReverseEngineerVCount ||
			bb > gReverseEngineerVCount ||
			cc > gReverseEngineerVCount)
		{
			mgtk_print("! %i %i %i invalid skipping", aa, bb, cc);
			continue;
		}

		freyjaBegin(FREYJA_POLYGON);
		freyjaPolygonVertex1i(aa);
		freyjaPolygonVertex1i(bb);
		freyjaPolygonVertex1i(cc);
		freyjaEnd();

		if (gReverseEngineerFSkip > 0)
		{
			for (j = 0; j < gReverseEngineerFSkip && b < size; ++j, ++b)
			{
				r.readInt8U();
			}
		}
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


void eReverseEngineerFOffset(unsigned int value)
{
	gReverseEngineerFOffset = value;
}


void eReverseEngineerFCount(unsigned int value)
{
	gReverseEngineerFCount = value;
}


void eReverseEngineerFSkip(unsigned int value)
{
	gReverseEngineerFSkip = value;
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
	ResourceEventCallbackUInt::add("eReverseEngineerFOffset", &eReverseEngineerFOffset);
	ResourceEventCallbackUInt::add("eReverseEngineerFCount", &eReverseEngineerFCount);
	ResourceEventCallbackUInt::add("eReverseEngineerFSkip", &eReverseEngineerFSkip);
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







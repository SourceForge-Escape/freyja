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
#include <mstl/SystemIO.h>
#include <freyja/PluginABI.h>
#include <freyja/ModelABI.h>
#include <freyja/LegacyABI.h>
#include <freyja/FreyjaImage.h>
#include <mgtk/ResourceEvent.h>
#include <mgtk/mgtk_events.h>

using namespace mstl;

extern "C" {

	void freyja_reverseengineer_init(void (*func)(const char*, void*));
}

void ReverseEngineerEventsAttach();
void ReverseEngineerGUIAttach();

uint32 gReverseEngineerISz = 2;
char *gReverseEngineerVString = 0x0;
uint32 gReverseEngineerVOffset = 0;
uint32 gReverseEngineerVCount = 0;
uint32 gReverseEngineerVSkip = 0;
char *gReverseEngineerFString = 0x0;
uint32 gReverseEngineerFOffset = 0;
uint32 gReverseEngineerFCount = 0;
int32 gReverseEngineerMeshId = -1;
char *gReverseEngineerFilename = 0x0;


void freyja_reverseengineer_init(void (*func)(const char*, void*))
{
	mgtk::ResourcePlugin *plugin;
	plugin = new mgtk::ResourcePlugin(ReverseEngineerEventsAttach,
									  ReverseEngineerGUIAttach);	
}


void eReverseEngineer()
{
	SystemIO::FileReader r;
	uint32 i, j, b, count, vCount, size;
	vec_t x, y, z, u, v, tmpf;
	uint32 aa, bb, cc, dd, material = 0, tmp;
	char lastCmd = 0;
	//vec3_t abcd[4];
	bool quad = false, uv = false;


	if (r.Open(gReverseEngineerFilename) == false)
		return;

	freyjaBegin(FREYJA_MESH);
	freyjaBegin(FREYJA_VERTEX_GROUP);

	gReverseEngineerMeshId = freyjaGetCurrent(FREYJA_MESH);

	mgtk_print("! eReverseEngineer> %ibytes in file", size);

	/* Vertex extractor */
	size = r.GetSize();
	r.SetOffset(gReverseEngineerVOffset);
	count = strlen(gReverseEngineerVString);

	for (i = 0, b = 0; i < gReverseEngineerVCount && b < size; ++i)
	{
		switch (gReverseEngineerVString[0]) // Main command
		{
		case 'V':
			quad = false;

			for (j = 1, lastCmd = 0; j < count; ++j)
			{
				switch (gReverseEngineerVString[j])
				{
				case '0':  // for visuallizing UVs and 2d vertices
					tmpf = 0.0f;
					break;

				case '1':  // for visuallizing UVs and 2d vertices
					tmpf = 1.0f;
					break;

				case '2':  // for visuallizing UVs and 2d vertices
					tmpf = 2.0f;
					break;

				case 'u':
					tmpf = r.ReadInt32U();
					b += 4;
					break;

				case 's':
					tmpf = r.ReadInt16U();
					b += 2;
					break;

				case 'b':
					tmpf = r.ReadInt8U();
					b += 1;
					break;

				case 'f':
					tmpf = r.ReadFloat32();
					b += 4;
					break;
				}

				switch (lastCmd)
				{
				case 'X':
					x = tmpf;
					break;

				case 'Y':
					y = tmpf;
					break;

				case 'Z':
					z = tmpf;
					break;

				case 'U':
					u = tmpf;
					uv = true;
					break;

				case 'V':
					v = tmpf;
					uv = true;
					break;

				case '/':
					x /= tmpf;
					y /= tmpf;
					z /= tmpf;
					break;

				case '*':
					x *= tmpf;
					y *= tmpf;
					z *= tmpf;
					break;

				default:
					;
				}

				lastCmd = gReverseEngineerVString[j];
			}

			++vCount;
			freyjaVertexCreate3f(x, y, z);
			break;

		default:
			;
		}

		if (gReverseEngineerVSkip > 0)
		{
			for (j = 0; j < gReverseEngineerVSkip && b < size; ++j, ++b)
			{
				r.ReadInt8U();
			}
		}
	}

	if (!gReverseEngineerFCount && vCount > 3)
	{
		freyjaBegin(FREYJA_POLYGON);
		freyjaPolygonVertex1i(0);
		freyjaPolygonVertex1i(1);
		freyjaPolygonVertex1i(2);
		freyjaEnd();
	}


	/* Face extractor */
	size = r.GetSize();
	r.SetOffset(gReverseEngineerFOffset);
	count = strlen(gReverseEngineerFString);

	for (i = 0, b = 0; i < gReverseEngineerFCount && b < size; ++i)
	{
		switch (gReverseEngineerFString[0]) // Main command
		{
		case 'F':
			quad = false;

			for (j = 1, lastCmd = 0; j < count; ++j)
			{
				switch (gReverseEngineerFString[j])
				{
				case '0':  // for visuallizing UVs and 2d vertices
					tmp = 0;
					break;

				case 'i':
					tmp = r.ReadInt32U();
					b += 4;
					break;

				case 's':
					tmp = r.ReadInt16U();
					b += 2;
					break;

				case 'f':
					r.ReadFloat32();
					b += 4;
					break;
				}

				switch (lastCmd)
				{
				case 'A':
					aa = tmp;
					break;

				case 'B':
					bb = tmp;
					break;

				case 'C':
					cc = tmp;
					break;

				case 'D':
					dd = tmp;
					quad = true;
					break;

				case 'M':
					material = tmp;
					break;

				default:
					;
				}

				lastCmd = gReverseEngineerFString[j];
			}

			if (aa > vCount ||
				bb > vCount ||
				cc > vCount)
			{
				mgtk_print("! A%i B%i C%i invalid skipping", aa, bb, cc);
				continue;
			}
			else
			{
				mgtk_print("! M%i (A%i B%i C%i)", material, aa, bb, cc);
			}

			freyjaBegin(FREYJA_POLYGON);
			freyjaPolygonVertex1i(aa);
			freyjaPolygonVertex1i(bb);
			freyjaPolygonVertex1i(cc);
			if (quad) freyjaPolygonVertex1i(dd);
			freyjaEnd();
			break;

		default:
			;
		}
	}

	freyjaEnd();
	freyjaEnd();

	r.Close();

	mgtk_print("! eReverseEngineer> %ibytes read; voff %i, vcount %i, vskip %i", i*12,
               gReverseEngineerVOffset, gReverseEngineerVCount,
               gReverseEngineerVSkip);
	mgtk_event_gl_refresh();
}


void eReverseEngineerDelete()
{
	//freyjaMeshDelete(gReverseEngineerMeshId);
	freyjaModelClear(0);
	gReverseEngineerMeshId = -1;
	mgtk_event_gl_refresh();
}


void eReverseEngineerFilename(char *filename)
{
	if (gReverseEngineerFilename)
		delete [] gReverseEngineerFilename;

	gReverseEngineerFilename = String::Strdup(filename);
}


void eReverseEngineerVString(char *s)
{
	if (gReverseEngineerVString)
		delete [] gReverseEngineerVString;

	gReverseEngineerVString = String::Strdup(s);
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


void eReverseEngineerFString(char *s)
{
	if (gReverseEngineerFString)
		delete [] gReverseEngineerFString;

	gReverseEngineerFString = String::Strdup(s);
}


void eReverseEngineerFOffset(unsigned int value)
{
	gReverseEngineerFOffset = value;
}


void eReverseEngineerFCount(unsigned int value)
{
	gReverseEngineerFCount = value;
}


void eDialogReverseEngineer()
{
	mgtk_event_dialog_visible_set(Resource::mInstance->getIntByName("eDialogReverseEngineer"), 1);
} 


void ReverseEngineerEventsAttach()
{
	ResourceEventCallbackString::add("eReverseEngineerFilename", &eReverseEngineerFilename);


	ResourceEventCallbackString::add("eReverseEngineerVString", &eReverseEngineerVString);
	ResourceEventCallbackUInt::add("eReverseEngineerVOffset", &eReverseEngineerVOffset);
	ResourceEventCallbackUInt::add("eReverseEngineerVCount", &eReverseEngineerVCount);
	ResourceEventCallbackUInt::add("eReverseEngineerVSkip", &eReverseEngineerVSkip);


	ResourceEventCallbackString::add("eReverseEngineerFString", &eReverseEngineerFString);
	ResourceEventCallbackUInt::add("eReverseEngineerFOffset", &eReverseEngineerFOffset);
	ResourceEventCallbackUInt::add("eReverseEngineerFCount", &eReverseEngineerFCount);

	ResourceEventCallback::add("eReverseEngineer", &eReverseEngineer);
	ResourceEventCallback::add("eReverseEngineerDelete", &eReverseEngineerDelete);
	ResourceEventCallback::add("eDialogReverseEngineer", &eDialogReverseEngineer);
}


void ReverseEngineerGUIAttach()
{
	char *filename;
	char *basename = "plugins/reverseengineer.mlisp";
	int id, menuId;
	uint32 e;


	id = Resource::mInstance->getIntByName("eDialogReverseEngineer");
	menuId = Resource::mInstance->getIntByName("ePluginMenu");
	//mgtk_append_item_to_menu(menuId, "Reverse Engineer", id);

	filename = mgtk_rc_map(basename);
	Resource::mInstance->Load(filename);
	delete [] filename;

	e = resourceGetEventId1s("eReverseEngineerFString");
	mgtk_textentry_value_set(e, "FAsBsCs");

	e = resourceGetEventId1s("eReverseEngineerVString");
	mgtk_textentry_value_set(e, "VXfZfYf");
}







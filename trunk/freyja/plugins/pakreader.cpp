/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://icculus.org/freyja
 * Email   : mongoose@icculus.org
 * Object  : pakreader plugin
 * License : No use without permission (c) 2000-2005 Mongoose
 * Comments: This is the backend of the modeler
 *
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
#include <freyja/FreyjaPluginABI.h>
#include <freyja/FreyjaFileReader.h>
#include <freyja/FreyjaFileWriter.h>
#include <freyja/FreyjaPakReader.h>
#include <freyja/FreyjaImage.h>
#include <mgtk/ResourceEvent.h>
#include <mgtk/mgtk_events.h>


extern "C" {

	void freyja_pakreader_init(void (*func)(const char*, void*));
}

void PakReaderEventsAttach();
void PakReaderGUIAttach();

uint32 gPakReaderUID = 0;
FreyjaPakDirectory *gPakReaderDir = 0x0;

void freyja_pakreader_init(void (*func)(const char*, void*))
{
	ResourceAppPluginTest *plugin;
	plugin = new ResourceAppPluginTest(PakReaderEventsAttach,
										PakReaderGUIAttach);	
}


void ePakReaderMenuUpdate()
{
	FreyjaPakReader *pak = freyjaGetPakReader(gPakReaderUID);
	FreyjaPakDirectory *dir, *tmpDir;
	FreyjaPakFile *file;
	uint32 menu = Resource::mInstance->getIntByName("ePakReaderMenu");
	uint32 event = Resource::mInstance->getIntByName("ePakReaderSelect");
	uint32 i, j, count;


	if (pak == 0x0)
		return;

	gPakReaderDir = dir = pak->getRoot();

	mgtk_remove_all_items_to_menu(menu);

	for (i = 0, count = dir->getDirCount(); i < count; ++i)
	{
		tmpDir = dir->getPakDir(i);
	
		if (tmpDir)
		{
			mgtk_append_item_to_menu2i(menu, tmpDir->getName(), event, i);
			freyjaPrintMessage("! %s/", tmpDir->getName());
		}
	}

	for (j = 0, count = dir->getFileCount(); j < count; ++j)
	{
		file = dir->getPakFile(j);

		if (file)
		{
			mgtk_append_item_to_menu2i(menu, file->getName(), event, i+j);
			freyjaPrintMessage("! %s", file->getName());
		}
	}
}


void ePakReaderMenu()
{
}


void pak_reader_try_upload(char *filename)
{
	FreyjaImage img;


	mgtk_print("!PakReader> Trying to upload '%s'", filename);

	if (!img.loadImage(filename))
	{
		uint32 tid;
		uint32 id = Resource::mInstance->getIntByName("eTextureUpload");
		uint32 w = img.getWidth();
		uint32 h = img.getHeight();
		byte *image;
		bool done = false;

		img.getImage(&image);

		switch (img.getColorMode())
		{
		case FreyjaImage::RGBA_32:
			tid = freyjaTextureCreateBuffer(image, 4, w, h, RGBA_32);
			done = true;
			break;

		case FreyjaImage::RGB_24:
			tid = freyjaTextureCreateBuffer(image, 3, w, h, RGB_24);
			done = true;
			break;

		case FreyjaImage::INDEXED_8:
			tid = freyjaTextureCreateBuffer(image, 1, w, h, INDEXED_8);
			done = true;
			break;

		default:
			;
		}
		
		if (image != 0x0)
			delete [] image;
		
		if (done)
		{
			mgtk_print("!Uploading texture %i, using function %i...", tid, id);

			if (ResourceEvent::listen(id - 10000, tid))
				mgtk_print("!ePakReader generated texture successfully.");

			freyjaTextureDelete(tid);
			return;
		}
	}

	uint32 event = Resource::mInstance->getIntByName("eModelUpload");

	if (ResourceEvent::listen(event - 10000, filename))
	{
		mgtk_print("!ePakReader loaded model '%s' successfully.", filename);
		mgtk_event_gl_refresh();
	}
}


void ePakReaderSelect(unsigned int value)
{
	FreyjaPakReader *pak = freyjaGetPakReader(gPakReaderUID);
	FreyjaPakDirectory *dir, *tmpDir;
	FreyjaPakFile *file;
	uint32 menu = Resource::mInstance->getIntByName("ePakReaderMenu");
	uint32 event = Resource::mInstance->getIntByName("ePakReaderSelect");
	uint32 i, j, count;
	byte *buffer;


	if (pak == 0x0)
		return;

	freyjaPrintMessage("! ePakReaderSelect(%i)", value);

	if (!gPakReaderDir)
		gPakReaderDir = pak->getRoot();

	dir = gPakReaderDir;

	mgtk_remove_all_items_to_menu(menu);

	if (value < dir->getDirCount())
	{
		gPakReaderDir = dir = dir->getPakDir(value);
	}
	else
	{
		FreyjaFileReader r;
		FreyjaFileWriter w;
		char tmpfilename[256];


		if (r.openFile(pak->getPakFile()))
		{
			file = dir->getPakFile(value - dir->getDirCount());
			buffer = file->getCopyOfData(r);

			// FIXME simple minded test importer
			if (buffer != 0x0)
			{
#ifdef WIN32
				snprintf(tmpfilename, 255, "C:\temp/%s", file->getName());
#else
				snprintf(tmpfilename, 255, "/tmp/%s", file->getName());
#endif
				w.openFile(tmpfilename);
				w.writeBuffer(file->getDataSize(), buffer);
				w.closeFile();

				mgtk_print("Wrote '%s' from pak", tmpfilename);

				delete [] buffer;

				pak_reader_try_upload(tmpfilename);
			}

			r.closeFile();
		}

		return;  // don't update menu
	}

	for (i = 0, count = dir->getDirCount(); i < count; ++i)
	{
		tmpDir = dir->getPakDir(i);
	
		if (tmpDir)
		{
			mgtk_append_item_to_menu2i(menu, tmpDir->getName(), event, i);
			freyjaPrintMessage("! %s/", tmpDir->getName());
		}
	}

	for (j = 0, count = dir->getFileCount(); j < count; ++j)
	{
		file = dir->getPakFile(j);

		if (file)
		{
			mgtk_append_item_to_menu2i(menu, file->getName(), event, i+j);
			freyjaPrintMessage("! %s", file->getName());
		}
	}
}


void eDialogPakReader()
{
	mgtk_event_dialog_visible_set(Resource::mInstance->getIntByName("eDialogPakReader"), 1);
} 


void PakReaderEventsAttach()
{
	ResourceEventCallbackUInt::add("ePakReaderSelect", &ePakReaderSelect);
	ResourceEventCallback::add("ePakReaderMenu", &ePakReaderMenu);
	ResourceEventCallback::add("eDialogPakReader", &eDialogPakReader);
	ResourceEventCallback::add("ePakReaderMenuUpdate", &ePakReaderMenuUpdate);
}


void PakReaderGUIAttach()
{
	char *filename;
	char *basename = "plugins/pakreader.mlisp";
	int id, menuId;

	id = Resource::mInstance->getIntByName("eDialogPakReader");
	menuId = Resource::mInstance->getIntByName("ePluginMenu");
	mgtk_append_item_to_menu(menuId, "PakReader", id);

	filename = mgtk_rc_map(basename);
	Resource::mInstance->Load(filename);
	delete [] filename;
}







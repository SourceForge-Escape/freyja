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
#include <freyja/PluginABI.h>
#include <freyja/TextureABI.h>
#include <freyja/FreyjaPakReader.h>
#include <freyja/FreyjaImage.h>
#include <mgtk/ResourceEvent.h>
#include <mgtk/mgtk_events.h>
#include <mstl/SystemIO.h>

using namespace mstl;
using namespace mgtk;


extern "C" {

	void freyja_pakreader_init(void (*func)(const char*, void*));
}

void PakReaderEventsAttach();
void PakReaderGUIAttach();

uint32 gPakReaderUID = 0;
FreyjaPakDirectory *gPakReaderDir = 0x0;
int gPakReaderFiles = -1;
int gPakReaderDirs = -1;

void freyja_pakreader_init(void (*func)(const char*, void*))
{
	ResourcePlugin *plugin;
	plugin = new ResourcePlugin(PakReaderEventsAttach, PakReaderGUIAttach);	
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

	mgtk_tree_t *dirs = mgtk_event_tree_new(0, "Dirs");

	for (i = 0, count = dir->getDirCount(); i < count; ++i)
	{
		tmpDir = dir->getPakDir(i);
	
		if (tmpDir)
		{
			mgtk_event_tree_add_new_child(dirs, i+1, tmpDir->getName());
			mgtk_append_item_to_menu2i(menu, tmpDir->getName(), event, i);
			freyjaPrintMessage("! %s/", tmpDir->getName());
		}
	}

	MSTL_MSG("*** Update %i %p", gPakReaderDirs, dirs);
	mgtk_event_update_tree(gPakReaderDirs, dirs);

	mgtk_tree_t *files = mgtk_event_tree_new(0, "Files");

	for (j = 0, count = dir->getFileCount(); j < count; ++j)
	{
		file = dir->getPakFile(j);

		if (file)
		{
			mgtk_event_tree_add_new_child(files, i+j+1, file->getName());
			mgtk_append_item_to_menu2i(menu, file->getName(), event, i+j);
			freyjaPrintMessage("! %s", file->getName());
		}
	}

	mgtk_event_update_tree(gPakReaderFiles, files);
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

	uint32 event = Resource::mInstance->getIntByName("EvOpenModel");

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
	uint32 menu = ResourceEvent::GetResourceIdBySymbol("ePakReaderMenu");
	uint32 event = ResourceEvent::GetResourceIdBySymbol("ePakReaderSelect");
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
		SystemIO::FileReader r;
		SystemIO::FileWriter w;
		char tmpfilename[256];

		if (r.Open(pak->getPakFile()))
		{
			file = dir->getPakFile(value - dir->getDirCount());

			if (file)
			{
				buffer = file->getCopyOfData(r);
			}
			else
			{
				buffer = NULL;
				mgtk_print("%s(): ERROR virtual file invalid.", __func__);
			}
			
			// FIXME simple minded test importer
			if (buffer != 0x0)
			{
#ifdef WIN32
				mkdir("utpak");
				snprintf(tmpfilename, 255, "utpak/%s", file->getName());
#else
				snprintf(tmpfilename, 255, "/tmp/%s", file->getName());
#endif
				w.Open(tmpfilename);
				w.WriteBuffer(file->getDataSize(), buffer);
				w.Close();

				mgtk_print("Wrote '%s' from pak", tmpfilename);

				delete [] buffer;

				pak_reader_try_upload(tmpfilename);
			}

			r.Close();
		}

		return;  // don't update menu
	}

	mgtk_tree_t *dirs = mgtk_event_tree_new(0, "Dirs");

	for (i = 0, count = dir->getDirCount(); i < count; ++i)
	{
		tmpDir = dir->getPakDir(i);
	
		if (tmpDir)
		{
			mgtk_event_tree_add_new_child(dirs, i+1, tmpDir->getName());
			mgtk_append_item_to_menu2i(menu, tmpDir->getName(), event, i);
			freyjaPrintMessage("! %s/", tmpDir->getName());
		}
	}

	MSTL_MSG("*** Update %i %p", gPakReaderDirs, dirs);
	mgtk_event_update_tree(gPakReaderDirs, dirs);

	mgtk_tree_t *files = mgtk_event_tree_new(0, "Files");

	for (j = 0, count = dir->getFileCount(); j < count; ++j)
	{
		file = dir->getPakFile(j);

		if (file)
		{
			mgtk_event_tree_add_new_child(files, i+j+1, file->getName());
			mgtk_append_item_to_menu2i(menu, file->getName(), event, i+j);
			freyjaPrintMessage("! %s", file->getName());
		}
	}

	mgtk_event_update_tree(gPakReaderFiles, files);
}


void eDialogPakReader()
{
	mgtk_event_dialog_visible_set(Resource::mInstance->getIntByName("eDialogPakReader"), 1);
} 


void ePakReaderTreeSelect(unsigned int value)
{
	ePakReaderSelect(value-1);
}


void ePakReaderTextEvent(char *text)
{
	FREYJA_INFOMSG(0, "Not implemented.\nNot handled '%s'.", text);
}


void ePakReaderOpenPak()
{
	char *path = mgtk_rc_map("/");
	char *filename =
	mgtk_filechooser_blocking("freyja - Open Pak...", path, 0,
							  "All Files (*.*)", "*.*");

	if (path) 
	{
		delete [] path;
	}

	ePakReaderTextEvent(filename);

	mgtk_filechooser_blocking_free(filename);
}


void PakReaderEventsAttach()
{
	ResourceEventCallbackString::add("eSetCurrentPakDirname", &ePakReaderTextEvent);
	ResourceEventCallbackString::add("eSetCurrentPakFilename", &ePakReaderTextEvent);
	ResourceEventCallback::add("ePakReaderPluginMenu", &ePakReaderMenu);

	ResourceEventCallbackUInt::add("ePakReaderFiles",&ePakReaderTreeSelect);
	gPakReaderFiles = ResourceEvent::GetResourceIdBySymbol("ePakReaderFiles");
	MSTL_MSG("*** gPakReaderFiles = %i", gPakReaderFiles);

	ResourceEventCallbackUInt::add("ePakReaderDirs", &ePakReaderTreeSelect);
	gPakReaderDirs = ResourceEvent::GetResourceIdBySymbol("ePakReaderDirs");
	MSTL_MSG("*** gPakReaderDirs = %i", gPakReaderDirs);

	ResourceEventCallbackUInt::add("ePakReaderSelect", &ePakReaderSelect);
	ResourceEventCallback::add("ePakReaderMenu", &ePakReaderMenu);
	ResourceEventCallback::add("eDialogPakReader", &eDialogPakReader);
	ResourceEventCallback::add("ePakReaderMenuUpdate", &ePakReaderMenuUpdate);
	ResourceEventCallback::add("ePakReaderOpenPak", &ePakReaderOpenPak);
}


void PakReaderGUIAttach()
{
	//int id = Resource::mInstance->getIntByName("eDialogPakReader");

	int menu = Resource::mInstance->getIntByName("ePluginMenu");
	int submenu = Resource::mInstance->getIntByName("ePakReaderPluginMenu");
	mgtk_append_menu_to_menu(menu, "PakReader", submenu);

	int item = Resource::mInstance->getIntByName("ePakReaderOpenPak");
	mgtk_append_item_to_menu(submenu, "Open Pak...", item);

	char *filename = mgtk_rc_map("plugins/pakreader.mlisp");
	Resource::mInstance->Load(filename);
	delete [] filename;
}







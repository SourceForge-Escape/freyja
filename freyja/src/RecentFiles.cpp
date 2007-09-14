/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : Freyja
 * Author  : Mongoose
 * Website : http://icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2007 Mongoose
 * Comments: Recent files list abstraction
 *
 * 
 *-- History ------------------------------------------------- 
 *
 * 2007.06.15:
 * Mongoose - Created, moved into its own class from FreyjaControl.
 ==========================================================================*/



#include "RecentFiles.h"

#include <mgtk/Resource.h>

using namespace mstl;


void RecentFiles::AddFilename(const char *filename)
{
	bool found = false;
	uint32 idx;


	if (!filename || !filename[0] || 
		!SystemIO::File::DoesFileExist(filename))
	{
		return;
	}

	// 2007.06.16, Mongoose - 
	// Auto update path here to ensure it's always synced
	// even from disk load.
	SetPathByFilename(filename);

	for (uint32 i = mRecentFiles.begin(), n = mRecentFiles.end(); i < n; ++i)
	{
		if (mRecentFiles[i] == filename)
		{
			idx = i;
			found = true;
			break;
		}
	}


	/* 'Boost' this file to top slot, push others down one */
	if (found)
	{
		// Already at top, no change to menu
		if (idx == 0)
			return;

		String swap, old;
		uint32 n = mRecentFiles.end();
		swap = mRecentFiles[0];
		mRecentFiles[0] = mRecentFiles[idx];

		for (uint32 i = 1; i < n; ++i)
		{
			if (i > idx)
			{
				break;
			}

			old = mRecentFiles[i];
			mRecentFiles[i] = swap;
			swap = old;
		}
	}
	else  /* Add new file to top slot, push others down one */
	{
		String swap, old;
		String insert(filename);

		// Bubble up hack
		if (mRecentFiles.end() >= mLimit)
		{
			swap = insert;
			uint32 n = mRecentFiles.end();
			for (uint32 i = mRecentFiles.begin(); i < n; ++i)
			{				
				old = mRecentFiles[i];
				mRecentFiles[i] = swap;
				swap = old;
			}
		}
		else
		{
			mRecentFiles.push_back(insert);
		}
	}

	/* Rebuild menu in order of mRecentFiles */
	uint32 menuId = Resource::mInstance->getIntByName( mMenu.c_str() );
	freyja_remove_all_items_from_menu(menuId);
		
	uint32 n = mRecentFiles.end();
	for (uint32 i = mRecentFiles.begin(); i < n; ++i)
	{
		mgtk_append_item_to_menu2i(menuId, mRecentFiles[i].c_str(), menuId, i);
	}


	/* Save recent_files to disk. */
	SystemIO::TextFileWriter w;

	if ( w.Open( freyja_rc_map_string( mFilename.c_str() ).c_str() ) )
	{
		String swap;
		uint32 n = mRecentFiles.end();
		for (uint32 i = mRecentFiles.begin(); i < n; ++i)
		{
			swap = mRecentFiles[i];
			
			if ( !swap.empty() )
			{
				w.Print("%s\n", swap.c_str() );
			}
		}

		w.Close();
	}
}


bool RecentFiles::LoadResource(const char *filename)
{
	/* Recent files persistance */
	SystemIO::TextFileReader r;
	
	if (r.Open(filename))
	{
		for (uint32 j = 0; j < mLimit && !r.IsEndOfFile(); ++j)
		{
			const char *sym = r.GetLine();
			AddFilename(sym);
		}
		
		r.Close();
		return true;
	}

	return false;
}


void RecentFiles::SetPathByFilename(const char *filename)
{
	if (!filename || !filename[0])
		return;

	mPath = filename;

	int off = mPath.find_last_of('/');

	if (off > 0)
	{
		mPath[off] = 0;
	}
}

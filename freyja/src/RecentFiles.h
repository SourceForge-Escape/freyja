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

#ifndef GUARD__FREYJA3D_RECENTFILES_H_
#define GUARD__FREYJA3D_RECENTFILES_H_

#include <mstl/String.h>
#include <mstl/Vector.h>
#include <mstl/SystemIO.h>

#include "freyja_events.h"

class RecentFiles
{
 public:

	RecentFiles(const char *filename, const char *menu) :
		mLimit(10), 
		mFilename(filename), mMenu(menu), mPath(),
		mRecentFiles() {}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	~RecentFiles() {}
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void AddFilename(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	const char *GetResourceFilename() { return mFilename.c_str(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	const char *GetFilename(unsigned int i) 
	{ return mRecentFiles[i].c_str(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	const char *GetPath() { return mPath.c_str(); }
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool LoadResource();
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	bool LoadResource(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/

	void SetPathByFilename(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 *
	 ------------------------------------------------------*/


protected:

	unsigned int mLimit;               /* Cutoff for list size. */

	mstl::String mFilename;            /* Recent files list serialized. */

	mstl::String mMenu;                /* mgtk menu (symbol) to update. */

	mstl::String mPath;                /* Keep track of filedialog path */

	mstl::Vector<mstl::String> mRecentFiles; /* Recent files list. */
};


inline
bool RecentFiles::LoadResource()
{
	SetPathByFilename( freyja_rc_map_string( "testpak" ).c_str() );
	return LoadResource( freyja_rc_map_string( mFilename.c_str() ).c_str() );
}

#endif // GUARD__FREYJA3D_RECENTFILES_H_

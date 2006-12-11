/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/freyja/
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2005,2006 Mongoose
 * Comments: This is the ABI to expose Pak VFS class and utils to C.
 *
 * 
 *-- History ------------------------------------------------- 
 *
 * 2006.12.09:
 * Mongoose - Created, Split from FreyjaPluginABI.cpp
 ==========================================================================*/

#ifndef GUARD__FREYJA_PAKABI_H_
#define GUARD__FREYJA_PAKABI_H_

#include "freyja.h"

extern "C" {

	///////////////////////////////////////////////////////////////////////
	//  Pak VFS ABI 0.9.3-0.9.5
	///////////////////////////////////////////////////////////////////////

	index_t freyjaPakBegin(const char *filename);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Starts a new VFS from a 'pak file'
	 *        Returns vfs index
	 *
	 ------------------------------------------------------*/

	void freyjaPakAddDecoderFunction2s(const char *module, const char *symbol);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Used to decrypt or uncompress files in a pak
	 *        using an external module and one of it's
	 *        C accessable functions using freyjaPak ABI.
	 *
	 *        NOT IMPLEMENTED!
	 *
	 ------------------------------------------------------*/

	void freyjaPakAddFullPathFile(index_t pakIndex, const char *vfsFilename,
								  int32 offset, int32 size);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Adds a new entry to VFS mapping a chunk from
	 *        offset to offset+size as a file named vfsFilename
	 *
	 *        Returns 1 if it is successfully 'loaded'
	 *
	 ------------------------------------------------------*/

	void freyjaPakEnd(index_t pakIndex);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Finalizes VFS for pakIndex
	 *
	 ------------------------------------------------------*/
}

#endif // GUARD__FREYJA_PAKABI_H_

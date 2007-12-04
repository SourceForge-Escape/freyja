/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : libmgtk
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://www.icculus.org/~mongoose/
 * Email   : mongoose@icculus.org
 * Object  : 
 * License : No use w/o permission (c) 2000-2004 Mongoose
 * Comments: This is the place where callbacks are 'linked' 
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 *
 *-- TODO ---------------------------------------------------- 
 *
 * This may be replaced with python or mlisp glue later, but for now it's
 * proven and stable code that's easy to use cross platform / languages.
 * 
 *-- History ------------------------------------------------- 
 *
 * 2007.04.01:
 * Mongoose - Moved callback linker into it's own file, minor API clean up.
 *
 * 2004.10.27:
 * Mongoose - Major API clean up, documentation update
 *
 * 2000.09.16:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__MGTK_LINKER_H_
#define GUARD__MGTK_LINKER_H_

extern "C" {

	int mgtk_link_import(const char *symbol, void *func);
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets function pointer <func> to matching <symbol>.
	 *        Returns 0 if callback 'link' failed.
	 ------------------------------------------------------*/
}

#endif // GUARD__MGTK_LINKER_H_

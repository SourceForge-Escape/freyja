/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2005 Mongoose
 * Comments: This is the Python ABI.
 *
 *
 *           This file was generated using Mongoose's C++ 
 *           template generator script.  <mongoose@icculus.org>
 *
 *-- History ------------------------------------------------ 
 *
 * 2006.12.11:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__FREYJA_PYTHONABI_H_
#define GUARD__FREYJA_PYTHONABI_H_

#include "freyja.h"

extern "C" {

	////////////////////////////////////////////////////////////////
	// Python ABI 0.9.5
	////////////////////////////////////////////////////////////////

	void freyjaPython1s(const char *plugin, const char *symbol, const char *s);
	/*------------------------------------------------------
	 * Pre  : <plugin> is the filename of the python plugin
	 *        <symbol> is the python varible name for <s>
	 *        <s> is the actual string data passed to the plugin
	 * 
	 * Post : 
	 *
	 ------------------------------------------------------*/

}

#endif // GUARD__FREYJA_PYTHONABI_H_

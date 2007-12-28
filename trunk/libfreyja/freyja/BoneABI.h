/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2005-2007 Mongoose
 * Comments: This is the Bone C ABI.
 *
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.12.12:
 * Mongoose - Refactored.
 *
 * 2005.07.12:
 * Mongoose - Created
 ==========================================================================*/

#ifndef GUARD__FREYJA_BONEABI_H_
#define GUARD__FREYJA_BONEABI_H_

#include "freyja.h"


extern "C" {

	///////////////////////////////////////////////////////////////////////
	// freyjaBone 0.10.0 ABI
	//
	// Types: Bone, Node, XMLSerializer
	///////////////////////////////////////////////////////////////////////

	freyja_ptr freyjaBoneCreate( freyja_ptr skeleton, const char* name ); 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns pointer to Bone or NULL.
	 ------------------------------------------------------*/

	void freyjaBoneDelete( freyja_ptr bone );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/


} // extern "C"

#endif // GUARD__FREYJA_BONEABI_H_

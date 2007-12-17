/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2005-2007 Mongoose
 * Comments: This is the Skeleton C ABI.
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

#ifndef GUARD__FREYJA_SKELETONABI_H_
#define GUARD__FREYJA_SKELETONABI_H_

#include "freyja.h"


extern "C" {

	///////////////////////////////////////////////////////////////////////
	// Skeleton 0.10.0 ABI
	//
	///////////////////////////////////////////////////////////////////////

	freyja_ptr freyjaSkeletonCreate( const char* name );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns pointer to skeleton.
	 ------------------------------------------------------*/

	void freyjaSkeletonDelete( freyja_ptr skeleton );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Deletes skeleton and removes it from resources.
	 ------------------------------------------------------*/

	freyja_ptr freyjaSkeletonGetBone( freyja_ptr skeleton, freyja_id bone ); 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns pointer to Bone or NULL.
	 ------------------------------------------------------*/

	uint32 freyjaSkeletonGetBoneCount( freyja_ptr skeleton );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : 
	 ------------------------------------------------------*/

	freyja_ptr freyjaSkeletonGetRoot( freyja_ptr skeleton );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns root bone or NULL.
	 ------------------------------------------------------*/

	
	///////////////////////////////////////////////////////////////////////
	// freyjaSkeletonSet
	///////////////////////////////////////////////////////////////////////

	void freyjaSkeletonSetRoot( freyja_ptr skeleton, freyja_ptr bone );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets root bone.
	 ------------------------------------------------------*/

} // extern "C"

#endif // GUARD__FREYJA_SKELETONABI_H_

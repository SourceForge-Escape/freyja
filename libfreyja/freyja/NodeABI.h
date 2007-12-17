/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: t; c-basic-offset: 4 -*- */
/*===========================================================================
 * 
 * Project : freyja
 * Author  : Terry 'Mongoose' Hendrix II
 * Website : http://icculus.org/freyja
 * Email   : mongooseichiban@gmail.com
 * Object  : 
 * License : No use w/o permission (C) 2005-2007 Mongoose
 * Comments: This is the Node C ABI.
 *
 *-- History ------------------------------------------------ 
 *
 * 2007.12.12:
 * Mongoose - Refactored.
 *
 * 2006.07.09:
 * Mongoose - Created, moved from other headers
 ==========================================================================*/

#ifndef GUARD__FREYJA_NODEABI_H_
#define GUARD__FREYJA_NODEABI_H_

#include "freyja.h"


extern "C" {

	///////////////////////////////////////////////////////////////////////
	// Node 0.10 ABI
	//
	///////////////////////////////////////////////////////////////////////

	freyja_ptr freyjaNodeDuplicate( freyja_id node, byte children ); 
	/*------------------------------------------------------
	 * Pre  : If children is non-zero a subgraph will be duplicated.
	 * Post : Returns pointer to clone or NULL.
	 ------------------------------------------------------*/

	byte freyjaNodeGetFlags( freyja_ptr node );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns option bitflags for <node>.
	 ------------------------------------------------------*/

	const char* freyjaNodeGetName( freyja_ptr node );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set human readable name for node.
	 ------------------------------------------------------*/

	freyja_ptr freyjaNodeGetParent( freyja_id node ); 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Returns pointer to parent or NULL if none.
	 ------------------------------------------------------*/

	void freyjaNodeGetPosition( freyja_ptr node, vec3_t xyz );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Gets <node> attribute position.
	 ------------------------------------------------------*/

	byte freyjaNodeIsSelected( freyja_ptr node );
	/*------------------------------------------------------
	 * Pre  :  
	 * Post : Checks flags for a selection set.
	 ------------------------------------------------------*/

	// FIXME: Could be World / Parent / Local relative!
	void freyjaNodeSetPosition3f( freyja_ptr node, vec_t x, vec_t y, vec_t z );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set parent relative position.
	 ------------------------------------------------------*/

	void freyjaNodeSetOrientationQuat4f( freyja_ptr node,
	                                     vec_t w, vec_t x, vec_t y, vec_t z );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set parent relative orientation.
	 ------------------------------------------------------*/


	///////////////////////////////////////////////////////////////////////
	// freyjaNodeSet
	///////////////////////////////////////////////////////////////////////

	void freyjaNodeSetFlags( freyja_ptr node, byte flags );
	/*------------------------------------------------------
	 * Pre  : Pass valid freyja_mesh_flags_t's bitmap
	 * Post : Sets flags for node.
	 ------------------------------------------------------*/

	void freyjaNodeSetName( freyja_ptr node, const char* name );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Set human readable name for node.
	 ------------------------------------------------------*/

	void freyjaNodeSetParent( freyja_ptr node, freyja_ptr node ); 
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Attaches node to parent's graph.
	 ------------------------------------------------------*/

	void freyjaNodeSetPosition( freyja_ptr node, const vec3_t xyz );
	/*------------------------------------------------------
	 * Pre  : 
	 * Post : Sets <node> attribute position.
	 ------------------------------------------------------*/


} // extern "C"

#endif // GUARD__FREYJA_NODEABI_H_
